FLAGS=-Wall -std=c++11 -I./src/socklib

all: server

server: src/main.cpp obj/request.o obj/server.o obj/socklib.o obj/readerwriter.o obj/bufferedreaderwriter.o
	g++ $(FLAGS) obj/request.o obj/bufferedreaderwriter.o obj/server.o obj/socklib.o obj/readerwriter.o src/main.cpp -o bin/server

obj/server.o: src/server.h src/server.cpp
	g++ $(FLAGS) -c src/server.cpp -o obj/server.o

obj/request.o: src/request.h src/request.cpp
	g++ $(FLAGS) -c src/request.cpp -o obj/request.o

obj/socklib.o: src/socklib/socklib.hpp src/socklib/socklib.cpp
	g++ $(FLAGS) -c src/socklib/socklib.cpp -o obj/socklib.o

obj/readerwriter.o: src/socklib/readerwriter.hpp src/socklib/readerwriter.cpp
	g++ $(FLAGS) -c src/socklib/readerwriter.cpp -o obj/readerwriter.o

obj/bufferedreaderwriter.o: src/socklib/bufferedreaderwriter.hpp src/socklib/bufferedreaderwriter.cpp
	g++ $(FLAGS) -c src/socklib/bufferedreaderwriter.cpp -o obj/bufferedreaderwriter.o

clean:
	rm obj/*.o