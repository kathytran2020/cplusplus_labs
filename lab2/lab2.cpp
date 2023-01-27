#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <memory>
#include <cstring>
#include <fstream>
#include <regex>
#include<sstream>
#include <deque>
using namespace std;
class Node {
    public:
    virtual float getFreq() = 0;
    virtual string getSymbol() = 0;
    virtual shared_ptr<Node> left() { return nullptr; }
    virtual shared_ptr<Node> right() { return nullptr; }

};

class Leaf : public Node {
    public:
    float freq;
    string symbol;
    //using polymorphism
    Leaf(string a, int s){
        this->freq = float(s);
        this->symbol = a;
    }
    Leaf(string symbol, float freq){
        this ->freq = freq;
        this->symbol = symbol;
    };
    float getFreq(){
        return freq;
    };
    string getSymbol(){
        return symbol;
    };
};

class Branch : public Node {
    public:
    shared_ptr<Node> lPtr;
    shared_ptr<Node> rPtr;
    Branch(){};
    Branch(shared_ptr<Node> left, shared_ptr<Node> right) {
        lPtr = left;
        rPtr = right;
    };
    float getFreq() {
        return lPtr->getFreq() + rPtr->getFreq();
    };
    string getSymbol() {
        return lPtr->getSymbol() + rPtr->getSymbol();
    };
    void setLeftPtr(shared_ptr<Node> ptr) {lPtr = ptr;}
    void setRightPtr(shared_ptr<Node> ptr) {rPtr = ptr;}
    // shared_ptr<Node> getLeftPtr() {return lPtr;}
    shared_ptr<Node> left() {return lPtr;}
    // shared_ptr<Node> getRightPtr() {return rPtr;}
    shared_ptr<Node> right() {return rPtr;}

};

class Priority_Queue
{
    vector<shared_ptr<Node>> vdata;
    public:
    Priority_Queue() { }
    shared_ptr<Node> top()
        {
        if (vdata.size() > 0)
        return vdata[0];
        return *vdata.end();
        }
    int size() { return vdata.size(); }
    bool empty() { return vdata.size() < 1; }
    void push(shared_ptr<Node> n)
    {
        vdata.push_back(n);
        stable_sort(vdata.begin(), vdata.end(),
        [](shared_ptr<Node> a, shared_ptr<Node> b)
        {
        // return a->symbol() < b->symbol();
        return a->getFreq() < b->getFreq();
        });
    }
    void pop()
    {
    vdata.erase(vdata.begin());
    }
    void print()
    {
    for_each(vdata.begin(), vdata.end(),
    [](shared_ptr<Node> n)
    {
    // cout << n->key() << '\t' << n->value() << endl;
    cout << n->getSymbol() << '\t' << n->getFreq() << endl;
    });
    }
};

//class to read the binary file
class BinFile {
    public:
    string readingFile(int numBer){
        string errorMsg = "not valid";
        return errorMsg;
    }
    string readingFile(string fname) {
        unsigned char character;
        string output;
        ifstream cBin(fname, ios::binary);

        if (!cBin) {
            cout << "Could not open file" << endl;
            return 0;
        };

        while (cBin.read(reinterpret_cast<char*>(&character), sizeof(unsigned char)))
        {
            std::bitset<8> bits(character);
            output += bits.to_string();
        }
        cBin.close();
        return output;
    }
};

//class to open the Asciifrequencies and use regex to parse the file
class RegexFile {
    public:
            deque<tuple<string,float>> processFile(string fileName) {
            string fileText;
            smatch matches;
            string ap;
            float fq;
            tuple<string,float>holdAscii;
            deque<tuple<string,float>> myContainer;
            const regex charReg("'\\\\?(.)'");
            const regex freqReg("([0-9]\\.[0-9]*)");
            ifstream file1(fileName);
            if (!file1) {
                cout << "Could not open file" << endl;
            }
            while (getline(file1, fileText)){
                if (regex_search(fileText, matches, charReg)) {
                    ap = matches[0];
                    ap.erase(remove(ap.begin(), ap.end(), '\''), ap.end());
                }
                if (regex_search(fileText, matches, freqReg)) {
                    string fs = matches[1];
                    fq = stof(fs);
                }
                holdAscii = make_tuple(ap,fq);
                myContainer.push_back(holdAscii);
            }
            file1.close();
            return myContainer;
        }
};

//creates tree and walksthrough for the answer
class PriorityTree {
    public:
    void createTree(Priority_Queue *fq) {
        while (fq->size() > 1) {
        shared_ptr<Node> qLeft = fq->top();
        fq->pop();
        shared_ptr<Node> qRight = fq->top();
        fq->pop();
        shared_ptr<Branch> node(new Branch(qLeft, qRight));
        fq->push(node);
        }
    }

    string walkThrough(Priority_Queue *tree, string binNum){
        createTree(tree);
        shared_ptr<Node> ptr = tree->top();
        string solution = "";
        for (int i = 0; i < binNum.length(); i++){
            if (binNum[i] == '0'){
                ptr = ptr->left();
            }
            else if(binNum[i] == '1'){
                ptr = ptr->right();
            }
            if (ptr->getSymbol().size() == 1){
                solution+=ptr->getSymbol();
                ptr = tree->top();
            }
        }
        return solution;
    }
};

int main() {
    //sets up the frequencies using regrex
    string name = "AsciiFrequenciesV3.txt";
    RegexFile tester;
    deque<tuple<string,float>>starter;
    starter = tester.processFile(name);
    
    //sets up the priority queue and pushes leaf nodes
    Priority_Queue qtree;

    for (int i = 0; i < starter.size(); i++){
        shared_ptr<Node> store = make_shared<Leaf>(get<0>(starter[i]), get<1>(starter[i]));
        qtree.push(store);
    }

    //reads compress.bin and stores binary into a string
    string cName = "Compress.bin";
    BinFile comp;
    string binaryData;
    binaryData = comp.readingFile(cName);

    //set up the tree and process data
    PriorityTree makeTree;
    string finalAnswer;

    //walk through the tree and get solution, then prints solution to console
    finalAnswer = makeTree.walkThrough(&qtree, binaryData);
    cout << finalAnswer << endl;

    //creates an english readable text file
    ofstream out("outputFile.txt"); 
    if(!out)
    {
        cout <<"Cannot open the output file."<<endl;
        return 1;
    }
    out<< finalAnswer << endl;
    out.close();
    return 0;
}