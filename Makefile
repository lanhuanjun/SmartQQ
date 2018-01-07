#Smart QQ Makefile
CXX = g++
CXXFLAGS = -Wall -std=c++14
SRC_PATH = Core/src
NET_SRC_PATH = $(SRC_PATH)/net
OTHER_SRC_PATH = $(SRC_PATH)/other
REQUEST_SRC_PATH = $(SRC_PATH)/request
SET_SRC_PATH = $(SRC_PATH)/set
THIRD_PATH = third
THIRD_INCLUDE_PATH = $(THIRD_PATH)/include
THIRD_LIB_PATH = $(THIRD_PATH)/lib
THIRD_UNIX_LIB_PATH = $(THIRD_PATH)/unix
OUT_PATH = out
net:
	$(CXX) $(CXXFLAGS) -c $(NET_SRC_PATH)/http_client.cc $(NET_SRC_PATH)/http_client.h -o $(OUT_PATH)/http_client.o
other:
	$(CXX) $(CXXFLAGS) -c $(OTHER_SRC_PATH)/base.h $(OTHER_SRC_PATH)/easylogging++.h $(OTHER_SRC_PATH)/easylogging++.cc \
	$(OTHER_SRC_PATH)/log.h $(OTHER_SRC_PATH)/platform.h $(OTHER_SRC_PATH)/platform.cpp \
	-o $(OUT_PATH)/other.o
set:
	$(CXX) $(CXXFLAGS) -c $(SET_SRC_PATH)/qq_info.h $(SET_SRC_PATH)/qq_message.h $(SET_SRC_PATH)/qq_message.cc \
	$(OTHER_SRC_PATH)/log.h $(OTHER_SRC_PATH)/platform.h $(OTHER_SRC_PATH)/platform.cpp \
	-o $(OUT_PATH)/other.o