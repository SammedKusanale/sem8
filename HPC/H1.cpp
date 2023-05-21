#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

// Function to perform parallel BFS
void parallelBFS(vector<vector<int>>& graph, int startNode) {
    int numNodes = graph.size();
    vector<bool> visited(numNodes, false); // Track visited nodes
    queue<int> bfsQueue;

    visited[startNode] = true;
    bfsQueue.push(startNode);

    while (!bfsQueue.empty()) {
        int currentSize = bfsQueue.size();

        #pragma omp parallel
        {
            #pragma omp for
            for (int i = 0; i < currentSize; ++i) {
                int currentNode;
                #pragma omp critical
                {
                    currentNode = bfsQueue.front();
                    bfsQueue.pop();
                }

                cout << "Processing node: " << currentNode << endl;

                // Process the node as needed

                // Enqueue unvisited neighbors
                for (int neighbor : graph[currentNode]) {
                    #pragma omp critical
                    {
                        if (!visited[neighbor]) {
                            visited[neighbor] = true;
                            bfsQueue.push(neighbor);
                        }
                    }
                }
            }
        }
    }
}

// Function to perform parallel DFS
void parallelDFS(vector<vector<int>>& graph, int startNode) {
    int numNodes = graph.size();
    vector<bool> visited(numNodes, false); // Track visited nodes
    stack<int> dfsStack;

    dfsStack.push(startNode);

    while (!dfsStack.empty()) {
        int currentNode;
        #pragma omp critical
        {
            currentNode = dfsStack.top();
            dfsStack.pop();
        }

        if (!visited[currentNode]) {
            visited[currentNode] = true;
            cout << "Processing node: " << currentNode << endl;

            // Process the node as needed

            // Push unvisited neighbors to the stack
            #pragma omp parallel for
            for (int neighbor : graph[currentNode]) {
                #pragma omp critical
                {
                    if (!visited[neighbor]) {
                        dfsStack.push(neighbor);
                    }
                }
            }
        }
    }
}

int main() {
    int numNodes = 7;
    vector<vector<int>> graph(numNodes);

    // Add edges to the graph
    graph[0] = {1, 2};
    graph[1] = {0, 3, 4};
    graph[2] = {0, 5, 6};
    graph[3] = {1};
    graph[4] = {1};
    graph[5] = {2};
    graph[6] = {2};

    // Perform parallel BFS
    cout << "Parallel BFS:" << endl;
    parallelBFS(graph, 0);

    cout << endl;

    // Perform parallel DFS
    cout << "Parallel DFS:" << endl;
    parallelDFS(graph, 0);

    return 0;
}
