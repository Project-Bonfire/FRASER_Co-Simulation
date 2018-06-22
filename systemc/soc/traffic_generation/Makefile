# Program name
PROG_NAME = traffic_gen

# Directories
SOURCEDIR 	     = .

PE_DIR 		       = ../../ip/test_processing_element
TEST_ROUTER_DIR  = ../../ip/test_router
FLIT_UTILS_DIR   = ../../utils/flit_utils

BUILDDIR	       = build

INC=$(TEST_ROUTER_DIR)/ $(PE_DIR)/ $(SOURCEDIR)/ $(FLIT_UTILS_DIR)/
INC_PARAMS=$(foreach d, $(INC), -I$d)

# Compiler flags
CFLAGS = -Wall -std=c++1y
CFLAGS += $(INC_PARAMS)

# Assembler flags
ASFLAGS = -Wall

# Assembler flags
LDFLAGS = #-Ttext 0 -e entry -s -N

# Toolchain commands
CC 	 = g++
AS 	 = as
LD 	 = ld

# System Commands
RM = rm -rf

# Program sources

PROG_MAIN_SRCS	    = $(wildcard $(SOURCEDIR)/traffic_generation.cpp)
PROG_C_SOURCES      = $(wildcard $(SOURCEDIR)/*.cpp)
PE_SOURCES 	        = $(wildcard $(PE_DIR)/*.cpp)
TEST_ROUTER_SOURCES = $(wildcard $(TEST_ROUTER_DIR)/*.cpp)
FLIT_UTILS_SOURCES  = $(wildcard $(FLIT_UTILS_DIR)/*.cpp)

PROG_SOURCES 	= $(filter-out $(PROG_MAIN_SRCS),$(PROG_C_SOURCES))

# Object files
PROG_OBJECTS	    = $(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(PROG_SOURCES))
PROG_MAIN_OBJECTS = $(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(PROG_MAIN_SRCS))

PE_OBJECTS          = $(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(PE_SOURCES))
TEST_ROUTER_OBJECTS = $(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(TEST_ROUTER_SOURCES))
FLIT_UTILS_OBJECTS  = $(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(FLIT_UTILS_SOURCES))



# Rules
PHONY: all

all: dir elf_files

# Create a directory
dir:
	mkdir -p $(BUILDDIR)

# Build the executable
elf_files: $(PE_OBJECTS) $(TEST_ROUTER_OBJECTS) $(FLIT_UTILS_OBJECTS) $(PROG_OBJECTS) $(PROG_MAIN_OBJECTS)
	$(foreach prog,$(PROG_MAIN_OBJECTS),\
		$(CC) $(CFLAGS) -o $(prog:%.o=%) \
		$(PE_OBJECTS) $(TEST_ROUTER_OBJECTS) $(FLIT_UTILS_OBJECTS) $(PROG_OBJECTS) $(prog);)
	#$(LD) $(LDFLAGS) -o $@ $^

# Build library related files
$(PE_OBJECTS): $(BUILDDIR)/%.o : $(PE_DIR)/%.cpp
	$(CC) -c $(CFLAGS) -o $@ $^

$(TEST_ROUTER_OBJECTS): $(BUILDDIR)/%.o : $(TEST_ROUTER_DIR)/%.asm
	$(CC) -c $(CFLAGS) -o $@ $^

$(FLIT_UTILS_OBJECTS): $(BUILDDIR)/%.o : $(FLIT_UTILS_DIR)/%.cpp
	$(CC) -c $(CFLAGS) -o $@ $^

# Build program sources
$(PROG_OBJECTS): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.cpp
	@$(CC) -c $(CFLAGS) -o $@ $^

$(PROG_MAIN_OBJECTS): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.cpp
	@$(CC) -c $(CFLAGS) -o $@ $^

Makefile: ;

clean:
	-$(RM) $(BUILDDIR)
