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

    ~SuffixArray() {
        delete[] T;
        delete[] RA;
        delete[] tempRA;
        delete[] SA;
        delete[] tempSA;
        delete[] c;
    }

    void init_from_idx(string &idxfile) {
        ifstream idx(idxfile);
        if (!idx.good())
            cout << ".idx nao foi gerado corretamente" << endl;
        idx >> n;
        int sz = n + 100;
        SA = new int[sz];
        for (int i=0; i < n; ++i) idx >> SA[i];
        memset(SA, 0, sz*sizeof(int));
        T = new char[sz];
        memset(T, ' ', sz*sizeof(char));
        // load_text_from_idx(idxfile);
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
        filename += ".idx";
        ofstream ofs;
        ofs.open(filename, ofstream::trunc);
        ofs << n;
        ofs << ' ';
        for (int i=0; i < n; ++i){
            if (i != n-1) ofs << SA[i] << ' ';
            else ofs << SA[i];
        }
        ofs.close();
    }

    void index(string filename, string content){
        filename += ".idx";
        strncpy(T, content.c_str(), content.size());
        n = content.size();
        constructSA();
    }

    string get_line_from_match(int pos) {
        int from = 0, to = 0;
        for (int i=pos; i < n; ++i) {
            if (T[i] == '\n') {
                from = i;
                break;
            }
        }
        for (int i=pos; i >= 0; --i) {
            if (T[i] == '\n') {
                to = i;
                break;
            }
        }
        string ret = "";
        for (int i=from; i <= to; ++i) {
            ret += T[i];
        }
        return ret;
    }

    void debugSA(){
        for (int i=0; i < n; ++i)
            printf("%2d\t%s\n", SA[i], T + SA[i]);
    }
};

void run_sarr_match(string &txtfile, string &pattern, bool silent) {
    
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

vector<LZTuple> lz77_encode(string &str)
{
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

string lz77_decode(vector<LZTuple> vec)
{
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

SuffixArray load_idx(string &idxfile) {
    ifstream infile(idxfile);
    string input = "";
    if (!infile.good()) {
        cout << "Arquivo idx << " << idxfile << " >> inválido" << endl;
        exit(1);
    }
    string line;
    int count;
    infile >> count;
    SuffixArray sa = SuffixArray(count);
    sa.n = count;
    for (int i = 0; i < count; ++i) {
        infile >> sa.SA[i];
    }
    string s;
    int pos, tam;
    char c;
    vector<LZTuple> vec;
    while (infile >> s) {
        pos = stoi(s);
        infile >> s;
        tam = stoi(s);
        infile >> s;
        c = s[0];
        if(c == '&'){
            c = '\n';
        }
        vec.push_back(LZTuple(pos,tam,c));
    }
    s = lz77_decode(vec);
    strncpy(sa.T, s.c_str(), s.size());
    return sa;
}

void basename(string &filename)
{
    const size_t period_idx = filename.rfind('.');
    if (string::npos != period_idx){
        filename.erase(period_idx);
    }
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
        line += '$';
        input += line;
    }
    return input.substr(0, input.length()-1);
}

void dumpText(string &filename, string &txt){
    ofstream ofs;
    ofs.open(filename, ofstream::app);
    ofs << endl;
    vector<LZTuple> tuples = lz77_encode(txt);
    for(auto &tuple : tuples){
        ofs << tuple.pos << " " << tuple.tam << " " << tuple.c << endl;
    }
    ofs.close();
}

void index(string &txtfile)
{
    string fileContent = getFileContent(txtfile);
    SuffixArray sa = SuffixArray(fileContent.size());
    basename(txtfile);
    sa.index(txtfile, fileContent);
    sa.dumpSA(txtfile);
    dumpText(txtfile, fileContent);
}

void search(string &txtfile){
    SuffixArray sa = load_idx(txtfile);
    //search here
}