#include <deque>
#include <iostream>
#include "/System/Volumes/Data/opt/homebrew/Cellar/raylib/5.5/include/raymath.h"
#include <map>
#include <set>

using namespace std;


struct Vector2Compare {
    bool operator()(const Vector2 a, const Vector2 b) const {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    }
};



class Graph {
public:
    map <Vector2, vector <Vector2>, Vector2Compare> adjacencyList;
    set <Vector2, Vector2Compare> vertices;


    void printGraph() const {
        cout << "=== Graph Adjacency List ===" << endl;
        cout << "Total vertices: " << vertices.size() << endl << endl;
        
        for (const auto& pair : adjacencyList) {
            Vector2 vertex = pair.first;
            const vector<Vector2>& neighbors = pair.second;
            
            cout << "(" << vertex.x << ", " << vertex.y << ") -> ";
            
            if (neighbors.empty()) {
                cout << "(no neighbors)";
            } else {
                for (auto neighbor: neighbors) {
                    cout << "(" << neighbor.x << ", " << neighbor.y << ") ";
                }
            }

            cout << endl;
        }

        cout << "=========================" << endl << endl;
    }


    void AddVertex(Vector2 v) {
        vertices.insert(v);

        if (!HasVertex(v)) {
            adjacencyList[v] = {};
        }
    }


    void AddEdge(Vector2 a, Vector2 b) {
        adjacencyList[a].push_back(b);
    }


    vector <Vector2> GetNeighbors(Vector2 v) const {
        auto it = adjacencyList.find(v);

        if (it != adjacencyList.end()) {
            return (*it).second;
        }

        return {};
    }


    set <Vector2, Vector2Compare> GetVertices() const {
        return vertices;
    }


    bool HasVertex(Vector2 v) const {
        return vertices.find(v) != vertices.end();
    }


    int Degree(Vector2 v) const { // Amount of neighbours
        auto it = adjacencyList.find(v);
        return (it != adjacencyList.end()) ? (*it).second.size() : 0; // "Second" is the Value of the map, first is the key.
    }

    bool IsValidTile(Vector2 v, deque <Vector2> visited, int totalVertices, Vector2 end) {

        if (find(visited.begin(), visited.end(), v) != visited.end()) { // Look for the coord in the list of coords already visited
            return false;
        }

        if (visited.size() < totalVertices - 1 && v == end) { // End coord is not valid unless it is the last vertex.
            return false;
        }

        return true;
    }


    vector <Vector2> GetValidNeighbors(Vector2 v, deque <Vector2> visited, int totalVertices, Vector2 end) {
        vector <Vector2> neighbors {};
      
        vector<Vector2> graphNeighbors = GetNeighbors(v);

        for (Vector2 neighbor : graphNeighbors) {

            if (IsValidTile(neighbor, visited, totalVertices, end)) {
                neighbors.push_back(neighbor);
            }
        }
        
        return neighbors;
    }



    int GetAmountOfValidNeighbors(Vector2 v, deque <Vector2> visited, int totalVertices, Vector2 end) {

        vector <Vector2> neighbors = GetValidNeighbors(v, visited, totalVertices, end);

        return neighbors.size();
    }

    bool IsValidGraph(Vector2 start, Vector2 end) {

        for (const auto pair : adjacencyList) { // Checking if vertices except for the start and end have only one neighbor.
            if (pair.second.size() < 2 && pair.first != start && pair.first != end) {
                return false;
            }   
        }
    
        return true;
    }
};



Graph BuildGraph(vector <Vector2> shape) {
    Graph g;

    // Add vertices
    for (Vector2 v : shape) {
        g.AddVertex(v);
    }
    
    // Add edges
    for (Vector2 v : shape) {
        Vector2 directions[] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
        
        for (Vector2 direction : directions) {
            Vector2 neighbor = Vector2Add(v, direction);

            if (g.HasVertex(neighbor)) {
                g.AddEdge(v, neighbor);
            }
        }
    }
    
    return g;
}



deque <Vector2> FindHamiltonianPath(Graph g, Vector2 start, Vector2 end) {

    if (g.IsValidGraph(start, end)) { // Running pruning before the search to rule out some graphs that cannot have a hamiltonian cycle.

        Vector2 current = start;
        int totalVertices = g.vertices.size();
        deque <Vector2> visited = {start};
        deque <Vector2> lastDecisionPoints = {start};
        map <Vector2, vector <Vector2>, Vector2Compare> availableMoves;

        while (visited.size() < totalVertices || current != end) {

            if (availableMoves.find(current) == availableMoves.end()) { // Compute available moves for coordinate if not already done.
                availableMoves[current] = g.GetValidNeighbors(current, visited, totalVertices, end);
            }

            vector <Vector2>& moves = availableMoves[current]; // This is a reference and not a copy because of the "&" symbol.

            cout << "current: " << "(" << current.x << ", " << current.y << ")" << " Available: ";
            for (Vector2 move : moves) {
                cout << "(" << move.x << ", " << move.y << ") ";
            }
            cout << endl;

            if (!moves.empty()) {
                int AmountOfValidMoves = moves.size();

                if (AmountOfValidMoves >= 2) {
                    lastDecisionPoints.push_back(current);
                }
                
                int fewestValidNeighbors = INT_MAX;
                Vector2 bestMove = {};
                int bestMoveIdx = -1;

                for (int i = 0; i < moves.size(); i++) {
                    int ValidNeighborsCount = g.GetAmountOfValidNeighbors(moves[i], visited, totalVertices, end);

                    if (ValidNeighborsCount < fewestValidNeighbors) {
                        fewestValidNeighbors = ValidNeighborsCount;
                        bestMove = moves[i];
                        bestMoveIdx = i;
                    }
                }

                moves.erase(availableMoves[current].begin() + bestMoveIdx);
                visited.push_back(bestMove);
                current = bestMove;
            }
            else {

                if (visited.size() <= 1) {
                    cout << "No Hamiltonian path found!" << endl;
                    return {};
                }
                
                cout << "Backtracking from (" << current.x << ", " << current.y << ")" << endl;

                // Remove data on the tiles that we are backtracking past while going to the last decision point.
                while (visited.back() != lastDecisionPoints.back()) {
                    cout << "Erasing: " << "(" << visited.back().x << ", " << visited.back().y << ") " << endl;

                    availableMoves.erase(visited.back());
                    visited.pop_back();
                }

                current = lastDecisionPoints.back();
                lastDecisionPoints.pop_back();
            }
        }

        return visited;
    }

    cout << "No hamiltonian cycle possible, determined via pruning" << endl;
    return {};
}












int main() {
    Vector2 startCoord = Vector2{-1, 2};
    Vector2 endCoord = Vector2{0, 1};

    vector <Vector2> shape = {
                            {2, 0}, {3, 0},
            {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1},
   {-1, 2}, {0, 2}, {1, 2},         {3, 2}, {4, 2},
                    {1, 3}, {2, 3}, {3, 3}, {4, 3},
                            {2, 4}, {3, 4}, {4, 4}
    };

    Graph graph = BuildGraph(shape);

    graph.printGraph();

    deque <Vector2> path = FindHamiltonianPath(graph, startCoord, endCoord);

    for (auto coord : path) {
        cout << "(" << coord.x << ", " << coord.y << ")" << endl;
    }
};
