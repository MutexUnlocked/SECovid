# g++ main.cpp `pkg-config --cflags glib-2.0` -lzephyr
CC = g++

ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
 
#-ljsoncpp -ljsonrpccpp-common -ljsonrpccpp-client
CPPFLAGS= -std=c++14 -I$(ROOT_DIR)/include -fomit-frame-pointer -fPIC -DQHASM -lgmp -lsodium -lmcl -lssl -lcrypto 
LDFLAGS= -shared -L/usr/local/lib  -lgmp -lsodium -lmcl -lssl -lcrypto 

SOURCES = $(shell echo *.cpp)
HEADERS = $(shell echo include/*.hpp)
OBJECTS=$(SOURCES:.cpp=.o)

TARGET=libsecovid.so

all: $(TARGET)

.PHONY : clean
clean:
	rm -f $(OBJECTS)  $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

install:
	cp $(HEADERS) /usr/local/include/secovid
	cp libsecovid.so /usr/local/lib