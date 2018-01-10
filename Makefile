#Smart QQ Makefile
CXX := g++
CXXFLAGS := -W -std=c++14
SRC_PATH = Core/src
NET_SRC_PATH = $(SRC_PATH)/net
OTHER_SRC_PATH = $(SRC_PATH)/other
REQUEST_SRC_PATH = $(SRC_PATH)/request
SET_SRC_PATH = $(SRC_PATH)/set
THIRD_PATH = third
THIRD_INCLUDE_PATH = $(THIRD_PATH)/include
THIRD_LIB_PATH = $(THIRD_PATH)/lib
THIRD_UNIX_LIB_PATH = $(THIRD_LIB_PATH)/unix

core : easylogging++.o http_client.o platform.o qq_contact.o qq_control.o  qq_login.o  qq_message.o  qq_poll.o  qq_temp.o main.o
	$(CXX) $(CXXFLAGS) -o core easylogging++.o http_client.o platform.o qq_contact.o qq_control.o  qq_login.o  qq_message.o  qq_poll.o  qq_temp.o main.o \
	$(THIRD_UNIX_LIB_PATH)/libjsoncpp.a -L $(THIRD_UNIX_LIB_PATH) -lcurl

http_client.o : $(NET_SRC_PATH)/http_client.cc $(NET_SRC_PATH)/http_client.h
	$(CXX) $(CXXFLAGS) -c $(NET_SRC_PATH)/http_client.cc

platform.o : $(OTHER_SRC_PATH)/base.h  $(OTHER_SRC_PATH)/log.h $(OTHER_SRC_PATH)/platform.h $(OTHER_SRC_PATH)/platform.cpp
	$(CXX) $(CXXFLAGS) -c $(OTHER_SRC_PATH)/platform.cpp

easylogging++.o : $(OTHER_SRC_PATH)/easylogging++.h $(OTHER_SRC_PATH)/easylogging++.cc
	$(CXX) $(CXXFLAGS) -c $(OTHER_SRC_PATH)/easylogging++.cc

set.o : $(SET_SRC_PATH)/qq_info.h $(SET_SRC_PATH)/qq_message.h $(SET_SRC_PATH)/qq_message.cc
	$(CXX) $(CXXFLAGS) -c $(SET_SRC_PATH)/qq_info.h $(SET_SRC_PATH)/qq_message.cc

qq_contact.o : $(REQUEST_SRC_PATH)/qq_contact.h $(REQUEST_SRC_PATH)/qq_contact.cc
	$(CXX) $(CXXFLAGS) -c $(REQUEST_SRC_PATH)/qq_contact.cc

qq_poll.o : $(REQUEST_SRC_PATH)/qq_poll.h $(REQUEST_SRC_PATH)/qq_poll.cpp
	$(CXX) $(CXXFLAGS) -c $(REQUEST_SRC_PATH)/qq_poll.cpp

qq_login.o : $(REQUEST_SRC_PATH)/qq_login.h $(REQUEST_SRC_PATH)/qq_login.cc
	$(CXX) $(CXXFLAGS) -c $(REQUEST_SRC_PATH)/qq_login.cc

qq_temp.o : $(REQUEST_SRC_PATH)/qq_temp.h $(REQUEST_SRC_PATH)/qq_temp.cc
	$(CXX) $(CXXFLAGS) -c $(REQUEST_SRC_PATH)/qq_temp.cc

qq_control.o : $(SRC_PATH)/qq_control.h $(SRC_PATH)/qq_control.cc 
	$(CXX) $(CXXFLAGS) -c $(SRC_PATH)/qq_control.cc
	
qq_message.o : $(SET_SRC_PATH)/qq_message.h $(SET_SRC_PATH)/qq_message.cc
	$(CXX) $(CXXFLAGS) -c $(SET_SRC_PATH)/qq_message.cc

main.o : $(SRC_PATH)/core_main.h $(SRC_PATH)/test.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_PATH)/test.cpp