#include "pch.h"
#include "SettingsBase.h"
#if __has_include("Core/Serialization/SettingsBase.g.cpp")
#include "Core/Serialization/SettingsBase.g.cpp"
#endif

using namespace winrt::Windows::Foundation;

namespace winrt::SystemExplorer::Core::Serialization::implementation
{
    void SettingsBase::Save()
    {
        if (!Values || dirtyKeys_.empty()) return;

        for (auto const& key : dirtyKeys_)
        {
            Values.Insert(key, cache_[key]);
        }
        dirtyKeys_.clear();
    }
    IInspectable SettingsBase::Get(hstring const& key)
    {
        return GetCached(key);
    }
    IInspectable SettingsBase::GetCached(hstring const& key)
    {
        if (auto it = cache_.find(key); it != cache_.end())
            return it->second;

        if (!Values)
            return nullptr;

        auto value = Values.TryLookup(key);
        cache_[key] = value;
        return value;
    }
}
