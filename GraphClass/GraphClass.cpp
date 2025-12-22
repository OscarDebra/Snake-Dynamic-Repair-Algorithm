#include <deque>
#include <iostream>
#include "/System/Volumes/Data/opt/homebrew/Cellar/raylib/5.5/include/raymath.h"
#include <map>
#include <set>
#include "GraphClass.h"

using namespace std;


void Graph::printGraph() const {
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


void Graph::AddVertex(Vector2 v) {
    vertices.insert(v);

    if (!HasVertex(v)) {
        adjacencyList[v] = {};
    }
}


void Graph::AddEdge(Vector2 a, Vector2 b) {
    adjacencyList[a].push_back(b);
}


vector <Vector2> Graph::GetNeighbors(Vector2 v) const {
    auto it = adjacencyList.find(v);

    if (it != adjacencyList.end()) {
        return (*it).second;
    }

    return {};
}


set <Vector2, Graph::Vector2Compare> Graph::GetVertices() const {
    return vertices;
}


bool Graph::HasVertex(Vector2 v) const {
    return vertices.find(v) != vertices.end();
}


int Graph::Degree(Vector2 v) const { // Amount of neighbours
    auto it = adjacencyList.find(v);
    return (it != adjacencyList.end()) ? (*it).second.size() : 0; // "Second" is the Value of the map, first is the key.
}


bool Graph::IsValidTile(Vector2 v, deque <Vector2> visited, int totalVertices, Vector2 end) {

    if (find(visited.begin(), visited.end(), v) != visited.end()) { // Look for the coord in the list of coords already visited
        return false;
    }

    if (visited.size() < totalVertices - 1 && v == end) { // End coord is not valid unless it is the last vertex.
        return false;
    }

    return true;
}


vector <Vector2> Graph::GetValidNeighbors(Vector2 v, deque <Vector2> visited, int totalVertices, Vector2 end) {
    vector <Vector2> neighbors {};

    vector<Vector2> graphNeighbors = GetNeighbors(v);

    for (Vector2 neighbor : graphNeighbors) {

        if (IsValidTile(neighbor, visited, totalVertices, end)) {
            neighbors.push_back(neighbor);
        }
    }

    return neighbors;
};