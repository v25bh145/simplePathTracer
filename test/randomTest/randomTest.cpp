//
// Created by v25bh on 2021/12/2.
//
#include <iostream>
//#include "../../src/sampling.h"
#include "../../src/pathTracer.h"

using namespace std;
using namespace pathTracer;

int main(int argc, char** argv) {
    RandomGenerator randomGenerator;
    //cout<<"generate 10 float number from 0 to 1"<<endl;
    //for(int i = 0; i < 10; ++i) {
    //    cout<<randomGenerator.uniform0To1()<<" ";
    //}

    //cout<<"generate 10 int number from 0 to 2"<<endl;
    //for(int i = 0; i < 10; ++i) {
    //    cout<<randomGenerator.uniformNToM(0, 2)<<" ";
    //}
    //cout << endl;

    cout << "shuffle" << endl;
    vector<int> arr = randomGenerator.shuffleN(67);
    for (int i = 0; i < arr.size(); ++i) {
        cout << arr[i] + 1 << endl;
    }

}