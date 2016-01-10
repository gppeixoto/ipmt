#include "ipmt_backend.h"


void help()
{
    cout << 
        "Uso: ipmt (index|search) [options] [pattern] idxfile [idxfiles...]"
        << endl << endl;
    
    cout <<
        "-h, --help\t\tEste guia de ajuda"
        << endl;
    
    cout << 
        "-c, --count\t\tImprime apenas o numero total de ocorrencias"
        << endl;
    
    cout << 
        "-p, --pattern_file\tUtiliza todos os padroes em 'file' como entrada"
        << endl;
    
    cout <<
        "-t, --time\tImprime o tempo de execução"
        << endl;

    exit(0);
}

inline vector<string> getTextFiles(const string& pat)
{
    glob_t glob_result;
    glob(pat.c_str(),GLOB_TILDE,NULL,&glob_result);
    vector<string> ret;
    for(unsigned int i=0;i<glob_result.gl_pathc;++i) {
        ret.push_back(string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return ret;
}


int main(int argc, char** argv)
{
    bool isIndex = false;
    bool hasPatternFile = false;
    bool isCount = false    ;
    bool isTime = false;

    string patternFile;

    vector<string> patterns;
    vector<string> textfiles;

    if (!strcmp(argv[1], "index")) {
        isIndex = true;
    } else if (!strcmp(argv[1], "search")) {
        isIndex = false;
    } else {
        help();
        exit(1);
    }
    int c;
    int options = 1;
    while (1) {
        static struct option long_options[] =
        {
          {"help",          no_argument,       0, 'h'},
          {"pattern",       required_argument, 0, 'p'},
          {"count",         no_argument,       0, 'c'},
          {"time",          no_argument,       0, 't'},
          {0, 0, 0, 0}
        };
        
        int option_index = 0;

        c = getopt_long (argc, argv, "hp:ct", long_options, &option_index); 
        if (c == -1) break;

        switch (c) {
            case 'p':
                hasPatternFile = true;
                patternFile = optarg;
                options += 2;
                break;
            case 'c':
                isCount = true;
                options++;
                break;
            case 't':
                isTime = true;
                options++;
                break;
            case 'l':
                options++;
                break;
            case 'h':
            case '?':
            default:
                help();
                break;
        }
    }
    if (options >= argc || argc == 1) {
        help();
    }

    if (!isIndex) {//search
        if (!hasPatternFile) {
            options += 1;
            string pat(argv[options]);
            patterns.push_back(pat);
        } else {
            ifstream infile(patternFile);
            if (!infile.good()) {
                cout << "Arquivo de padrao <"
                     << patternFile 
                     << "> invalido" 
                     << endl;
            }
            string pat;
            while (infile >> pat) {
                patterns.push_back(pat);
            }
        }
    }

    for (int i = options+1; i < argc; i++) {
        string str = argv[i];
        vector<string> matches = getTextFiles(str);
        for(string &match : matches) {
            textfiles.push_back(match);
        }
    }

    if (isIndex) {
        for (string &file : textfiles) {
            clock_t t = clock();
            index(file, isTime);
            t = clock() - t;
            if (isTime)
                printf("Indexing time: %fs\n",((float)t)/CLOCKS_PER_SEC);
        }
    } else {
        for (string &file : textfiles) {
            for (string &pat : patterns) {
                clock_t t = clock();
                int num_occs = search(file, pat, isCount, isTime);
                t = clock() - t;
                if (isCount) cout << num_occs << endl;
            }
        }
    }
	
	return 0;
}
