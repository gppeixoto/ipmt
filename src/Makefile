FLAGS=-std=c++11 -Wall -Wextra -O2

all: makedir ipmt clean

ipmt: ipmt.o ipmt_backend.o
	g++ $(FLAGS) ipmt.o ipmt_backend.o -o ../bin/ipmt

ipmt.o: ipmt.cpp
	g++ $(FLAGS) -c ipmt.cpp

ipmt_backend.o: ipmt_backend.cpp
	g++ $(FLAGS) -c ipmt_backend.cpp

clean:
	rm -rf *o ipmt

makedir:
	mkdir -p ../bin/