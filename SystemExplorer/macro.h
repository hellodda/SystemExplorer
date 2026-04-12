#pragma once
#include <utility>
#include <type_traits>

#define PROPERTY_FIELD(NAME) NAME ## _

#define DECLARE_PROPERTY_GETTER(TYPE, NAME) TYPE NAME() const noexcept;

#define DECLARE_PROPERTY_SETTER(TYPE, NAME) void NAME(TYPE const& value) noexcept;

#define DECLARE_PROPERTY_FUNCS(TYPE, NAME) \
	DECLARE_PROPERTY_GETTER(TYPE, NAME) \
	DECLARE_PROPERTY_SETTER(TYPE, NAME) \

#define DECLARE_PROPERTY(TYPE, NAME) \
private: \
	TYPE PROPERTY_FIELD(NAME); \
	\
public: \
	DECLARE_PROPERTY_FUNCS(TYPE, NAME)


#define FACTORY(NAMESPACE, CLASS) \
namespace NAMESPACE::factory_implementation \
{ \
	struct CLASS : CLASS ## T<CLASS, implementation::CLASS> \
	{ \
	}; \
}
