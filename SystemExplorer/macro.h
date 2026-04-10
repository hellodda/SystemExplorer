#pragma once
#include <utility>
#include <type_traits>

#define PROPERTY_FIELD(NAME) NAME ## _
#define DECLARE_PROPERTY_FUNCS(TYPE, NAME, FIELD) \
	TYPE NAME() const noexcept(std::is_nothrow_copy_constructible_v<TYPE>); \
	void NAME(TYPE const& value) noexcept(std::is_nothrow_move_assignable_v<TYPE>); \

#define DECLARE_PROPERTY(TYPE, NAME) \
private: \
	TYPE PROPERTY_FIELD(NAME); \
	\
public: \
	DECLARE_PROPERTY_FUNCS(TYPE, NAME, PROPERTY_FIELD(NAME))

#define FACTORY(NAMESPACE, CLASS) \
namespace NAMESPACE::factory_implementation \
{ \
	struct CLASS : CLASS ## T<CLASS, implementation::CLASS> \
	{ \
	}; \
}