#pragma once
#ifndef PTREEMETHOD_HPP
#define PTREEMETHOD_HPP

#include <libtransport/libtransport_config.h>
#include <libcommon/properties.hpp>
#include <vector>

namespace FBi
{

class LIBTRANSPORT_DECL XMLRPC_Msg_Ptree
{
public:
	XMLRPC_Msg_Ptree(const std::string&, const std::string&);
	virtual ~XMLRPC_Msg_Ptree() {}

	void AddString(const std::string&);
	void AddInteger(int);
	void AddBool(bool);
	void AddDouble(double);
	void AddProperties(const Properties&);
	void AddArray(const XMLRPCArray&);
	void AddDateTime(const XMLRPCDateTime&);
	bool AddValue(const Properties::ValueType&);
	bool GetParameters(std::vector<Properties::ValueType>&);

	void Parse(const char *);
	void Parse(const char*, size_t);
	void ComposeTree();
	std::string GetXML(bool use_indent = false) const;
	void Clear();
	size_t GetStreamLength() const;

protected:
	Properties::ValueType get_rpc_value(boost::property_tree::ptree::const_iterator&);

protected:
	boost::property_tree::ptree m_pt;
	std::stringstream m_stream;
	const std::string param_path_;
	const std::string params_path_;
};


class LIBTRANSPORT_DECL MethodCall : public XMLRPC_Msg_Ptree 
{
public:
	MethodCall();


	std::string GetMethodName();
	void SetMethodName(const std::string&);
private:
	// Copy constructor for std::stringstream, because it does not be copied.
	// So we have two method to solve it.
	// 1. Make our class to non-copyable
	// 2. Public the copy constructor etc. (However, we can not use method 2 here. 
	// Because we can not know the content in std::stringstream is that we want.)
	MethodCall(const MethodCall&);
	MethodCall& operator=(const MethodCall&);

};

class LIBTRANSPORT_DECL MethodResponse : public XMLRPC_Msg_Ptree
{
public:
	MethodResponse();

	void SetFault(int, const std::string&);
	std::string GetErrMsg();
	int GetErrValue();
	bool IsOK();
	Properties::ValueType GetResult();
private:
	// Copy constructor for std::stringstream, because it does not be copied.
	// So we have two method to solve it.
	// 1. Make our class to non-copyable
	// 2. Public the copy constructor etc. (However, we can not use method 2 here. 
	// Because we can not know the content in std::stringstream is that we want.)
	MethodResponse(const MethodResponse&);
	MethodResponse& operator=(const MethodResponse&);

};


typedef boost::shared_ptr<MethodCall> MethodCallPtr;

typedef boost::shared_ptr<MethodResponse> MethodResponsePtr;

}

#endif
