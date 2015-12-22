#include "ipmt_backend.h"

struct SuffixArray
{
    char T[MAX_N];
    int n;
    int RA[MAX_N], tempRA[MAX_N], SA[MAX_N], tempSA[MAX_N], c[MAX_N];

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

    void run() {
        n = (int)strlen(gets(T));
        T[n++] = '$';
        constructSA();
        for (int i=0; i < n; ++i) printf("%2d\t%s\n", SA[i], T+SA[i]);
    }
};

struct LZTuple
{
    int pos;
    int tam;
    char c;
    LZTuple(int p, int t, char ch){
        pos = p;
        tam = t;
        c = ch;
    }
};

vector<LZTuple> lz77_encode(string &str){
    int window_size = 8;
    int buffer_size = 4;

    vector<LZTuple> ret;
    int i = 0, beginWindow;
    string window, buffer;
    while(i < str.size()){
        beginWindow = i - window_size;
        if(beginWindow < 0){
            beginWindow = 0;
        }
        window = str.substr(beginWindow, i - beginWindow);
        cout << "window: " << window << endl;
        buffer = str.substr(i, buffer_size);
        cout << "buffer: " << buffer << endl;
        LZTuple tuple = LZTuple(0,0,str[i]);
        for (int k = buffer.size(); k >= 0; --k)
        {
            int index = window.find(buffer.substr(0,k));
            cout << "index " << index << endl;
            if(index != -1){//found
                char literal = '&';
                if(i + k < str.size()){
                    literal = str[i+k];
                }
                tuple = LZTuple(window.size() - index, k, literal);
                break;
            }
        }
        i += tuple.tam + 1;
        ret.push_back(tuple);
    }
    return ret;
}

void printencode(string s){
    vector<LZTuple> vec = lz77_encode(s);

    for (auto tuple : vec)
    {
        cout << tuple.pos << " " << tuple.tam << " " << tuple.c << endl;
    }
}