#ifndef _UTIL
#define _UTIL

#include <sstream>
#include <vector>
#include <queue>
#include <iostream>
#include <cstring>
#include <set>
#include <map>
#include <algorithm>
#include <cstdio> 
#include <getopt.h>
#include <stdlib.h>
#include <fstream>
#include <dirent.h>
#include <sys/types.h>
#include <assert.h>
#include <glob.h>
#include <string>

using namespace std;

#define ALPHABET_SIZE 256

string getFileContent(string &textFile);

string basename(string filename);

void vectorToString(vector<int> &v, string &ret);

void vectorToString(int* arr, int len, string &ret);

void stringToVector(string &encoded, vector<int> &ret);

#endif