#include <libcommon/properties_value_general_limitation.hpp>
#include <libcommon/properties.hpp>

namespace FBi
{

// ========== PropertiesRangeLimitation ==========
template<typename ValueT>
PropertiesValueRangeLimitation<ValueT>::PropertiesValueRangeLimitation(const ValueT& min, const ValueT& max): min_(min), max_(max)
{
}

template<typename ValueT>
bool PropertiesValueRangeLimitation<ValueT>::Check(const Properties::ValueType& value)
{
	try
	{
		const ValueT& tempValue = static_cast<const ValueT&>(value);
		return ( (tempValue >= min_) && (tempValue <= max_) );
	}
	catch(const boost::bad_any_cast&)
	{
		return false;
	}
	catch(...)
	{
		return false;
	}
}

// We need to add these such that some compiler can find symbol. It will not link error.
template class PropertiesValueRangeLimitation<int>;
template class PropertiesValueRangeLimitation<double>;

}


