#pragma once
#include <deque>
#include "/System/Volumes/Data/opt/homebrew/Cellar/raylib/5.5/include/raymath.h"
#include <map>
#include <set>

using namespace std;





class Graph {
public:
    struct Vector2Compare {
        bool operator()(const Vector2 a, const Vector2 b) const {
            if (a.x != b.x) return a.x < b.x;
            return a.y < b.y;
        }
    };


    map <Vector2, vector <Vector2>, Vector2Compare> adjacencyList;
    set <Vector2, Vector2Compare> vertices;

    void printGraph() const;
    void AddVertex(Vector2 v);
    void AddEdge(Vector2 a, Vector2 b);
    vector <Vector2> GetNeighbors(Vector2 v) const;
    set <Vector2, Vector2Compare> GetVertices() const;
    bool HasVertex(Vector2 v) const;
    int Degree(Vector2 v) const;
    bool IsValidTile(Vector2 v, deque <Vector2> visited, int totalVertices, Vector2 end);
    vector <Vector2> GetValidNeighbors(Vector2 v, deque <Vector2> visited, int totalVertices, Vector2 end);
};