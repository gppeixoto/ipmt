#include "Util.h"
#include <string>

using namespace std;

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

    string toString(){
    	string ret = "";
    	ret += to_string(pos) + " " + to_string(tam) + " ";
    	ret.push_back(c);
    	return ret;
    }
};

class LZ77{
public:
	string encode(string &str){
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
	    return tupleVecToString(ret);
	}

	string decode(string &str){
		// DEBUG(str);
		vector<LZTuple> vec = stringToTupleVec(str);
		string ret = "";
	    int pos;
	    for(LZTuple tuple : vec){
	        pos = ret.size() - tuple.pos - 1;
	        // DEBUG(pos);
	        // DEBUG(tuple.tam);
	        ret.append(ret.substr(pos < 0? 0 : pos, tuple.tam));
	        if(tuple.c == '&')break;
	        ret.append(1,tuple.c);
	    }
	    //DEBUG(ret);
	    return ret;
	}

private:
	string tupleVecToString(vector<LZTuple> &vec){
		ostringstream os;
		for (auto &tuple : vec) {
			os << tuple.toString() << " ";
		}
		return os.str();
	}

	vector<LZTuple> stringToTupleVec(string &str){
	    stringstream ss(str);
		vector<LZTuple> vec;
		string s;
		int pos, tam;
		char c;
	    while (ss >> s) {
	        pos = stoi(s);
	        ss >> s;
	        tam = stoi(s);
	        ss >> c;
	        if(c == '$'){
	            c = '\n';
	        }
	        DEBUG(pos);
	        DEBUG(tam);
	        DEBUG(c);
	        vec.push_back(LZTuple(pos,tam,c));
	    }
	    return vec;
	}
};