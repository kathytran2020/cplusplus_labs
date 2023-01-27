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
#include <cmath>
#include <algorithm>

using namespace std;

const int TWO = 2;

class RegexFile {
    public:
            deque<tuple<int,vector<double>>> processFile(string fileName) {
            string fileText;
            smatch matches;
            int oldYear = 1959;
            int ap;
            double fq;
            //using stl containers tuple, vector, deque
            tuple<int, vector<double>>yearInter;
            deque<tuple<int, vector<double>>> myContainer;
            vector<double> allInter;
            const regex dataReg("([0-9]{4}),[0-9]+,[0-9]+.[0-9]+,[-0-9]+.[0-9]+,([0-9]*.[0-9]*)");
            ifstream file1(fileName);
            if (!file1) {
                cout << "Could not open file" << endl;
            }
            while (getline(file1, fileText)){
                if (regex_search(fileText, matches, dataReg)) {
                    ap = stoi(matches[1]);
                    fq = stof(matches[2]);

                    if (ap != oldYear) {

                        yearInter = make_tuple(oldYear, allInter);
                        myContainer.push_back(yearInter);
                        allInter.clear();
                        oldYear = ap;
                    }
                    allInter.push_back(fq);

                }
            }

            yearInter = make_tuple(ap,allInter);
            myContainer.push_back(yearInter);
            file1.close();
            return myContainer;
        }
};

class LinearRegression {
    public:
    int n;
    LinearRegression(int n = 0): n(n){};
    //use of polymorphism
    int getN(){return n;}
    int getN(int num){
        n = num;
        return n;
    }

    double getConstantA(double sumY, double sumXY, int sumX, int sumXtwo, double cb) {
        double ansA;
        //using casting
        ansA = (sumY - cb*sumX)/static_cast<double>(n);

        return ansA;
    }

    double getConstantB(double sumY,double sumXY, int sumX, int sumXtwo){
        double ansB;
        ansB = ((n*sumXY)-(sumX*sumY))/((n*sumXtwo)-sumX*sumX);

        return ansB;
    }
};

int main() {

    RegexFile openCSV;
    deque<tuple<int,vector<double>>> carbonValues = openCSV.processFile("co2.csv");

    double sumY1;
    double sumXY1;
    double sumX1;
    double sumXSquared1 =0;
    double sumInter;
    vector<double>justAverages;


    for (auto eachTup: carbonValues) {
        for (auto eachVec: get<1>(eachTup)) {
            sumInter+=eachVec;
        }

        double avgInter = sumInter/double(get<1>(eachTup).size());
        justAverages.push_back(avgInter);
        sumY1+=avgInter;
        sumXY1+=avgInter*double(get<0>(eachTup));
        sumX1+=get<0>(eachTup);
        sumXSquared1 += (get<0>(eachTup)*get<0>(eachTup));

        sumInter = 0;
    };
  
    //y = 1.58035x + (-2787.98)
    //sum_y double 21675.368333333339 sum_xy double 43142192.008333318  sum_x_squared double 241342291 sum_x double 121329
    vector<int>carbonYears;
    double testerSum=0;
    for (auto tup: carbonValues){
        carbonYears.push_back(get<0>(tup));
        testerSum+=accumulate(get<1>(tup).begin(), get<1>(tup).end(), 0.000)/static_cast<double>(get<1>(tup).size());
    }

    // for_each(carbonYears.begin(), carbonYears.end(), [](int&num){static_cast<double>(num);});
    //using stl algorithms
    auto bigYear = *max_element(carbonYears.begin(), carbonYears.end());
    auto smallYear = *min_element(carbonYears.begin(), carbonYears.end());

    LinearRegression solveIt;
    solveIt.n = carbonValues.size();
    double b = solveIt.getConstantB(sumY1, sumXY1, sumX1, sumXSquared1);
    double a = solveIt.getConstantA(sumY1, sumXY1, sumX1, sumXSquared1, b);
    double startYear = smallYear;
    double endYear = bigYear;
    shared_ptr<double> ptr1 = make_shared<double>(startYear);
    // cout << *ptr1 << endl;


    // cout << startYear << endl;
    // cout << endYear << endl;
    // cout << sumY1 <<endl;
    cout << "a = "<< a <<endl;
    cout << "b = " << b << endl;
    cout << bigYear << endl;
    cout << smallYear << endl;
};