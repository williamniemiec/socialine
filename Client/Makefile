OBJS	= 	ClientApp.o\
		HomeController.o\
		HomeCLIController.o\
		InvalidCredentialsException.o\
		HomeView.o\
		HomeCLIView.o\
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
		src/controllers/HomeCLIController.cpp\
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
		src/views/HomeCLIView.cpp
HEADER	= 	../Utils/Types.h\
			../Utils/StringUtils.hpp
OBJ_WITHOUT_SOURCE 	= ../Utils/wniemiec/io/consolex/Consolex.o\
					../Utils/wniemiec/io/consolex/HistoryConsolex.o\
					../Utils/wniemiec/io/consolex/InputConsolex.o\
					../Utils/wniemiec/io/consolex/LoggerConsolex.o\
					../Utils/wniemiec/io/consolex/LogLevel.o\
					../Utils/wniemiec/io/consolex/LogMessage.o\
					../Utils/wniemiec/io/consolex/OutputConsolex.o\
					../Utils/wniemiec/util/data/StringUtils.o
						

OUT	= app_cliente.out
CC	 = g++ 
FLAGS	 = -g -c `wx-config --cflags` -Winvalid-pch -include include/wx_pch.h
LFLAGS	 =`wx-config --libs`

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(OBJ_WITHOUT_SOURCE) $(LFLAGS) -std=c++17

ClientApp.o: ClientApp.cpp
	$(CC) $(FLAGS) ClientApp.cpp -std=c++17

HomeController.o: src/controllers/HomeController.cpp
	$(CC) $(FLAGS) src/controllers/HomeController.cpp -std=c++17

HomeCLIController.o: src/controllers/HomeCLIController.cpp
	$(CC) $(FLAGS) src/controllers/HomeCLIController.cpp -std=c++17

InvalidCredentialsException.o: src/exceptions/InvalidCredentialsException.cpp
	$(CC) $(FLAGS) src/exceptions/InvalidCredentialsException.cpp -std=c++17

HomeView.o: src/views/HomeView.cpp
	$(CC) $(FLAGS) src/views/HomeView.cpp -std=c++17

HomeCLIView.o: src/views/HomeCLIView.cpp
	$(CC) $(FLAGS) src/views/HomeCLIView.cpp -std=c++17

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

<<<<<<< HEAD
wx-brew:
=======
wx-mac:
>>>>>>> 46ed9a2d937d10b2e636462a13ba84ecdd2e8b31
	brew install wxmac --dev --use-llvm

wx-ubuntu:
	sudo apt-add-repository universe
	sudo apt-get update
	sudo apt-get install libwxbase3.0-dev libwxgtk3.0-dev

run: $(OUT)
	./$(OUT) $(user) $(server) $(port) $(debug) 
