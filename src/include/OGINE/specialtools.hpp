#pragma once

#include "specialmath.hpp"
#include <random>

random_device rd;
mt19937 gen(rd());

int randint(int a, int b){
    uniform_int_distribution<> dist(a,b);
    return dist(gen);
}

float uniform(float a, float b){
    uniform_int_distribution<> dist(a,b);
    return dist(gen);    
}

float normFcolor(float c){
    return c/255;
}

vec3 normFcolor(vec3 c){
    return {
        c.x/255,
        c.y/255,
        c.z/255
    };
}

template<class vc>
int index(vector<vc> vecs, vc key){
    int ind = 0;
    for(vc vkey : vecs){
        if(vkey == key) return ind;
        ind++;
    }

    return -1;
}

template<class vc>
bool in(vector<vc> vecs, vc key){
    for(vc vkey : vecs){
        if(vkey == key) return true;
    }

    return false;
}

vector<string> split(string line, char delim = ' '){
    vector<string> elems{""};
    bool isSpace = true;
    for(char ch : line){
        if(isSpace && ch!=delim || !isSpace && ch!=delim){
            elems[elems.size()-1]+=ch;
        }
        isSpace = ch == delim;
        if (isSpace) elems.push_back("");
    }

    return {elems};
}

string join(vector<string> vs, char udel = ' '){
    string o;
    for(string s : vs){
        o+=s;
        o+=udel;
    }

    return {o};
}

template<class T>
void smartadd(vector<T> &vec, T ob){
    if(!in(vec, ob)) vec.push_back(ob);
}

int sindex(string s, char c){
    return s.find_first_of(c);
}

template<class kc, class vc>
class dict{
    public:
    vector<kc> keys;
    vector<vc> values;
    dict(){
        ;
    }

    int size(){
        return values.size();
    }

    void set(kc key, vc val){
        if(in(keys, key)){
            values.at(index(keys, key)) = val;
        } else {
            keys.push_back(key);
            values.push_back(val);
        }
    }

    vc get(kc key){
        return values[index(keys, key)];
    }

    vc *getPointerTo(kc key){
        return &values[index(keys, key)];
    }
};

template<class kc, class vc>
class paar{
    public:
    kc key;
    vc value;

    paar(kc k, vc v){
        key=k;
        value=v;
    }

    void setv(vc val){ value=val; }
    void setk(kc key_){ key=key_; }
    vc get(){ return value; }
    
    paar(){}
};