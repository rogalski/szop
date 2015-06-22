#!bin/sh

echo "Prepare BOA binary"
mv boa /boa
chmod 777 /boa


echo "Prepare BOA config"
mv etc/boa/* /etc/boa/
rm -rf etc

echo "Prepare CGI"
chmod 777 cgi-bin/monitoring

echo "DONE."
