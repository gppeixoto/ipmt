#include "ipmt_backend.h"

struct SuffixArray
{

    char *T;
    int n;
    int *RA, *tempRA, *SA, *tempSA, *c;
    vi occs;

    SuffixArray(int size) {
        n = size;
        size *= 2; size += 400;
        T = new char[size];
        memset(T, 0, size);
        RA = new int[size];
        memset(RA, 0, size*sizeof(int));
        tempRA = new int[size];
        memset(tempRA, 0, size*sizeof(int));
        SA = new int[size];
        memset(SA, 0, size*sizeof(int));
        tempSA = new int[size];
        memset(tempSA, 0, size*sizeof(int));
        c = new int[size];
        memset(c, 0, size*sizeof(int));
    }

    void countingSort(int k) {
        int i, sum, maxi = max(300, n);
        memset(c, 0, n*sizeof(int));
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

    void debugSA() {
        for (int i=0; i < n; ++i)
            printf("%2d\t%s\n", SA[i], T + SA[i]);    
    }

    void dumpSA(string &filename) {
        ofstream ofs;
        ofs.open(filename);
        ofs << n << ' ';
        for (int i=0; i < n; ++i)
            ofs << SA[i] << ' ';
        ofs.close();
    }

    void index(string filename, string content) {
        filename += ".idx";
        strncpy(T, content.c_str(), content.size());
        for (int i=0; i < n; ++i) cout << T[i];
        cout << endl;
        constructSA();
        debugSA();
        // dumpSA(filename);
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
    int beginWindow;
    string window, buffer;
    unsigned long i = 0;
    while (i < str.size()) {
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

void index(string &txtfile){
    string fileContent = getFileContent(txtfile);
    SuffixArray sa = SuffixArray(fileContent.length());
    basename(txtfile);
    sa.index(txtfile, fileContent);
}