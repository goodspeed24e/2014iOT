#ifndef SSL_CONTEXT_INFO_HPP
#define SSL_CONTEXT_INFO_HPP

#include <http_server/libhttp_server_config.h>
#include <string>

namespace FBi
{

/// Struct to store the information about the SSL context.
struct LIBHTTPSERVER_DECL SSLContextInfo
{
	SSLContextInfo()
		: is_use_default_verify_paths(false),
		is_use_private_key_password_in_code(true)
	{
	}

	SSLContextInfo(const std::string& rsa_private_key_file_arg,
			const std::string& private_key_file_arg,
			const std::string& private_key_password_arg,
			const std::string& certificate_file_arg,
			const std::string& certificate_authority_file_arg,
			const std::string& ssl_dh_file_arg,
			bool is_use_default_verify_paths_arg,
			bool is_use_private_key_password_in_code_arg)
		: rsa_private_key_file(rsa_private_key_file_arg),
		private_key_file(private_key_file_arg),
		private_key_password(private_key_password_arg),
		certificate_file(certificate_file_arg),
		certificate_authority_file(certificate_authority_file_arg),
		ssl_dh_file(ssl_dh_file_arg),
		is_use_default_verify_paths(is_use_default_verify_paths_arg),
		is_use_private_key_password_in_code(is_use_private_key_password_in_code_arg)
	{
	}

	/// RSA private key path and filename.
	std::string rsa_private_key_file;
	/// Private key path and filename.
	std::string private_key_file;
	/// The password that be used when you generate the private key.
	std::string private_key_password;
	/// The certificate file.
	std::string certificate_file;
	/// The certification authority file that is used to verify.
	std::string certificate_authority_file;
	/// The temporary Diffie-Hellman parameters file.
	std::string ssl_dh_file;
	/// Use the default directories for finding certification authority certificates or not.
	bool is_use_default_verify_paths;
	/// Use the password of private key in the code or not.
	bool is_use_private_key_password_in_code;
};

}

#endif
