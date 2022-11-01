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
#include <map>

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
struct lesser_h{
    inline bool operator() (const node* n1, const node* n2){
        return (n1->h < n2->h);
    }
};
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

//List of different tests
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
        cout << "[";
        for (unsigned j = 0; j < puzzle[i].size() - 1; ++j){
            cout << puzzle[i][j] << ", ";
        }
        cout << puzzle[i][puzzle[i].size() - 1] << "]";
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
            sort(totalStatesList.begin(), totalStatesList.end(), lesser_h());
            //Splitting the search to bottom half and upper half all depending on the f value
            //Bottom half of the statesList
            if (puzzle->h < (totalStatesList.at(totalStatesList.size()/2)->h )){
                for (unsigned i = 0; i <= totalStatesList.size()/2; ++i){
                    if ( puzzle->STATE == totalStatesList.at(i)->STATE ){
                        return true;
                    }   //If the statesList puzzle being seen is already higher f, then we can conclude our curr node is new
                    if ( puzzle->h < totalStatesList.at(i)->h ){
                        return false;
                    }
                }
            }
            else{   //Top half - 1
                for (unsigned long i = totalStatesList.size()/2 - 1 ; i < totalStatesList.size(); ++i){
                    if ( puzzle->STATE == totalStatesList.at(i)->STATE ){
                        return true;
                    }
                    if ( puzzle->h < totalStatesList.at(i)->h ){
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
//        int minH = sortedOrder.at(0)->h;
//        while ( sortedOrder.at(sortedOrder.size() - 1)->h != minH ){
//            sortedOrder.pop_back();
//        }
        
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
            node* front = localNodes.front();
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
    unsigned long maxQueueSize = 1;
    //MAKE QUEUE
    queue<node*> nodes;
    //MAKE-NODE WITH problem.INITIAL-STATE
    root->STATE = problem.INITIALSTATE; root->depth = 0; root->zeroPoint = findPos(root->STATE, 0);
    nodesExpanded = nodesExpanded + 1;
    //NODES = MAKE-QUEUE(MAKE-NODE(PROBLEM.INITIALSTATE))
    nodes.push(root);
    
    //LOOP DO and also IF EMPTY(NODES) THEN RETURN FAILURE
    while ( !nodes.empty() ){
        //node = REMOVE-FRONT(nodes)
       
        node* headNode = nodes.front();
        nodes.pop();
        
        cout << endl;
        //if problem.GOAL-TEST(node.STATE) succeeds then RETURN NODE
        if ( problem.GOALTEST(headNode->STATE) ){
            cout << "Goal state!" << endl;
            cout << "Solution Depth was " << headNode->depth << endl;
            cout << "Number of nodes expanded: " << nodesExpanded << endl;
            cout << "Max queue size: " << maxQueueSize << endl;
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
        if (nodes.size() > maxQueueSize){
            maxQueueSize = nodes.size();
        }
        cout << "The best state to expand with a g(n) = " << headNode->depth << " and a h(n) = " << headNode->h << " is:" << endl;
        printPuzzle(headNode->STATE);
        
    }
    cout << "Failure" << endl;
    return root;
}

//Checks to see if user-inputted puzzle contains all different numbers


//For choosing the correct test state based on user input
vector<vector<int>> getTest (int num){
    switch(num){
        case 1:
            return EamonnTest;
        case 2:
            return Depth2;
        case 3:
            return Depth4;
        case 4:
            return Depth8;
        case 5:
            return Depth12;
        case 6:
            return Depth16;
        case 7:
            return Depth20;
        case 8:
            return Depth24;
        default:
            return Depth0;
            break;
    }
}

vector<vector<int>> makeTest (int row, int col){
    int i = 0;
    int j = 0;
    int n1;
    //I initialized 3x3 vector just for ease of use
    vector<vector<int>> puzzle{
        {0,0,0},
        {0,0,0},
        {0,0,0}
    };
    
    for ( i = 0; i < row; ++i){
        for ( j = 0; j < col; ++j){
            cout << "Type row " << i + 1 << " column " << j + 1 << " value: ";
            cin >> n1;
            puzzle[i][j] = n1;
        }
    }
    printPuzzle(puzzle);
    return puzzle;
}


int main(int argc, const char * argv[]) {
    
    Problem problem;
    node *Test = new node;
    string input;
    int num;
    
    cout << "~Welcome to Karan's 8-Puzzle Search Algorithm Demonstration~\n\n" << "Would you like to pick a default puzzle or enter your own?\n" << endl;
    cout << "Press 1 to see the default cases or Press 2 to enter your own\nPress Enter after: " << endl;
    cin >> input;
    while ( !(input == "1" || input == "2") ){
        cout << "Please enter a valid input\n";
        cin >> input;
    }
    
    if (input == "1"){
        cout << "Press 1: Depth 0, Press 2: Depth 2, Press 3: Depth 4, Press 4: Depth 8,\nPress 5: Depth 12, Press 6: Depth 16, Press 7: Depth 20, Press 8: Depth 24" << endl;
        cin >> num;
        while ( !(num == 1 || num == 2 || num == 3 || num == 4 || num == 5 || num == 6 || num == 7 || num == 8) ){
            cout << "Please enter a valid input\n";
            cin >> input;
        }
        problem.INITIALSTATE = getTest(num);
        
    }
    else{
        problem.INITIALSTATE = makeTest(3, 3);
    }
    
    cout << "Which Algorithm would you like to use?\n0 for Uniform Cost Distance, 1 for A* Misplaced Tile, 2 for A* Manhattan Distance" << endl;
    cin >> num;
    while ( !(num == 0 || num == 1 || num == 2)){
        cout << "Please enter a valid number: ";
        cin >> num;
    }
    
    QUEUEING_FUNCTION QUEUEINGFUNCTION;
    QUEUEINGFUNCTION.type = num;
    Test = general_search(problem, QUEUEINGFUNCTION);
    
    printPuzzle(Test->STATE);
    
    
    cout << endl;
    
    return 0;
}
