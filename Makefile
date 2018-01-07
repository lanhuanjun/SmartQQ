#Smart QQ Makefile
CXXFLAGS = -Wall
SRC_PATH = Core/src
NET_SRC_PATH = $(SRC_PATH)/net
OTHER_SRC_PATH = $(SRC_PATH)/other
REQUEST_SRC_PATH = $(SRC_PATH)/request
SET_SRC_PATH = $(SRC_PATH)/set
THIRD_PATH = third
THIRD_INCLUDE_PATH = $(THIRD_PATH)/include
THIRD_LIB_PATH = $(THIRD_PATH)/lib
THIRD_UNIX_LIB_PATH = $(THIRD_PATH)/unix

net:
	CXX $(CXXFLAGS) $(NET_SRC_PATH)http_client.cc $(NET_SRC_PATH)http_client.h -o http_client.o