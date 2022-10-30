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
    vector<vector<int>> INITIALSTATE{
        {7,1,2},
        {4,8,5},
        {6,3,0}
    };
    
    vector<vector<int>> Depth0
    {
        {1,2,3},
        {4,5,6},
        {7,8,0}
    };
    
    vector<vector<int>> Depth2
    {
        {1,2,3},
        {4,5,6},
        {0,7,8}
    };
    
    vector<vector<int>> Depth4
    {
        {1,2,3},
        {5,0,6},
        {4,7,8}
    };
    
    vector<vector<int>> Depth8
    {
        {1,3,6},
        {5,0,2},
        {4,7,8}
    };
    
    vector<vector<int>> Depth12
    {
        {1,3,6},
        {5,0,7},
        {4,8,2}
    };
    
    vector<vector<int>> Depth16
    {
        {1,6,7},
        {5,0,3},
        {4,8,2}
    };
    
    vector<vector<int>> Depth20
    {
        {7,1,2},
        {4,8,5},
        {6,3,0}
    };
    
    vector<vector<int>> Depth24
    {
        {0,7,2},
        {4,6,1},
        {3,5,8}
    };
    
    vector<vector<int>> EamonnTest
    {
        {4,1,2},
        {5,3,0},
        {7,8,6}
    };
    
    vector<vector<int>> PreetTest
    {
        {2,6,5},
        {6,4,3},
        {7,1,0}
    };
    
    vector<vector<int>> DiscordTest
    {
        {8,6,7},
        {2,5,4},
        {3,0,1}
    };
    
    //Point zeroOriginalPosition; //= findPos(INITIALSTATE, 0);
    
    
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
unsigned long nodesExpanded = 0;

//Create Heuristics for different Algorithms:
//Operators: moveUp, moveDown, moveLeft, moveRight
//Treating Base class Queueing Function as Uniform Cost for now since H is hardcoded zero
class QUEUEING_FUNCTION {
public:
    
    Point puzzleSolutionPoints[9];
    queue<node*> localNodes;    //Uused to store nodes from General Search Algorithm
    vector<node*> sortedOrder;  //Used in all algorithms to hold the children of our headNode
    int type = 0;
    
    QUEUEING_FUNCTION(){}
    
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
        
        nodesExpanded = nodesExpanded + 1;
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
    
    //Crucial to expand function
    vector<node*> makeChildren ( node* headNode, Problem problem){
        
        vector<node*>sorted;
        
        Point zeroPos = headNode->zeroPoint;
        int depth = headNode->depth + 1;
        
        
        if ( zeroPos.y != 0 ){
            node *left = new node;
            left->depth = depth;
            left->zeroPoint.x = zeroPos.x;
            left->zeroPoint.y = zeroPos.y - 1;
            left->STATE = problem.moveLeft(headNode->STATE, zeroPos);
            if (!checkRepeatedStates(left, statesList)){
                sorted.push_back(left);
            }
        }
        if ( zeroPos.y != 2 ){
            node *right = new node;
            right->depth = depth;
            right->zeroPoint.x = zeroPos.x;
            right->zeroPoint.y = zeroPos.y + 1;
            right->STATE = problem.moveRight(headNode->STATE, zeroPos);
            if (!checkRepeatedStates(right, statesList)){
                sorted.push_back(right);
            }
        }
        if ( zeroPos.x != 0 ){
            node *up = new node;
            up->depth = depth;
            up->zeroPoint.x = zeroPos.x - 1;
            up->zeroPoint.y = zeroPos.y;
            up->STATE = problem.moveUp(headNode->STATE, zeroPos);
            
            if (!checkRepeatedStates(up, statesList)){
                sorted.push_back(up);
            }
        }
        if ( zeroPos.x != 2 ){
            node *down = new node;
            down->depth = depth;
            down->zeroPoint.x = zeroPos.x + 1;
            down->zeroPoint.y = zeroPos.y;
            down->STATE = problem.moveDown(headNode->STATE, zeroPos);
            if (!checkRepeatedStates(down, statesList)){
                sorted.push_back(down);
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
        nodesExpanded = nodesExpanded + 1;
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
        for (unsigned i = 0; i < headNode->STATE.size(); i++){
            for (unsigned j = 0; j < headNode->STATE[i].size(); ++j){
                if ( headNode->STATE[i][j] != 0 && headNode->STATE[i][j] != puzzleSolution[i][j] ) {
                    headNode->h = headNode->h + calculateDistance(i, j, headNode->STATE[i][j], puzzleSolutionPoints);
                }
            }
        }
    }
    
    queue<node*> EXPAND( node* headNode, Problem problem){
        //This places the Initial Problem state into our repeated states list
        nodesExpanded = nodesExpanded + 1;
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

//Function general-search
node* general_search( Problem problem, QUEUEING_FUNCTION q){
    int type = q.type;
    node* root = new node;
    //MAKE QUEUE
    queue<node*> nodes;
    //MAKE-NODE WITH problem.INITIAL-STATE
    root->STATE = problem.INITIALSTATE;
    root->depth = 0;
    root->zeroPoint = findPos(root->STATE, 0);
    nodesExpanded = nodesExpanded + 1;
    //NODES = MAKE-QUEUE(MAKE-NODE(PROBLEM.INITIALSTATE))
    nodes.push(root);
    
    //LOOP DO and also IF EMPTY(NODES) THEN RETURN FAILURE
    while ( !nodes.empty() ){
        //node = REMOVE-FRONT(nodes)
        node* headNode = nodes.front();
        nodes.pop();
        cout << "Head node state: " << endl;
        printPuzzle(headNode->STATE);
        cout << endl;
        //if problem.GOAL-TEST(node.STATE) succeeds then RETURN NODE
        if ( problem.GOALTEST(headNode->STATE) ){
            cout << "Depth: " << headNode->depth << endl;
            return headNode;
        }
        //nodes = QUEUEING-FUNCTION(NODES,EXPAND(NODE,PROBLEM.operators))
        if (type == 0){
            QUEUEING_FUNCTION QUEUEINGFUNCTION(nodes);
            nodes = QUEUEINGFUNCTION.EXPAND(headNode, problem);
        }
        else if (type == 1){
            AstarMisplacedTile QUEUEINGFUNCTION(nodes);
            nodes = QUEUEINGFUNCTION.EXPAND(headNode, problem);
        }
        else{
            AstarManhattanDistance QUEUEINGFUNCTION(nodes);
            nodes = QUEUEINGFUNCTION.EXPAND(headNode, problem);
        }
        
    }
    cout << "Failure" << endl;
    return root;
}

//Checks to see if user-inputted puzzle contains all different numbers
bool isValid(vector<vector<int>> puzzle){
    int arr[puzzle.size()*puzzle.size()];
    unsigned i = 0; while( i != (puzzle.size()*puzzle.size()) ){arr[i] = 0;}
    
    for (i = 0; i < puzzle.size(); ++i){
        for (unsigned j = 0; j < puzzle[i].size(); j++){
            if ( arr[puzzle[i][j]] == 0 ){
                arr[puzzle[i][j]] = 1;
            }
            else{
                return false;
            }
        }
    }
    return true;
}


int main(int argc, const char * argv[]) {
    
    Problem problem;
    node *Test = new node;
    
    cout << "~Welcome to Karan's 8-Puzzle Search Algorithm Demonstration~\n\n" << "Would you like to pick a default puzzle or enter your own?" << endl;
    cout << "Press 1 to see the default cases or Press 2 to type your own\n" << endl;
    
    problem.INITIALSTATE = problem.DiscordTest;
    //QUEUEING_FUNCTION q;
    
    
    QUEUEING_FUNCTION qtest;
    Test = general_search(problem, qtest);
    
    printPuzzle(Test->STATE);
    
    cout << "Nodes expanded: " << nodesExpanded << endl;
    cout << endl;
    
    return 0;
}
