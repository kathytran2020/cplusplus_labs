#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <tuple>

using namespace std;

const string rotor = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 .,;!?()-'\"";

class encryptedText {
    public:
        vector<int> numString;
        vector<int> decryptionIndex(string unconverted) {
            //make space for vector
            numString.reserve(unconverted.length());
            //iterate through ecrypt.txt's string to convert into rotor indexes
            for (int start = 0; start < unconverted.length(); start++) {
                numString.push_back(rotor.find(unconverted[start]));
            }
            return numString;
        };
};

class enigmaMachine {
    public:
        string solved;
        char whiteSpace = ' ';
        int counter;
        int rotor_len = rotor.size();
        tuple <string, int> decrypt(vector<int> &unsolved, int rotor1, int rotor2) {
            counter = 0;
            solved = "";
            //feeds unsolved indexes through the rotors
            for (int num = 0; num < unsolved.size(); num++) {
                solved += rotor[((unsolved[num]-rotor1-rotor2)%rotor_len+rotor_len)%rotor_len];
                rotor1++;
                if (rotor1%rotor_len==0){
                    rotor2++;
                };
                //counts the number of whitespaces in each decryption
                if(solved[num] == whiteSpace) {
                    counter+=1;
                };
            };
            return {solved, counter};
        };
};

class handleFile {
    public:
        string fileText;
        string processFile(string fileName) {
            ifstream file1(fileName);
            if (!file1) {
                cout << "Could not open file" << endl;
                return 0;
            };
            while (getline(file1, fileText)){
            };
            file1.close();
            return fileText;
        }
};

class printFinal {
    public:
        void decryptAndRotors(string sol, int r1, int r2) {
            cout << sol << endl;
            cout << "rotor 1 setting: " << r1 << endl;
            cout << "rotor 2 setting: " << r2 << endl;
        };
};

int main() {
    string name = "encrypt.txt";
    handleFile openFile;
    openFile.processFile(name);
    string message = openFile.fileText;
    
    encryptedText convertIt;
    vector<int> encryptedIndexes = convertIt.decryptionIndex(message);

    enigmaMachine solveIt;
    //finds the decryption set with largest amount of whitespaces
    int oldCount = 0;
    int savert1;
    int savert2;
    for (int i = 0; i < rotor.length(); i++){
        for(int j = 0; j < rotor.length(); j++){
            solveIt.decrypt(encryptedIndexes, i, j);
            if (solveIt.counter > oldCount) {
                oldCount = solveIt.counter;
                savert1 = i;
                savert2 = j;
            }            
        }
    }

    solveIt.decrypt(encryptedIndexes, savert1, savert2);
    printFinal finishOutput;
    finishOutput.decryptAndRotors(solveIt.solved, savert1, savert2);
    return 0;
};