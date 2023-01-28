#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include <fstream>
#include <regex>
#include <map>
#include <queue>
#include <iterator>
#include <string>
#include <sstream>

using namespace std;

map <int, string> lookup;
mutex gMutex;


class ProduceDatabase {
    public:
            //finds matching character in lookup chart
            string makeShort(bitset<9> pbits) {
                string shorthand = "";
                if(lookup.find(pbits.to_ullong()) != lookup.end()) {
                    shorthand = lookup[pbits.to_ullong()];
                }
                return shorthand;
            }
            
            map<string, tuple<string, float>> processFile(string fileName) {

            string fileText;
            smatch matches;
            string ap;
            float fq;
            string abv;
            bitset<9> tempSet;
            map<string, tuple<string, float>> produce;
            const regex charReg(">([A-z!-\\/ ^-]*)<");
            const regex binReg(">([01]*)<");
            const regex decReg(">(\\d*\\.\\d*)<");

            ifstream file1(fileName);

            if (!file1) {
                cout << "Could not open file" << endl;
            }
            //parses xml using regex
            while (getline(file1, fileText)) {
                if (regex_search(fileText, matches, charReg)) {
                    ap = matches[1];
                }
                if (regex_search(fileText, matches, binReg)) {
                    string bs = matches[1];
                    abv = "";

                    //splits string of produce binary into bitset 9
                    for (int i = 0; i < bs.length(); i+=9) {
                        string temp = bs.substr(i, 9);
                        tempSet = bitset<9>(temp);
                        abv += makeShort(tempSet);
                    }
                }
                if (regex_search(fileText, matches, decReg)) {
                    string fs = matches[1];
                    fq = stof(fs);

                    tuple<string, float>proPrice(ap, fq);
                    produce[abv] = proPrice;
                }
            }

            file1.close();
            return produce;
        }

        tuple<string, float> getProduct(string &thing, map<string, tuple<string,float>> &dbase) {
            tuple<string, float> retreivedItem;
            if(dbase.find(thing) != dbase.end()) {
                retreivedItem = dbase[thing];
            }
            return retreivedItem;
        }
};

struct Carts {
    string cartName;
    vector<string> cartContents;
    Carts(){cartName = "";};
};

class CartManager {
    public:
    queue<Carts> cartQueuesMaker(string fileName) {
        queue<Carts>holder;
        const regex cartReg("(Cart\\d+)");
        const regex barReg("([A-z0-9]{15})");
        ifstream file2(fileName);
        string fileContents;
        smatch matches2;
        string cartId;
        string barcodeString;
        vector<string> bcode;

        if (!file2){
            cout << "Could not open csv file." << endl;
        };
        
        while(getline(file2, fileContents)) {
            if (regex_search(fileContents, matches2, cartReg)) {
                bcode.clear();
                cartId = matches2[1];
            }
            else if (regex_search(fileContents, matches2, barReg)) {
                auto words_begin = sregex_iterator(fileContents.begin(), fileContents.end(), barReg);
                auto words_end = sregex_iterator();
                for (sregex_iterator i = words_begin; i != words_end; ++i){
                    smatch match = *i;
                    string match_string = match.str();
                    barcodeString = match_string;
                    bcode.push_back(barcodeString);
                }
                Carts myCart;
                myCart.cartName = cartId;
                myCart.cartContents = bcode;
                holder.push(myCart);
            }
        };
        return holder;
    };
};

class Process {
    public:
    void receipt(queue<Carts>& qCarts) {
        ProduceDatabase findThing;
        map<string, tuple<string, float>> database = findThing.processFile("ProductPrice.xml");
        while (!qCarts.empty()){

            Carts placeholderCart = qCarts.front();
            string total;
            float totalCost = 0;
            cout << placeholderCart.cartName << endl;
            //mutex lock
            std::lock_guard<std::mutex> guard(gMutex);
            for (auto& c: placeholderCart.cartContents) {
                //decode the cart's barcodes
                string tmpItem = decodeStuff(c);
                tuple<string, float>gotStuff = findThing.getProduct(tmpItem, database);
                totalCost += get<1>(gotStuff);
                cout << get<0>(gotStuff) << " " << get<1>(gotStuff) << endl;
                
            }
            cout << "Total: "<< totalCost << "\n" << endl;
            qCarts.pop();
        }
    }

    string decodeStuff(string &hexbCode) {
        const int LENGTH = 3;
        string decoded = "";
        for (int index = 0; index < 5; index++) {
            string tempStr = hexbCode.substr(LENGTH*index, LENGTH);
            stringstream ss(tempStr);
            unsigned st;
            ss >> std::hex >> st;
            bitset<9>out(st);
            if(lookup.find(out.to_ullong()) != lookup.end()){
                decoded += lookup[out.to_ullong()]; 
            }
        }
        return decoded;
    }

};

int main() {
    lookup[0b010000101] = "-";
    lookup[0b010101000] = "$";
    lookup[0b000101010] = "%";
    lookup[0b010010100] = "*";
    lookup[0b110000100] = ".";
    lookup[0b010100010] = "/";
    lookup[0b010001010] = "+";
    lookup[0b000110100] = "0";
    lookup[0b100100001] = "1";
    lookup[0b001100001] = "2";
    lookup[0b101100000] = "3";
    lookup[0b000110001] = "4";
    lookup[0b100110000] = "5";
    lookup[0b001110000] = "6";
    lookup[0b000100101] = "7";
    lookup[0b100100100] = "8";
    lookup[0b001100100] = "9";
    lookup[0b100001001] = "A";
    lookup[0b001001001] = "B";
    lookup[0b101001000] = "C";
    lookup[0b000011001] = "D";
    lookup[0b100011000] = "E";
    lookup[0b001011000] = "F";
    lookup[0b000001101] = "G";
    lookup[0b100001100] = "H";
    lookup[0b001001100] = "I";
    lookup[0b000011100] = "J";
    lookup[0b100000011] = "K";
    lookup[0b001000011] = "L";
    lookup[0b101000010] = "M";
    lookup[0b000010011] = "N";
    lookup[0b100010010] = "O";
    lookup[0b001010010] = "P";
    lookup[0b000000111] = "Q";
    lookup[0b100000110] = "R";
    lookup[0b001000110] = "S";
    lookup[0b011000100] = "^";
    lookup[0b000010110] = "T";
    lookup[0b110000001] = "U";
    lookup[0b011000001] = "V";
    lookup[0b111000000] = "W";
    lookup[0b010010001] = "X";
    lookup[0b110010000] = "Y";
    lookup[0b011010000] = "Z";



    CartManager cartsFile;
    //vector of all 1000 carts
    queue<Carts>allCarts = cartsFile.cartQueuesMaker("Carts.csv");

    //makes a vector of queue<carts>
    vector<queue<Carts>> cartArray;
    for (auto i=0; i<10; i++)
    cartArray.push_back(queue<Carts>());


    for (auto& k : cartArray){
        while (k.size() < 100) {
            k.push(allCarts.front());
            allCarts.pop();
        }
    }

    Process processStuff;
    deque<thread> cartthreads;
    for (auto& m : cartArray) {
        cartthreads.push_back(thread(&Process::receipt, processStuff, ref(m)));
    }
    for_each(cartthreads.begin(), cartthreads.end(), [](thread& a) { a.join(); });

}
