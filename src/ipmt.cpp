#include "ipmt_backend.h"


void help()
{
    cout << "Uso: ipmt [options] [pattern] textfile [textfile...]" << endl << endl;
    cout << "-h, --help\t\tEste guia de ajuda" << endl;
    cout << "-s, --silent\t\tNao imprime os matches na tela - usado para testes" << endl;
    cout << "-e, --edit n\t\tFaz uma busca aproximada em uma distancia de edicao maxima 'n'" << endl;
    cout << "-p, --pattern file\tUtiliza todos os padroes em 'file' como entrada" << endl;

    exit(0);
}

inline vector<string> getTextFiles(const string& pat){
    glob_t glob_result;
    glob(pat.c_str(),GLOB_TILDE,NULL,&glob_result);
    vector<string> ret;
    for(unsigned int i=0;i<glob_result.gl_pathc;++i){
        ret.push_back(string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return ret;
}


int main(int argc, char** argv){
    bool isIndex = false;
    bool isSuffixArray = false;
    bool hasPatternFile = false;

    string patternFile;

    vector<string> patterns;
    vector<string> textfiles;

    if(!strcmp(argv[1], "index")){
        isIndex = true;
    }else if(!strcmp(argv[1], "search")){
        isIndex = false;
    }else{
        help();
        exit(1);
    }
    int c;
    int options = 1;
    while (1){
        static struct option long_options[] =
        {
          {"help",       no_argument,       0, 'h'},
          {"pattern",    required_argument, 0, 'p'},
          {"suffixarray", no_argument,      0, 's'},
          {0, 0, 0, 0}
        };
        
        int option_index = 0;

        c = getopt_long (argc, argv, "hp:s", long_options, &option_index); 

        if (c == -1) break;

        switch (c){
            case 'p':
                hasPatternFile = true;
                patternFile = optarg;
                options += 2;
                break;
            case 'a':
                isSuffixArray = true;
                options += 1;
                break;
            case 'h':
            case '?':
            default:
                help();
                break;
        }
    }
    if (options >= argc || argc == 1){
        help();
    }

    if(!isIndex){//search
        if(!hasPatternFile){
            options += 1;
            string pat(argv[options]);
            patterns.push_back(pat);
        }else{
            ifstream infile(patternFile);
            if (!infile.good()){
                cout << "Arquivo de padrão << " << patternFile << " >> inválido" << endl;
            }
            string pat;
            while (infile >> pat){
                patterns.push_back(pat);
            }
        }
    }

    for (int i = options+1; i < argc; i++){
        string str = argv[i];
        vector<string> matches = getTextFiles(str);
        for(string &match : matches){
            textfiles.push_back(match);
        }
    }

    if(isIndex){
        for (string &file : textfiles){
            index(file);
        }
    }else{
        for (string &file : textfiles){
            search(file);
        }
    }
	
	return 0;
}
