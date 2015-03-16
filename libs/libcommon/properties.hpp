#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

#include <libcommon/libcommon_config.h>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <map>
#include <deque>
#include <boost/shared_ptr.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/function.hpp>
#include <libcommon/xmlrpc_datetime.hpp>

//#include <loki/Singleton.h>

// Because the datetime of XMLRPC will use dot in the tag <dateTime.iso8601>, we need to use other separator here.
/// The separator character for boost property tree
#define PT_SEP_CH '/'
/// The separator string for boost property tree
#define PT_SEP_STR "/"

namespace FBi
{
// Forward declaration
class IPropertiesValueLimitation;

class LIBCOMMON_DECL Properties
{
public:
	// Forward declaration
	struct PropertyNode;

	typedef boost::function<void (const FBi::Properties&)> NotifySlotFunc;

	class ValueType
	{
		typedef std::vector<ValueType> XMLRPCArray;
		boost::any	m_value;
	public:
		ValueType(){}
		ValueType(const std::string& str):m_value(str){}
		ValueType(const char* str):m_value(std::string(str)){}
		ValueType(int v):m_value(v){}
		ValueType(double v):m_value(v){}
		ValueType(bool v):m_value(v){}
		ValueType(const Properties& c):m_value(c){}
		ValueType(const XMLRPCArray& v):m_value(v){}
		ValueType(const XMLRPCDateTime& v):m_value(v){}

		const std::type_info& GetType() const { return m_value.type(); }

		operator bool() const
		{
			return boost::any_cast<bool>(m_value);
		}

		operator bool()
		{
			return boost::any_cast<bool>(m_value);
		}

		operator int() const
		{
			return boost::any_cast<int>(m_value);
		}

		operator int()
		{
			return boost::any_cast<int>(m_value);
		}

		operator double() const
		{
			return boost::any_cast<double>(m_value);
		}

		operator double()
		{
			return boost::any_cast<double>(m_value);
		}

		operator std::string() const
		{
			return boost::any_cast<std::string>(m_value);
		}

		operator const Properties &() const
		{
			return boost::any_cast<const Properties&>(m_value);
		}

		operator Properties &()
		{
			return boost::any_cast<Properties&>(m_value);
		}

		operator const Properties *() const
		{
			return boost::any_cast<Properties>(&m_value);
		}

		operator Properties *()
		{
			return boost::any_cast<Properties>(&m_value);
		}

		operator const XMLRPCArray &() const
		{
			return boost::any_cast<const XMLRPCArray&>(m_value);
		}

		operator XMLRPCArray &()
		{
			return boost::any_cast<XMLRPCArray&>(m_value);
		}

		operator const XMLRPCArray *() const
		{
			return boost::any_cast<XMLRPCArray>(&m_value);
		}

		operator XMLRPCArray *()
		{
			return boost::any_cast<XMLRPCArray>(&m_value);
		}

		operator const XMLRPCDateTime &() const
		{
			return boost::any_cast<const XMLRPCDateTime&>(m_value);
		}

		operator XMLRPCDateTime &()
		{
			return boost::any_cast<XMLRPCDateTime&>(m_value);
		}

		operator const XMLRPCDateTime *() const
		{
			return boost::any_cast<XMLRPCDateTime>(&m_value);
		}

		operator XMLRPCDateTime *()
		{
			return boost::any_cast<XMLRPCDateTime>(&m_value);
		}

		const ValueType& operator[](const std::string& str) const
		{
			return boost::any_cast<const Properties&>(m_value)[str];
		}

		ValueType& operator[](const std::string& str)
		{
			return boost::any_cast<Properties&>(m_value)[str];
		}

		ValueType& operator[](const char* str)
		{
			return boost::any_cast<Properties&>(m_value)[str];
		}

		const ValueType& operator[](const char* str) const
		{
			return boost::any_cast<const Properties&>(m_value)[str];
		}

		std::map< std::string, PropertyNode>* GetContainerPtr()
		{
			return boost::any_cast<Properties&>(m_value).GetContainerPtr();
		}

		bool IsBool() const
		{
			return m_value.type() == typeid(bool);
		}

		bool IsInteger() const
		{
			return m_value.type() == typeid(int);
		}

		bool IsString() const
		{
			return m_value.type() == typeid(std::string);
		}

		bool IsDouble() const
		{
			return m_value.type() == typeid(double);
		}

		bool IsProperties() const
		{
			return m_value.type() == typeid(Properties);
		}

		bool IsXMLRPCArray() const
		{
			return m_value.type() == typeid(XMLRPCArray);
		}

		bool IsXMLRPCDateTime() const
		{
			return m_value.type() == typeid(XMLRPCDateTime);
		}

		bool IsEqualType(const ValueType& v)
		{
			return m_value.type() == v.m_value.type();
		}
	};

	typedef std::vector<ValueType> XMLRPCArray;
public:
	struct PropertyNode
	{
	public:
		PropertyNode(): is_writable(true), value(Properties()) {}
        explicit PropertyNode(const ValueType& v): is_writable(true), value(v) {}
		void Reset()
		{
			is_writable = true;
			limitation_list.clear();
			value = ValueType();
		}

	public:
		bool is_writable;
		std::vector< boost::shared_ptr<IPropertiesValueLimitation> > limitation_list;
		ValueType value;
	};
public:
	typedef std::map<std::string, PropertyNode> Container;

	Properties();

	const Container* GetContainerPtr() const
	{
		return map_.get();
	}

	Container* GetContainerPtr()
	{
		return map_.get();
	}

	Properties(const Properties& rhs)
	{
		map_ = rhs.map_;
	}

	ValueType& operator[](const std::string& str)
	{
		return search(str);
	}

	const ValueType& operator[](const std::string& str) const
	{
		return search(str);
	}

	ValueType& operator[](const char* str)
	{
		return search(str);
	}

	const ValueType& operator[](const char* str) const
	{
		return search(str);
	}

	Properties& operator=(const Properties& rhs)
	{
		if(&rhs != this)
		{
			map_ = rhs.map_;
		}

		return *this;
	}

	bool Empty() const
	{
		return map_->empty();
	}       

public:
	static void PropertyTreeToArray(const boost::property_tree::ptree& pt, XMLRPCArray& array);
	static void PropertyTreeToProperties(const boost::property_tree::ptree& pt, FBi::Properties& s1);
	static void ArrayToPropertyTree(const XMLRPCArray& array, boost::property_tree::ptree& pt);
	static void PropertiesToPropertyTree(const FBi::Properties& s1, boost::property_tree::ptree& pt);

	bool Import(const std::string& buf);
	bool Load(const std::string& filename);
	bool Save(const std::string& filename, bool use_indent = true) const;

	void Export(boost::property_tree::ptree&) const;

	ValueType& search(const std::string& str) const
	{
		return (*map_)[str].value;
		/*
		Container::iterator itor = map_.find(str);

		if (itor == map_.end())
		{
			return map_[str].value = Properties();
		}
		return itor->second.value;*/
	}

	// The following interface are ported from old Properties of hdvr
	boost::signals2::connection RegisterValueChangedSlot(const NotifySlotFunc& slot);

	bool Get(const std::string& node_path, ValueType& value) const;
	bool Set(const std::string& node_path, const ValueType& value);

	bool Get(const std::string& node_path, PropertyNode& extension) const;
	bool Put(const std::string& node_path, const PropertyNode& extension);
	
	bool Add(const std::string& node_path, const ValueType& value);
	bool Del(const std::string& node_path);

	bool ImportValues(Properties& imp_p, bool ignore_err);

	// Copy behaviour -- start
	bool GetProperties(const std::string& node_path, Properties& prop) const;
	bool Merge(const Properties&);
	static bool CopyXMLRPCArray(const XMLRPCArray& org_array, XMLRPCArray& result_array);
	// Copy behaviour -- end

private:
	void compose_map(const std::string& strPath, const boost::property_tree::ptree&);

	int tokenize(const std::string& str, const std::string& separator, std::deque<std::string>& tokens) const;
	void rollback(const std::deque<std::string>& rollback_nodes_paths, Properties& rollback_data);

	bool get_all_sub_properties(const Properties& org_prop, Properties& result_prop) const;
	bool handle_one_sub_properties(std::deque<std::string>& keys, const std::deque<std::string>::iterator& current_key_iter, const Properties& org_prop, Properties& result_prop) const;
	bool get_sub_properties(std::deque<std::string>& keys, const std::deque<std::string>::iterator& current_key_iter, const Properties& org_prop, Properties& result_prop) const;

	boost::shared_ptr<Container> map_;
	boost::signals2::signal<void (FBi::Properties&)> prop_signal_;
};

typedef std::vector<Properties::ValueType> XMLRPCArray;

} // namespace FBi
/*
typedef Loki::SingletonHolder
<
FBi::Properties
>
GlobalProperties;
*/
#endif
