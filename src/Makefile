CPPFLAGS=-g -std=c++11 -Wall -Wextra -O2

all: makedir ipmt clean

ipmt: ipmt.o ipmt_backend.o Util.o LZ77.o LZW.o SuffixArray.o
	g++ $(CPPFLAGS) ipmt.o ipmt_backend.o Util.o LZ77.o LZW.o SuffixArray.o -o ../bin/ipmt

ipmt.o: ipmt.cpp
	g++ $(CPPFLAGS) -c ipmt.cpp

ipmt_backend.o: ipmt_backend.cpp
	g++ $(CPPFLAGS) -c ipmt_backend.cpp

L77.o: LZ77.cpp
	g++ $(CPPFLAGS) -c LZ77.cpp

SuffixArray.o: SuffixArray.cpp
	g++ $(CPPFLAGS) -c SuffixArray.cpp

Util.o: Util.cpp
	g++ $(CPPFLAGS) -c Util.cpp

LZW.o: LZW.cpp
	g++ $(CPPFLAGS) -c LZW.cpp

clean:
	rm -rf *o ipmt

makedir:
	mkdir -p ../bin/