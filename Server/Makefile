OBJS	= ServerApp.o\
		FileManager.o\
		ServerNotificationManager.o\
		ProfileSessionManager.o\
		TaskManager.o\
		ServerCommunicationManager.o\
		StringUtils.o\
		Logger.o\
		Scheduler.o\
		ReplicManager.o\
		Server.o\
		RingLeaderElection.o
SOURCE	= ServerApp.cpp\
		../Utils/StringUtils.cpp\
		../Utils/Logger.cpp\
		FileManager.cpp\
		src/ServerNotificationManager.cpp\
		src/ProfileSessionManager.cpp\
		src/TaskManager.cpp\
		src/ServerCommunicationManager.cpp\
		../Utils/Scheduler.cpp\
		src/ReplicManager.cpp
HEADER	= ../Utils/Types.h\
		../Utils/StringUtils.h\
		../Utils/Logger.h\
		include/FileManager.h\
		include/ServerNotificationManager.h\
		include/ProfileSessionManager.h\
		include/TaskManager.h\
		include/ServerCommunicationManager.h
OUT	= app_server.out
CC	 = g++
FLAGS	 = -g -c -w
LFLAGS	 = -pthread

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS) -std=c++17

ServerApp.o: ServerApp.cpp
	$(CC) $(FLAGS) ServerApp.cpp -std=c++17

StringUtils.o: ../Utils/StringUtils.cpp
	$(CC) $(FLAGS) ../Utils/StringUtils.cpp -std=c++17

Logger.o: ../Utils/Logger.cpp
	$(CC) $(FLAGS) ../Utils/Logger.cpp -std=c++17

FileManager.o: src/FileManager.cpp
	$(CC) $(FLAGS) src/FileManager.cpp -std=c++17

ServerNotificationManager.o: src/ServerNotificationManager.cpp
	$(CC) $(FLAGS) src/ServerNotificationManager.cpp -std=c++17

ProfileSessionManager.o: src/ProfileSessionManager.cpp
	$(CC) $(FLAGS) src/ProfileSessionManager.cpp -std=c++17

TaskManager.o: src/TaskManager.cpp
	$(CC) $(FLAGS) src/TaskManager.cpp -std=c++17

ServerCommunicationManager.o: src/ServerCommunicationManager.cpp
	$(CC) $(FLAGS) src/ServerCommunicationManager.cpp -std=c++17

Scheduler.o: ../Utils/Scheduler.cpp
	$(CC) $(FLAGS) ../Utils/Scheduler.cpp -std=c++17

ReplicManager.o: src/ReplicManager.cpp
	$(CC) $(FLAGS) src/ReplicManager.cpp -std=c++17

Server.o: src/Server.cpp
	$(CC) $(FLAGS) src/Server.cpp -std=c++17

RingLeaderElection.o: src/RingLeaderElection.cpp
	$(CC) $(FLAGS) src/RingLeaderElection.cpp -std=c++17

clean:
	rm -f $(OBJS) $(OUT) *.o

run: $(OUT)
	./$(OUT) $(mode)
