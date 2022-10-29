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
#include <stack>
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
    Point zeroPoint;
    //int g;
    int f;
    
};

//Helps me sort by h to create proper queue

//Helps me with sorting children when two or more children have equal h values
struct lesser_f{
    inline bool operator() (const node* n1, const node* n2){
        return (n1->f < n2->f);
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
        {4,1,2},
        {5,3,0},
        {7,8,6}
    };
    
    Point zeroOriginalPosition = findPos(INITIALSTATE, 0);
    
    
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
    vector<node*> sortedOrder;
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
            sort(totalStatesList.begin(), totalStatesList.end(), lesser_f());
            
            if (puzzle->f < (totalStatesList.at(totalStatesList.size()/2)->f )){
                for (unsigned i = 0; i <= totalStatesList.size()/2; ++i){
                    if ( puzzle->STATE == totalStatesList.at(i)->STATE ){
                        return true;
                    }
                    if ( puzzle->f < totalStatesList.at(i)->f ){
                        return false;
                    }
                }
            }
            else{
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
    
    void calculateH(node* headNode){
        headNode->h = 0;
    }
        
    queue<node*> EXPAND( node* headNode, Problem problem){
        Point zeroPos = findPos(headNode->STATE,0);
        vector<node*> sortedOrder;
        
        if ( zeroPos.x != 0 ){
            node *up = new node;
            //up->depth = headNode->depth + 1;
            up->zeroPoint.x = zeroPos.x - 1;
            up->zeroPoint.y = zeroPos.y;
            up->STATE = problem.moveUp(headNode->STATE, zeroPos);
            calculateH(up);
            if (!checkRepeatedStates(up, statesList)){
                cout << "Up State: " << endl;
                printPuzzle(up->STATE);
                sortedOrder.push_back(up);
            }
        }
        if ( zeroPos.x != 2 ){
            node *down = new node;
            //down->depth = headNode->depth + 1;
            down->zeroPoint.x = zeroPos.x + 1;
            down->zeroPoint.y = zeroPos.y;
            down->STATE = problem.moveDown(headNode->STATE, zeroPos);
            calculateH(down);
            if (!checkRepeatedStates(down, statesList)){
                cout << "Down State: " << endl;
                printPuzzle(down->STATE);
                sortedOrder.push_back(down);
            }
        }
        if ( zeroPos.y != 0 ){
            node *left = new node;
            //left->depth = headNode->depth + 1;
            left->zeroPoint.x = zeroPos.x;
            left->zeroPoint.y = zeroPos.y - 1;
            left->STATE = problem.moveLeft(headNode->STATE, zeroPos);
            calculateH(left);
            if (!checkRepeatedStates(left, statesList)){
                cout << "Left State: " << endl;
                printPuzzle(left->STATE);
                sortedOrder.push_back(left);
            }
        }
        if ( zeroPos.y != 2 ){
            node *right = new node;
            //right->depth = headNode->depth + 1;
            right->zeroPoint.x = zeroPos.x;
            right->zeroPoint.y = zeroPos.y + 1;
            right->STATE = problem.moveRight(headNode->STATE, zeroPos);
            calculateH(right);
            if (!checkRepeatedStates(right, statesList)){
                cout << "Right State: " << endl;
                printPuzzle(right->STATE);
                sortedOrder.push_back(right);
            }
        }
        
        sort(sortedOrder.begin(), sortedOrder.end(), lesser_f());
        
        
        for (unsigned i = 0; i < sortedOrder.size(); ++i){
            //if ( !checkRepeatedStates(sortedOrder.at(i), statesList)){
                //cout << "H equals: " << sortedOrder.at(i)->h << endl;
                statesList.push_back(sortedOrder.at(i));
                localNodes.push(sortedOrder.at(i));
            //}
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
        Point zeroPos = headNode->zeroPoint;
        //findPos(headNode->STATE,0);
        if ( zeroPos.x != 0 ){
            node *up = new node;
            //up->depth = headNode->depth + 1;
            up->zeroPoint.x = zeroPos.x - 1;
            up->zeroPoint.y = zeroPos.y;
            up->STATE = problem.moveUp(headNode->STATE, zeroPos);
            calculateH(up);
            sortedOrder.push_back(up);
        }
        if ( zeroPos.x != 2 ){
            node *down = new node;
            //down->depth = headNode->depth + 1;
            down->zeroPoint.x = zeroPos.x + 1;
            down->zeroPoint.y = zeroPos.y;
            down->STATE = problem.moveDown(headNode->STATE, zeroPos);
            calculateH(down);
            sortedOrder.push_back(down);
        }
        if ( zeroPos.y != 0 ){
            node *left = new node;
            //left->depth = headNode->depth + 1;
            left->zeroPoint.x = zeroPos.x;
            left->zeroPoint.y = zeroPos.y - 1;
            left->STATE = problem.moveLeft(headNode->STATE, zeroPos);
            calculateH(left);
            sortedOrder.push_back(left);
        }
        if ( zeroPos.y != 2 ){
            node *right = new node;
            //right->depth = headNode->depth + 1;
            right->zeroPoint.x = zeroPos.x;
            right->zeroPoint.y = zeroPos.y + 1;
            right->STATE = problem.moveRight(headNode->STATE, zeroPos);
            calculateH(right);
            sortedOrder.push_back(right);
        }

        sort(sortedOrder.begin(), sortedOrder.end(), lesser_f());

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
    sum = abs(actual_i )+ abs(actual_j);
    return sum;
}

class AstarManhattanDistance: public QUEUEING_FUNCTION {
public:
    
    AstarManhattanDistance ( queue<node*> nodes) : QUEUEING_FUNCTION(nodes){
        localNodes = nodes;
    }
    
    //Calculate manhattan distance
    void calculateH(node* headNode){
        headNode->h = 0;
        //printPuzzle(headNode->STATE);
        cout << endl;
        for (unsigned i = 0; i < headNode->STATE.size(); i++){
            for (unsigned j = 0; j < headNode->STATE[i].size(); ++j){
                if ( headNode->STATE[i][j] != 0 && headNode->STATE[i][j] != puzzleSolution[i][j] ) {
                    //cout << "Number currently: " << headNode->STATE[i][j] << " Actual number: " << puzzleSolution[i][j] << endl;
                    headNode->h = headNode->h + calculateDistance(i, j, headNode->STATE[i][j], puzzleSolutionPoints);
                 //   cout << "H is currently: " << headNode->h << endl;;
                }
            }
        }
        //cout << "Final H: " << headNode->h << endl;
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
                //cout << "F equals: " << sortedOrder.at(i)->f << endl;
                //printPuzzle(sortedOrder.at(i)->STATE);
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
    
    //Simple function to make children
    /*
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
            calculateH(up);
            up->f = up->h + up->depth;
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
            calculateH(down);
            down->f = down->h + down->depth;
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
            calculateH(left);
            left->f = left->h + left->depth;
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
            calculateH(right);
            right->f = right->h + right->depth;
            if (!checkRepeatedStates(right, statesList)){
                //cout << "Right State: " << endl;
                //printPuzzle(right->STATE);
                sorted.push_back(right);
            }
        }
        
        return sorted;
        
    }
     */
    
//    void checkChildren(vector<node*> sorted1, Problem problem){
//        for ( unsigned i = 0; i < sorted1.size(); ++i ){
//            if ( sorted1.at(i)->childsmallestH < sorted1.at(0)->childsmallestH ){
//                iter_swap(sorted1.begin(), sorted1.begin()+i);
//            }
//        }
//    }
//
//    void calculateChildSmallestH(node* headNode, Problem problem){
//        int smallestH = 100;
//        if ( headNode->up != NULL ){
//            if ( headNode->up->h < smallestH){
//                smallestH = headNode->up->h;
//            }
//        }
//        if ( headNode->down != NULL ){
//            if ( headNode->down->h < smallestH){
//                smallestH = headNode->down->h;
//            }
//        }
//        if ( headNode->left != NULL ){
//            if ( headNode->left->h < smallestH){
//                smallestH = headNode->left->h;
//            }
//        }
//        if ( headNode->right != NULL ){
//            if ( headNode->right->h < smallestH){
//                smallestH = headNode->right->h;
//            }
//        }
//        headNode->childsmallestH = smallestH;
//    }
           
    
    
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
        
        AstarManhattanDistance QUEUEINGFUNCTION(nodes);
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
