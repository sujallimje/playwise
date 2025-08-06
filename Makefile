# Makefile for PlayWise Music Engine
# Author: PlayWise Development Team
# Version: 1.0

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
DEBUG_FLAGS = -std=c++17 -Wall -Wextra -g -DDEBUG
PROFILE_FLAGS = -std=c++17 -Wall -Wextra -O2 -pg

# Directories
SRC_DIR = src
BUILD_DIR = build
TEST_DIR = tests
DOC_DIR = docs

# Source files
MAIN_SRC = src/playwise_engine.cpp
TEST_SRC = tests/test_playwise.cpp
BENCHMARK_SRC = benchmark_playwise.cpp

# Executables
MAIN_EXEC = $(BUILD_DIR)/playwise_engine
DEBUG_EXEC = $(BUILD_DIR)/playwise_engine_debug
TEST_EXEC = $(BUILD_DIR)/test_playwise
BENCHMARK_EXEC = $(BUILD_DIR)/benchmark_playwise
PROFILE_EXEC = $(BUILD_DIR)/playwise_engine_profile

# Default target
all: $(MAIN_EXEC)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Main executable
$(MAIN_EXEC): $(MAIN_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(MAIN_SRC) -o $(MAIN_EXEC)

# Debug build
debug: $(DEBUG_EXEC)

$(DEBUG_EXEC): $(MAIN_SRC) | $(BUILD_DIR)
	$(CXX) $(DEBUG_FLAGS) $(MAIN_SRC) -o $(DEBUG_EXEC)

# Profile build  
profile: $(PROFILE_EXEC)

$(PROFILE_EXEC): $(MAIN_SRC) | $(BUILD_DIR)
	$(CXX) $(PROFILE_FLAGS) $(MAIN_SRC) -o $(PROFILE_EXEC)

# Test executable
test: $(TEST_EXEC)

$(TEST_EXEC): $(TEST_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(TEST_SRC) -o $(TEST_EXEC)

# Benchmark executable
benchmark: $(BENCHMARK_EXEC)

$(BENCHMARK_EXEC): $(BENCHMARK_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(BENCHMARK_SRC) -o $(BENCHMARK_EXEC)

# Run the main program
run: $(MAIN_EXEC)
	./$(MAIN_EXEC)

# Run with debug
run-debug: $(DEBUG_EXEC)
	./$(DEBUG_EXEC)

# Run tests
run-test: $(TEST_EXEC)
	./$(TEST_EXEC)

# Run benchmarks
run-benchmark: $(BENCHMARK_EXEC)
	./$(BENCHMARK_EXEC)

# Memory check with valgrind
memcheck: $(DEBUG_EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(DEBUG_EXEC)

# Generate gprof profile
gen-profile: $(PROFILE_EXEC)
	./$(PROFILE_EXEC)
	gprof $(PROFILE_EXEC) gmon.out > profile_analysis.txt

# Clean build files
clean:
	rm -rf $(BUILD_DIR)
	rm -f gmon.out profile_analysis.txt

# Generate documentation (requires Doxygen)
docs:
	doxygen Doxyfile

# Code formatting (requires clang-format)
format:
	clang-format -i *.cpp *.h

# Static analysis (requires cppcheck)
analyze:
	cppcheck --enable=all --std=c++17 *.cpp

# Install (copy to /usr/local/bin - requires sudo)
install: $(MAIN_EXEC)
	sudo cp $(MAIN_EXEC) /usr/local/bin/playwise

# Uninstall
uninstall:
	sudo rm -f /usr/local/bin/playwise

# Package for distribution
package: clean all
	tar -czf playwise_engine_v1.0.tar.gz *.cpp *.h Makefile README.md docs/

# Show help
help:
	@echo "PlayWise Music Engine Build System"
	@echo "=================================="
	@echo "Available targets:"
	@echo "  all         - Build main executable (default)"
	@echo "  debug       - Build with debug symbols"
	@echo "  profile     - Build with profiling support"
	@echo "  test        - Build test executable"
	@echo "  benchmark   - Build benchmark executable"
	@echo "  run         - Build and run main program"
	@echo "  run-debug   - Build and run debug version"
	@echo "  run-test    - Build and run tests"
	@echo "  run-benchmark - Build and run benchmarks"
	@echo "  memcheck    - Run with valgrind memory checker"
	@echo "  gen-profile - Generate gprof performance profile"
	@echo "  clean       - Remove build files"
	@echo "  docs        - Generate documentation"
	@echo "  format      - Format code with clang-format"
	@echo "  analyze     - Run static code analysis"
	@echo "  install     - Install to system (requires sudo)"
	@echo "  uninstall   - Remove from system"
	@echo "  package     - Create distribution package"
	@echo "  help        - Show this help message"

# Phony targets
.PHONY: all debug profile test benchmark run run-debug run-test run-benchmark memcheck gen-profile clean docs format analyze install uninstall package help