OBJS	= ServerApp.o\
		FileManager.o\
		ServerNotificationManager.o\
		ProfileSessionManager.o\
		TaskManager.o\
		ServerCommunicationManager.o\
		StringUtils.o\
		Consolex.o\
		HistoryConsolex.o\
		InputConsolex.o\
		LoggerConsolex.o\
		LogLevel.o\
		LogMessage.o\
		OutputConsolex.o
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
OUT	= app_server.out
CC	 = g++
FLAGS	 = -g -c -Wno-deprecated-declarations
LFLAGS	 = -pthread

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS) -std=c++17

ServerApp.o: ServerApp.cpp
	$(CC) $(FLAGS) ServerApp.cpp -std=c++17

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

Consolex.o: ../Utils/wniemiec/io/consolex/Consolex.cpp
	$(CC) $(FLAGS) ../Utils/wniemiec/io/consolex/Consolex.cpp -std=c++17

HistoryConsolex.o: ../Utils/wniemiec/io/consolex/HistoryConsolex.cpp
	$(CC) $(FLAGS) ../Utils/wniemiec/io/consolex/HistoryConsolex.cpp -std=c++17

InputConsolex.o: ../Utils/wniemiec/io/consolex/InputConsolex.cpp
	$(CC) $(FLAGS) ../Utils/wniemiec/io/consolex/InputConsolex.cpp -std=c++17

LoggerConsolex.o: ../Utils/wniemiec/io/consolex/LoggerConsolex.cpp
	$(CC) $(FLAGS) ../Utils/wniemiec/io/consolex/LoggerConsolex.cpp -std=c++17

LogLevel.o: ../Utils/wniemiec/io/consolex/LogLevel.cpp
	$(CC) $(FLAGS) ../Utils/wniemiec/io/consolex/LogLevel.cpp -std=c++17

LogMessage.o: ../Utils/wniemiec/io/consolex/LogMessage.cpp
	$(CC) $(FLAGS) ../Utils/wniemiec/io/consolex/LogMessage.cpp -std=c++17

OutputConsolex.o: ../Utils/wniemiec/io/consolex/OutputConsolex.cpp
	$(CC) $(FLAGS) ../Utils/wniemiec/io/consolex/OutputConsolex.cpp -std=c++17

StringUtils.o: ../Utils/wniemiec/util/data/StringUtils.cpp
	$(CC) $(FLAGS) ../Utils/wniemiec/util/data/StringUtils.cpp -std=c++17

clean:
	rm -f $(OBJS) $(OUT)

run: $(OUT)
	./$(OUT) $(debug) 
