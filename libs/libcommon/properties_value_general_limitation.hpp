#ifndef PROPERTIES_VALUE_GENERAL_LIMITATION_HPP
#define PROPERTIES_VALUE_GENERAL_LIMITATION_HPP

#include <libcommon/libcommon_config.h>
#include <libcommon/properties_value_limitation_if.hpp>
#include <typeinfo>

namespace FBi
{

template<typename ValueT>
class LIBCOMMON_DECL PropertiesValueRangeLimitation : public IPropertiesValueLimitation
{
public:
	PropertiesValueRangeLimitation(const ValueT& min, const ValueT& max);

	virtual bool Check(const Properties::ValueType& value);
private:
	ValueT min_;
	ValueT max_;
};

template<typename ValueT>
class PropertiesValueTypeLimitation : public IPropertiesValueLimitation
{
public:
	virtual bool Check(const Properties::ValueType& value)
	{
		return (typeid(ValueT) == value.GetType());
	}
};

}

#endif

