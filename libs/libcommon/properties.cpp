#include <libcommon/properties.hpp>
#include <libcommon/properties_value_limitation_if.hpp>
#include <boost/foreach.hpp>
#include <iostream>

using namespace std;
using namespace boost::property_tree;
using namespace FBi;

static const boost::property_tree::path value_struct_ph("value" PT_SEP_STR "struct", PT_SEP_CH);
static const boost::property_tree::path value_boolean_ph("value" PT_SEP_STR "boolean", PT_SEP_CH);
static const boost::property_tree::path value_i4_ph("value" PT_SEP_STR "i4", PT_SEP_CH);
static const boost::property_tree::path value_string_ph("value" PT_SEP_STR "string", PT_SEP_CH);
static const boost::property_tree::path name_ph("name", PT_SEP_CH);
static const boost::property_tree::path value_double_ph("value" PT_SEP_STR "double", PT_SEP_CH);
static const boost::property_tree::path value_int_ph("value" PT_SEP_STR "int", PT_SEP_CH);
static const boost::property_tree::path value_array_ph("value" PT_SEP_STR "array", PT_SEP_CH);
static const boost::property_tree::path value_datetime_ph("value" PT_SEP_STR "dateTime.iso8601", PT_SEP_CH);

static const boost::property_tree::path data_ph("data", PT_SEP_CH);
static const boost::property_tree::path string_ph("string", PT_SEP_CH);
static const boost::property_tree::path i4_ph("i4", PT_SEP_CH);
static const boost::property_tree::path boolean_ph("boolean", PT_SEP_CH);
static const boost::property_tree::path double_ph("double", PT_SEP_CH);
static const boost::property_tree::path int_ph("int", PT_SEP_CH);
static const boost::property_tree::path struct_ph("struct", PT_SEP_CH);
static const boost::property_tree::path array_ph("array", PT_SEP_CH);
static const boost::property_tree::path datetime_ph("dateTime.iso8601", PT_SEP_CH);

//===========================================================================
/// Constructor of Properties
Properties::Properties() : map_(new Container)
{}
//===========================================================================
/**
 * @brief Function to import Properties from string.
 *
 * @param[in] buf The string which contains the XML of Properties.
 */
bool Properties::Import(const string& buf)
{
	ptree tmp_pt;
	std::stringstream ss(buf);
	try{
		boost::property_tree::xml_parser::read_xml(ss, tmp_pt);
	}
	catch(...)
	{
		return false;
	}

	map_->clear();
	compose_map("value", tmp_pt);

	return true;
}
//===========================================================================
/**
 * @brief Function to save this Properties to file.
 *
 * @param[in] filename The filename of output file.
 * @param[in] use_indent true: use indent in the XML, false: will not use indent in the XML.
 */
bool Properties::Save(const string& filename, bool use_indent) const
{
	ptree tmp_sub_pt;
	ptree tmp_pt;

	PropertiesToPropertyTree(*this, tmp_sub_pt);
	tmp_pt.add_child(boost::property_tree::path("value", PT_SEP_CH), tmp_sub_pt);
	try
	{
		if(use_indent)
		{
			boost::property_tree::xml_writer_settings<char> xml_settings('\t', 1);
			boost::property_tree::xml_parser::write_xml(filename, tmp_pt, locale::classic(), xml_settings);
		}
		else
		{
			boost::property_tree::xml_parser::write_xml(filename, tmp_pt);
		}
		return true;
	}
	catch(...)
	{}
	return false;
}

//===========================================================================
/**
 * @brief Function to load the Properties from file.
 *
 * @param[in] filename The filename of input file.
 */
bool Properties::Load(const string& filename)
{
	ptree tmp_pt;
	try{
		boost::property_tree::xml_parser::read_xml(filename, tmp_pt);
	}
	catch(...)
	{
		return false;
	}

	map_->clear();
	compose_map("value", tmp_pt);

	return true;
}
//===========================================================================
/**
 * @brief Function to transfer this Properties to the property tree.
 *
 * @param[out] pt The output property tree with root "struct".
 */
void Properties::Export(ptree& pt) const
{
	PropertiesToPropertyTree(*this, pt);
}
//===========================================================================
/**
 * @brief Function to transfer the XMLRPCArray to the property tree.
 *
 * @param[in] array The input XMLRPCArray.
 * @param[out] pt The output property tree with root "array".
 */
void Properties::ArrayToPropertyTree(const XMLRPCArray& array, boost::property_tree::ptree& pt)
{
	try
	{
		if(array.empty())
		{
			pt.add_child(boost::property_tree::path("array" PT_SEP_STR "data", PT_SEP_CH), boost::property_tree::ptree());
			return;
		}

		for(size_t i = 0; i < array.size(); ++i)
		{
			boost::property_tree::ptree temp_tree;

			if(array[i].IsInteger())
			{
				temp_tree.add(i4_ph, static_cast<int>(array[i]));
				pt.add_child(boost::property_tree::path("array" PT_SEP_STR "data" PT_SEP_STR "value", PT_SEP_CH), temp_tree);
			}
			else if(array[i].IsString())
			{
				temp_tree.add(string_ph, static_cast<std::string>(array[i]));
				pt.add_child(boost::property_tree::path("array" PT_SEP_STR "data" PT_SEP_STR "value", PT_SEP_CH), temp_tree);
			}
			else if(array[i].IsDouble())
			{
				temp_tree.add(double_ph, static_cast<double>(array[i]));
				pt.add_child(boost::property_tree::path("array" PT_SEP_STR "data" PT_SEP_STR "value", PT_SEP_CH), temp_tree);
			}
			else if(array[i].IsProperties())
			{
				PropertiesToPropertyTree(static_cast<FBi::Properties>(array[i]), temp_tree);
				pt.add_child(boost::property_tree::path("array" PT_SEP_STR "data" PT_SEP_STR "value", PT_SEP_CH), temp_tree);
			}
			else if(array[i].IsXMLRPCArray())
			{
				ArrayToPropertyTree(array[i], temp_tree);
				pt.add_child(boost::property_tree::path("array" PT_SEP_STR "data" PT_SEP_STR "value", PT_SEP_CH), temp_tree);
			}
			else if(array[i].IsXMLRPCDateTime())
			{
				temp_tree.add(datetime_ph, static_cast<XMLRPCDateTime>(array[i]).GetDateTimeString());
				pt.add_child(boost::property_tree::path("array" PT_SEP_STR "data" PT_SEP_STR "value", PT_SEP_CH), temp_tree);
			}
			else if(array[i].IsBool())
			{
				temp_tree.add(boolean_ph, (int)static_cast<bool>(array[i]));
				pt.add_child(boost::property_tree::path("array" PT_SEP_STR "data" PT_SEP_STR "value", PT_SEP_CH), temp_tree);
			}

		}

	} catch(...) {}
}
//===========================================================================
/**
 * @brief Function to transfer the Properties to property tree.
 *
 * @param[in] s1 The input Properties.
 * @param[out] pt The output property tree with root "struct".
 */
void Properties::PropertiesToPropertyTree(const FBi::Properties& s1, boost::property_tree::ptree& pt)
{
	Container::iterator iter = s1.map_->begin();

	if(s1.map_->empty())
	{
		pt.add_child(struct_ph, boost::property_tree::ptree());
		return;
	}

	for (;iter!=s1.map_->end(); ++iter) 
	{
		boost::property_tree::ptree tmp_pt;
		if (iter->second.value.IsProperties())
		{
			boost::property_tree::ptree temp_sub_pt;
			const FBi::Properties& temp_properties = static_cast<FBi::Properties>(iter->second.value);

			tmp_pt.add(name_ph, iter->first);
			if (!temp_properties.Empty())
			{
				PropertiesToPropertyTree(temp_properties, temp_sub_pt);
				tmp_pt.add_child(boost::property_tree::path("value", PT_SEP_CH), temp_sub_pt);
			}
			else
			{
				tmp_pt.add(value_struct_ph, "");
			}
		}
		else if (iter->second.value.IsString())
		{
			tmp_pt.add(name_ph, iter->first);
			tmp_pt.add(value_string_ph, static_cast<string>(iter->second.value));
		}
		else if (iter->second.value.IsInteger())
		{
			tmp_pt.add(name_ph, iter->first);
			tmp_pt.add(value_i4_ph, static_cast<int>(iter->second.value));
		}
		else if (iter->second.value.IsDouble())
		{
			tmp_pt.add(name_ph, iter->first);
			tmp_pt.add(value_double_ph, static_cast<double>(iter->second.value));
		}
		else if (iter->second.value.IsXMLRPCArray())
		{
			tmp_pt.add(name_ph, iter->first);
			const XMLRPCArray& array = iter->second.value;
			if(!array.empty())
			{
				boost::property_tree::ptree temp_array_tree;
				ArrayToPropertyTree(iter->second.value, temp_array_tree);
				tmp_pt.add_child(boost::property_tree::path("value", PT_SEP_CH), temp_array_tree);
			}
			else
			{
				tmp_pt.add(boost::property_tree::path("value" PT_SEP_STR "array" PT_SEP_STR "data", PT_SEP_CH), "");
			}
		}
		else if(iter->second.value.IsXMLRPCDateTime())
		{
			tmp_pt.add(name_ph, iter->first);
			tmp_pt.add(value_datetime_ph, static_cast<XMLRPCDateTime>(iter->second.value).GetDateTimeString());
		}
		else if (iter->second.value.IsBool())
		{
			tmp_pt.add(name_ph, iter->first);
			tmp_pt.add(value_boolean_ph, (int)static_cast<bool>(iter->second.value));
		}


		pt.add_child(boost::property_tree::path("struct" PT_SEP_STR "member", PT_SEP_CH), tmp_pt);
	}
}
//===========================================================================
/**
 * @brief Function to transfer the property tree to XMLRPCArray.
 *
 * @param[in] pt The input property tree with root "array".
 * @param[out] array The output XMLRPCArray.
 */
void Properties::PropertyTreeToArray(const boost::property_tree::ptree& pt, XMLRPCArray& array)
{
	boost::optional<const boost::property_tree::ptree&> data_tree_optional = pt.get_child_optional(data_ph);

	if(data_tree_optional)
	{
		ptree::const_iterator itor = data_tree_optional.get().begin();
		for (; itor != data_tree_optional.get().end(); ++itor)
		{
			if(itor->first != "value")
				continue;

			boost::optional<std::string> string_optional = itor->second.get_optional<std::string>(string_ph);
			if(string_optional)
			{
				array.push_back(string_optional.get());
				continue;
			}

			boost::optional<int> i4_optional = itor->second.get_optional<int>(i4_ph);
			if(i4_optional)
			{
				array.push_back(i4_optional.get());
				continue;
			}

			boost::optional<bool> bool_optional = itor->second.get_optional<bool>(boolean_ph);
			if(bool_optional)
			{
				array.push_back(bool_optional.get());
				continue;
			}

			boost::optional<double> double_optional = itor->second.get_optional<double>(double_ph);
			if(double_optional)
			{
				array.push_back(double_optional.get());
				continue;
			}

			boost::optional<int> int_optional = itor->second.get_optional<int>(int_ph);
			if(int_optional)
			{
				array.push_back(int_optional.get());
				continue;
			}

			boost::optional<const boost::property_tree::ptree&> struct_ptree = itor->second.get_child_optional(struct_ph);
			if(struct_ptree)
			{
				FBi::Properties temp_properties;
				PropertyTreeToProperties(struct_ptree.get(), temp_properties);
				array.push_back(temp_properties);
				continue;
			}

			boost::optional<const boost::property_tree::ptree&> array_ptree = itor->second.get_child_optional(array_ph);
			if(array_ptree)
			{
				XMLRPCArray temp_array;
				PropertyTreeToArray(array_ptree.get(), temp_array);
				array.push_back(temp_array);
				continue;
			}

			boost::optional<std::string> datetime_optional = itor->second.get_optional<std::string>(datetime_ph);
			if(datetime_optional)
			{
				XMLRPCDateTime temp_date_time;
				temp_date_time.SetDateTime(datetime_optional.get());
				array.push_back(temp_date_time);
				continue;
			}

			// default
			array.push_back(ValueType());
		}
	}
}
//===========================================================================
/**
 * @brief Function to transfer the property tree to Properties.
 *
 * @param[in] pt The input property tree with root "struct".
 * @param[out] s1 The output Properties.
 */
void Properties::PropertyTreeToProperties(const boost::property_tree::ptree& pt, FBi::Properties& s1)
{
	ptree::const_iterator itor = pt.begin();
	for (; itor != pt.end(); ++itor)
	{
		if (itor->first != "member")
			continue;

		string strResponse = itor->second.get<string>(name_ph, "");
		if (strResponse.empty())
			continue;

		FBi::Properties::Container* map_ptr = s1.GetContainerPtr();
		boost::optional<string> string_v = itor->second.get_optional<string>(value_string_ph);
		if (string_v)
		{
			map_ptr->insert(make_pair(strResponse, PropertyNode(string_v.get())));
			continue;
		}

		boost::optional<const boost::property_tree::ptree&> ptree_v = itor->second.get_child_optional(value_struct_ph);
		if (ptree_v)
		{
			FBi::Properties temp_properties;
			PropertyTreeToProperties(ptree_v.get(), temp_properties);
			map_ptr->insert(make_pair(strResponse, PropertyNode(temp_properties)));
			continue;
		}
		boost::optional<int> i4_v = itor->second.get_optional<int>(value_i4_ph);
		if (i4_v)
		{
			map_ptr->insert(make_pair(strResponse, PropertyNode(i4_v.get())));
			continue;
		}
		boost::optional<bool> boolean_v = itor->second.get_optional<bool>(value_boolean_ph);
		if (boolean_v)
		{
			map_ptr->insert(make_pair(strResponse, PropertyNode(boolean_v.get())));
			continue;
		}
		boost::optional<double> double_v = itor->second.get_optional<double>(value_double_ph);
		if (double_v)
		{
			map_ptr->insert(make_pair(strResponse, PropertyNode(double_v.get())));
			continue;
		}

		boost::optional<int> int_v = itor->second.get_optional<int>(value_int_ph);
		if (int_v)
		{
			map_ptr->insert(make_pair(strResponse, PropertyNode(int_v.get())));
			continue;
		}

		boost::optional<const boost::property_tree::ptree&> array_v = itor->second.get_child_optional(value_array_ph);
		if (array_v)
		{
			XMLRPCArray temp_array;
			PropertyTreeToArray(array_v.get(), temp_array);
			map_ptr->insert(make_pair(strResponse, PropertyNode(temp_array)));
			continue;
		}
		boost::optional<string> datatime_v = itor->second.get_optional<string>(value_datetime_ph);
		if (datatime_v)
		{
			XMLRPCDateTime temp_date_time;
			temp_date_time.SetDateTime(datatime_v.get());
			map_ptr->insert(make_pair(strResponse, PropertyNode(temp_date_time)));
			continue;
		}

		map_ptr->insert(make_pair(strResponse, PropertyNode(FBi::Properties())));
	}
}
//===========================================================================
/**
 * @brief Function to transfer this Properties to property tree in the specific path.
 *
 * @param[in] strPath The node path before the .struct or struct
 * @param[out] pt The output property tree.
 */
void Properties::compose_map(const std::string& strPath, const ptree & pt)
{
	try
	{
		std::string temp_path;
		size_t len = strPath.length();

		if((len > 0) && (strPath[len - 1] != PT_SEP_CH))
			temp_path = strPath + PT_SEP_STR"struct";
		else
			temp_path = strPath + "struct";

		const boost::property_tree::ptree& temp_sub_pt = pt.get_child(boost::property_tree::path(temp_path, PT_SEP_CH));
		PropertyTreeToProperties(temp_sub_pt, *this);
	}
	catch(...)
	{}
}
//===========================================================================
/**
 * @brief Function to tokenize string with separator colon.
 *
 * @param[in] str The string need to be tokenized.
 * @param[out] tokens The output tokens.
 * @return The number of tokens.
 */
int Properties::tokenize(const string& str, const std::string& separator, deque<string>& tokens) const
{
	// Skip delimiters at beginning.
	string::size_type lastPos = str.find_first_not_of(separator, 0); 
	// Find first "non-delimiter". 
	string::size_type pos = str.find_first_of(separator, lastPos);

	while (string::npos != pos || string::npos != lastPos)
	{
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		lastPos = str.find_first_not_of(separator, pos);
		pos = str.find_first_of(separator, lastPos);
	}

	return tokens.size();
}
//===========================================================================
/**
 * @brief Function to register the slot for the value of property is changed.
 *
 * @param[in] slot The callback function.
 * @return The connection of boost signal2.
 */
boost::signals2::connection Properties::RegisterValueChangedSlot(const NotifySlotFunc& slot)
{
	return this->prop_signal_.connect(slot);
}
//===========================================================================
/**
 * @brief Function to get the value in the specific path.
 *
 * @param[in] node_path The path that we want to get the value.
 * @param[out] value The value in the specific path.
 * @return true: If we can get the value. false: If we can not.
 */
bool Properties::Get(const string& node_path, ValueType& value) const
{
	std::deque<std::string> keys;
	this->tokenize(node_path, ":", keys);

	if(keys.empty())
		return false;

	std::deque<std::string>::iterator itor = keys.begin();
	const Properties* properties = this;
	Container::const_iterator map_itor;

	for(; itor != keys.end(); ++itor)
	{
		map_itor = properties->GetContainerPtr()->find(*itor);
		// Can not find the key.
		if(map_itor == properties->GetContainerPtr()->end())
			return false;

		if(map_itor->second.value.IsProperties())
		{
			// Current node is a property.
			properties = static_cast<const Properties*>(map_itor->second.value);
			if(properties)
				continue;

			return false;
		}
		else
		{
			// Current node is not a property and it is last searching node.
			if((itor + 1) == keys.end())
			{
				value = map_itor->second.value;
				return true;
			}

			return false;
		}
	}

	// If it is last searching node and a property.
	if((itor == keys.end()) && properties)
	{
		value = *properties;
		return true;
	}

	return false;
}
//===========================================================================
/**
 * @brief Set the value to existent key (It will emit signal when success)
 *
 * @param[in] node_path The node path which we want to set.
 * @param[in] value The new value will be set.
 * @return false: 1) key doesn't exist 2) value doesn't change. Otherwise, return true.
 */
bool Properties::Set(const std::string& node_path, const ValueType& value)
{
	std::deque<std::string> keys;
	this->tokenize(node_path, ":", keys);

	if(keys.empty())
		return false;

	std::deque<std::string>::iterator itor = keys.begin();
	Properties* properties = this;
	Container::iterator map_itor;

	for(; itor != keys.end(); ++itor)
	{
		map_itor = properties->GetContainerPtr()->find(*itor);
		// Can not find the key.
		if(map_itor == properties->GetContainerPtr()->end())
			return false;

		if( ((itor + 1) == keys.end()) && (map_itor->second.is_writable) && (map_itor->second.value.IsEqualType(value)))
		{
			// It is last node and writable then check it.
			for(size_t i = 0; i < map_itor->second.limitation_list.size(); ++i)
			{
				if( (map_itor->second.limitation_list[i]) && (! (map_itor->second.limitation_list[i]->Check(value)) ) )
					return false;
			}

			map_itor->second.value = value;

			if(!this->prop_signal_.empty())
			{
				//emit SIGNAL........
				Properties p;
				p.Add(node_path, value);
				this->prop_signal_(p);
			}

			return true;
		}

		if(map_itor->second.value.IsProperties())
		{
			// Current node is a property and is not last node.
			properties = static_cast<Properties*>(map_itor->second.value);
			if(properties)
				continue;

			//return false;
		}

		return false;
	}

	return false;
}
//===========================================================================
/**
 * @brief Function to get the property node in the specific path.
 *
 * @param[in] node_path The path that we want to get the property node.
 * @param[out] extension The property node in the specific path.
 * @return true: If we can get the property node. false: If we can not.
 */
bool Properties::Get(const std::string& node_path, PropertyNode& extension) const
{
	std::deque<std::string> keys;
	this->tokenize(node_path, ":", keys);

	if(keys.empty())
		return false;

	std::deque<std::string>::iterator itor = keys.begin();
	const Properties* properties = this;
	const PropertyNode* temp_ext = NULL;
	Container::const_iterator map_itor;

	for(; itor != keys.end(); ++itor)
	{
		map_itor = properties->GetContainerPtr()->find(*itor);
		// Can not find the key.
		if(map_itor == properties->GetContainerPtr()->end())
			return false;

		if(map_itor->second.value.IsProperties())
		{
			// Current node is a property.
			temp_ext = &(map_itor->second);
			properties = static_cast<const Properties*>(map_itor->second.value);

			if(properties)
				continue;

			return false;
		}
		else
		{
			// Current node is not a property and it is last searching node.
			if((itor + 1) == keys.end())
			{
				extension = map_itor->second;
				return true;
			}

			return false;
		}
	}

	if(temp_ext)
	{
		extension = *temp_ext;
		return true;
	}

	return false;
}
//===========================================================================
/**
 * @brief Function to put the property node in the specific path. When path does no exist, it will establish the path and node. It will emit signal when success.
 *
 * @param[in] node_path The node path which we want to put property node.
 * @param[in] extension The property node which we want to set.
 * @return false: 1) value doesn't change. Otherwise, return true.
 */
bool Properties::Put(const std::string& node_path, const PropertyNode& extension)
{
	std::deque<std::string> keys;
	this->tokenize(node_path, ":", keys);

	if(keys.empty())
		return false;

	std::deque<std::string>::iterator itor = keys.begin();
	Properties* properties = this;
	Container::iterator map_itor;

	for(size_t i = 0; i < extension.limitation_list.size(); ++i)
	{
		if( (extension.limitation_list[i]) && !(extension.limitation_list[i]->Check(extension.value)) )
			return false;
	}

	for(; itor != keys.end(); ++itor)
	{
		map_itor = properties->GetContainerPtr()->find(*itor);
		// Can not find the key.
		if(map_itor == properties->GetContainerPtr()->end())
		{
			// not last node
			if((itor + 1) != keys.end())
			{
				// Auto extend
				PropertyNode temp_ext;
				temp_ext.value = Properties();
				pair<Container::iterator, bool> temp_ret = properties->GetContainerPtr()->insert(make_pair(*itor, temp_ext));
				if(!(temp_ret.second))
					return false;
				map_itor = temp_ret.first;
			}
			else
			{
				// Last node in node path
				pair<Container::iterator, bool> temp_ret = properties->GetContainerPtr()->insert(make_pair(*itor, extension));
				if(!(temp_ret.second))
					return false;

				if(!this->prop_signal_.empty())
				{
					//emit SIGNAL........
					Properties p;
					p.Add(node_path, extension.value);
					this->prop_signal_(p);
				}

				return true;
			}
		}

		if((itor + 1) == keys.end())
		{
			// It is last node.
			map_itor->second = extension;

			if(!this->prop_signal_.empty())
			{
				//emit SIGNAL........
				Properties p;
				p.Add(node_path, extension.value);
				this->prop_signal_(p);
			}

			return true;
		}

		if(map_itor->second.value.IsProperties())
		{
			// Current node is a property and is not last node.
			properties = static_cast<Properties*>(map_itor->second.value);
			if(properties)
				continue;

			//return false;
		}

		return false;
	}

	return false;
}
//===========================================================================
/**
 * @brief Add a nonexistent node to indicated node path, if path is invalid, it will auto create path.
 *
 * @param[in] node_path The node path which we want to add value.
 * @param[in] value The value that should be added.
 * @return true: If we can add the value. false: The node is existent or other errors occur.
 */
bool Properties::Add(const std::string& node_path, const ValueType& value)
{
	std::deque<std::string> keys;
	this->tokenize(node_path, ":", keys);

	if(keys.empty())
		return false;

	std::deque<std::string>::iterator itor = keys.begin();
	Properties* properties = this;
	Container::iterator map_itor;

	for(; itor != keys.end(); ++itor)
	{
		map_itor = properties->GetContainerPtr()->find(*itor);
		// Can not find the key.
		if(map_itor == properties->GetContainerPtr()->end())
		{
			// not last node
			PropertyNode temp_ext;
			if((itor + 1) != keys.end())
			{
				// Auto extend
				temp_ext.value = Properties();
				pair<Container::iterator, bool> temp_ret = properties->GetContainerPtr()->insert(make_pair(*itor, temp_ext));
				if(!(temp_ret.second))
					return false;
				map_itor = temp_ret.first;
			}
			else
			{
				// Last node in node path
				temp_ext.value = value;
				pair<Container::iterator, bool> temp_ret = properties->GetContainerPtr()->insert(make_pair(*itor, temp_ext));
				if(!(temp_ret.second))
					return false;

				// We do not emit the signal here.
				return true;
			}
		}

		if((itor + 1) == keys.end())
		{
			// It is last node and we found the key. We do not update the value???
			return false;
		}

		if(map_itor->second.value.IsProperties())
		{
			// Current node is a property and is not last node.
			properties = static_cast<Properties*>(map_itor->second.value);
			if(properties)
				continue;

			//return false;
		}

		return false;
	}

	return false;

}
//===========================================================================
/**
 * @brief Delete "end node" in the specific path.
 *
 * @param[in] node_path The node path which we want to delete.
 * @return true: If we can delete the node. false: The node is nonexistent or other errors occur.
 */
bool Properties::Del(const std::string& node_path)
{
	std::deque<std::string> keys;
	this->tokenize(node_path, ":", keys);

	if(keys.empty())
		return false;

	std::deque<std::string>::iterator itor = keys.begin();
	Properties* properties = this;
	Container::iterator map_itor;

	for(; itor != keys.end(); ++itor)
	{
		map_itor = properties->GetContainerPtr()->find(*itor);
		// Can not find the key.
		if(map_itor == properties->GetContainerPtr()->end())
			return false;

		// If it is last searching node.
		if((itor + 1) == keys.end())
		{
			properties->GetContainerPtr()->erase(map_itor);
			return true;
		}

		if(map_itor->second.value.IsProperties())
		{
			// Current node is a property.
			properties = static_cast<Properties*>(map_itor->second.value);
			if(properties)
				continue;

			//return false;
		}

		return false;
	}

	return false;
}
//===========================================================================
/**
 * @brief Rollback the value in the specific path.
 *
 * @param[in] rollback_nodes_paths The node path which we want to rollback.
 * @param[in] rollback_data The partial data from the original Properties.
 */
void Properties::rollback(const std::deque<string>& rollback_nodes_paths, Properties& rollback_data)
{
	std::deque<std::string>::const_iterator itor = rollback_nodes_paths.begin();
	std::deque<std::string>::const_iterator end_itor = rollback_nodes_paths.end();

	for(; itor != end_itor ; ++itor)
	{
		std::cout << "rollback node: " << *itor << std::endl;
		ValueType v;
		rollback_data.Get(*itor, v);
		this->Set(*itor, v);
	}
}
//===========================================================================
/**
 * @brief Import another Properties. (It will emit signal when success)
 *
 * @param[in] imp_p The properties should be imported.
 * @param[in] ignore_err
 * false: Like set() function, node path must exist. Node type must be equal.
 * true: Skip any error in check, it will only signal the node which successfully assigned.
 * @return true: If we can import values from input Properties. false: If we can not.
 */
bool Properties::ImportValues(Properties& imp_p, bool ignore_err)
{
	Container* imp_c = imp_p.GetContainerPtr();
	Container::iterator imp_itor;

	Container* sys_c = this->GetContainerPtr();

	std::deque< std::pair<Container*, Container*> > found_map;
	std::deque<std::string> found_nodes;	// store the path during loop
	std::deque<std::string> rollback_nodes;	// store path of nodes from root node, for rollback use

	Properties rollback_p;			// store original ValueType, for rollback
	Properties signal_p;			// store successful assignment, for emit signal

	string current_node_path;

	do
	{
		imp_itor = imp_c->begin();

		// Search each keys in current node.
		for(; imp_itor != imp_c->end() ; ++imp_itor)
		{
			Container::iterator sys_itor = sys_c->find(imp_itor->first);

			// If we can not find the key in the original property.
			if(sys_itor == sys_c->end())
			{
				if(ignore_err)
					continue;

				if(!rollback_nodes.empty())
					this->rollback(rollback_nodes, rollback_p);
				return false;
			}

			if(sys_itor->second.value.IsProperties() && imp_itor->second.value.IsProperties())
			{
				// Remember the nodes those are properties because we will handle them later.
				found_map.push_back(make_pair(sys_itor->second.value.GetContainerPtr(), imp_itor->second.value.GetContainerPtr()));

				if(current_node_path.empty())
					found_nodes.push_back(imp_itor->first);
				else
					found_nodes.push_back(current_node_path + ":" + imp_itor->first);
			}
			else
			{
				ValueType v = sys_itor->second.value; // Backup the original value for rollback

				// Check the value and access rights
				bool is_valid_value = (sys_itor->second.is_writable) && (sys_itor->second.value.IsEqualType(imp_itor->second.value));

				if(is_valid_value)
				{
					for(size_t i = 0; i < sys_itor->second.limitation_list.size(); ++i)
					{
						if( (sys_itor->second.limitation_list[i]) && (! (sys_itor->second.limitation_list[i]->Check(imp_itor->second.value)) ) )
						{
							is_valid_value = false;
							break;
						}
					}
				}

				if(is_valid_value)
				{
					sys_itor->second.value = imp_itor->second.value;

					rollback_p.Add( current_node_path + ":" + imp_itor->first, v);

					if(ignore_err)
						signal_p.Add(current_node_path + ":" + imp_itor->first, imp_itor->second.value);

					rollback_nodes.push_back(current_node_path + ":" + imp_itor->first);
				}
				else
				{
					if(ignore_err)
						continue;

					if(!rollback_nodes.empty())
						this->rollback(rollback_nodes, rollback_p);

					return false;
				}
			}
		}

		if(found_map.empty())
			break;

		sys_c = found_map.front().first;
		imp_c = found_map.front().second;
		current_node_path = found_nodes.front();

		found_map.pop_front();
		found_nodes.pop_front();
	}
	while(true);

	// Emit signal
	if(!prop_signal_.empty())
	{
		if(ignore_err)
			this->prop_signal_(signal_p);
		else
			this->prop_signal_(imp_p);
	}

	return true;
}
//===========================================================================
/**
 * @brief Copy the XMLRPC array.
 *
 * @param[in] org_array The original input array.
 * @param[out] result_array The output copied array.
 * @return Always return true now.
 */
bool Properties::CopyXMLRPCArray(const XMLRPCArray& org_array, XMLRPCArray& result_array)
{
	bool ret = true;
	for(size_t i = 0; i < org_array.size(); ++i)
	{
		// If the value is properties, we copy all properties.
		if(org_array[i].IsProperties())
		{
			const FBi::Properties& temp_org_prop = org_array[i];
			FBi::Properties temp_prop;
			if(!temp_org_prop.GetProperties("*", temp_prop))
				ret = false;
			result_array.push_back(temp_prop);
			continue;
		}

		// If the value is array, we copy all array.
		if(org_array[i].IsXMLRPCArray())
		{
			const XMLRPCArray& temp_org_array = org_array[i];
			XMLRPCArray temp_array;
			if(!Properties::CopyXMLRPCArray(temp_org_array, temp_array))
				ret = false;
			result_array.push_back(temp_array);
			continue;
		}

		// If it is other types, we copy the value.
		result_array.push_back(org_array[i]);
	}

	return ret;
}
//===========================================================================
/**
 * @brief Get the all sub-properties.
 *
 * @param[in] org_prop The properties need to be got.
 * @param[out] result_prop The output properties.
 * @return Always return true now.
 */
bool Properties::get_all_sub_properties(const Properties& org_prop, Properties& result_prop) const
{
	bool ret = true;
	// For all keys in this level, we get their value.
	Container::const_iterator map_iter = org_prop.map_->begin();
	for(; map_iter != org_prop.map_->end(); ++map_iter)
	{
		// If it is properties, we get all sub-properties recursively.
		if(map_iter->second.value.IsProperties())
		{
			this->get_all_sub_properties(org_prop[map_iter->first], result_prop[map_iter->first]);
			continue;
		}

		// If it is array, we copy all array.
		if(map_iter->second.value.IsXMLRPCArray())
		{
			const XMLRPCArray& temp_org_array = map_iter->second.value;
			XMLRPCArray temp_array;
			if(!Properties::CopyXMLRPCArray(temp_org_array, temp_array))
				ret = false;

			PropertyNode temp_node;
			temp_node.value = temp_array;
			temp_node.is_writable = map_iter->second.is_writable;
			temp_node.limitation_list = map_iter->second.limitation_list;
			(*result_prop.map_)[map_iter->first] = temp_node;

			continue;
		}

		// If it is value, we get the value.
		(*result_prop.map_)[map_iter->first] = map_iter->second;
	}

	return ret;
}
//===========================================================================
/**
 * @brief Handle one sub-properties those specified by the current key.
 *
 * @param[in] keys The specific keys.
 * @param[in] current_key_iter The iterator to pointer to the current level key.
 * @param[in] org_prop The properties need to be got.
 * @param[out] result_prop The output properties.
 * @return Always return true now.
 */
bool Properties::handle_one_sub_properties(std::deque<std::string>& keys, const std::deque<std::string>::iterator& current_key_iter, const Properties& org_prop, Properties& result_prop) const
{
	// If there is no any key, we do nothing.
	if(current_key_iter == keys.end())
		return true;

	// If the key does not exist in this properties, we do nothing.
	Container::const_iterator map_iter = org_prop.map_->find(*current_key_iter);
	if(map_iter == org_prop.map_->end())
		return true;

	// If the value is a properties.
	if(map_iter->second.value.IsProperties())
	{
		bool ret = true;
		std::deque<std::string>::iterator temp_next_key_iter = current_key_iter + 1;
		if(temp_next_key_iter != keys.end())
		{
			// If it is not last specific key on the keys then we use next key to get sub-properties recursively.
			Properties temp_sub_result_prop;
			// We get it's sub-properties according to the other keys of next levels.
			if(!this->get_sub_properties(keys, temp_next_key_iter, org_prop[*current_key_iter], temp_sub_result_prop))
				ret = false;
			// If the result properties is not empty then we create new node and assign value to output result properties.
			if(!temp_sub_result_prop.Empty())
			{
				PropertyNode temp_node;
				temp_node.value = temp_sub_result_prop;
				temp_node.is_writable = map_iter->second.is_writable;
				temp_node.limitation_list = map_iter->second.limitation_list;
				(*result_prop.map_)[*current_key_iter] = temp_node;
			}
		}
		else
		{
			// If it is last specific key on the keys then we get all it's sub-properties.
			if(!this->get_all_sub_properties(org_prop[*current_key_iter], result_prop[*current_key_iter]))
				ret = false;
		}
		return ret;
	}

	// If the value is array, we copy all array.
	if(map_iter->second.value.IsXMLRPCArray())
	{
		bool ret = true;
		const XMLRPCArray& temp_org_array = map_iter->second.value;
		XMLRPCArray temp_array;
		if(!Properties::CopyXMLRPCArray(temp_org_array, temp_array))
			ret = false;

		PropertyNode temp_node;
		temp_node.value = temp_array;
		temp_node.is_writable = map_iter->second.is_writable;
		temp_node.limitation_list = map_iter->second.limitation_list;
		(*result_prop.map_)[*current_key_iter] = temp_node;

		return ret;
	}

	// If the value is not a properties.
	(*result_prop.map_)[*current_key_iter] = map_iter->second;

	return true;
}
//===========================================================================
/**
 * @brief Get the sub-properties those specified by keys.
 *
 * @param[in] keys The specific keys.
 * @param[in] current_key_iter The iterator to pointer to the current level key.
 * @param[in] org_prop The properties need to be got.
 * @param[out] result_prop The output properties.
 * @return Always return true now.
 */
bool Properties::get_sub_properties(std::deque<std::string>& keys, const std::deque<std::string>::iterator& current_key_iter, const Properties& org_prop, Properties& result_prop) const
{
	// If there is no any key, we do nothing.
	if(current_key_iter == keys.end())
		return true;

	// It is separated by ",". For example: The "1,4,5" in the "cam:1,4,5".
	if(current_key_iter->find_first_of(',') != std::string::npos)
	{
		// Get all the key in the current level.
		std::deque<std::string> temp_current_level_keys;
		this->tokenize(*current_key_iter, ",", temp_current_level_keys);
		if(temp_current_level_keys.empty())
			return true;

		bool ret = true;
		// Backup the current key because we will modify it temporarily.
		const std::string org_current_key = *current_key_iter;

		// For all specific keys in the current level.
		std::deque<std::string>::iterator temp_new_key_iter = temp_current_level_keys.begin();
		for(; temp_new_key_iter != temp_current_level_keys.end(); temp_new_key_iter++)
		{
			// Because the current key is special key, we replace the it with normal key temporarily.
			*current_key_iter = *temp_new_key_iter;
			if(!this->handle_one_sub_properties(keys, current_key_iter, org_prop, result_prop))
				ret = false;
		}

		// Restore the current key from backup.
		*current_key_iter = org_current_key;

		return ret;
	}

	// It is "*". For example: The "*" in the "cam:*:name".
	if(current_key_iter->compare("*") == 0)
	{
		bool ret = true;
		// Backup the current key because we will modify it temporarily.
		const std::string org_current_key = *current_key_iter;

		// For all keys in the current level.
		Container::const_iterator map_iter = org_prop.map_->begin();
		for(; map_iter != org_prop.map_->end(); ++map_iter)
		{
			// Because the current key is special key, we replace the it with normal key temporarily.
			*current_key_iter = map_iter->first;
			if(!this->handle_one_sub_properties(keys, current_key_iter, org_prop, result_prop))
				ret = false;
		}

		// Restore the current key from backup.
		*current_key_iter = org_current_key;

		return ret;
	}

	// It is normal key. For example: The "cam", "1" or "name" in the "cam:1:name"
	this->handle_one_sub_properties(keys, current_key_iter, org_prop, result_prop);

	return true;
}
//===========================================================================
/**
 * @brief Get the sub-properties those specified by path.
 *
 * (Important: [1] Three characters *,: are used as token.)
 * ex: cam:1,4,5 or *:2,4,8:name or cam:*:name,status or * etc.
 *
 * @param[in] node_path The specific path to get sub-properties.
 * @param[out] prop The output properties.
 * @return false: If we can not get token from specific path. Else true will be returned.
 */
bool Properties::GetProperties(const string& node_path, Properties& prop) const
{
	// Get the keys in the each level from the path.
	std::deque<std::string> keys;
	this->tokenize(node_path, ":", keys);

	if(keys.empty())
		return false;

	std::deque<std::string>::iterator itor = keys.begin();

	// Start get sub-properties.
	return ( this->get_sub_properties(keys, itor, *this, prop) );
}
//===========================================================================
/**
 * @brief Merge and update two properties.
 * Important:
 * [1] This method do not emit signal.
 * [2] If the "from properties" is empty, it will do nothing.
 * [3] If there is the same path in the "from" and "to" properties, however their values are not 
 * the same type, then the value of "from properties" will overwrite the value of "to properties".
 * [4] The "to properties" will be changed after this method call.
 *
 * @param[in] from_prop The properties need to be merged into the other properties.
 */
bool Properties::Merge(const Properties& from_prop)
{
	bool ret = true;
	// For all nodes in the current level of from properties.
	Container::const_iterator from_map_iter = from_prop.map_->begin();
	for(; from_map_iter != from_prop.map_->end(); ++from_map_iter)
	{
		// If we can not find the same key in the "to properties" then we create new node and assign the value.
		Container::iterator to_map_iter = map_->find(from_map_iter->first);

		// If we can find the same key in the "to properties".
		// If these node are all properties then we merge and update their sub-properties.
		if( (to_map_iter != this->map_->end()) && from_map_iter->second.value.IsProperties() && to_map_iter->second.value.IsProperties())
		{
			FBi::Properties& temp_prop = to_map_iter->second.value;
			temp_prop.Merge(from_prop[from_map_iter->first]);
			continue;
		}

		// Get the referece of new node or existent node.
		PropertyNode& to_node_ref = (to_map_iter == this->map_->end()) ? ((*(this->map_))[from_map_iter->first]) : (to_map_iter->second);

		// If the "from" is properties and the "to" is not a properties then we copy all properties.
		if(from_map_iter->second.value.IsProperties())
		{
			const FBi::Properties& temp_org_prop = from_map_iter->second.value;
			FBi::Properties temp_prop;
			if(!temp_org_prop.GetProperties("*", temp_prop))
				ret = false;

			to_node_ref.value = temp_prop;
			to_node_ref.is_writable = from_map_iter->second.is_writable;
			to_node_ref.limitation_list = from_map_iter->second.limitation_list;

			continue;
		}

		// If the "from" is array, we copy all array.
		if(from_map_iter->second.value.IsXMLRPCArray())
		{
			const XMLRPCArray& temp_org_array = from_map_iter->second.value;
			XMLRPCArray temp_array;
			if(!Properties::CopyXMLRPCArray(temp_org_array, temp_array))
				ret = false;

			to_node_ref.value = temp_array;
			to_node_ref.is_writable = from_map_iter->second.is_writable;
			to_node_ref.limitation_list = from_map_iter->second.limitation_list;

			continue;
		}

		// If these node are not the same type then we use the value of "from properties" to overwrite the value of "to properties".
		to_node_ref.value = from_map_iter->second.value;
		to_node_ref.is_writable = from_map_iter->second.is_writable;
		to_node_ref.limitation_list = from_map_iter->second.limitation_list;
	}
	return ret;
}
//===========================================================================

