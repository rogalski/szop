SZOP
===========================
SZOP - System Zautomatyzowanej Obserwacji Pacjentów.

How to create build.tar.gz
-------------------------

    python build.py

How to install via tftpd
-------------------------

    cd /var/www
    tftp -g -r build.tar.gz 192.168.50.79:69
    tar -xzf build.tar.gz
    chmod a+x install.sh start.sh stop.sh
    sh install.sh

How to start
-------------------------

    cd /var/www
    sh start.sh

How to stop
-------------------------

    cd /var/www
    sh stop.sh
