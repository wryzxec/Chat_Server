# Define paths
VCPKG_PATH = C:/vcpkg
NLOHMANN_JSON_PATH = $(VCPKG_PATH)/installed/x64-windows/include

# Compilation flags
CPPFLAGS = -Iinclude -I$(NLOHMANN_JSON_PATH)
CXX = g++
CXXFLAGS = -Wall $(CPPFLAGS)
LDFLAGS = -lws2_32 -mconsole

# Define targets
TARGETS = server.exe client.exe
OBJECTS = src/server.o src/client.o src/message.o

all: $(TARGETS)

server.exe: src/server_main.cpp src/server.cpp src/message.cpp
	$(CXX) $(CXXFLAGS) -o $@ src/server_main.cpp src/server.cpp src/message.cpp $(LDFLAGS)

client.exe: src/client_main.cpp src/client.cpp src/message.cpp
	$(CXX) $(CXXFLAGS) -o $@ src/client_main.cpp src/client.cpp src/message.cpp $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/server.o: src/server.cpp include/server.h
	$(CXX) $(CXXFLAGS) -c src/server.cpp -o src/server.o

src/client.o: src/client.cpp include/client.h
	$(CXX) $(CXXFLAGS) -c src/client.cpp -o src/client.o

src/message.o: src/message.cpp include/message.h
	$(CXX) $(CXXFLAGS) -c src/message.cpp -o src/message.o

run-server: server.exe
	./server.exe

run-client: client.exe
	./client.exe

clean:
	del /q $(TARGETS) src/*.o

.PHONY: all run-server run-client clean
