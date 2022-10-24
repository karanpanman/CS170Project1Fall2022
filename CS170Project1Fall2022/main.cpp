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

//Used to track the 0/Blank position in the vector
struct Point {
    int x;
    int y;
    
};

struct node {
    
    vector<vector<int>> STATE;
    
};

struct QUEUEING_FUNCTION {
    
};

vector<vector<int>> puzzleSolution
{
    {1,2,3},
    {4,5,6},
    {7,8,0}
};

struct Problem{
    vector<vector<int>> INITIALSTATE;
    
    bool GOALTEST(vector<vector<int>> puzzle){
        for (unsigned i = 0; i < puzzle.size(); i++){
            for (unsigned j = 0; j < puzzle[i].size(); ++j){
                if ( puzzle[i][j] != puzzleSolution[i][j] ) { return false; }
            }
        }
        return true;
    }
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

//Operators: moveUp, moveDown, moveLeft, moveRight
vector<vector<int>> moveUp(vector<vector<int>> puzzle){
    Point zero = findZeroPos(puzzle);
    puzzle[zero.x][zero.y] = puzzle[zero.x-1][zero.y];
    puzzle[zero.x-1][zero.y] = 0;
    return puzzle;
}

vector<vector<int>> moveDown(vector<vector<int>> puzzle){
    Point zero = findZeroPos(puzzle);
    puzzle[zero.x][zero.y] = puzzle[zero.x+1][zero.y];
    puzzle[zero.x+1][zero.y] = 0;
    return puzzle;
}

vector<vector<int>> moveLeft(vector<vector<int>> puzzle){
    Point zero = findZeroPos(puzzle);
    puzzle[zero.x][zero.y] = puzzle[zero.x][zero.y-1];
    puzzle[zero.x][zero.y-1] = 0;
    return puzzle;
}

vector<vector<int>> moveRight(vector<vector<int>> puzzle){
    Point zero = findZeroPos(puzzle);
    puzzle[zero.x][zero.y] = puzzle[zero.x][zero.y+1];
    puzzle[zero.x][zero.y+1] = 0;
    return puzzle;
}

//Create Heuristics for different Algorithms:


node general_search( Problem problem, QUEUEING_FUNCTION q){
    queue<node> nodes = problem.INITIALSTATE;
}



int main(int argc, const char * argv[]) {
    
    //Initialize Our own custom Puzzle as a 3x3 2-D Vector
//    vector<vector<int>> vectPuzzle;
//    vectPuzzle.resize(3);
//    for (unsigned i = 0; i < vectPuzzle.size(); ++i){
//        vectPuzzle[i].resize(3);
//    }
    
//    node *parent = new node;
//    parent->val = 5;
//    cout << parent->val << endl;
    
    {
        {1,2,3},
        {4,5,6},
        {0,7,8}
    };
    
    //The end goal state we are striving for
    
    
//    Point zeroPos;
//    zeroPos = findZeroPos(puzzleSolution);
    
    //Print Vector Puzzles
//    printPuzzle(puzzleSolution);
//    cout << endl;
//    puzzleSolution = moveUp(puzzleSolution);
//    printPuzzle(puzzleSolution);
//    cout << endl;
//    puzzleSolution = moveUp(puzzleSolution);
//    printPuzzle(puzzleSolution);
//    cout << endl;
//
//    puzzleSolution = moveLeft(puzzleSolution);
//    printPuzzle(puzzleSolution);
//    cout << endl;
//    puzzleSolution = moveLeft(puzzleSolution);
//    printPuzzle(puzzleSolution);
//    cout << endl;
//
//    puzzleSolution = moveDown(puzzleSolution);
//    printPuzzle(puzzleSolution);
//    cout << endl;
//    puzzleSolution = moveDown(puzzleSolution);
//    printPuzzle(puzzleSolution);
//    cout << endl;
//
//    puzzleSolution = moveRight(puzzleSolution);
//    printPuzzle(puzzleSolution);
//    cout << endl;
//    puzzleSolution = moveRight(puzzleSolution);
//    printPuzzle(puzzleSolution);
    cout << endl;
    
    //cout << "X: " << zeroPos.x << " Y:" << zeroPos.y << endl;
    
    
    
    //cout << slidePuzzle[1][2] << endl;
    return 0;
}
