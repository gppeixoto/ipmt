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