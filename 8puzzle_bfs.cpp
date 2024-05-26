#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

// Define the 3x3 puzzle board as a 2D array
typedef vector<vector<int>> PuzzleState;

// Define the possible actions (movements)
enum Action { UP, DOWN, LEFT, RIGHT };

// Define a map to convert actions to string representations
map<Action, string> actionStrings = {
    {UP, "UP"},
    {DOWN, "DOWN"},
    {LEFT, "LEFT"},
    {RIGHT, "RIGHT"}
};

// Function to print the puzzle board
void printPuzzle(const PuzzleState &state) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << state[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// Function to check if a state is the goal state
bool isGoalState(const PuzzleState& state, const PuzzleState& goalState) {
    return state == goalState;
}

// Function to get valid actions for a given state
vector<Action> getValidActions(const PuzzleState& state) {
    vector<Action> validActions;

    // Find the position of the empty space (0)
    int emptyRow = -1, emptyCol = -1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (state[i][j] == 0) {
                emptyRow = i;
                emptyCol = j;
            }
        }
    }

    // Check if UP action is valid
    if (emptyRow > 0) {
        validActions.push_back(UP);
    }

    // Check if DOWN action is valid
    if (emptyRow < 2) {
        validActions.push_back(DOWN);
    }

    // Check if LEFT action is valid
    if (emptyCol > 0) {
        validActions.push_back(LEFT);
    }

    // Check if RIGHT action is valid
    if (emptyCol < 2) {
        validActions.push_back(RIGHT);
    }

    return validActions;
}

// Function to apply an action and get the resulting state
PuzzleState applyAction(const PuzzleState& state, Action action) {
    PuzzleState newState = state;

    // Find the position of the empty space (0)
    int emptyRow = -1, emptyCol = -1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (state[i][j] == 0) {
                emptyRow = i;
                emptyCol = j;
            }
        }
    }

    // Apply the action to swap the empty space with an adjacent tile
    if (action == UP) {
        swap(newState[emptyRow][emptyCol], newState[emptyRow - 1][emptyCol]);
    } else if (action == DOWN) {
        swap(newState[emptyRow][emptyCol], newState[emptyRow + 1][emptyCol]);
    } else if (action == LEFT) {
        swap(newState[emptyRow][emptyCol], newState[emptyRow][emptyCol - 1]);
    } else if (action == RIGHT) {
        swap(newState[emptyRow][emptyCol], newState[emptyRow][emptyCol + 1]);
    }

    return newState;
}

// Function to calculate the Manhattan distance heuristic
int manhattanDistance(const PuzzleState& state, const PuzzleState& goalState) {
    int distance = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (state[i][j] != 0) {
                for (int x = 0; x < 3; x++) {
                    for (int y = 0; y < 3; y++) {
                        if (state[i][j] == goalState[x][y]) {
                            distance += abs(i - x) + abs(j - y);
                        }
                    }
                }
            }
        }
    }
    return distance;
}

// Best-First Search function
bool bestFirstSearch(const PuzzleState& initialState, const PuzzleState& goalState) {
    auto compare = [&](const pair<PuzzleState, vector<Action>>& a, const pair<PuzzleState, vector<Action>>& b) {
        return manhattanDistance(a.first, goalState) > manhattanDistance(b.first, goalState);
    };
    priority_queue<pair<PuzzleState, vector<Action>>, vector<pair<PuzzleState, vector<Action>>>, decltype(compare)> frontier(compare);
    set<PuzzleState> visited;

    // Enqueue the initial state with an empty path
    frontier.push({initialState, {}});

    while (!frontier.empty()) {
        auto currentPair = frontier.top();
        PuzzleState currentState = currentPair.first;
        vector<Action> currentPath = currentPair.second;
        frontier.pop();

        if (isGoalState(currentState, goalState)) {
            // Print the solution path
            PuzzleState tempState = initialState;
            for (const auto& action : currentPath) {
                tempState = applyAction(tempState, action);
                printPuzzle(tempState);
            }
            return true;
        }

        if (visited.find(currentState) == visited.end()) {
            visited.insert(currentState);
            vector<Action> validActions = getValidActions(currentState);

            for (Action action : validActions) {
                PuzzleState nextState = applyAction(currentState, action);
                if (visited.find(nextState) == visited.end()) {
                    vector<Action> newPath = currentPath;
                    newPath.push_back(action);
                    frontier.push({nextState, newPath});
                }
            }
        }
    }

    return false; // No solution found
}

// Function to read a puzzle state from user input
PuzzleState readPuzzleState() {
    PuzzleState state(3, vector<int>(3)); //board with 3 rows each containing a vector of 3 integers.
    cout << "Enter the puzzle state (3x3 matrix, use 0 for the empty space):" << endl;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cin >> state[i][j];
        }
    }
    return state;
}

int main() {
    // Read the initial state and goal state from user input
    cout << "Initial State:" << endl;
    PuzzleState initialState = readPuzzleState();
    
    cout << "Goal State:" << endl;
    PuzzleState goalState = readPuzzleState();

    // Call Best-First Search starting from the initial state
    bool solutionFound = bestFirstSearch(initialState, goalState);

    if (solutionFound) {
        cout << "Solution found." << endl;
    } else {
        cout << "No solution found." << endl;
    }

    return 0;
}
