#include "ipmt_backend.h"

struct SuffixArray
{
    char T[MAX_N];
    int n;
    int RA[MAX_N], tempRA[MAX_N], SA[MAX_N], tempSA[MAX_N], c[MAX_N];
    vi occs;

    void countingSort(int k) {
        int i, sum, maxi = max(300, n);
        memset(c, 0, sizeof c);
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
                tempRA[SA[i]] = (RA[SA[i]] == RA[SA[i-1]] && RA[SA[i]+k] == RA[SA[i-1]+k]) ? r : ++r;
            for (i=0; i < n; ++i)
                RA[i] = tempRA[i];
            if (RA[SA[n-1]] == n-1) break;
        }
    }

    ii stringMatch(string pat) {
        int lo = 0, hi = n-1, mid = lo, m = pat.length();
        char P[m];
        strncpy(P, pat.c_str(), sizeof(P));
        while (lo < hi) {
            mid = (lo + hi) / 2;
            int res = strncmp(T + SA[mid], P, m);
            if (res >= 0) hi = mid;
            else lo = mid + 1;
        }
        if (strncmp(T + SA[lo], P, m) != 0) return ii(-1, -1); //not found 404
        ii ans; ans.first = lo;
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

    void dumpSA() {
        ofstream ofs;
        ofs.open("sarr.txt");
        for (int i=0; i < n; ++i)
            ofs << SA[i] << endl;
        ofs.close();
    }

    void readSA() {
        ifstream ifs("sarr.txt");
        if (!ifs.good()) cout << "Arquivo de texto nao foi indexado" << endl;
        string line;
        int i = 0;
        while (getline(ifs, line)) {
            SA[i] = stoi(line);
            ++i;
        }
    }

    void run() {
        n = (int)strlen(gets(T));
        readSA();
        for (int i=0; i < n; ++i)
            printf("%2d\t%s\n", SA[i], T + SA[i]);
        string pat = "ad";
        ii pos = stringMatch(pat);
        if (pos.first != -1 && pos.second != -1) {
            printf("%s found, SA[%d..%d] of %s\n", pat.c_str(), pos.first, pos.second, T);
            printf("They are:\n");
            for (int i=pos.first; i <= pos.second; ++i){
                printf("    %s\n", T+SA[i]);
                occs.push_back(SA[i]);
            }
        } else {
            cout << ":(" << endl;
        }
    }
};

void run_sarr_match(string &txtfile, string &pattern, bool silent) {
    // todo
}

int main(){
    return 0;
}