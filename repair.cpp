#include <deque>
#include <iostream>
#include "/System/Volumes/Data/opt/homebrew/Cellar/raylib/5.5/include/raymath.h"
#include <map>
#include <set>


using namespace std;

int gridWidth  = 6;
int gridHeight = 5;
int totalSquares = 30;
deque <Vector2> visited = {};


Vector2 startCoord = Vector2{0, 0};
Vector2 endCoord = Vector2{0, 1};

class Graph {
private:
    map <Vector2, vector <Vector2>, Vector2Compare> adjacencyList;
    set <Vector2, Vector2Compare> vertices;

public:
    // Add a vertex to the graph
    void addVertex(Vector2 v) {
        vertices.insert(v);
        if (adjacencyList.find(v) == adjacencyList.end()) {
            adjacencyList[v] = {};
        }
    }

    // Add an edge between two vertices (bidirectional)
    void addEdge(Vector2 a, Vector2 b) {
        adjacencyList[a].push_back(b);
        adjacencyList[b].push_back(a);
    }

    // Get neighbors of a vertex
    vector <Vector2> getNeighbors(Vector2 v) const {
        auto it = adjacencyList.find(v);
        if (it != adjacencyList.end()) {
            return it->second;
        }
        return {};
    }

    // Get all vertices
    set <Vector2, Vector2Compare> getVertices() const {
        return vertices;
    }

    // Check if vertex exists
    bool hasVertex(Vector2 v) const {
        return vertices.find(v) != vertices.end();
    }

    // Get degree (number of neighbors)
    int degree(Vector2 v) const {
        auto it = adjacencyList.find(v);
        return (it != adjacencyList.end()) ? it->second.size() : 0;
    }
};



struct Vector2Compare {
    bool operator()(const Vector2 a, const Vector2 b) const {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    }
};

Graph BuildGraph() {
    Graph g;

    vector<Vector2> shape = {
        {0, 0}, {1, 0}, {2, 0}, {3, 0},
        {0, 1}, {1, 1}, {2, 1}, {3, 1},
        {0, 2}, {1, 2}, {2, 2}, {3, 2},
        {0, 3}, {1, 3}, {2, 3}, {3, 3}
    };
    
    // Add all vertices
    for (Vector2 v : shape) {
        g.addVertex(v);
    }
    
    // Add edges based on adjacency (4-directional)
    for (Vector2 v : shape) {
        Vector2 directions[] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
        
        for (Vector2 dir : directions) {
            Vector2 neighbor = Vector2Add(v, dir);
            if (g.hasVertex(neighbor)) {
                g.addEdge(v, neighbor);
            }
        }
    }
    
    return g;
}



bool IsValidTile(Vector2 coord) {

    if (coord.x < 0 or coord.y < 0) { // Out of bounds
        return false;
    }

    else if (coord.x >= gridWidth or coord.y >= gridHeight) { // Out of bounds
        return false;
    }

    else if (find(visited.begin(), visited.end(), coord) != visited.end()) { // Look for the coord in the list of coords already visited
        return false;
    }

    else if (visited.size() < totalSquares - 1 && coord == endCoord) { // End coord is not valid unless it is the last vertex.
        return false;
    }

    return true;
}



deque <Vector2> ValidNeighbors(Vector2 coord) { // Valid means that the snake CAN move there, not necessarily should
    deque <Vector2> neighbors {};

    Vector2 directions[] = {
        {0, -1},
        {0, 1},
        {-1, 0},
        {1, 0}
    };

    for (Vector2 direction : directions) {

        Vector2 next = Vector2Add(coord, direction);

        if (IsValidTile(next)) {
            neighbors.push_back(next);
        }
    }
    
    return neighbors;
};



int AmountOfValidNeighbors(Vector2 coord) {
    deque <Vector2> neighbors = ValidNeighbors(coord);

    return neighbors.size();
}



deque <Vector2> FindHamiltonianPath() {
    Vector2 currentCoord = startCoord;
    visited.push_back(startCoord);
    deque <Vector2> lastDecisionPoints = {startCoord};

    map <Vector2, deque <Vector2>, Vector2Compare> availableMoves;

    while (visited.size() < totalSquares || currentCoord != endCoord) {

        if (availableMoves.find(currentCoord) == availableMoves.end()) { // Compute available moves for coordinate if not already done.
            availableMoves[currentCoord] = ValidNeighbors(currentCoord);
        }

        deque <Vector2>& moves = availableMoves[currentCoord]; // This is a reference and not a copy because of the "&" symbol.

        cout << "current: " << "(" << currentCoord.x << ", " << currentCoord.y << ")" << " Available: ";
        for (Vector2 move : moves) {
            cout << "(" << move.x << ", " << move.y << ") ";
        }
        cout << endl;

        if (!moves.empty()) {
            int AmountOfValidMoves = moves.size();

            if (AmountOfValidMoves >= 2) {
                lastDecisionPoints.push_back(currentCoord);
            }
            
            int fewestValidNeighbors = INT_MAX;
            Vector2 bestMove = {};
            int bestMoveIdx = -1;

            for (int i = 0; i < moves.size(); i++) {
                int ValidNeighborsCount = AmountOfValidNeighbors(moves[i]);

                if (ValidNeighborsCount < fewestValidNeighbors) {
                    fewestValidNeighbors = ValidNeighborsCount;
                    bestMove = moves[i];
                    bestMoveIdx = i;
                }
            }

            moves.erase(availableMoves[currentCoord].begin() + bestMoveIdx);
            visited.push_back(bestMove);
            currentCoord = bestMove;
        }
        else {
            if (visited.size() <= 1) {
                cout << "No Hamiltonian path found!" << endl;
                return {};
            }
            
            cout << "Backtracking from (" << currentCoord.x << ", " << currentCoord.y << ")" << endl;

            // Remove data on the tiles that we are backtracking past while going to the last decision point.
            while (visited.back() != lastDecisionPoints.back()) {
                cout << "Erasing: " << "(" << visited.back().x << ", " << visited.back().y << ") " << endl;

                availableMoves.erase(visited.back());
                visited.pop_back();
            }

            currentCoord = lastDecisionPoints.back();
            lastDecisionPoints.pop_back();
        }
    }

    return visited;
}












int main() {
    deque <Vector2> path = FindHamiltonianPath();
    Graph graph = BuildGraph();

    for (auto coord : path) {
        cout << "(" << coord.x << ", " << coord.y << ")" << endl;
    }
};
