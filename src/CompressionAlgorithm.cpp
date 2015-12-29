#ifndef COMPRESSION_ALGORITHM
#define COMPRESSION_ALGORITHM
#include "Util.h"

using namespace std;

class CompressionAlgorithm{
public:
	virtual string encode(string &str) = 0;
	virtual string decode(string &str) = 0;
};

#endif