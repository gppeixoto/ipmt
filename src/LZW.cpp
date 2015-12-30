#include "Util.h"

using namespace std;

class LZW {
public:
	string encode(string &str){
		string ret;
		vector<int> encoded;
	    unordered_map<string,int> dictionary;
	    for (int i = 0; i < 256; i++){
	        dictionary[string(1, i)] = i;
	    }

	    string s = "";
	    char ch;

	    for ( int i = 0; i < (int) str.length(); i++){
	        ch = str[i];

	        string sch = s + ch;

	        if (dictionary.find(sch) != dictionary.end()){
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
	    return ret;
	}

	string decode(string &str){
		string ret;
		vector<int> encoded;
	    stringToVector(str, encoded);

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
	    return ret;
	}
};