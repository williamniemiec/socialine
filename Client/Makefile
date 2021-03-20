OBJS	= ClientApp.o Interface.o ClientCommunicationManager.o ClientNotificationManager.o
SOURCE	= ClientApp.cpp src/Interface.cpp src/ClientCommunicationManager.cpp src/ClientNotificationManager.cpp
HEADER	= ../Utils/Types.h include/Interface.h include/ClientCommunicationManager.h include/ClientNotificationManager.h
OUT	= app_cliente.out
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 =

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

ClientApp.o: ClientApp.cpp
	$(CC) $(FLAGS) ClientApp.cpp -std=c++17

Interface.o: src/Interface.cpp
	$(CC) $(FLAGS) src/Interface.cpp -std=c++17

ClientCommunicationManager.o: src/ClientCommunicationManager.cpp
	$(CC) $(FLAGS) src/ClientCommunicationManager.cpp -std=c++17

ClientNotificationManager.o: src/ClientNotificationManager.cpp
	$(CC) $(FLAGS) src/ClientNotificationManager.cpp -std=c++17


clean:
	rm -f $(OBJS) $(OUT)


run: $(OUT)
	./$(OUT) $(p1) $(p2) $(p3) $(p4) $(p5)