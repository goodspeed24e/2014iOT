#ifndef PROPERTIES_VALUE_LIMITATION_IF_HPP
#define PROPERTIES_VALUE_LIMITATION_IF_HPP

#include <libcommon/libcommon_config.h>
#include <libcommon/properties.hpp>

namespace FBi
{

class LIBCOMMON_DECL IPropertiesValueLimitation
{
public:
	virtual ~IPropertiesValueLimitation() {}

	virtual bool Check(const Properties::ValueType& value) = 0;
};

}

#endif

