#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <tuple>
#include <bitset>
#include <sstream>
#include <algorithm>
#include <memory>

// use bitsets, binary operations, pointers, vectors, tuples, no maps
using namespace std;
string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890,?.!:'=\"";
string morseCode[44] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....",
"..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", 
"...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", ".----", "..---", 
"...--", "....-", ".....", "-....", "--...", "---..", "----.", "-----", 
 "--..--", "..--..", ".-.-.-", "..--.", "---...", ".----.", "-...-", ".-..-."};


class FileOpener {
    public:
    string fileData;
    string readingFile(string fname) {
        ifstream morseBin(fname, ios::binary);

        if (!morseBin) {
            cout << "Could not open file" << endl;
            return 0;
        };

        while (getline(morseBin, fileData)) {
        };

        morseBin.close();
        return fileData;
    }

};


class MorseAlgo {
    public:
    string decryptedString;
    string morseHold;
    string convToMorse;

    string toMorse(bitset<8>chrBits){
        morseHold = "";
        bitset<8>dash("00000001");
        bitset<8>dot("00000010");
        bitset<8>wrdSpace("00000011");
        bitset<8>lSpace("00000000");
        if (chrBits == dash) {
            morseHold+="-";
        }
        else if(chrBits == dot) {
            morseHold+=".";
        }
        else if(chrBits == wrdSpace) {
            morseHold+="#";
        }
        else if(chrBits == lSpace) {
            morseHold+="*";
        }

        return morseHold;
    }

    string Decrypt(string enStr){
        bitset<8>cp2("00000011");
        convToMorse = "";
    
        for (int chr = 0; chr < enStr.size(); chr++) {
            //creates bitset for each char in morse.bin
            bitset<8>tempBits(enStr[chr]);
            //bitset operations to shift bits
            convToMorse+=toMorse((tempBits>>6)&cp2);
            convToMorse+=toMorse((tempBits>>4)&cp2);
            convToMorse+=toMorse((tempBits>>2)&cp2);
            convToMorse+=toMorse((tempBits>>0)&cp2);
        }

        return convToMorse;

    }

};

class Printer{
    public:
    void printAnswer (string something){
        cout << something << endl;
    }

};

int main() {
    //opens morse.bin, reads data
    string fileName = "Morse.bin";
    FileOpener Morse;
    string binData = Morse.readingFile(fileName);

    //creates vector of tuples to store ASCII, morse, binary as a key
    vector< tuple <char, string, bitset<8> > > morseKey;
    char achar;
    string astr;
    bitset<8>abset;
    //insert values into vector of tuples
    for (int beg = 0; beg < alphabet.size(); beg++){
        achar = alphabet[beg];
        astr = morseCode[beg];
        abset = bitset<8>(alphabet[beg]);
        morseKey.push_back( make_tuple(achar, astr, abset) );
    };

    MorseAlgo midMorse;
    string helper = midMorse.Decrypt(binData);

    string doneAns;
    // split into strings deliminated by *, store in doneAns
    stringstream s_stream(helper);
    while(s_stream.good()) {
        string substr;
        getline(s_stream, substr, '*');
        for (int ec = 0; ec < morseKey.size(); ec++){
            if (substr[0] == '#'){
                doneAns+=' ';
                substr.erase(0,1);
            }
            if (substr == get<1>(morseKey[ec])){
                doneAns+=get<0>(morseKey[ec]);
            }
        }
    }

    //smart pointer to print answer
    std::unique_ptr<Printer> printing(new Printer());
    printing->printAnswer(doneAns);

    return 0;
}
