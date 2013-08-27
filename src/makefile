
all : Board.h Board.cpp Bot.h Bot.cpp main.cpp
	#g++ -g main.cpp Bot.cpp Board.cpp -Wall -o main
	g++ `pkg-config --cflags opencv` `pkg-config --libs opencv` -lX11 -g Board.cpp MouseClick.cpp ImageParse.cpp -o main -Wall
