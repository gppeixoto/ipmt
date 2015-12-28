#include "ipmt_backend.h"

struct SuffixArray
{
    char *T;
    int n;
    int *RA, *tempRA, *SA, *tempSA, *c;
    vi occs;
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

void vectorToString(vector<int> &v, string &ret){
    ostringstream os;
    
    for (vector<int>::iterator it = v.begin() ; it != v.end(); ++it){
        os << *it << ' ';
    }
    ret = os.str();
}

void vectorToString(int* arr, int len, string &ret){
    ostringstream os;

    for (int i = 0; i < len; ++i){
        os << arr[i] << ' ';
    }
    ret = os.str();
}

void stringToVector(string &encoded, vector<int> &ret){
    ret.clear();
    int val;
    stringstream ss(encoded);
    while ( ss >> val ) {
        ret.push_back(val);
    }
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

vector<LZTuple> lz77_encode(string &str)
{
    int window_size = 1024;
    int buffer_size = 128;

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

void lzw_encode(const string & text, string &ret)
{
    vector<int> encoded;
    map<string,int> dictionary;
    for (int i = 0; i < 256; i++){
        dictionary[string(1, i)] = i;
    }

    string s = "";
    char ch;

    for ( int i = 0; i < (int) text.length(); i++){
        ch = text[i];

        string sch = s + ch;

        if (dictionary.count(sch)){
            s = sch;
        }else{
            encoded.push_back(dictionary[s]);
            int size = dictionary.size() + 1;
            dictionary[sch] = size;

            s = ch;
        }
    }
    if (! s.empty()){
        encoded.push_back(dictionary[s]);
    }

    
    vectorToString(encoded, ret);

}

void lzw_decode(string &encoded_str, string &ret)
{
    vector<int> encoded;
    stringToVector(encoded_str, encoded);


    map<int, string> dictionary;
    for (int i = 0; i < ALPHABET_SIZE; i++){
        dictionary[i] = string(1, i);
    }

    int prevcode, currcode;
    string entry;
    string aux;

    prevcode = encoded[0];
    ret = dictionary[prevcode];
    aux = ret;
    for (int i = 1; i < (int) encoded.size(); i++){
        currcode = encoded[i];

        if (dictionary.count(currcode)){
            entry = dictionary[currcode];
        } else{
            entry = aux + aux[0];
        }
        ret += entry;

        int size = dictionary.size() + 1;
        dictionary[size] = aux + entry[0];

        prevcode = currcode;
        aux = entry;

    }
}


SuffixArray load(string &idxfile) {
    ifstream infile(idxfile);
    string input = "";
    if (!infile.good()) {
        cout << "Arquivo de padrão << " << idxfile << " >> inválido" << endl;
        exit(1);
    }
    string line, ret;
    getline(infile, line);
    lzw_decode(line, ret);
    vector<int> arr;
    stringToVector(ret, arr);
    getline(infile, line);
    string content; 
    lzw_decode(line, content);
    SuffixArray sa = SuffixArray((int)arr.size());
    for (int i = 0; i < (int) arr.size(); ++i){
        sa.SA[i] = arr[i];
    }
    strncpy(sa.T, content.c_str(), content.size());
    sa.n = content.size();
    return sa;
}

void dump(SuffixArray &sa, string &fileContent, string &filename){
    filename += ".idx";
    ostringstream os;
    string str, ret;
    vectorToString(sa.SA, sa.n, str);
    lzw_encode(str, ret);
    os << ret;
    os << '\n';
    lzw_encode(fileContent, ret);
    os << ret;
    os << '\n';
    ofstream ofs;
    ofs.open(filename, ofstream::trunc);
    ofs << os.str();
    ofs.close();
}

void index(string &txtfile)
{
    string fileContent = getFileContent(txtfile);
    SuffixArray sa = SuffixArray(fileContent.size());
    basename(txtfile);
    cout << "index" << endl;
    sa.index(txtfile, fileContent);
    cout << "dump" << endl;
    dump(sa, fileContent, txtfile);
}


int search(string &idxfile, string &pat, bool silent)
{
    cout << "load" <<endl;
    SuffixArray sa = load(idxfile);
    cout << "match" << endl;
    ii pos = sa.stringMatch(pat);
    if (pos.first == -1 || pos.second == -1)
        return -1; //not found
    int num_occs = pos.second-pos.first+1;
    if (silent)
        return num_occs;
    int fst = pos.first, snd = pos.second;
    for (int i=fst; i <= snd; ++i) {
        cout << sa.get_line_from_match(sa.SA[i]) << endl;
    }
    return num_occs;
}