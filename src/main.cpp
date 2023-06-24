#include "raylib.h"
#include <vector>
#include <queue>
#include <set>
#include <string>
#include <algorithm>

using namespace std;

typedef struct {
    int x, y;
    set<int> neighbors;
} Node;

int* BFS(vector<Node> Q, int v) {
    queue<int> que;
    que.push(v);

    bool visited[Q.size()];

    for (unsigned int i = 0; i < Q.size(); i++) {
        visited[i] = false;
    }

    visited[v] = true;

    int* prev = new int [Q.size()];

    for (unsigned int i = 0; i < Q.size(); i++) {
        prev[i] = -1;
    }

    while (!que.empty()) {
        int node = que.front();
        que.pop();
        set<int> neigh = Q.at(node).neighbors;

        for (auto e : neigh) {
            if (!visited[e]) {
                que.push(e);
                visited[e] = true;
                prev[e] = node;
            }
        }
    }
    return prev;
}

vector<int> Reconstruct(int s, int e, int* prev, vector<Node> Q) {
    vector<int> path;
    for (int i = e; i != -1; i = prev[i]) {
        path.push_back(i);
    }
    
    reverse(path.begin(), path.end());

    if (path[0] == s) {
        return path;
    }
    return {};
}

vector<int> ShortestPath(int s, int e, vector<Node> Q) {
    int* prev = BFS(Q, s); 

    return Reconstruct(s, e, prev, Q);
}

bool CheckObInVec(vector<int> p, int s) {
    for (int i : p) {
        if (i == s) {
            return true;
        }
    }
    return false;
}

vector<int> path;

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Graphs");

    vector<Node> nodes;

    SetTargetFPS(60);

    bool pressed = false;

    int first;

    while (!WindowShouldClose())
    {
        if (IsMouseButtonPressed(0)) {
            Node nd;
            nd.x = GetMouseX();
            nd.y = GetMouseY();
            nodes.push_back(nd);
        }


        if (IsMouseButtonPressed(1)) {
            int i = 0;
            for (auto e: nodes) {
                bool isNode = GetMouseX() < e.x + 30 &&
                              GetMouseX() > e.x &&
                              GetMouseY() < e.y + 30 &&
                              GetMouseY() > e.y;
                if (isNode) {
                    first = i;
                    pressed = true;
                    break;
                }
                i++;
            }
        }

        if (IsMouseButtonReleased(1) && pressed) {
            pressed = false;
            int i = 0;
            for (auto e: nodes) {
                bool isNode = GetMouseX() < e.x + 30 &&
                              GetMouseX() > e.x &&
                              GetMouseY() < e.y + 30 &&
                              GetMouseY() > e.y;
                if (isNode && i != first) {
                    nodes.at(i).neighbors.insert(first);
                    nodes.at(first).neighbors.insert(i);
                    path = ShortestPath(0, nodes.size() - 1, nodes);
                    break;
                }
                i++;
            }
        }

        BeginDrawing();
            ClearBackground(BLACK);

            for (unsigned int i = 0; i < nodes.size(); i++) {
                DrawText(to_string(i).c_str(), nodes.at(i).x, nodes.at(i).y, 30, BLUE);
                set<int> neig = nodes.at(i).neighbors;
                for (auto e: nodes.at(i).neighbors) {
                    if(CheckObInVec(path, i) && CheckObInVec(path, e)) {
                        DrawLine(nodes.at(i).x, nodes.at(i).y, nodes.at(e).x, nodes.at(e).y, GREEN);
                    } else {
                        DrawLine(nodes.at(i).x, nodes.at(i).y, nodes.at(e).x, nodes.at(e).y, RAYWHITE);
                    }
                }
            }

        EndDrawing();
    }

    CloseWindow();
}
