#-------------------------------
# Simple 3DS Homebrew Makefile
#-------------------------------

# Project name
TARGET := morpion

# Directories
BUILD     := build
SRC       := src
INCLUDE   := include
LIBCTRU   := /c/devkitPro/libctru
DEVKITPRO := /c/devkitPro
DEVKITARM := $(DEVKITPRO)/devkitARM

# Compiler flags
CXX      := $(DEVKITARM)/bin/arm-none-eabi-g++
CXXFLAGS := -O2 -Wall -std=gnu++11 -D__3DS__ \
            -I$(LIBCTRU)/include -I$(INCLUDE) \
            -mcpu=mpcore -mfloat-abi=hard -mfpu=vfp
LDFLAGS := -specs=3dsx.specs -L$(LIBCTRU)/lib \
           -lcitro2d -lcitro3d -lctru -lm \
           -mcpu=mpcore -mfloat-abi=hard -mfpu=vfp
# Source files (all .cpp in src/)
SRCS := $(wildcard $(SRC)/*.cpp)
OBJS := $(SRCS:$(SRC)/%.cpp=$(BUILD)/%.o)

# 3DSX conversion tool
3DSXTOOL := 3dsxtool

# Default target
all: $(BUILD)/$(TARGET).3dsx

# Compile objects
$(BUILD)/%.o: $(SRC)/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link ELF
$(BUILD)/$(TARGET).elf: $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Convert ELF to 3DSX and embed SMDH
$(BUILD)/$(TARGET).3dsx: $(BUILD)/$(TARGET).elf icon.smdh
	$(3DSXTOOL) $< $@ --smdh=icon.smdh

# Create build directory if it doesn't exist
$(BUILD):
	mkdir -p $(BUILD)

# Clean
clean:
	rm -rf $(BUILD)/*

.PHONY: all clean
