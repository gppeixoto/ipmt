#include "ipmt_backend.h"

struct SuffixArray
{
    char *T;
    int n;
    int *RA, *tempRA, *SA, *tempSA, *c;
    vi occs;
    int arraySize;

    SuffixArray(int sz){
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

    void dumpSA(string &filename) {
        ofstream ofs;
        ofs.open(filename);
        for (int i=0; i < n; ++i)
            ofs << SA[i] << endl;
        ofs.close();
    }

    void readSA(string &filename) {
        ifstream ifs(filename);
        if (!ifs.good()) cout << "Arquivo de texto nao foi indexado" << endl;
        string line;
        int i = 0;
        while (getline(ifs, line)) {
            SA[i] = stoi(line);
            ++i;
        }
    }

    void index(string filename, string content){
        filename += ".idx";
        strncpy(T, content.c_str(), content.size());
        n = content.size();
        constructSA();
        dumpSA(filename);
    }
    void print(){
        for (int i=0; i < n; ++i)
        printf("%2d\t%s\n", SA[i], T + SA[i]);
    }
};

void run_sarr_match(string &txtfile, string &pattern, bool silent) {
    // todo
}

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

void basename(string &filename){
    const size_t period_idx = filename.rfind('.');
    if (string::npos != period_idx){
        filename.erase(period_idx);
    }
}

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
        buffer = str.substr(i, buffer_size);
        LZTuple tuple = LZTuple(0,0,str[i]);
        for (int k = buffer.size(); k >= 0; --k)
        {
            int index = window.find(buffer.substr(0,k));
            if(index != -1){//found
                char literal = '&';
                if(i + k < str.size()){
                    literal = str[i+k];
                }
                tuple = LZTuple(window.size() - index-1, k, literal);
                break;
            }
        }
        i += tuple.tam + 1;
        ret.push_back(tuple);
    }
    return ret;
}

string lz77_decode(vector<LZTuple> vec){
    string ret = "";
    int pos;
    for(auto tuple : vec){
        pos = ret.size() - tuple.pos - 1;
        ret.append(ret.substr(pos, tuple.tam));
        if(tuple.c == '&')break;
        ret.append(1,tuple.c);
    }
    return ret;
}


string getFileContent(string &textFile)
{
    ifstream infile(textFile);
    string input = "";
    if (!infile.good()) {
        cout << "Arquivo de padrão << " << textFile << " >> inválido" << endl;
        exit(1);
    }
    string line;
    while ( getline( infile, line) ) {
        line += '\n';
        input += line;
    }
    return input.substr(0, input.length()-1);
}

void index(string txtfile){
    string fileContent = getFileContent(txtfile);
    SuffixArray sa = SuffixArray(fileContent.size());
    basename(txtfile);
    sa.index(txtfile, fileContent);
    sa.print();
}