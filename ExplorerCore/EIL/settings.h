#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h> // Для WideCharToMultiByte / MultiByteToWideChar

#include <absl/container/flat_hash_map.h>
#include <absl/container/flat_hash_set.h>
#include <absl/hash/hash.h>
#include <glaze/glaze.hpp>
#include <filesystem>
#include <fstream>
#include <optional>
#include <string>

#if __has_include(<winrt/base.h>)
#include <winrt/base.h>
#endif

namespace eil::details
{
    // Полноценная конвертация UTF-16 -> UTF-8 для Windows
    inline std::string to_utf8(std::wstring_view wstr)
    {
        if (wstr.empty()) return std::string();
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()), NULL, 0, NULL, NULL);
        std::string strTo(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()), strTo.data(), size_needed, NULL, NULL);
        return strTo;
    }

    // Полноценная конвертация UTF-8 -> UTF-16 для Windows
    inline std::wstring to_utf16(std::string_view str)
    {
        if (str.empty()) return std::wstring();
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), NULL, 0);
        std::wstring wstrTo(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), wstrTo.data(), size_needed);
        return wstrTo;
    }

    // SFINAE проверка на наличие default_path в TConfig
    template<typename T, typename = void>
    struct has_default_path : std::false_type {};

    template<typename T>
    struct has_default_path<T, std::void_t<decltype(T::default_path)>> : std::true_type {};
}

namespace glz
{
    template <>
    struct from<JSON, std::wstring>
    {
        template <auto Opts>
        static void op(std::wstring& value, is_context auto&& ctx, auto&& it, auto&& end) noexcept
        {
            std::string str;
            from<JSON, std::string>::op<Opts>(str, ctx, it, end);
            value = eil::details::to_utf16(str);
        }
    };

    template <>
    struct to<JSON, std::wstring>
    {
        template <auto Opts, class... Args>
        static void op(const std::wstring& value, is_context auto&& ctx, Args&&... args) noexcept
        {
            std::string utf8_str = eil::details::to_utf8(value);
            to<JSON, std::string>::op<Opts>(utf8_str, ctx, std::forward<Args>(args)...);
        }
    };

#if __has_include(<winrt/base.h>)
    template <>
    struct from<JSON, winrt::hstring>
    {
        template <auto Opts>
        static void op(winrt::hstring& value, is_context auto&& ctx, auto&& it, auto&& end) noexcept
        {
            std::string str;
            from<JSON, std::string>::op<Opts>(str, ctx, it, end);
            value = winrt::to_hstring(str);
        }
    };

    template <>
    struct to<JSON, winrt::hstring>
    {
        template <auto Opts, class... Args>
        static void op(const winrt::hstring& value, is_context auto&& ctx, Args&&... args) noexcept
        {
            to<JSON, std::string>::op<Opts>(winrt::to_string(value), ctx, std::forward<Args>(args)...);
        }
    };
#endif
}

namespace eil
{
    namespace details
    {
        struct glaze_options : glz::opts {};

        template <size_t N>
        struct fixed_string
        {
            char buf[N]{};
            consteval fixed_string(const char(&str)[N])
            {
                std::copy_n(str, N, buf);
            }
        };

        struct i_settings_context
        {
            static inline thread_local i_settings_context* current_initializing{ nullptr };

            virtual ~i_settings_context() = default;

            virtual std::optional<glz::generic> get_setting(std::string_view key) const = 0;
            virtual void set_setting(std::string_view key, glz::generic const& value) = 0;
        };

        template <typename T, eil::details::fixed_string name>
        struct single_threaded_setting_base
        {
            single_threaded_setting_base(T default_val = T{})
                : key_(name.buf), default_value_(std::move(default_val))
            {
                context_ = i_settings_context::current_initializing;
            }

            operator T () const
            {
                if (!context_) return default_value_;

                auto json_opt = context_->get_setting(key_);
                if (!json_opt) return default_value_;

                T value{};
                auto err = glz::read < eil::details::glaze_options{} > (value, *json_opt);
                return err ? default_value_ : value;
            }

            template <typename Q>
            auto operator=(Q&& q)
            {
                if (context_)
                {
                    glz::generic jsonVal;
                    std::string buffer;

                    // КРИТИЧЕСКОЕ ИСПРАВЛЕНИЕ: приводим тип к T (std::wstring), 
                    // чтобы Glaze нашел специализацию и не падал на const wchar_t[N]
                    T val_to_serialize = std::forward<Q>(q);

                    (void)glz::write_json(val_to_serialize, buffer);
                    (void)glz::read_json(jsonVal, buffer);

                    context_->set_setting(key_, jsonVal);
                }
                return *this;
            }

            std::string_view key_{};
            T default_value_{};
            i_settings_context* context_{ nullptr };
        };

        struct StringHash
        {
            using is_transparent = void;
            size_t operator()(std::string_view sv) const {
                return absl::Hash<std::string_view>{}(sv);
            }
        };
    }

    template <typename T, eil::details::fixed_string name>
    struct single_threaded_setting : eil::details::single_threaded_setting_base<T, name>
    {
        using base_type = eil::details::single_threaded_setting_base<T, name>;

        single_threaded_setting(T def_val = T{}) : base_type(std::move(def_val)) {}

        operator T() const
        {
            return static_cast<const base_type&>(*this).operator T();
        }

        T operator()() const
        {
            return *this;
        }

        template <typename Q>
        auto& operator=(Q&& q)
        {
            static_cast<base_type&>(*this) = std::forward<Q>(q);
            return *this;
        }
    };

    template <typename T, eil::details::fixed_string name>
    struct single_threaded_rw_setting : single_threaded_setting<T, name>
    {
        using base_type = single_threaded_setting<T, name>;

        single_threaded_rw_setting(T def_val = T{}) : base_type(std::move(def_val)) {}

        using base_type::operator();

        template <typename Q>
        auto& operator()(Q&& q)
        {
            return *this = std::forward<Q>(q);
        }

        template <typename Q>
        auto& operator=(Q&& q)
        {
            base_type::operator=(std::forward<Q>(q));
            return *this;
        }
    };

    struct standard_settings_configuration
    {
        static inline std::filesystem::path path;
        static inline std::filesystem::path default_path;
    };

    struct generic_t : glz::generic
    {
        generic_t(glz::generic object) : glz::generic(std::move(object)) {};

        std::wstring get_wstring() const
        {
            // ИСПРАВЛЕНИЕ: Используем правильную конвертацию вместо наивного копирования итераторов,
            // которое ломало не-ASCII символы
            return eil::details::to_utf16(get_string());
        }

#if __has_include(<winrt/base.h>)
        winrt::hstring get_hstring() const
        {
            return winrt::to_hstring(get_string());
        }
#endif
    };

    template<typename T, typename TConfig = standard_settings_configuration>
    struct settings_base : details::i_settings_context
    {
    private:
        using event_t = eil::event<eil::action_t<std::string_view, eil::generic_t const&>>;

        std::filesystem::path settingsPath_ = TConfig::path;
        std::string sectionName_;

        mutable bool isFileRead_{ false };
        mutable std::optional<glz::generic::object_t> rawSection_;
        mutable std::optional<glz::generic::object_t> defaultSection_;

        mutable absl::flat_hash_map<std::string, glz::generic, details::StringHash, std::equal_to<>> cache_;
        absl::flat_hash_set<std::string> dirtyKeys_;

    private:
        static std::string get_section_name()
        {
            std::string_view name = typeid(T).name();

            auto lastColon = name.rfind("::");
            if (lastColon != std::string_view::npos)
            {
                name = name.substr(lastColon + 2);
            }
            else
            {
                auto space = name.find(' ');
                if (space != std::string_view::npos)
                {
                    name = name.substr(space + 1);
                }
            }
            return std::string(name);
        }

        void load_json_section(const std::filesystem::path& path, std::optional<glz::generic::object_t>& target) const
        {
            if (path.empty() || !std::filesystem::exists(path)) return;

            std::string fileData;
            {
                std::ifstream ifs(path, std::ios::binary | std::ios::ate);
                if (!ifs) return;

                auto size = ifs.tellg();
                ifs.seekg(0, std::ios::beg);

                fileData.resize(size);
                if (!ifs.read(fileData.data(), size)) return;
            }

            glz::generic root;
            if (glz::read_json(root, fileData) == glz::error_code::none)
            {
                if (root.contains(sectionName_) && root[sectionName_].is_object())
                {
                    target = std::move(root[sectionName_].get_object());
                }
            }
        }

        void ensure_section_loaded() const
        {
            if (isFileRead_) return;
            isFileRead_ = true;

            load_json_section(settingsPath_, rawSection_);

            if constexpr (details::has_default_path<TConfig>::value)
            {
                load_json_section(TConfig::default_path, defaultSection_);
            }
        }

    protected:
        [[nodiscard]] std::optional<glz::generic> get_setting(std::string_view key) const override
        {
            auto it = cache_.find(key);
            if (it != cache_.end()) return it->second;

            ensure_section_loaded();
            std::string keyStr(key);

            if (rawSection_ && rawSection_->contains(keyStr))
            {
                auto val = (*rawSection_)[keyStr];
                cache_.emplace(keyStr, val);
                return val;
            }

            if (defaultSection_ && defaultSection_->contains(keyStr))
            {
                auto val = (*defaultSection_)[keyStr];
                cache_.emplace(std::move(keyStr), val);
                return val;
            }

            return std::nullopt;
        }

        void set_setting(std::string_view key, glz::generic const& value) override
        {
            bool isChanged{ false };

            ensure_section_loaded();

            std::string keyStr(key);
            auto& currentVal = cache_[keyStr];

            std::string currentStrVal;
            std::string newStrVal;

            (void)glz::write_json(currentVal, currentStrVal);
            (void)glz::write_json(value, newStrVal);

            if (currentStrVal != newStrVal)
            {
                currentVal = value;
                dirtyKeys_.insert(keyStr);
                isChanged = true;
            }

            if (isChanged)
            {
                SettingChaged.invoke(key, eil::generic_t{ value });
            }
        }

    public:
        event_t SettingChaged;

        settings_base()
        {
            details::i_settings_context::current_initializing = this;
            sectionName_ = get_section_name();
        }

        ~settings_base() noexcept override
        {
            write_changes();
        }

        void write_changes() noexcept
        {
            try
            {
                if (dirtyKeys_.empty()) return;

                absl::flat_hash_map<std::string, glz::generic, details::StringHash, std::equal_to<>> cache_copy = cache_;
                absl::flat_hash_set<std::string> dirty_copy = std::move(dirtyKeys_);
                dirtyKeys_.clear();

                glz::generic root;

                if (std::filesystem::exists(settingsPath_))
                {
                    std::string fileData;
                    std::ifstream ifs(settingsPath_, std::ios::binary | std::ios::ate);
                    if (ifs)
                    {
                        auto size = ifs.tellg();
                        ifs.seekg(0, std::ios::beg);
                        fileData.resize(size);
                        if (ifs.read(fileData.data(), size))
                        {
                            (void)glz::read_json(root, fileData);
                        }
                    }
                }

                if (!root.is_object()) root = glz::generic::object_t{};

                if (!root.contains(sectionName_) || !root[sectionName_].is_object())
                {
                    root[sectionName_] = glz::generic::object_t{};
                }

                auto& section = root[sectionName_].get_object();

                for (const auto& key : dirty_copy)
                {
                    if (auto it = cache_copy.find(key); it != cache_copy.end())
                    {
                        section[key] = it->second;
                    }
                }

                std::string buffer;
                (void)glz::write_json(root, buffer);

                auto tempPath = settingsPath_;
                tempPath += ".tmp";

                {
                    std::ofstream ofs(tempPath, std::ios::binary | std::ios::trunc);
                    if (ofs)
                    {
                        ofs << buffer;
                    }
                }

                std::error_code ec;
                std::filesystem::rename(tempPath, settingsPath_, ec);
            }
            catch (...)
            {
            }
        }
    };
}