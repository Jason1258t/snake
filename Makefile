# === Project Settings ===
TARGET := snake
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2

# === Directory Structure ===
SRC_DIRS := domain input objects render utils
BUILD_DIR := build

# === Automatically collect all source files ===
SRCS := $(wildcard main.cpp $(addsuffix /*.cpp, $(SRC_DIRS)))
OBJS := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# === Choose mkdir command depending on OS ===
ifeq ($(OS),Windows_NT)
	MKDIR = if not exist "$(subst /,\\,$(dir $@))" mkdir "$(subst /,\\,$(dir $@))"
	RM = rmdir /s /q
else
	MKDIR = mkdir -p $(dir $@)
	RM = rm -rf
endif

# === Default target ===
all: $(TARGET)

# === Linking stage ===
$(TARGET): $(OBJS)
	@echo Linking $(TARGET)...
	$(CXX) $(OBJS) -o $@

# === Compilation rule ===
$(BUILD_DIR)/%.o: %.cpp
	@$(MKDIR)
	@echo Compiling $< ...
	$(CXX) $(CXXFLAGS) -c $< -o $@

# === Cleanup ===
clean:
	@echo Cleaning...
	-$(RM) $(BUILD_DIR)
	@if exist $(TARGET).exe del $(TARGET).exe
	@if exist $(TARGET) del $(TARGET)

# === Rebuild ===
rebuild: clean all

.PHONY: all clean rebuild
