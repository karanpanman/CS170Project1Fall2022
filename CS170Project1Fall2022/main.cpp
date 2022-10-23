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

void PrintPuzzle(vector<vector<int>> printPuzzle){
    for (unsigned i = 0; i < printPuzzle.size(); i++){
        for (unsigned j = 0; j < printPuzzle.size(); ++j){
            cout << printPuzzle[i][j] << " ";
        }
        cout << endl;
    }
}

int main(int argc, const char * argv[]) {
    int slidePuzzle [3][3] =
    {
        {1,2,3}, {4,5,6},{7,8,0}
    };
    
    vector<vector<int>> vectPuzzle;
    vectPuzzle.resize(3);
    for (unsigned i = 0; i < vectPuzzle.size(); ++i){
        vectPuzzle[i].resize(3);
    }
    
    
    //The end goal state we are striving for
    vector<vector<int>> vectSolution
    {
        {1,2,3},
        {4,5,6},
        {7,8,0}
    };
    
    //Print Vector Puzzles
    PrintPuzzle(vectSolution);
    
    
    //cout << slidePuzzle[1][2] << endl;
    return 0;
}
