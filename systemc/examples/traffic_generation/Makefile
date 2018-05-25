# Program name
PROG_NAME = traffic_gen

# Directories
BONFIRE_ROOT   = ../../
SOURCEDIR 	   = .
LIBDIR 		   = ../../inc
TRAFFIC_LIBDIR = ./traffic_test_inc
BUILDDIR	   = build
# UTIL_FOLDER = $(BONFIRE_ROOT)/tmp/bin/plasma_utils
# SIMUL_TEMP	= $(BONFIRE_ROOT)/tmp/simul_temp

INC=$(TRAFFIC_LIBDIR)/ $(LIBDIR)/
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
PROG_C_SOURCES	= $(wildcard $(SOURCEDIR)/*.cpp)
PROG_MAIN_SRCS	= $(wildcard $(SOURCEDIR)/traffic_generation.cpp)

PROG_SOURCES 	= $(filter-out $(PROG_MAIN_SRCS),$(PROG_C_SOURCES))

# Library sources
LIB_ASM_SOURCES  = $(wildcard $(LIBDIR)/*.asm)
LIB_SOURCES 	 = $(wildcard $(LIBDIR)/*.cpp)
TRAFFIC_LIB_SRCS = $(wildcard $(TRAFFIC_LIBDIR)/*.cpp)

# Object files
PROG_OBJECTS	  = $(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(PROG_SOURCES))
PROG_MAIN_OBJECTS = $(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(PROG_MAIN_SRCS))

LIB_OBJECTS 		= $(patsubst $(LIBDIR)/%.cpp,$(BUILDDIR)/%.o,$(LIB_SOURCES))
TRAFFIC_LIB_OBJECTS = $(patsubst $(TRAFFIC_LIBDIR)/%.cpp,$(BUILDDIR)/%.o,$(TRAFFIC_LIB_SRCS))
LIB_ASM_OBJECTS 	= $(patsubst $(LIBDIR)/%.asm,$(BUILDDIR)/%.o,$(LIB_ASM_SOURCES))


# Rules
PHONY: all

all: dir elf_files

# Create a directory
dir:
	mkdir -p $(BUILDDIR)

# Build the executable
elf_files: $(LIB_ASM_OBJECTS) $(TRAFFIC_LIB_OBJECTS) $(LIB_OBJECTS) $(PROG_OBJECTS) $(PROG_MAIN_OBJECTS)
	$(foreach prog,$(PROG_MAIN_OBJECTS),\
		$(CC) $(CFLAGS) -o $(prog:%.o=%) \
		$(LIB_ASM_OBJECTS) $(TRAFFIC_LIB_OBJECTS) $(LIB_OBJECTS) $(PROG_OBJECTS) $(prog);)
	#$(LD) $(LDFLAGS) -o $@ $^

# Build library related files
$(TRAFFIC_LIB_OBJECTS): $(BUILDDIR)/%.o : $(TRAFFIC_LIBDIR)/%.cpp
	$(CC) -c $(CFLAGS) -o $@ $^

$(LIB_ASM_OBJECTS): $(BUILDDIR)/%.o : $(LIBDIR)/%.asm
	$(AS) $(ASFLAGS) -o $@ $^

$(LIB_OBJECTS): $(BUILDDIR)/%.o : $(LIBDIR)/%.cpp
	$(CC) -c $(CFLAGS) -o $@ $^

# Build program sources
$(PROG_OBJECTS): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.cpp
	@$(CC) -c $(CFLAGS) -o $@ $^

$(PROG_MAIN_OBJECTS): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.cpp
	@$(CC) -c $(CFLAGS) -o $@ $^

Makefile: ;

# # Generate code.txt files
# code_txt:
# 	@if [ ! -f $(UTIL_FOLDER)/convert_to_bin ]; then \
# 		echo "Plasma utils not compiled. I will compile them for you" && \
# 		echo && cd $(BONFIRE_ROOT)/Tools/Plasma_Utils/ && \
# 		make clean && make all; \
# 	fi
# 	$(foreach prog,$(PROG_MAIN_OBJECTS),\
# 		$(UTIL_FOLDER)/convert_to_bin \
# 		-i $(prog:%.o=%).elf \
# 		-t $(SIMUL_TEMP)/code_$(shell echo $(prog) | tail -c 4 | head -c 1).txt;)


clean:
	-$(RM) $(BUILDDIR)
