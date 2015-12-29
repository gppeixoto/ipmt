#include <cstdio>
#include "Util.h"

using namespace std;

struct SuffixArray
{
    char *T;
    int n;
    int *RA, *tempRA, *SA, *tempSA, *c;
    vector<int> occs;
    int arraySize;

    SuffixArray(int sz) {
        arraySize = 2*sz + 400;
        RA = new int[arraySize];
        memset(RA,0, arraySize * sizeof(int));
        SA = new int[arraySize];
        memset(SA,0, arraySize * sizeof(int));
        tempRA = new int[arraySize];
        memset(tempRA,0, arraySize * sizeof(int));
        tempSA = new int[arraySize];
        memset(tempSA,0, arraySize * sizeof(int));
        c = new int[arraySize];
        memset(c,0, arraySize * sizeof(int));
        T = new char[arraySize];
        memset(T,0, arraySize);
    }

    ~SuffixArray() {
        delete[] T;
        delete[] RA;
        delete[] tempRA;
        delete[] SA;
        delete[] tempSA;
        delete[] c;
    }

    void countingSort(int k) {
        int i, sum, maxi = max(ALPHABET_SIZE, n);
        memset(c, 0, arraySize * sizeof(int));
        for (i=0; i < n; ++i)
            c[i + k < n ? RA[i + k] : 0]++;
        for (i = sum = 0; i < maxi; ++i) {
            int t = c[i]; c[i] = sum; sum += t;
        }
        for (i=0; i < n; ++i) //shuffle the suffix array if necessary
            tempSA[c[SA[i]+k < n ? RA[SA[i]+k] : 0]++] = SA[i];
        for (i=0; i < n; ++i)
            SA[i] = tempSA[i];
    }

    void constructSA() {
        int i, k, r;
        for (i=0; i < n; ++i) RA[i] = T[i];
        for (i=0; i < n; ++i) SA[i] = i;
        for (k=1; k < n; k <<= 1) {
            countingSort(k);
            countingSort(0);
            tempRA[SA[0]] = r = 0;
            for (i=1; i < n; ++i)
                tempRA[SA[i]] = 
                    (RA[SA[i]] == RA[SA[i-1]] && RA[SA[i]+k] == RA[SA[i-1]+k])?
                    r : ++r;
            for (i=0; i < n; ++i)
                RA[i] = tempRA[i];
            if (RA[SA[n-1]] == n-1) break;
        }
    }
    
    pair<int, int> stringMatch(string pat) {
        int lo = 0, hi = n-1, mid = lo, m = pat.length();
        char P[m];
        strncpy(P, pat.c_str(), sizeof(P));
        while (lo < hi) {
            mid = (lo + hi) / 2;
            int res = strncmp(T + SA[mid], P, m);
            if (res >= 0) hi = mid;
            else lo = mid + 1;
        }
        if (strncmp(T + SA[lo], P, m) != 0) return pair<int,int>(-1, -1); //not found 404
        pair<int,int> ans; ans.first = lo;
        lo = 0; hi = n-1; mid = lo;
        while (lo < hi) {
            mid = (lo + hi) / 2;
            int res = strncmp(T + SA[mid], P, m);
            if (res > 0) hi = mid;
            else lo = mid+1;
        }
        if (strncmp(T + SA[hi], P, m) != 0) --hi;
        ans.second = hi;
        return ans;
    }

    void index(string filename, string content){
        filename += ".idx";
        strncpy(T, content.c_str(), content.size());
        n = content.size();
        constructSA();
    }

    string get_line_from_match(int pos) {
        // cout << T + pos << endl;
        int beg = 0, end = n-1;
        for (int i=pos; i >= 0; --i) {
            if (T[i] == '$') {
                beg = i+1;
                break;
            }
        }
        for (int i=pos; i < n; ++i) {
            if (T[i] == '$') {
                end = i-1;
                break;
            }
        }
        string ret = "";
        for (int i=beg; i <= end; ++i) ret += T[i];
        return ret;
    }

    void debugSA(){
        for (int i=0; i < n; ++i)
            printf("%2d\t%s\n", SA[i], T + SA[i]);
    }
};