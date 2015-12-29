#ifndef _IPMT_BACKEND
#define _IPMT_BACKEND

#include "SuffixArray.cpp"

#define DEBUG(x) cout << #x << " = " << x << endl;

using namespace std;

typedef pair<int,int> ii;
typedef vector<string> vs;
typedef vector<int> vi;
typedef pair<string, int> psi;
typedef vector<psi> v_psi;
typedef vector<ii> vii;
typedef unsigned long long ull;

void index(string &txtfile);
int search(string &idxfile, string &pat, bool silent);

#endif