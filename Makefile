# Define variables for the compiler and flags
CXX = g++
CXXFLAGS = -Wall -Iinclude
LDFLAGS = -lws2_32

# Define the target executable
TARGET = main.exe

# Define source files and object files
SRCS = src/main.cpp src/server.cpp src/client.cpp
OBJS = $(SRCS:src/%.cpp=src/%.o)

# Default target to build all
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

# Compile source files to object files
src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to run the program
run: $(TARGET)
	$(TARGET)

# Clean up build artifacts
clean:
	del $(OBJS) $(TARGET)

# Phony targets
.PHONY: all run clean
