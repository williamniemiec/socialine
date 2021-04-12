OBJS	= ServerApp.o\
		StringUtils.o\
		FileManager.o\
		ServerNotificationManager.o\
		ProfileSessionManager.o\
		TaskManager.o\
		ServerCommunicationManager.o
SOURCE	= ServerApp.cpp\
		../Utils/StringUtils.cpp\
		FileManager.cpp\
		src/ServerNotificationManager.cpp\
		src/ProfileSessionManager.cpp\
		src/TaskManager.cpp\
		src/ServerCommunicationManager.cpp
HEADER	= ../Utils/Types.h\
		../Utils/StringUtils.h\
		include/FileManager.h\
		include/ServerNotificationManager.h\
		include/ProfileSessionManager.h\
		include/TaskManager.h\
		include/ServerCommunicationManager.h
OBJ_WITHOUT_SOURCE 	= ../Utils/wniemiec/io/consolex/Consolex.o\
					../Utils/wniemiec/io/consolex/HistoryConsolex.o\
					../Utils/wniemiec/io/consolex/InputConsolex.o\
					../Utils/wniemiec/io/consolex/LoggerConsolex.o\
					../Utils/wniemiec/io/consolex/LogLevel.o\
					../Utils/wniemiec/io/consolex/LogMessage.o\
					../Utils/wniemiec/io/consolex/OutputConsolex.o\
					../Utils/wniemiec/util/data/StringUtils.o
OUT	= app_server.out
CC	 = g++
FLAGS	 = -g -c -Wno-deprecated-declarations
LFLAGS	 = -pthread

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(OBJ_WITHOUT_SOURCE) $(LFLAGS) -std=c++17

ServerApp.o: ServerApp.cpp
	$(CC) $(FLAGS) ServerApp.cpp -std=c++17

StringUtils.o: ../Utils/StringUtils.cpp
	$(CC) $(FLAGS) ../Utils/StringUtils.cpp -std=c++17

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

clean:
	rm -f $(OBJS) $(OUT)

run: $(OUT)
	./$(OUT) $(debug) 
