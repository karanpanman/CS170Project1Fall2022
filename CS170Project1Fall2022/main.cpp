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
#include <algorithm>

using namespace std;

//Used to track the 0/Blank position in the vector
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

struct node {
    
    vector<vector<int>> STATE;
    int h;
    
};

//Helps me sort by h to create proper queue
struct lesser_h{
    inline bool operator() (const node* n1, const node* n2){
        return (n1->h < n2->h);
    }
};



vector<vector<int>> puzzleSolution
{
    {1,2,3},
    {4,5,6},
    {7,8,0}
};

struct Problem{
    vector<vector<int>> INITIALSTATE
    {
        {1,3,6},
        {5,0,2},
        {4,7,8}
    };
    
//    void inputProblem(){
//        vector<vector<int>> finalVec;
//        int num;
//        unsigned j;
//        for (unsigned i = 0; i < 3; ++i){
//            j = 0;
//            while ( j < 3 ){
//                cin >> num;
//                finalVec[i][j] = num;
//                ++j;
//            }
//            cout << endl;
//        }
//        INITIALSTATE = finalVec;
//    }
    
    bool GOALTEST(vector<vector<int>> puzzle){
        if ( puzzle == puzzleSolution ){
            return true;
        }
        return false;
    }
    
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
    
};



void printPuzzle(vector<vector<int>> puzzle){
    for (unsigned i = 0; i < puzzle.size(); i++){
        for (unsigned j = 0; j < puzzle[i].size(); ++j){
            cout << puzzle[i][j] << " ";
        }
        cout << endl;
    }
}

//Operators: moveUp, moveDown, moveLeft, moveRight


//Create Heuristics for different Algorithms:

//Treating Base class Queueing Function as Uniform Cost for now since H is hardcoded zero
class QUEUEING_FUNCTION {
public:
    queue<node*> localNodes;
    //come back to this and fix h value
    
    
    QUEUEING_FUNCTION( queue<node*> nodes){
        localNodes = nodes;
    }
    
    void calculateH(node* headNode){
        headNode->h = 0;
    }
        
    queue<node*> EXPAND( node* headNode, Problem problem){
        Point zeroPos = findZeroPos(headNode->STATE);
        vector<node*> sortedOrder;
        
        if ( zeroPos.x != 0 ){
            node *up = new node;
            up->STATE = problem.moveUp(headNode->STATE);
            calculateH(up);
            sortedOrder.push_back(up);
        }
        if ( zeroPos.x != 2 ){
            node *down = new node;
            down->STATE = problem.moveDown(headNode->STATE);
            calculateH(down);
            sortedOrder.push_back(down);
        }
        if ( zeroPos.y != 0 ){
            node *left = new node;
            left->STATE = problem.moveLeft(headNode->STATE);
            calculateH(left);
            sortedOrder.push_back(left);
        }
        if ( zeroPos.y != 2 ){
            node *right = new node;
            right->STATE = problem.moveRight(headNode->STATE);
            calculateH(right);
            sortedOrder.push_back(right);
        }
        
        sort(sortedOrder.begin(), sortedOrder.end(), lesser_h());
        
        for (unsigned i = 0; i < sortedOrder.size(); ++i){
            cout << "H equals: " << sortedOrder.at(i)->h << endl;
            localNodes.push(sortedOrder.at(i));
        }
        
        
        return localNodes;
    }
    
};

class AstarMisplacedTile: public QUEUEING_FUNCTION {
public:
    
    AstarMisplacedTile ( queue<node*> nodes) : QUEUEING_FUNCTION(nodes){
        localNodes = nodes;
    }
    
    void calculateH(node* headNode){
        headNode->h = 0;
        for (unsigned i = 0; i < headNode->STATE.size(); i++){
            for (unsigned j = 0; j < headNode->STATE[i].size(); ++j){
                if ( headNode->STATE[i][j] != puzzleSolution[i][j] ) { ++headNode->h; }
            }
        }
    }
    
    queue<node*> EXPAND( node* headNode, Problem problem){
        Point zeroPos = findZeroPos(headNode->STATE);
        vector<node*> sortedOrder;

        if ( zeroPos.x != 0 ){
            node *up = new node;
            up->STATE = problem.moveUp(headNode->STATE);
            calculateH(up);
            sortedOrder.push_back(up);
        }
        if ( zeroPos.x != 2 ){
            node *down = new node;
            down->STATE = problem.moveDown(headNode->STATE);
            calculateH(down);
            sortedOrder.push_back(down);
        }
        if ( zeroPos.y != 0 ){
            node *left = new node;
            left->STATE = problem.moveLeft(headNode->STATE);
            calculateH(left);
            sortedOrder.push_back(left);
        }
        if ( zeroPos.y != 2 ){
            node *right = new node;
            right->STATE = problem.moveRight(headNode->STATE);
            calculateH(right);
            sortedOrder.push_back(right);
        }

        sort(sortedOrder.begin(), sortedOrder.end(), lesser_h());

        for (unsigned i = 0; i < sortedOrder.size(); ++i){
            cout << "H equals: " << sortedOrder.at(i)->h << endl;
            localNodes.push(sortedOrder.at(i));
        }
        cout << endl;


        return localNodes;
    }
    
};

class AstarManhattanDistance: public QUEUEING_FUNCTION {
public:
    
    AstarManhattanDistance ( queue<node*> nodes) : QUEUEING_FUNCTION(nodes){
        localNodes = nodes;
    }
    
    void calculateH(node* headNode){
        headNode->h = 0;
        for (unsigned i = 0; i < headNode->STATE.size(); i++){
            for (unsigned j = 0; j < headNode->STATE[i].size(); ++j){
                if ( headNode->STATE[i][j] != puzzleSolution[i][j] ) { ++headNode->h; }
            }
        }
    }
    
    queue<node*> EXPAND( node* headNode, Problem problem){
        Point zeroPos = findZeroPos(headNode->STATE);
        vector<node*> sortedOrder;

        if ( zeroPos.x != 0 ){
            node *up = new node;
            up->STATE = problem.moveUp(headNode->STATE);
            calculateH(up);
            sortedOrder.push_back(up);
        }
        if ( zeroPos.x != 2 ){
            node *down = new node;
            down->STATE = problem.moveDown(headNode->STATE);
            calculateH(down);
            sortedOrder.push_back(down);
        }
        if ( zeroPos.y != 0 ){
            node *left = new node;
            left->STATE = problem.moveLeft(headNode->STATE);
            calculateH(left);
            sortedOrder.push_back(left);
        }
        if ( zeroPos.y != 2 ){
            node *right = new node;
            right->STATE = problem.moveRight(headNode->STATE);
            calculateH(right);
            sortedOrder.push_back(right);
        }

        sort(sortedOrder.begin(), sortedOrder.end(), lesser_h());

        for (unsigned i = 0; i < sortedOrder.size(); ++i){
            cout << "H equals: " << sortedOrder.at(i)->h << endl;
            localNodes.push(sortedOrder.at(i));
        }
        cout << endl;


        return localNodes;
    }
    
};


node* general_search( Problem problem){
    node* root = new node;
    //MAKE QUEUE
    queue<node*> nodes;
    //MAKE-NODE WITH problem.INITIAL-STATE
    root->STATE = problem.INITIALSTATE;
    nodes.push(root);

    while ( !nodes.empty() ){
        node* headNode = nodes.front();
        nodes.pop();
        if ( problem.GOALTEST(headNode->STATE) ){
            return headNode;
        }
        AstarMisplacedTile QUEUEINGFUNCTION(nodes);
        nodes = QUEUEINGFUNCTION.EXPAND(headNode, problem);
    }
    return root;
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
    node *parent = new node;
    parent->STATE =
    {
        {1,2,3},
        {4,5,6},
        {0,7,8}
    };
    
    node *test = new node;
    test->STATE =
    {
        {1,2,3},
        {4,3,6},
        {0,7,8}
    };
    

    if (parent->STATE == test->STATE){
        cout << "LET'S GOOOOO" << endl;
    }
    
    Problem check1;
    printPuzzle(check1.INITIALSTATE);
    cout << endl;
    
    Problem problem;
    //QUEUEING_FUNCTION q;
    
    node *Test = new node;
    
    Test = general_search(problem);
    
    printPuzzle(Test->STATE);
    
    cout << endl;
    
    //cout << "X: " << zeroPos.x << " Y:" << zeroPos.y << endl;
    
    
    
    //cout << slidePuzzle[1][2] << endl;
    return 0;
}
