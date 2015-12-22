#ifndef _IPMT_BACKEND
#define _IPMT_BACKEND

#include <string>

#include <vector>
#include <queue>
#include <iostream>
#include <cstring>
#include <set>
#include <algorithm>
#include <cstdio> 
#include <getopt.h>
#include <stdlib.h>
#include <fstream>
#include <dirent.h>
#include <sys/types.h>
#include <assert.h>

#include <glob.h>

#define FAIL 0
#define START 1
#define MIN_CHAR 32
#define MAX_CHAR 126
#define SIZE MAX_CHAR - MIN_CHAR
#define MAX_ST 30
#define CHECK_BIT(var,pos) ((var) & (1LL<<(pos)))
#define MAX_N 1010
#define ALPHABET_SIZE 256

using namespace std;

typedef vector<string> vs;
typedef vector<int> vi;
typedef pair<string, int> psi;
typedef vector<psi> v_psi;
typedef unsigned long long ull;

//vector<LZTuple> lz77_encode(string &str);

void printencode(string s);

#endif