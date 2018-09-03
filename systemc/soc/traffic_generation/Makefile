PROJECT := traffic_gen
# CXXFLAGS := -DNO_SYSTEMC # Uncomment for compliing without systemc. FIXME: Currently not compiling when this is enabled

# Directories
SOURCEDIR 	     = .

PE_DIR 		     = ../../ip/test_processing_element
TEST_ROUTER_DIR  = ../../ip/test_router
FLIT_UTILS_DIR   = ../../utils/flit_utils

# Program sources
PROG_MAIN_SRCS	    = $(wildcard $(SOURCEDIR)/traffic_generation.cpp)
PROG_C_SOURCES      = $(wildcard $(SOURCEDIR)/*.cpp)
PE_SOURCES 	        = $(wildcard $(PE_DIR)/*.cpp)
TEST_ROUTER_SOURCES = $(wildcard $(TEST_ROUTER_DIR)/*.cpp)
FLIT_UTILS_SOURCES  = $(wildcard $(FLIT_UTILS_DIR)/*.cpp)


# SRCS    := $(wildcard *.cpp)
SRCS    := $(PROG_MAIN_SRCS) $(PROG_C_SOURCES) $(PE_SOURCES) $(TEST_ROUTER_SOURCES) $(FLIT_UTILS_SOURCES)
OBJS    := $(SRCS:.cpp=.o)
INCDIR  := -I$(PE_DIR) -I$(TEST_ROUTER_DIR) -I$(FLIT_UTILS_DIR)

include ../../Makefile.rules
