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
#include <map>

using namespace std;

struct Point {
    int x;
    int y;
    
};


Point findZeroPos ( vector<vector<int>> puzzle){
    Point pos;
    for (unsigned i = 0; i < puzzle.size(); i++){
        for (unsigned j = 0; j < puzzle[i].size(); ++j){
            if ( puzzle[i][j] == 0 ) {
                pos.x = i;
                pos.y = j;
                return pos;
            }
        }
    }
    return pos;
}

void printPuzzle(vector<vector<int>> puzzle){
    for (unsigned i = 0; i < puzzle.size(); i++){
        for (unsigned j = 0; j < puzzle[i].size(); ++j){
            cout << puzzle[i][j] << " ";
        }
        cout << endl;
    }
}

vector<vector<int>> moveUp(vector<vector<int>> puzzle, Point zero){
    puzzle[zero.x][zero.y] = puzzle[zero.x-1][zero.y];
    puzzle[zero.x-1][zero.y] = 0;
    zero.x = zero.x-1;
    return puzzle;
}

int main(int argc, const char * argv[]) {
    int slidePuzzle [3][3] =
    {
        {1,2,3}, {4,5,6},{7,8,0}
    };
    
    //Initialize Our own custom Puzzle as a 3x3 2-D Vector
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
    
    Point zeroPos;
    zeroPos = findZeroPos(vectSolution);
    
    //Print Vector Puzzles
    printPuzzle(vectSolution);
    cout << endl;
    printPuzzle(moveUp(vectSolution, zeroPos));
    
    cout << "X: " << zeroPos.x << " Y:" << zeroPos.y << endl;
    
    
    
    //cout << slidePuzzle[1][2] << endl;
    return 0;
}
