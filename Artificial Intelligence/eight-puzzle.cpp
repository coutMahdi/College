#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;
int counter = 0;
const int SIZE = 3;
const int GOAL[SIZE][SIZE] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 0}
};
struct PuzzleState {
    int values[SIZE][SIZE]; // Current state of the puzzle
    int x, y; // coordinates of the empty space (0)
    PuzzleState* parent = nullptr;
};
bool IDDFS1(PuzzleState&, int);
bool DLS1(PuzzleState&, int);

bool IDDFS2(PuzzleState&, int);
bool DLS2(PuzzleState&, int);

bool IDDFS3(const PuzzleState&, int);
bool DLS3(const PuzzleState&, int);

bool isGoal(const int[SIZE][SIZE]);
bool isPossible_Position(int, int);
bool isRepeating_state(PuzzleState);

void initialize_0_coordinates(PuzzleState&);
bool is_Equal(PuzzleState, PuzzleState);
// int count_childs(PuzzleState);
// void swap(int&, int&);

int main() {
    PuzzleState start = {
        {
        {6, 8, 0},
        {7, 4, 2},
        {3, 5, 1}
        }
    };
    
    initialize_0_coordinates(start);

    // Measure performance of IDDFS version 1
    auto start_time = high_resolution_clock::now();
    bool found = IDDFS1(start, 75); // Set maxDepth according to problem constraints
    auto end_time = high_resolution_clock::now();
    duration<double> elapsed = end_time - start_time;
    cout << "IDDFS1: Goal " << (found ? "found" : "not found") << " in " << elapsed.count() << " seconds.\n";

    // Measure performance of IDDFS version 2
    start_time = high_resolution_clock::now();
    found = IDDFS2(start, 18); // Set maxDepth according to problem constraints
    end_time = high_resolution_clock::now();
    elapsed = end_time - start_time;
    cout << "IDDFS2: Goal " << (found ? "found" : "not found") << " in " << elapsed.count() << " seconds.\n";

    return 0;
}
//------------------------------------------------core functions---------------------------------------
//------------------------------------------------IDDFS Version 1--------------------------------------
bool IDDFS1(PuzzleState& root, int maxDepth = 16) {
    for (int depth = 0; depth <= maxDepth; ++depth) {
        if (DLS1(root, depth)) return true;
    }
    return false;
}
bool DLS1(PuzzleState& current_state, int depth) {
    if (isGoal(current_state.values)) return true;
    if (depth == 0) return false;
    PuzzleState* successors[4];
    int count = 0;//successorCount 2,3 or 4
    // Possible moves (right, left, down, up)
    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};
    for (int i = 0; i < 4; ++i) {
        int newX = current_state.x + dx[i];
        int newY = current_state.y + dy[i];
        if (isPossible_Position(newX,newY)) {
            if(isRepeating_state(current_state)) continue;
            // Copy
            successors[count] = new PuzzleState(current_state);//heap memory allocation
            successors[count]->parent = &current_state;
            // Edit the new state
            swap(successors[count]->values[current_state.x][current_state.y], successors[count]->values[newX][newY]);
            successors[count]->x = newX;
            successors[count]->y = newY;
            count++;
        }
    }
    for (int i = 0; i < count; ++i) {
        if (DLS1(*successors[i], depth - 1)) return true;
    }
    // Free memory
    for (int i = 0; i < count; ++i) {
        delete successors[i];
    }
    return false;
}
//------------------------------------------------IDDFS Version 2--------------------------------------
bool IDDFS2(PuzzleState& root, int maxDepth = 16) {
    for (int depth = 0; depth <= maxDepth; ++depth) {
        if (DLS2(root, depth)) return true;
    }
    return false;
}
bool DLS2(PuzzleState& current_state, int depth) {
    if (isGoal(current_state.values)) return true;  // Goal check
    if (depth == 0) return false;                  // Depth limit check
    // Possible moves (down, up, right, left)
    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};
    for (int i = 0; i < 4; ++i) {
        int newX = current_state.x + dx[i];
        int newY = current_state.y + dy[i];
        if (isPossible_Position(newX,newY)) {
            swap(current_state.values[current_state.x][current_state.y], current_state.values[newX][newY]);

            // Backup = Store empty space old coordinates before updating for probable backtrack
            int oldX = current_state.x, oldY = current_state.y;
            // Update empty space coordinates
            current_state.x = newX;
            current_state.y = newY;
            
            if (DLS2(current_state, depth - 1)) return true;
            
            // 1-level Backtrack to parent = Undo the move
            swap(current_state.values[current_state.x][current_state.y], current_state.values[oldX][oldY]);
            current_state.x = oldX;
            current_state.y = oldY;
        }
    }
    return false;
}
//------------------------------------------------important Checkings----------------------------------
bool isGoal(const int state[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (state[i][j] != GOAL[i][j]) return false;
        }
    }
    return true;
}
bool isPossible_Position(int newX, int newY){//0,1,2 are only valid coordinates for both x,y
    return (newX >= 0 && newX < SIZE && newY >= 0 && newY < SIZE);
}
bool isRepeating_state(PuzzleState leaf){
    if(leaf.parent == nullptr) return false;
    PuzzleState* finder = leaf.parent;
    while(finder->parent != nullptr){
        if (is_Equal(leaf, *finder->parent)){counter++; return true;}
        finder = finder->parent;
    }
    return false;
}
// to test isRepeating_state add this snippet to main
/*
    PuzzleState child1 = start;
    swap(child1.values[2][2], child1.values[2][1]);
    child1.x = 0;
    child1.y = 1;
    child1.parent = &start;
    PuzzleState child2 = start;
    child2.parent = &child1;
    cout << isRepeating_state(child1);
*/
//-------------------------------------------------Supporting Functions--------------------------------
// Find the position of the blank tile(empty space)
void initialize_0_coordinates(PuzzleState& root){
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (root.values[i][j] == 0) {
                root.x = i;
                root.y = j;
            }
}
bool is_Equal(PuzzleState s1, PuzzleState s2){
    for (int i = 0; i < 3; ++i) 
        for (int j = 0; j < 3; ++j)
            if (s1.values[i][j] != s2.values[i][j])
                return false;
    return true;
}
// int count_childs(PuzzleState parent){//2,3 or 4
//     if (parent.x == 1 && parent.y == 1) return 4;    
//     if (parent.x != 1 && parent.y != 1) return 2;  
//     return 3;
// }
// std::swap  :|
// void swap(int& a, int& b) {
//     int temp = a;
//     a = b;
//     b = temp;
// }
//------------------------------------------------IDDFS Version 3--------------------------------------
bool IDDFS3(const PuzzleState& root, int maxDepth = 16) {
    for (int depth = 0; depth <= maxDepth; ++depth) {
        if (DLS3(root, depth)) return true;
    }
    return false;
}
bool DLS3(const PuzzleState& current_state, int depth) {
    if (isGoal(current_state.values)) return true;
    if (depth == 0) return false;
    PuzzleState successors[4];
    int count = 0;//successorCount 2,3 or 4
    // Possible moves (right, left, down, up)
    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};
    for (int i = 0; i < 4; ++i) {
        int newX = current_state.x + dx[i];
        int newY = current_state.y + dy[i];
        if (isPossible_Position(newX,newY)) {
            // Copy
            successors[count] = current_state;            
            // Edit
            swap(successors[count].values[current_state.x][current_state.y], successors[count].values[newX][newY]);
            successors[count].x = newX;
            successors[count].y = newY;
            count++;
        }
    }
    for (int i = 0; i < count; ++i) {
        if (DLS3(successors[i], depth - 1)) return true;
    }
    return false;
}
/*
Explanations:
chrono is a namespace in chrono header
high_resolution_clock is a class in chrono namespace
now() is a static member function in high_resolution_clock class

duration is a Templated class:(Rep REPresents number of ticks --> defined as arithmetic types --> for precision --> double)
    template<
    class Rep, //tick count
    class Period = std::ratio<1> //rational number//tick period = 1 second (if(60) 1 minute)
    > class duration;
The only data stored in a duration is a tick count of type Rep. If Rep is floating point, then the duration can represent fractions of ticks. Period is included as part of the duration's type, and is only used when converting between different durations.
.count() is a member function of duration class returns The number of ticks for this duration.
timepoint is a typedef

auto datatype for automatic type deduction --> can be used instead of --> high_resolution_clock::time_point
*/