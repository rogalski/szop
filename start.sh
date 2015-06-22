#!sh
echo "Synchronize clocks"
ntpd -nq -p 212.244.36.227

echo "Start BOA"
/boa
echo "Start uploader in background"
./uploader /dev/ttySAC3 database.db > /dev/null 2>&1
