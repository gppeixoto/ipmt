#include "ipmt_backend.h"
#include "Util.h"
#include "LZW.cpp"
#include "LZ77.cpp"


SuffixArray load(string &idxfile) {
    LZW lzw = LZW();
    ifstream infile(idxfile);
    string input = "";
    if (!infile.good()) {
        cout << "Arquivo de padrão << " << idxfile << " >> inválido" << endl;
        exit(1);
    }
    string line, ret;
    getline(infile, line);
    ret = lzw.decode(line);
    vector<int> arr;
    stringToVector(ret, arr);
    getline(infile, line);
    string content; 
    content = lzw.decode(line);
    SuffixArray sa = SuffixArray((int)arr.size());
    for (int i = 0; i < (int) arr.size(); ++i){
        sa.SA[i] = arr[i];
    }
    strncpy(sa.T, content.c_str(), content.size());
    sa.n = content.size();
    return sa;
}

void dump(SuffixArray &sa, string &fileContent, string &filename){
    LZW lzw = LZW();
    filename += ".idx";
    ostringstream os;
    string str, ret;
    vectorToString(sa.SA, sa.n, str);
    ret = lzw.encode(str);
    os << ret;
    os << '\n';
    ret = lzw.encode(fileContent);
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
    sa.index(txtfile, fileContent);
    dump(sa, fileContent, txtfile);
}


int search(string &idxfile, string &pat, bool silent)
{
    SuffixArray sa = load(idxfile);
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