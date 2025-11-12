# === Project Settings ===
TARGET := snake
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2

# === macOS specific flags ===
# Определяем что это macOS
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    # Флаги для ncurses через Homebrew
    CXXFLAGS += -I/opt/homebrew/opt/ncurses/include
    LDFLAGS += -L/opt/homebrew/opt/ncurses/lib -lncurses
else
    # Флаги для Linux и других систем
    LDFLAGS += -lncurses
endif

# === Directory Structure ===
SRC_DIRS := domain input objects render utils
BUILD_DIR := build

# === Automatically collect all source files ===
SRCS := $(wildcard main.cpp $(addsuffix /*.cpp, $(SRC_DIRS)))
OBJS := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# === Choose commands depending on OS ===
ifeq ($(OS),Windows_NT)
	MKDIR = if not exist "$(subst /,\\,$(dir $@))" mkdir "$(subst /,\\,$(dir $@))"
	RM = rmdir /s /q
	CLEAN_TARGET = @if exist $(TARGET).exe del $(TARGET).exe && @if exist $(TARGET) del $(TARGET)
else
	MKDIR = mkdir -p $(dir $@)
	RM = rm -rf
	CLEAN_TARGET = @rm -f $(TARGET) $(TARGET).exe
endif

# === Default target ===
all: $(TARGET)

# === Linking stage ===
$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# === Compilation rule ===
$(BUILD_DIR)/%.o: %.cpp
	@$(MKDIR)
	@echo "Compiling $< ..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# === Cleanup ===
clean:
	@echo "Cleaning..."
	-$(RM) $(BUILD_DIR)
	$(CLEAN_TARGET)

# === Rebuild ===
rebuild: clean all

# === Run target ===
run: $(TARGET)
	@echo "Running $(TARGET)..."
	./$(TARGET)

# === Install dependencies (macOS) ===
install-deps:
ifeq ($(UNAME_S),Darwin)
	@echo "Installing dependencies for macOS..."
	@which brew > /dev/null || (echo "Homebrew not found. Installing..." && /bin/bash -c "$$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)")
	brew install ncurses
else
	@echo "Please install ncurses development package manually"
	@echo "Ubuntu/Debian: sudo apt-get install libncurses5-dev"
	@echo "CentOS/RHEL: sudo yum install ncurses-devel"
endif

# === Debug build ===
debug: CXXFLAGS += -g -DDEBUG -O0
debug: rebuild

# === Release build ===
release: CXXFLAGS += -O3 -DNDEBUG
release: rebuild

.PHONY: all clean rebuild run install-deps debug release