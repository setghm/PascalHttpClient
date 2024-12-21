.PHONY: all clean

BUILD_DIR = build
EXAMPLES_BUILD_DIR = $(BUILD_DIR)\examples
BUILD_DIRS = $(BUILD_DIR) $(EXAMPLES_BUILD_DIR)

UNIT_DIR = src
UNIT_SOURCES = $(UNIT_DIR)/PascalWeb.pp $(wildcard $(UNIT_DIR)/*.inc)
UNIT_TARGET = $(BUILD_DIR)/PascalWeb.ppu

EXAMPLES_DIR = examples
EXAMPLES_SOURCES = $(wildcard $(EXAMPLES_DIR)/*.pas)
EXAMPLES_TARGETS = $(patsubst $(EXAMPLES_DIR)/%.pas, $(BUILD_DIR)/%, $(EXAMPLES_SOURCES))

FPC = fpc

all: $(UNIT_TARGET) $(EXAMPLES_TARGETS)

# Create the build directories.
$(BUILD_DIRS):
	mkdir $(BUILD_DIR)
	mkdir $(EXAMPLES_BUILD_DIR)

# Compile the PascalWeb unit and generates two files: .o and .ppu
$(UNIT_TARGET): $(UNIT_SOURCES) | $(BUILD_DIRS)
	$(FPC) $< -FE$(BUILD_DIR)

# Compile all the examples in the examples directory.
$(BUILD_DIR)/%: $(EXAMPLES_DIR)/%.pas $(UNIT_TARGET) | $(BUILD_DIRS)
	$(FPC) -FE$(EXAMPLES_BUILD_DIR) $<

clean:
	del /Q $(BUILD_DIR)\*
	rmdir /S /Q $(BUILD_DIR)
