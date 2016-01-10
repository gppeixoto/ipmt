#include "ipmt_backend.h"
#include "Util.h"
#include "LZW.cpp"


SuffixArray load(string &idxfile, bool isTime) {
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
    vector<int> encodedText(sz);
    infile.read(reinterpret_cast<char*>(encodedText.data()), encodedText.size() * sizeof(int));
    clock_t c = clock();
    string decoded = lzw.decode(encodedText);
    c = clock() - c;
    if (isTime)
        printf("Decode time: %fs\n",((float)c)/CLOCKS_PER_SEC);
    strncpy(sa.T, decoded.c_str(), decoded.size());
    sa.n = decoded.size();
    return sa;
}

void dump(SuffixArray &sa, string &fileContent, string &filename, bool isTime){
    LZW lzw = LZW();
    filename += ".idx";
    ofstream outfile(filename, ios::out | ios::binary); 
    outfile << sa.n;
    outfile.write((char*)sa.SA, sa.n * sizeof(int));
    clock_t t = clock();
    vector<int> encoded = lzw.encode(fileContent);
    t = clock() - t;
    if (isTime)
        printf("Encode time: %fs\n",((float)t)/CLOCKS_PER_SEC);
    outfile << encoded.size();
    outfile.write(reinterpret_cast<char*>(encoded.data()), encoded.size() * sizeof(int));
    outfile.close();
}

void index(string &txtfile, bool isTime)
{
    string fileContent = getFileContent(txtfile);
    SuffixArray sa = SuffixArray(fileContent.size());
    txtfile = basename(txtfile);
    sa.index(txtfile, fileContent);
    dump(sa, fileContent, txtfile, isTime);
}


int search(string &idxfile, string &pat, bool silent, bool isTime)
{
    SuffixArray sa = load(idxfile, isTime);
    clock_t t = clock();
    pair<int,int> pos = sa.stringMatch(pat);
    t = clock();
    if (isTime)
        printf("Search time: %fs\n",((float)t)/CLOCKS_PER_SEC);
    if (pos.first == -1 || pos.second == -1)
        return -1;
    int num_occs = pos.second-pos.first+1;
    if (silent)
        return num_occs;
    int fst = pos.first, snd = pos.second;
    for (int i=fst; i <= snd; ++i)
        cout << sa.get_line_from_match(sa.SA[i]) << endl;
    return num_occs;
}