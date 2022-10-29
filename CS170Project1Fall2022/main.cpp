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


//Main Structure that will carry out our algorithms: The Node
struct node {
    
    vector<vector<int>> STATE;
    int h;          //This is our hill climbing/greedy/heuristic value
    int depth;      //Depth is in essence our g(n) value for this problem
    Point zeroPoint;
    int f;          //f(n) = g(n) + h(n)
};

//Helps me sort by f to create the proper queue for A Star: f(n) = g(n) + h(n);
struct lesser_f{
    inline bool operator() (const node* n1, const node* n2){
        return (n1->f < n2->f);
    }
};
//For quick goal testing purposes
vector<vector<int>> puzzleSolution
{
    {1,2,3},
    {4,5,6},
    {7,8,0}
};

//Helps with calculating H for Manhattan Distance
int calculateDistance (int i, int j, int num, Point solution[]){
    int sum = 0;
    int actual_i = solution[num].x - i;
    int actual_j = solution[num].y - j;
    sum = abs(actual_i )+ abs(actual_j);
    return sum;
}

struct Problem{
    vector<vector<int>> INITIALSTATE
    {
        {7,1,2},
        {4,8,5},
        {6,3,0}
    };
    
    Point zeroOriginalPosition = findPos(INITIALSTATE, 0);
    
    
    bool GOALTEST(vector<vector<int>> puzzle){
        if ( puzzle == puzzleSolution ){
            return true;
        }
        return false;
    }
    
    //All of our operators are listed here below:
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


//Prints out the passed in node->STATE puzzle
void printPuzzle(vector<vector<int>> puzzle){
    for (unsigned i = 0; i < puzzle.size(); i++){
        for (unsigned j = 0; j < puzzle[i].size(); ++j){
            cout << puzzle[i][j] << " ";
        }
        cout << endl;
    }
}

//statesList is comprised of all repeat states
vector<node*> statesList;

//Create Heuristics for different Algorithms:
//Operators: moveUp, moveDown, moveLeft, moveRight
//Treating Base class Queueing Function as Uniform Cost for now since H is hardcoded zero
class QUEUEING_FUNCTION {
public:
    
    Point puzzleSolutionPoints[9];
    queue<node*> localNodes;    //Uused to store nodes from General Search Algorithm
    vector<node*> sortedOrder;  //Used in all algorithms to hold the children of our headNode
    
    QUEUEING_FUNCTION( queue<node*> nodes){
        localNodes = nodes;
        //Stores the specific point values to use for quick O(1) comparison instead of extra for loops
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
    
    //This will look at the child node being passed in to verify if it is a repeat or not
    //Return 0 = Unseen State, Return 1 = Repeat State
    bool checkRepeatedStates ( node* puzzle, vector<node*> totalStatesList ){
       
        if (!totalStatesList.empty()){
            sort(totalStatesList.begin(), totalStatesList.end(), lesser_f());
            //Splitting the search to bottom half and upper half all depending on the f value
            //Bottom half of the statesList
            if (puzzle->f < (totalStatesList.at(totalStatesList.size()/2)->f )){
                for (unsigned i = 0; i <= totalStatesList.size()/2; ++i){
                    if ( puzzle->STATE == totalStatesList.at(i)->STATE ){
                        return true;
                    }   //If the statesList puzzle being seen is already higher f, then we can conclude our curr node is new
                    if ( puzzle->f < totalStatesList.at(i)->f ){
                        return false;
                    }
                }
            }
            else{   //Top half - 1
                for (unsigned long i = totalStatesList.size()/2 ; i < totalStatesList.size(); ++i){
                    if ( puzzle->STATE == totalStatesList.at(i)->STATE ){
                        return true;
                    }
                    if ( puzzle->f < totalStatesList.at(i)->f ){
                        return false;
                    }
                }
            }
        }
        return false;
    }
    //H is hard coded to zero for uniform cost
    void calculateH(node* headNode){
        headNode->h = 0;
    }
        
    queue<node*> EXPAND( node* headNode, Problem problem){
        //This places the Initial Problem state into our repeated states list
        if (statesList.empty()){
            calculateH(headNode);
            headNode->f = headNode->h + headNode->depth;
            statesList.push_back(headNode);
        }
        
        //Make our children and store them in the sorted order vector
        sortedOrder = makeChildren(headNode, problem);
        
        for ( unsigned i = 0; i < sortedOrder.size(); ++i ){
            calculateH(sortedOrder.at(i));
            sortedOrder.at(i)->f = sortedOrder.at(i)->h + sortedOrder.at(i)->depth;
        }
        
        sort(sortedOrder.begin(), sortedOrder.end(), lesser_f());
        vector<node*> finalVals;
        //While we check if any of the children have been repeated, we want to put all the new ones in our Final Values vector
        for (unsigned i = 0; i < sortedOrder.size(); ++i){
            if ( !checkRepeatedStates(sortedOrder.at(i), statesList)){
                statesList.push_back(sortedOrder.at(i));
                finalVals.push_back(sortedOrder.at(i));
            }
        }
        
        //Final values now takes in the prior nodes in our original nodes queue and will then sort the new children with old parents
        while (!localNodes.empty()){
            node* front = new node;
            front = localNodes.front();
            finalVals.push_back(front);
            localNodes.pop();
        }
        
        //Place all the sorted by f(n) nodes from final values vector back into our nodes queue
        sort(finalVals.begin(), finalVals.end(), lesser_f());
        for (unsigned i = 0; i < finalVals.size(); ++i){
            localNodes.push(finalVals.at(i));
        }
        
        
        return localNodes;
    }
    
    vector<node*> makeChildren ( node* headNode, Problem problem){
        
        vector<node*>sorted;
        
        Point zeroPos = headNode->zeroPoint;
        int depth = headNode->depth + 1;
        
        if ( zeroPos.x != 0 ){
            node *up = new node;
            //headNode->up = up;
            up->depth = depth;
            up->zeroPoint.x = zeroPos.x - 1;
            up->zeroPoint.y = zeroPos.y;
            up->STATE = problem.moveUp(headNode->STATE, zeroPos);
            
            if (!checkRepeatedStates(up, statesList)){
                //cout << "Up State: " << endl;
                //printPuzzle(up->STATE);
                sorted.push_back(up);
            }
        }
        if ( zeroPos.x != 2 ){
            node *down = new node;
            //headNode->down = down;
            down->depth = depth;
            down->zeroPoint.x = zeroPos.x + 1;
            down->zeroPoint.y = zeroPos.y;
            down->STATE = problem.moveDown(headNode->STATE, zeroPos);
            //calculateH(down);
            //down->f = down->h + down->depth;
            if (!checkRepeatedStates(down, statesList)){
                //cout << "Down State: " << endl;
                //printPuzzle(down->STATE);
                sorted.push_back(down);
            }
        }
        if ( zeroPos.y != 0 ){
            node *left = new node;
            //headNode->left = left;
            left->depth = depth;
            left->zeroPoint.x = zeroPos.x;
            left->zeroPoint.y = zeroPos.y - 1;
            left->STATE = problem.moveLeft(headNode->STATE, zeroPos);
            //calculateH(left);
            //left->f = left->h + left->depth;
            if (!checkRepeatedStates(left, statesList)){
                //cout << "Left State: " << endl;
                //printPuzzle(left->STATE);
                sorted.push_back(left);
            }
        }
        if ( zeroPos.y != 2 ){
            node *right = new node;
            //headNode->right = right;
            right->depth = depth;
            right->zeroPoint.x = zeroPos.x;
            right->zeroPoint.y = zeroPos.y + 1;
            right->STATE = problem.moveRight(headNode->STATE, zeroPos);
            //calculateH(right);
            //right->f = right->h + right->depth;
            if (!checkRepeatedStates(right, statesList)){
                //cout << "Right State: " << endl;
                //printPuzzle(right->STATE);
                sorted.push_back(right);
            }
        }
        
        return sorted;
        
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
        //This places the Initial Problem state into our repeated states list
        if (statesList.empty()){
            calculateH(headNode);
            headNode->f = headNode->h + headNode->depth;
            statesList.push_back(headNode);
        }
        
        //Make our children and store them in the sorted order vector
        sortedOrder = makeChildren(headNode, problem);
        
        for ( unsigned i = 0; i < sortedOrder.size(); ++i ){
            calculateH(sortedOrder.at(i));
            sortedOrder.at(i)->f = sortedOrder.at(i)->h + sortedOrder.at(i)->depth;
        }
        
        sort(sortedOrder.begin(), sortedOrder.end(), lesser_f());
        vector<node*> finalVals;
        //While we check if any of the children have been repeated, we want to put all the new ones in our Final Values vector
        for (unsigned i = 0; i < sortedOrder.size(); ++i){
            if ( !checkRepeatedStates(sortedOrder.at(i), statesList)){
                statesList.push_back(sortedOrder.at(i));
                finalVals.push_back(sortedOrder.at(i));
            }
        }
        
        //Final values now takes in the prior nodes in our original nodes queue and will then sort the new children with old parents
        while (!localNodes.empty()){
            node* front = new node;
            front = localNodes.front();
            finalVals.push_back(front);
            localNodes.pop();
        }
        
        //Place all the sorted by f(n) nodes from final values vector back into our nodes queue
        sort(finalVals.begin(), finalVals.end(), lesser_f());
        for (unsigned i = 0; i < finalVals.size(); ++i){
            localNodes.push(finalVals.at(i));
        }
        
        
        return localNodes;
    }
    
};



class AstarManhattanDistance: public QUEUEING_FUNCTION {
public:
    
    AstarManhattanDistance ( queue<node*> nodes) : QUEUEING_FUNCTION(nodes){
        localNodes = nodes;
    }
    
    //Calculate manhattan distance
    void calculateH(node* headNode){
        headNode->h = 0;
        cout << endl;
        for (unsigned i = 0; i < headNode->STATE.size(); i++){
            for (unsigned j = 0; j < headNode->STATE[i].size(); ++j){
                if ( headNode->STATE[i][j] != 0 && headNode->STATE[i][j] != puzzleSolution[i][j] ) {                    headNode->h = headNode->h + calculateDistance(i, j, headNode->STATE[i][j], puzzleSolutionPoints);
                }
            }
        }
    }
    
    queue<node*> EXPAND( node* headNode, Problem problem){
        //This places the Initial Problem state into our repeated states list
        if (statesList.empty()){
            calculateH(headNode);
            headNode->f = headNode->h + headNode->depth;
            statesList.push_back(headNode);
        }
        
        //Make our children and store them in the sorted order vector
        sortedOrder = makeChildren(headNode, problem);
        
        for ( unsigned i = 0; i < sortedOrder.size(); ++i ){
            calculateH(sortedOrder.at(i));
            sortedOrder.at(i)->f = sortedOrder.at(i)->h + sortedOrder.at(i)->depth;
        }
        
        sort(sortedOrder.begin(), sortedOrder.end(), lesser_f());
        vector<node*> finalVals;
        //While we check if any of the children have been repeated, we want to put all the new ones in our Final Values vector
        for (unsigned i = 0; i < sortedOrder.size(); ++i){
            if ( !checkRepeatedStates(sortedOrder.at(i), statesList)){
                statesList.push_back(sortedOrder.at(i));
                finalVals.push_back(sortedOrder.at(i));
            }
        }
        
        //Final values now takes in the prior nodes in our original nodes queue and will then sort the new children with old parents
        while (!localNodes.empty()){
            node* front = new node;
            front = localNodes.front();
            finalVals.push_back(front);
            localNodes.pop();
        }
        
        //Place all the sorted by f(n) nodes from final values vector back into our nodes queue
        sort(finalVals.begin(), finalVals.end(), lesser_f());
        for (unsigned i = 0; i < finalVals.size(); ++i){
            localNodes.push(finalVals.at(i));
        }
        
        
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
    root->zeroPoint = problem.zeroOriginalPosition;
    nodes.push(root);
    
    while ( !nodes.empty() ){
        node* headNode = nodes.front();
        nodes.pop();
        cout << "Head node state: " << endl;
        printPuzzle(headNode->STATE);
        cout << endl;
        if ( problem.GOALTEST(headNode->STATE) ){
            cout << "Depth: " << headNode->depth << endl;
            return headNode;
        }
        
        AstarMisplacedTile QUEUEINGFUNCTION(nodes);
        nodes = QUEUEINGFUNCTION.EXPAND(headNode, problem);
        //cout << "Nodes queue currently contains: H = " << nodes.front()->h << " Puzzle looks like: " << endl;
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
