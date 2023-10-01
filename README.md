# MQTT Mutual Authentication Examples
## Generating Certificates
We are going to self sign our certificates (we are the certificate authority).

1. Install openssl
```
sudo apt-get update
sudo apt-get install openssl
```
2. Generate CA key and certificate. Set password for ca.key.
```
openssl genrsa -des3 -out ca.key 2048

openssl req -new -x509 -days 1826 -key ca.key -out ca.crt
```
3. Generate server key and certificate signing request.Ensure the common name is the broker 
   hostname.
```
openssl genrsa -out server.key 2048
openssl req -new -out server.csr -key server.key
```
4. Sign the server certificate as the CA.
```
openssl x509 -req -in server.csr -CA ca.crt -CAkey ca.key -CA createserial -out server.crt -days 360
```
5. Generate the client key and certificate signing request. Ensure the common name is something 
   unique to the client. This will be its username.
```
openssl genrsa -out client.key 2048
openssl req -new -out client.csr -key client.key
```
6. Sign the client certificate as the CA.
```
openssl x509 -req -in client.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out client.crt -days 360
```

## Configuring Mosquitto
Ensure you already have mosquitto installed on your server, exposed to the internet and has a 
domain.

1. Place the server.crt, server.key & ca.crt inside /etc/mosquitto/certs
2. Create custom mosquitto configuration file:
```
sudo nano /etc/mosquitto/conf.d/custom.conf
```
3. Set the following configurations
```
allow_anonymous false
listener 8883
log_type error
log_type notice
log_type information
log_type debug
use_identity_as_username true
cafile /etc/mosquitto/certs/ca.crt
keyfile /etc/mosquitto/certs/server.key
certfile /etc/mosquitto/certs/server.crt
tls_version tlsv1.2
require_certificate true
```
4. Restart the mosquitto service:
```
sudo service mosquitto restart
```

See mosquittoConfig for example configuration files.

## Python Client Example
1. Install requirements (you should create venv first):
```
pip3 install -r requirements.txt 
```
2. Modify values for your configuration.
3. Run script:
```
python3 mqtt_client.py
```

## ESP32 Client
1. Setup VS Code.
2. Install platformIO plugin for VS Code.
3. Modify values to match your configuration.
4. Build project with platformIO.
5. Upload and Monitor to your ESP32.