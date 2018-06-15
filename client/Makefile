FLAGS = -Wall -std=c++11
FLAGS += -I./src/socklib
FLAGS += `pkg-config gtkmm-2.4 --cflags --libs` -lX11
FLAGS += -L./src/libb64-1.2/src -lb64 -I./src/libb64-1.2/include -Werror -pedantic -DBUFFERSIZE=16777216 
FLAGS += -lmagic

all: client

client: src/main.cpp obj/Form.o obj/Mail.o obj/socklib.o obj/readerwriter.o obj/bufferedreaderwriter.o
	g++ obj/Form.o obj/Mail.o obj/socklib.o obj/readerwriter.o obj/bufferedreaderwriter.o src/main.cpp $(FLAGS) -o bin/client

obj/Form.o: src/Form.h src/Form.cpp
	g++ $(FLAGS) -c src/Form.cpp -o obj/Form.o

obj/Mail.o: src/Mail.h src/Mail.cpp
	g++ $(FLAGS) -c src/Mail.cpp ./src/libb64-1.2/src/libb64.a -o obj/Mail.o
 
obj/socklib.o: src/socklib/socklib.hpp
	g++ $(FLAGS) -c src/socklib/socklib.cpp -o obj/socklib.o

obj/readerwriter.o: src/socklib/readerwriter.hpp
	g++ $(FLAGS) -c src/socklib/readerwriter.cpp -o obj/readerwriter.o

obj/bufferedreaderwriter.o: src/socklib/bufferedreaderwriter.hpp
	g++ $(FLAGS) -c src/socklib/bufferedreaderwriter.cpp -o obj/bufferedreaderwriter.o

clean:
	rm obj/*.o
