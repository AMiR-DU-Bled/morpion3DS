#--------------------------------------
# 3DS Homebrew Makefile (Windows, with prebuilt ROMFS)
#--------------------------------------

# Project name
TARGET := morpion

# Directories
BUILD     := build
SRC       := src
INCLUDE   := include
LIBCTRU   := /c/devkitPro/libctru
DEVKITPRO := /c/devkitPro
DEVKITARM := /c/devkitPro/devkitARM
#ROMFS_SRC := bin/romfs.bin  # Already built manually

# Compiler and flags
CXX := "$(DEVKITARM)/bin/arm-none-eabi-g++"
CXXFLAGS := -g -O0 -Wall -std=gnu++11 -D__3DS__ \
            -I"$(LIBCTRU)/include" -I"$(INCLUDE)" \
            -mcpu=mpcore -mfloat-abi=hard -mfpu=vfp

LDFLAGS := -specs=3dsx.specs -L"$(LIBCTRU)/lib" \
           -lcitro2d -lcitro3d -lctru -lm \
           -mcpu=mpcore -mfloat-abi=hard -mfpu=vfp

# Tools
3DSXTOOL := "$(DEVKITPRO)/tools/bin/3dsxtool.exe"
MAKEROM  := "$(DEVKITPRO)/tools/bin/makerom.exe"

# Source files
SRCS := $(wildcard $(SRC)/*.cpp)
OBJS := $(SRCS:$(SRC)/%.cpp=$(BUILD)/%.o)

# Default target
all: $(BUILD)/$(TARGET).3dsx

# Compile objects
$(BUILD)/%.o: $(SRC)/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link ELF
$(BUILD)/$(TARGET).elf: $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Convert ELF to 3DSX and embed SMDH + prebuilt ROMFS
$(BUILD)/$(TARGET).3dsx: $(BUILD)/$(TARGET).elf icon.smdh $(ROMFS_SRC)
	$(3DSXTOOL) $< $@ --smdh=icon.smdh  
#--romfs=$(ROMFS_SRC)
# Create build directory if missing
$(BUILD):
	mkdir -p $(BUILD)

# Clean
clean:
	rm -rf $(BUILD)/*

.PHONY: all clean
