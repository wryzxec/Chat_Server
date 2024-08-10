all: server.exe client.exe

server.exe: src/server_main.cpp src/server.cpp include/server.h
	g++ -Wall -Iinclude -o server.exe src/server_main.cpp src/server.cpp -lws2_32 -mconsole

client.exe: src/client_main.cpp src/client.cpp include/client.h
	g++ -Wall -Iinclude -o client.exe src/client_main.cpp src/client.cpp -lws2_32 -mconsole

run-server: server.exe
	./server.exe

run-client: client.exe
	./client.exe

clean:
	del server.exe client.exe

.PHONY: all run-server run-client clean
