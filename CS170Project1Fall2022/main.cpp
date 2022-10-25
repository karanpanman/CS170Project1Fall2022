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

Point findPos ( vector<vector<int>> puzzle, int num){
    Point pos;
    for (unsigned i = 0; i < puzzle.size(); i++){
        for (unsigned j = 0; j < puzzle[i].size(); ++j){
            if ( puzzle[i][j] == num ) {
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
    int depth;
    
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
        {1,6,7},
        {5,0,3},
        {4,8,2}
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
    
    vector<vector<int>> moveUp(vector<vector<int>> puzzle, Point zero){
        puzzle[zero.x][zero.y] = puzzle[zero.x-1][zero.y];
        puzzle[zero.x-1][zero.y] = 0;
        return puzzle;
    }

    vector<vector<int>> moveDown(vector<vector<int>> puzzle, Point zero){
        puzzle[zero.x][zero.y] = puzzle[zero.x+1][zero.y];
        puzzle[zero.x+1][zero.y] = 0;
        return puzzle;
    }

    vector<vector<int>> moveLeft(vector<vector<int>> puzzle, Point zero){
        puzzle[zero.x][zero.y] = puzzle[zero.x][zero.y-1];
        puzzle[zero.x][zero.y-1] = 0;
        return puzzle;
    }

    vector<vector<int>> moveRight(vector<vector<int>> puzzle, Point zero){
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

vector<node*> statesList;
//Create Heuristics for different Algorithms:

//Treating Base class Queueing Function as Uniform Cost for now since H is hardcoded zero
class QUEUEING_FUNCTION {
public:
    //Point point1;
    //Point point2;

    Point puzzleSolutionPoints[9];
    queue<node*> localNodes;
    
    //come back to this and fix h value
    
    
    QUEUEING_FUNCTION( queue<node*> nodes){
        localNodes = nodes;
        puzzleSolutionPoints[0].x = 2; puzzleSolutionPoints[0].y = 2;
        puzzleSolutionPoints[1].x = 0; puzzleSolutionPoints[1].y = 0;
        puzzleSolutionPoints[2].x = 0; puzzleSolutionPoints[2].y = 1;
        puzzleSolutionPoints[3].x = 0; puzzleSolutionPoints[3].y = 2;
        puzzleSolutionPoints[4].x = 1; puzzleSolutionPoints[4].y = 0;
        puzzleSolutionPoints[5].x = 1; puzzleSolutionPoints[5].y = 1;
        puzzleSolutionPoints[6].x = 1; puzzleSolutionPoints[6].y = 2;
        puzzleSolutionPoints[7].x = 2; puzzleSolutionPoints[7].y = 0;
        puzzleSolutionPoints[8].x = 2; puzzleSolutionPoints[8].y = 1;
    }
    
    bool checkRepeatedStates ( node* puzzle, vector<node*> totalStatesList ){
        if (!totalStatesList.empty()){
            sort(totalStatesList.begin(), totalStatesList.end(), lesser_h());
            
            if (puzzle->h < (totalStatesList.at(totalStatesList.size()/2)->h )){
                for (unsigned i = 0; i <= totalStatesList.size()/2; ++i){
                    if ( puzzle->STATE == totalStatesList.at(i)->STATE ){
                        return true;
                    }
                }
            }
            else{
                for (unsigned long i = totalStatesList.size()/2 ; i < totalStatesList.size(); ++i){
                    if ( puzzle->STATE == totalStatesList.at(i)->STATE ){
                        return true;
                    }
                }
            }
        }
        return false;
    }
    
    void calculateH(node* headNode){
        headNode->h = 0;
    }
        
    queue<node*> EXPAND( node* headNode, Problem problem){
        Point zeroPos = findPos(headNode->STATE,0);
        vector<node*> sortedOrder;
        
        if ( zeroPos.x != 0 ){
            node *up = new node;
            up->depth = headNode->depth + 1;
            up->STATE = problem.moveUp(headNode->STATE, zeroPos);
            calculateH(up);
            if (!checkRepeatedStates(up, statesList)){
                sortedOrder.push_back(up);
            }
        }
        if ( zeroPos.x != 2 ){
            node *down = new node;
            down->depth = headNode->depth + 1;
            down->STATE = problem.moveDown(headNode->STATE, zeroPos);
            calculateH(down);
            if (!checkRepeatedStates(down, statesList)){
                sortedOrder.push_back(down);
            }
        }
        if ( zeroPos.y != 0 ){
            node *left = new node;
            left->depth = headNode->depth + 1;
            left->STATE = problem.moveLeft(headNode->STATE, zeroPos);
            calculateH(left);
            if (!checkRepeatedStates(left, statesList)){
                sortedOrder.push_back(left);
            }
        }
        if ( zeroPos.y != 2 ){
            node *right = new node;
            right->depth = headNode->depth + 1;
            right->STATE = problem.moveRight(headNode->STATE, zeroPos);
            calculateH(right);
            if (!checkRepeatedStates(right, statesList)){
                sortedOrder.push_back(right);
            }
        }
        
        sort(sortedOrder.begin(), sortedOrder.end(), lesser_h());
        
        for (unsigned i = 0; i < sortedOrder.size(); ++i){
            //if ( !checkRepeatedStates(sortedOrder.at(i), statesList)){
                cout << "H equals: " << sortedOrder.at(i)->h << endl;
                statesList.push_back(sortedOrder.at(i));
                localNodes.push(sortedOrder.at(i));
            //}
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
        Point zeroPos = findPos(headNode->STATE,0);
        vector<node*> sortedOrder;

        if ( zeroPos.x != 0 ){
            node *up = new node;
            up->depth = headNode->depth + 1;
            up->STATE = problem.moveUp(headNode->STATE, zeroPos);
            calculateH(up);
            sortedOrder.push_back(up);
        }
        if ( zeroPos.x != 2 ){
            node *down = new node;
            down->depth = headNode->depth + 1;
            down->STATE = problem.moveDown(headNode->STATE, zeroPos);
            calculateH(down);
            sortedOrder.push_back(down);
        }
        if ( zeroPos.y != 0 ){
            node *left = new node;
            left->depth = headNode->depth + 1;
            left->STATE = problem.moveLeft(headNode->STATE, zeroPos);
            calculateH(left);
            sortedOrder.push_back(left);
        }
        if ( zeroPos.y != 2 ){
            node *right = new node;
            right->depth = headNode->depth + 1;
            right->STATE = problem.moveRight(headNode->STATE, zeroPos);
            calculateH(right);
            sortedOrder.push_back(right);
        }

        sort(sortedOrder.begin(), sortedOrder.end(), lesser_h());

        for (unsigned i = 0; i < sortedOrder.size(); ++i){
            if ( !checkRepeatedStates(sortedOrder.at(i), statesList)){
                cout << "H equals: " << sortedOrder.at(i)->h << endl;
                statesList.push_back(sortedOrder.at(i));
                localNodes.push(sortedOrder.at(i));
            }
        }
        cout << endl;


        return localNodes;
    }
    
};

int calculateDistance (int i, int j, int num, Point solution[]){
    int sum = 0;
    
    int actual_i = solution[num].x - i;
    int actual_j = solution[num].y - j;
    sum = abs(actual_i + actual_j);
    return sum;
}

class AstarManhattanDistance: public QUEUEING_FUNCTION {
public:
    
    AstarManhattanDistance ( queue<node*> nodes) : QUEUEING_FUNCTION(nodes){
        localNodes = nodes;
    }
    
    void calculateH(node* headNode){
        headNode->h = 0;
        for (unsigned i = 0; i < headNode->STATE.size(); i++){
            for (unsigned j = 0; j < headNode->STATE[i].size(); ++j){
                if ( headNode->STATE[i][j] != puzzleSolution[i][j] ) {
                    headNode->h = headNode->h + calculateDistance(i, j, headNode->STATE[i][j], puzzleSolutionPoints);
                }
            }
        }
    }
    
    queue<node*> EXPAND( node* headNode, Problem problem){
        Point zeroPos = findPos(headNode->STATE,0);
        vector<node*> sortedOrder;

        if ( zeroPos.x != 0 ){
            node *up = new node;
            up->depth = headNode->depth + 1;
            up->STATE = problem.moveUp(headNode->STATE, zeroPos);
            calculateH(up);
            if (!checkRepeatedStates(up, statesList)){
                sortedOrder.push_back(up);
            }
        }
        if ( zeroPos.x != 2 ){
            node *down = new node;
            down->depth = headNode->depth + 1;
            down->STATE = problem.moveDown(headNode->STATE, zeroPos);
            calculateH(down);
            if (!checkRepeatedStates(down, statesList)){
                sortedOrder.push_back(down);
            }
        }
        if ( zeroPos.y != 0 ){
            node *left = new node;
            left->depth = headNode->depth + 1;
            left->STATE = problem.moveLeft(headNode->STATE, zeroPos);
            calculateH(left);
            if (!checkRepeatedStates(left, statesList)){
                sortedOrder.push_back(left);
            }
        }
        if ( zeroPos.y != 2 ){
            node *right = new node;
            right->depth = headNode->depth + 1;
            right->STATE = problem.moveRight(headNode->STATE, zeroPos);
            calculateH(right);
            if (!checkRepeatedStates(right, statesList)){
                sortedOrder.push_back(right);
            }
        }

        sort(sortedOrder.begin(), sortedOrder.end(), lesser_h());

        for (unsigned i = 0; i < sortedOrder.size(); ++i){
            
            if ( !checkRepeatedStates(sortedOrder.at(i), statesList)){
                cout << "H equals: " << sortedOrder.at(i)->h << endl;
                statesList.push_back(sortedOrder.at(i));
                localNodes.push(sortedOrder.at(i));
            }
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
    root->depth = 0;
    nodes.push(root);
    while ( !nodes.empty() ){
        node* headNode = nodes.front();
        nodes.pop();
        if ( problem.GOALTEST(headNode->STATE) ){
            cout << "Depth: " << headNode->depth << endl;
            return headNode;
        }
        AstarManhattanDistance QUEUEINGFUNCTION(nodes);
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
