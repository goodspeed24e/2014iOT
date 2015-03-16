#!/bin/sh


#/usr/lib/ssl/openssl.cnf	# Default configuration file.
#/usr/lib/ssl/misc/CA.sh	# Default create CA script.


dir=./demoCA					# Where everything is kept
certs=$dir/certs				# Where the issued certs are kept
crl_dir=$dir/crl				# Where the issued crl are kept
database=$dir/index.txt				# database index file.
new_certs_dir=$dir/newcerts			# default place for new certs.

ca_root_certificate=$dir/cacert.pem		# The CA certificate
serial=$dir/serial				# The current serial number
crl=$dir/crl.pem				# The current CRL
ca_root_private_key=$dir/private/cakey.pem	# Private key file of Root CA

config_file=./instek_openssl.cnf

key_length=2048
days=3650
dh_param_length=1024

signing_ca_private_key=ca-signing-key.pem
signing_ca_request=ca-signing-req.pem
signing_ca_certificate=ca-signing-cert.pem

ca_chain_file=ca-chain.pem

server_private_key=server-key.pem
server_request=server-req.pem
server_certificate=server-cert.pem
server_certificate_chain=server-cert-chain.pem

client_private_key=client-key.pem
client_request=client-req.pem
client_certificate=client-cert.pem
client_certificate_chain=client-cert-chain.pem

df_parameters_file=dHParam.pem

mkdir -p ${dir}
mkdir -p ${dir}/certs
mkdir -p ${dir}/crl
mkdir -p ${dir}/newcerts
mkdir -p ${dir}/private
touch ${dir}/index.txt

# Root certificate authority
openssl genrsa -des3 -out ${ca_root_private_key} ${key_length}
openssl req -new -x509 -key ${ca_root_private_key} -out ${ca_root_certificate} -days ${days} -config ${config_file}

# Serial file and number
if [ ! -f "${serial}" ]; then
	openssl x509 -in ${ca_root_certificate} -noout -next_serial -out ${serial}
fi

# Signing certificate authority
openssl genrsa -des3 -out ${signing_ca_private_key} ${key_length}
openssl req -new -key ${signing_ca_private_key} -out ${signing_ca_request} -days ${days} -config ${config_file}
openssl ca -policy policy_anything -out ${signing_ca_certificate} -config ${config_file} -infiles ${signing_ca_request}
rm -f ${signing_ca_request}

# Certificate chain file
cat ${signing_ca_certificate} ${ca_root_certificate} > ${ca_chain_file}

# Certificate file of server
openssl genrsa -des3 -out ${server_private_key} ${key_length}
openssl req -new -key ${server_private_key} -out ${server_request} -days ${days} -config ${config_file}
openssl ca -policy policy_anything -keyfile ${signing_ca_private_key} -cert ${signing_ca_certificate} -out ${server_certificate} -config ${config_file} -infiles ${server_request}
rm -f ${server_request}

# Certificate chain file of server
cat ${server_certificate} ${signing_ca_certificate} ${ca_root_certificate} > ${server_certificate_chain}

# Certificate file of client
openssl genrsa -des3 -out ${client_private_key} ${key_length}
openssl req -new -key ${client_private_key} -out ${client_request} -days ${days} -config ${config_file}
openssl ca -policy policy_anything -keyfile ${signing_ca_private_key} -cert ${signing_ca_certificate} -out ${client_certificate} -config ${config_file} -infiles ${client_request}
rm -f ${client_request}

# Certificate chain file of client
cat ${client_certificate} ${signing_ca_certificate} ${ca_root_certificate} > ${client_certificate_chain}

# Diffie-Hellman parameters
openssl dhparam -outform PEM -out ${df_parameters_file} ${dh_param_length}

# We move or copy the file we need to one directory.
outdir=./instek_ca_output
mkdir -p ${outdir}
cp ${ca_root_certificate} ${ca_root_private_key} ${signing_ca_private_key} ${signing_ca_certificate} ${ca_chain_file} ${outdir}
mv ${server_private_key} ${server_certificate} ${client_private_key} ${client_certificate} ${df_parameters_file} ${server_certificate_chain} ${client_certificate_chain} ${outdir}

