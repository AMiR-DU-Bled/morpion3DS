#--------------------------------------
# 3DS Homebrew Makefile (Windows, auto ROMFS)
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
ROMFS     := romfs

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

#--------------------------------------
# Default: build both 3DSX and CIA
#--------------------------------------
all: $(BUILD)/$(TARGET).3dsx $(BUILD)/$(TARGET).cia

#--------------------------------------
# Object build
#--------------------------------------
$(BUILD)/%.o: $(SRC)/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

#--------------------------------------
# ELF
#--------------------------------------
$(BUILD)/$(TARGET).elf: $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

#--------------------------------------
# 3DSX
#--------------------------------------
$(BUILD)/$(TARGET).3dsx: $(BUILD)/$(TARGET).elf icon.smdh
	$(3DSXTOOL) $< $@ --smdh=icon.smdh --romfs=$(ROMFS)

#--------------------------------------
# CIA build
#--------------------------------------
$(BUILD)/$(TARGET).cia: $(BUILD)/$(TARGET).elf icon.smdh cia.rsf
	@echo "Building CIA..."
	"$(MAKEROM)" -f cia -o $@ -elf $< \
		-icon icon.smdh -rsf cia.rsf \
		-target t -exefslogo -romfs $(ROMFS)
	@echo "CIA built: $@"

#--------------------------------------
# Build folder
#--------------------------------------
$(BUILD):
	mkdir -p $(BUILD)

#--------------------------------------
# Clean
#--------------------------------------
clean:
	rm -rf $(BUILD)/*

.PHONY: all clean cia
