PROJECT := pe

TRAFFIC_SRC = ../../cpp/traffic_generator
UTIL_SRC = ../../cpp
CXXFLAGS := -std=c++14 -I$(TRAFFIC_SRC) -I$(UTIL_SRC)
SRCS    := $(wildcard *.cpp) $(wildcard $(TRAFFIC_SRC)/*.cpp) $(wildcard $(UTIL_SRC)/utils/*.cpp)
OBJS    := $(SRCS:.cpp=.o)

include ../Makefile.rules
