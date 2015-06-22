CC=arm-linux-gnueabi-gcc
SQLITE3_FLAGS=-DSQLITE_THREADSAFE=0 -DSQLITE_OMIT_LOAD_EXTENSION
FLAGS=-O3 $(SQLITE3_FLAGS)

#CC=gcc
#FLAGS=-O0 -g3 $(SQLITE3_FLAGS)
FLAGS+=-Wall

.autostyle:
	astyle *.c -A2 -s2 -f -p -H  -U -xe -k1 -z2 --exclude="sqlite3.c" -xi

.clean:
		rm -f *.o
