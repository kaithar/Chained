#! /bin/sh

echo "Creating cert..."
mkdir -p conf
openssl req -new -out conf/certreq.pem -keyout conf/key.pem -nodes
openssl req -x509 -days 3650 -in conf/certreq.pem -key conf/key.pem -out conf/cert.pem
