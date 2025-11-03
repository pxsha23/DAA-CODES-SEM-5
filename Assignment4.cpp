// In a modern smart city infrastructure, efficient emergency response is critical to saving lives. One
// major challenge is ensuring that ambulances reach hospitals in the shortest possible time, especially
// during peak traffic conditions. The city's road network can be modeled as a graph, where
// intersections are represented as nodes and roads as edges with weights indicating real-time travel
// time based on current traffic congestion.
// The goal is to design and implement an intelligent traffic management system that dynamically
// computes the fastest route from an ambulance’s current location (source node S) to the nearest
// hospital (destination node D) in the network. Due to ever-changing traffic conditions, edge weights
// must be updated in real-time, requiring the system to adapt and re-compute optimal routes as
// necessary.

#include <bits/stdc++.h>
using namespace std;

void dijkstra(int startNode, vector<vector<pair<int, int>>> &adjList, vector<int> &distances, vector<int> &prev) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, startNode});
    distances[startNode] = 0;

    while (!pq.empty()) {
        int currDist = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (currDist > distances[u])
            continue;

        for (auto &[v, w] : adjList[u]) {
            if (distances[v] > distances[u] + w) {
                distances[v] = distances[u] + w;
                prev[v] = u;
                pq.push({distances[v], v});
            }
        }
    }
}

int main() {
    int numIntersections, numRoads;
    cout << "Enter number of intersections (vertices): ";
    cin >> numIntersections;

    cout << "Enter number of roads (edges): ";
    cin >> numRoads;

    vector<vector<pair<int, int>>> graph(numIntersections);
    cout << "\nEnter the edges in format (u v w) - u: start, v: end, w: travel time:\n";
    for (int i = 0; i < numRoads; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
    }

    int numHospitals;
    cout << "\nEnter number of hospitals: ";
    cin >> numHospitals;

    vector<int> hospitals(numHospitals);
    cout << "Enter hospital nodes:\n";
    for (int i = 0; i < numHospitals; i++) {
        cin >> hospitals[i];
    }

    int startLocation;
    cout << "\nEnter your start location (source node): ";
    cin >> startLocation;

    vector<int> distances(numIntersections, INT_MAX);
    vector<int> prev(numIntersections, -1);
    dijkstra(startLocation, graph, distances, prev);

    int nearestHospital = -1;
    int minTime = INT_MAX;

    for (int h : hospitals) {
        if (distances[h] < minTime) {
            minTime = distances[h];
            nearestHospital = h;
        }
    }

    if (nearestHospital == -1) {
        cout << "No hospital is reachable!\n";
    } else {
        cout << "Nearest hospital: " << nearestHospital << " reachable in " << minTime << " minutes.\n";
        cout << "Path: ";
        stack<int> path;
        int curr = nearestHospital;
        while (curr != -1) {
            path.push(curr);
            curr = prev[curr];
        }
        while (!path.empty()) {
            cout << path.top() << " ";
            path.pop();
        }
        cout << "\n";
    }

    return 0;
}
