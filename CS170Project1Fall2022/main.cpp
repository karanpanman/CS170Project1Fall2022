//
//  main.cpp
//  CS170Project1Fall2022
//
//  Created by Karan Bhogal on 10/18/22.
//
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <queue>
#include <vector>
#include <array>

using namespace std;

int main(int argc, const char * argv[]) {
    int slidePuzzle [3][3] =
    {
        {1,2,3}, {4,5,6},{7,8,0}
    };
    
    vector<vector<int>> vectPuzzle;
    
    //The end goal state we are striving for
    vector<vector<int>> vectSolution
    {
        {1,2,3},
        {4,5,6},
        {7,8,0}
    };
    
    vectPuzzle.resize(3);
    for (unsigned i = 0; i < vectPuzzle.size(); ++i){
        vectPuzzle[i].resize(3);
    }
    
    cout << slidePuzzle[1][2] << endl;
    return 0;
}
