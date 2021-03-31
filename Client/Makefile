OBJS	= 	ClientApp.o\
		HomeController.o\
		InvalidCredentialsException.o\
		HomeView.o\
		ArgumentValidator.o\
		CLIAuthenticator.o\
		CommandBuilder.o\
		CommandParser.o\
		ClientCommunicationManager.o\
		ClientConsoleManager.o\
		ClientNotificationManager.o\
		MessageHandler.o\
		Notification.o\
		User.o\
		StringUtils.o
SOURCE	= 	ClientApp.cpp\
		src/controllers/HomeController.cpp\
		src/exceptions/InvalidCredentialsException.cpp\
		src/models/auth/CLIAuthenticator.cpp\
		src/models/command/CommandBuilder.cpp\
		src/models/command/CommandParser.cpp\
		src/models/data/ArgumentValidator.cpp\
		src/models/manager/ClientCommunicationManager.cpp\
		src/models/manager/ClientConsoleManager.cpp\
		src/models/manager/ClientNotificationManager.cpp\
		src/models/MessageHandler.cpp\
		src/models/Notification.cpp\
		src/models/User.cpp\
		src/views/HomeView.cpp\
HEADER	= 	../Utils/Types.h\
			../Utils/StringUtils.hpp 
OUT	= app_cliente.out
CC	 = g++ 
FLAGS	 = -g -c -Wall `wx-config --cflags` -Winvalid-pch -include include/wx_pch.h
LFLAGS	 =`wx-config --libs`

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS) -std=c++17

ClientApp.o: ClientApp.cpp
	$(CC) $(FLAGS) ClientApp.cpp -std=c++17

HomeController.o: src/controllers/HomeController.cpp
	$(CC) $(FLAGS) src/controllers/HomeController.cpp -std=c++17

InvalidCredentialsException.o: src/exceptions/InvalidCredentialsException.cpp
	$(CC) $(FLAGS) src/exceptions/InvalidCredentialsException.cpp -std=c++17

HomeView.o: src/views/HomeView.cpp
	$(CC) $(FLAGS) src/views/HomeView.cpp -std=c++17

ArgumentValidator.o: src/models/data/ArgumentValidator.cpp
	$(CC) $(FLAGS) src/models/data/ArgumentValidator.cpp -std=c++17

CLIAuthenticator.o: src/models/auth/CLIAuthenticator.cpp
	$(CC) $(FLAGS) src/models/auth/CLIAuthenticator.cpp -std=c++17

CommandBuilder.o: src/models/command/CommandBuilder.cpp
	$(CC) $(FLAGS) src/models/command/CommandBuilder.cpp -std=c++17

CommandParser.o: src/models/command/CommandParser.cpp
	$(CC) $(FLAGS) src/models/command/CommandParser.cpp -std=c++17

ClientCommunicationManager.o: src/services/ClientCommunicationManager.cpp
	$(CC) $(FLAGS) src/services/ClientCommunicationManager.cpp -std=c++17

ClientConsoleManager.o: src/models/manager/ClientConsoleManager.cpp
	$(CC) $(FLAGS) src/models/manager/ClientConsoleManager.cpp -std=c++17

ClientNotificationManager.o: src/models/manager/ClientNotificationManager.cpp
	$(CC) $(FLAGS) src/models/manager/ClientNotificationManager.cpp -std=c++17

MessageHandler.o: src/models/MessageHandler.cpp
	$(CC) $(FLAGS) src/models/MessageHandler.cpp -std=c++17

Notification.o: src/models/Notification.cpp
	$(CC) $(FLAGS) src/models/Notification.cpp -std=c++17

User.o: src/models/User.cpp
	$(CC) $(FLAGS) src/models/User.cpp -std=c++17

StringUtils.o: ../Utils/StringUtils.cpp
	$(CC) $(FLAGS) ../Utils/StringUtils.cpp -std=c++17

clean:
	rm -f $(OBJS) $(OUT)

run: $(OUT)
	./$(OUT) $(user) $(server) $(port) 
