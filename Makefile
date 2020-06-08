# g++ main.cpp `pkg-config --cflags glib-2.0` -lzephyr
CC = g++
MAKE = make
CPPFLAGS= -std=c++14 -fomit-frame-pointer -fPIC -DQHASM -lrpc
LDFLAGS= -shared -L/usr/local/lib -lcrypto -lssl -lsodium -lpthread -lopendht -lgnutls

#SOURCES = $(shell echo *.cpp)
OBJECTS=$(shell echo pkg/*.o  server/*.o  user/*.o) 

ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
HEADERS = $(shell echo pkg/include/*.hpp  server/include/*.hpp user/include/*.hpp)



TARGET=libsecovid.so

all: ready $(TARGET)

.PHONY : clean
clean:
	rm -f $(OBJECTS)  $(TARGET)

ready:
	cd pkg && $(MAKE) && $(MAKE) install
	cd server && $(MAKE)  && $(MAKE) install
	cd user && $(MAKE) && $(MAKE) install
	$(CC) $(OBJECTS) -o $(TARGET) -shared -L/usr/local/lib
	echo "DONE"

install:
	mkdir -p /usr/local/include/secovid
	cp $(HEADERS) /usr/local/include/secovid
	cp libsecovid.so /usr/local/lib
