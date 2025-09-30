CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS =

# Source files
SRCDIR = src
TESTDIR = test
BUILDDIR = build

# Directories
SRC_FILES = $(wildcard $(SRCDIR)/*.cpp)
TEST_FILES = $(wildcard $(TESTDIR)/*.cpp)

# Output
TARGET = tiny-grep
TEST_TARGET = test_runner

# Create build directories
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Main executable
$(TARGET): $(BUILDDIR) $(SRC_FILES)
	$(CXX) $(CXXFLAGS) -I$(SRCDIR) $(filter-out $(SRCDIR)/main.cpp,$(SRC_FILES)) $(SRCDIR)/main.cpp -o $(TARGET)

# Test runner
$(TEST_TARGET): $(BUILDDIR) $(SRC_FILES) $(TEST_FILES)
	$(CXX) $(CXXFLAGS) -I$(SRCDIR) $(filter-out $(SRCDIR)/main.cpp,$(SRC_FILES)) $(TEST_FILES) -o $(TEST_TARGET)

# Build both
all: $(TARGET) $(TEST_TARGET)

# Run tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Clean build artifacts
clean:
	rm -rf $(BUILDDIR) $(TARGET) $(TEST_TARGET)

# Install (optional)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

.PHONY: all test clean install