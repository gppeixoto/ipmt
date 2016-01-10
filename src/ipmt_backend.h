#ifndef _IPMT_BACKEND
#define _IPMT_BACKEND

#include "SuffixArray.cpp"

using namespace std;

typedef pair<int,int> ii;
typedef vector<string> vs;
typedef vector<int> vi;
typedef pair<string, int> psi;
typedef vector<psi> v_psi;
typedef vector<ii> vii;
typedef unsigned long long ull;

void index(string &txtfile, bool isTime);
int search(string &idxfile, string &pat, bool silent, bool isTime);

#endif