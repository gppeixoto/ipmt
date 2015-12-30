#include "ipmt_backend.h"
#include "Util.h"
#include "LZW.cpp"
#include "LZ77.cpp"


SuffixArray load(string &idxfile, bool is77) {
    LZ77 lz77 = LZ77();
    LZW lzw = LZW();
    ifstream infile(idxfile);
    string input = "";
    if (!infile.good()) {
        cout << "Arquivo de padrão << " << idxfile << " >> inválido" << endl;
        exit(1);
    }
    string line, ret;
    getline(infile, line);
    if(is77)
        ret = lz77.decode(line);
    else
        ret = lzw.decode(line);
    vector<int> arr;
    stringToVector(ret, arr);
    getline(infile, line);
    string content;
    if(is77)
        content = lz77.decode(line);
    else
        content = lzw.decode(line);
    SuffixArray sa = SuffixArray((int)arr.size());
    for (int i = 0; i < (int) arr.size(); ++i){
        sa.SA[i] = arr[i];
    }
    strncpy(sa.T, content.c_str(), content.size());
    sa.n = content.size();
    return sa;
}

void dump(SuffixArray &sa, string &fileContent, string &filename, bool is77){
    LZ77 lz77 = LZ77();
    LZW lzw = LZW();
    filename += ".idx";
    ostringstream os;
    string str, ret;
    vectorToString(sa.SA, sa.n, str);
    if(is77)
        ret = lz77.encode(str);
    else
        ret = lzw.encode(str);
    os << ret;
    os << '\n';
    if(is77)
        ret = lz77.encode(fileContent);
    else
        ret = lzw.encode(fileContent);
    os << ret;
    os << '\n';
    ofstream ofs;
    ofs.open(filename, ofstream::trunc);
    ofs << os.str();
    ofs.close();
}

void index(string &txtfile, bool is77)
{
    string fileContent = getFileContent(txtfile);
    SuffixArray sa = SuffixArray(fileContent.size());
    txtfile = basename(txtfile);
    sa.index(txtfile, fileContent);
    dump(sa, fileContent, txtfile, is77);
}


int search(string &idxfile, string &pat, bool silent, bool is77)
{
    SuffixArray sa = load(idxfile, is77);
    pair<int,int> pos = sa.stringMatch(pat);
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