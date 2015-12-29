FLAGS=-std=c++11 -Wall -Wextra -O2

all: makedir ipmt clean

ipmt: ipmt.o ipmt_backend.o util.o LZ77.o suffixarray.o
	g++ $(FLAGS) ipmt.o ipmt_backend.o LZ77.o suffixarray.o util.o -o ../bin/ipmt

ipmt.o: ipmt.cpp
	g++ $(FLAGS) -c ipmt.cpp

ipmt_backend.o: ipmt_backend.cpp
	g++ $(FLAGS) -c ipmt_backend.cpp

L77.o: LZ77.cpp
	g++ $(FLAGS) -c LZ77.cpp

suffixarray.o: SuffixArray.cpp
	g++ $(FLAGS) -c SuffixArray.cpp

util.o: Util.cpp
	g++ $(FLAGS) -c Util.cpp

clean:
	rm -rf *o ipmt

makedir:
	mkdir -p ../bin/