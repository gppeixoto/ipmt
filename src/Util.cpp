#include "Util.h"

using namespace std;

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

string basename(string filename)
{
    const size_t period_idx = filename.rfind('.');
    if (string::npos != period_idx){
        filename.erase(period_idx);
    }
    return filename;
}

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