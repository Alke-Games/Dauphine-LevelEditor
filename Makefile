# Folders containing files
INCLUDES	:= include
SOURCES		:= src
RESOURCES	:= res
BUILD		:= build

# Compiler
export LD := $(CXX)
# Compiler flags
CPPFLAGS=-O2 -g -W -Wall -pedantic -ansi -std=c++0x -Wextra -Wshadow
# Linking flags
LDFLAGS = -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lSDL2_gfx -llua -ldl -lm

# Target program name
TARGET := DauphineLevelEditor
# Automatically build a list of object files
CPPFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
# Object files
export OFILES := $(addprefix $(BUILD)/,$(CPPFILES:.cpp=.o))
# 
CPPFILES := $(addprefix $(SOURCES)/,$(CPPFILES))
# Build a list of include paths
export INCLUDE := $(foreach dir,$(INCLUDES), -I$(CURDIR)/$(dir)) -I$(CURDIR)/$(BUILD)

$(BUILD)/%.o:$(SOURCES)/%.cpp
	@mkdir -p $(BUILD)
	@echo Building $^ ...
	@$(LD) $(CPPFLAGS) -c $^ -o $@ $(INCLUDE)

main:$(OFILES)
	@mkdir -p $(BUILD)
	@echo Linking necessary libraries...
	@$(LD) $(OFILES) $(CPPFLAGS) $(LDFLAGS) -o $(TARGET)
	@echo Built executable "$(TARGET)"

clean:
	@echo Cleaning...
	@rm -rf $(BUILD)/
	@rm -f $(TARGET)
	@rm -f $(TARGET).exe
	@rm -f *.dll
	@rm -f LICENSES.txt
