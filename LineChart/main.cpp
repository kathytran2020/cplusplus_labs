/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
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
#include <memory>


QT_USE_NAMESPACE
using namespace std;


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
    //polymorphism
    int getN(){return n;}
    int getN(int num){
        n = num;
        return n;
    }
    double getConstantA(double sumY, int sumX, double cb) {
        double ansA;
        //casting int to double
        ansA = (sumY - cb*sumX)/static_cast<double>(n);

        return ansA;
    }

    double getConstantB(double sumY,double sumXY, int sumX, int sumXtwo){
        double ansB;
        ansB = ((n*sumXY)-(sumX*sumY))/((n*sumXtwo)-sumX*sumX);

        return ansB;
    }
};


class QTComponents {
    public:

};

class QTStuff {
    public:
    int draw(int argc, char *argv[]) {
            QApplication a(argc, argv);

        //![1]
            QLineSeries *series = new QLineSeries();
            QLineSeries *linReg = new QLineSeries();

        //![1]


            RegexFile openCSV;
            deque<tuple<int,vector<double>>> carbonValues = openCSV.processFile("co2.csv");


            double sumY1=0;
            double sumXY1=0;
            double sumX1 = 0;
            double sumXSquared1 = 0;
            double sumInter;

            //lambda to create plot
            auto p = [=](auto v1, auto v2) { series->append(v1, v2); };

            //advanced cpp iterator
            for (auto eachTup: carbonValues) {
                for (auto eachVec: get<1>(eachTup)) {
                    sumInter+=eachVec;
                }

                double avgInter = sumInter/double(get<1>(eachTup).size());

                p(get<0>(eachTup), avgInter);
                sumXY1+=avgInter*double(get<0>(eachTup));
                sumX1+=get<0>(eachTup);
                sumXSquared1 += (get<0>(eachTup)*get<0>(eachTup));

                sumInter = 0;
            };

            double testerSumY = 0;
            vector<int>carbonYears;
            //use of stl algorithm accumulate to get sum
            for (auto tup: carbonValues){
                carbonYears.push_back(get<0>(tup));
                testerSumY+=accumulate(get<1>(tup).begin(), get<1>(tup).end(), 0.000)/static_cast<double>(get<1>(tup).size());
            }
            sumY1 = testerSumY;
            //using stl algorithms max_element, min_element
            auto bigYear = *max_element(carbonYears.begin(), carbonYears.end());
            auto smallYear = *min_element(carbonYears.begin(), carbonYears.end());


            LinearRegression solveIt;
            solveIt.n = carbonValues.size();

            double myB = solveIt.getConstantB(sumY1, sumXY1, sumX1, sumXSquared1);
            double myA = solveIt.getConstantA(sumY1,sumX1, myB);
            double firstY = smallYear*myB + myA;
            double secondY = bigYear*myB + myA;
            //smart pointers
            shared_ptr<double> ptr1 = make_shared<double>(firstY);
            shared_ptr<double> ptr2 = make_shared<double>(secondY);


            auto g = [=](auto c1, auto c2) { linReg->append(c1, c2); };
            g(smallYear, *ptr1);
            g(bigYear, *ptr2);

        //![3]
            QChart *chart = new QChart();
            chart->legend()->hide();
            chart->addSeries(series);
            chart->addSeries(linReg);
            chart->createDefaultAxes();

            chart->setTitle("Years x Carbon Dioxide Emissions");


        //![3]

        //![4]
            QChartView *chartView = new QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);

            QValueAxis *axisX = new QValueAxis();
            axisX->setTitleText("Years");
            chartView->chart()->setAxisX(axisX, series);

            QValueAxis *axisY = new QValueAxis();
            axisY->setTitleText("Metric Ton");
            chartView->chart()->setAxisY(axisY, series);


        //![4]


        //![5]
            QMainWindow window;
            window.setCentralWidget(chartView);
            window.resize(400, 300);
            window.show();
        //![5]

            return a.exec();
    }
};

int main(int argc, char *argv[])
{
    QTStuff makeChart;
    makeChart.draw(argc,argv);

}
