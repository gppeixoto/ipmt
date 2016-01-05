#include "ipmt_backend.h"
#include "Util.h"
#include "LZW.cpp"


SuffixArray load(string &idxfile) {
    LZW lzw = LZW();
    ifstream infile(idxfile, ios::binary);
    if (!infile.good()) {
        cout << "Arquivo de padrão << " << idxfile << " >> inválido" << endl;
        exit(1);
    }
    int sz;
    infile >> sz;
    SuffixArray sa = SuffixArray(sz);
    infile.read((char*)sa.SA, sz * sizeof(int));
    infile >> sz;
    vector<BYTE> encodedText(sz);
    infile.read((char*) &encodedText[0], sz);
    string decoded = lzw.decode(encodedText);
    strncpy(sa.T, decoded.c_str(), decoded.size());
    sa.n = decoded.size();
    return sa;
}

void dump(SuffixArray &sa, string &fileContent, string &filename){
    LZW lzw = LZW();
    filename += ".idx";
    ofstream outfile(filename, ios::out | ios::binary); 
    outfile << sa.n;
    outfile.write((char*)sa.SA, sa.n * sizeof(int));
    vector<BYTE> encoded = lzw.encode(fileContent);
    outfile << encoded.size();
    outfile.write((char*)&encoded[0], encoded.size());
    outfile.close();
}

void index(string &txtfile)
{
    string fileContent = getFileContent(txtfile);
    SuffixArray sa = SuffixArray(fileContent.size());
    txtfile = basename(txtfile);
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