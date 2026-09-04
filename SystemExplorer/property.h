#pragma once
#define WIDEN_(x) L##x
#define WIDEN(x) WIDEN_(#x)

#define RAISE_PROPERTY_CHANGED RaisePropertyChanged(__FUNCTIONW__);

#define PROPERTY_FIELD(NAME) NAME ## _

#define DECLARE_PROPERTY_GETTER(TYPE, NAME) TYPE NAME() const noexcept;
#define DECLARE_PROPERTY_GETTER_NC(TYPE, NAME) TYPE NAME() noexcept;

#define DECLARE_PROPERTY_SETTER(TYPE, NAME) void NAME(TYPE value) noexcept;

#define DECLARE_PROPERTY_FUNCS(TYPE, NAME) \
	DECLARE_PROPERTY_GETTER(TYPE, NAME) \
	DECLARE_PROPERTY_SETTER(TYPE, NAME) \

#define DECLARE_PROPERTY(TYPE, NAME) \
private: \
	TYPE PROPERTY_FIELD(NAME); \
	\
public: \
	DECLARE_PROPERTY_FUNCS(TYPE, NAME)

#define DECLARE_ONLY_SETTER(TYPE, NAME, INIT) \
private: \
    TYPE PROPERTY_FIELD(NAME){ INIT }; \
public: \
    TYPE NAME() const noexcept \
    { \
    return PROPERTY_FIELD(NAME); \
    } \
    DECLARE_PROPERTY_SETTER(TYPE, NAME)

#define DECLARE_ONLY_GETTER(TYPE, NAME, INIT) \
private: \
    TYPE PROPERTY_FIELD(NAME){ INIT }; \
public: \
    void NAME(TYPE value) noexcept \
    { \
        if (NAME ## _ != value) \
        {\
            NAME ## _ = std::move(value); \
            RaisePropertyChanged(WIDEN(NAME)); \
        } \
    } \
    DECLARE_PROPERTY_GETTER_NC(TYPE, NAME)



#define NOTIFYING_SETTING(TYPE, NAME, DEFAULT)                      \
    [[nodiscard]] TYPE NAME()                                       \
    {                                                               \
        return Get(WIDEN(NAME), DEFAULT);                           \
    }                                                               \
                                                                    \
    void NAME(TYPE const& value)                                    \
    {                                                               \
        if (NAME() != value)                                        \
        {                                                           \
            Set(WIDEN(NAME), value);                                \
            RaisePropertyChanged(WIDEN(NAME));                      \
        }                                                           \
    }