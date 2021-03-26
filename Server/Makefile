OBJS	= ServerApp.o FileManager.o ProfileSessionManager.o TaskManager.o ServerCommunicationManager.o ServerNotificationManager.o
SOURCE	= ServerApp.cpp FileManager.cpp src/ProfileSessionManager.cpp src/TaskManager.cpp src/ServerCommunicationManager.cpp src/ServerNotificationManager.cpp
HEADER	= ../Utils/Types.h include/FileManager.h include/ProfileSessionManager.h include/TaskManager.h include/ServerCommunicationManager.h include/ServerNotificationManager.h
OUT	= app_cliente.out
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 =

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

ServerApp.o: ServerApp.cpp
	$(CC) $(FLAGS) ServerApp.cpp -std=c++17

FileManager.o: src/FileManager.cpp
	$(CC) $(FLAGS) src/FileManager.cpp -std=c++17

ProfileSessionManager.o: src/ProfileSessionManager.cpp
	$(CC) $(FLAGS) src/ProfileSessionManager.cpp -std=c++17

TaskManager.o: src/TaskManager.cpp
	$(CC) $(FLAGS) src/TaskManager.cpp -std=c++17

ServerCommunicationManager.o: src/ServerCommunicationManager.cpp
	$(CC) $(FLAGS) src/ServerCommunicationManager.cpp -std=c++17

ServerNotificationManager.o: src/ServerNotificationManager.cpp
	$(CC) $(FLAGS) src/ServerNotificationManager.cpp -std=c++17


clean:
	rm -f $(OBJS) $(OUT)


run: $(OUT)
	./$(OUT)