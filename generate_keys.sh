openssl genrsa -des3 -out ca.key 2048;
openssl req -new -x509 -days 1826 -key ca.key -out ca.crt;
openssl genrsa -out server.key 2048;
openssl req -new -out server.csr -key server.key;
openssl x509 -req -in server.csr -CA ca.crt -CAkey ca.key -CA createserial -out server.crt -days 360;
openssl genrsa -out client.key 2048;
openssl req -new -out client.csr -key client.key;
openssl x509 -req -in client.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out client.crt -days 360