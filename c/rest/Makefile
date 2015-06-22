include ../common.mk

all: .autostyle
	$(CC) $(FLAGS) -c monitoring.c rest_helpers.c ../thirdparty/sqlite3.c
	$(CC) *.o -o monitoring
	mv monitoring ../../cgi-bin/monitoring
