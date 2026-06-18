#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <climits>

using namespace std;

// 1. Delivery Request Handling Using Priority Queue
enum Priority {
    ROUTINE = 1,
    EXPRESS = 2,
    EMERGENCY = 3
};

struct DeliveryRequest {
    int id;
    string destination;
    Priority priority;
    string item;

    // Max-heap: highest priority should be at the top
    bool operator<(const DeliveryRequest& other) const {
        return priority < other.priority;
    }
};

// 2. Drone Structure for Binary Search
struct Drone {
    int id;
    string status; // e.g., "Available", "Maintenance", "In Transit"
    int batteryLevel;

    // Sort by ID to enable binary search
    bool operator<(const Drone& other) const {
        return id < other.id;
    }
};

// 3. Network Graph for Route Optimization (Dijkstra) and Connectivity (BFS)
class DroneNetwork {
private:
    int numLocations;
    vector<string> locationNames;
    unordered_map<string, int> locationIds;
    vector<vector<pair<int, int>>> adjList; // pair<neighbor_id, weight>

public:
    DroneNetwork() : numLocations(0) {}

    void addLocation(const string& name) {
        if (locationIds.find(name) == locationIds.end()) {
            locationIds[name] = numLocations++;
            locationNames.push_back(name);
            adjList.resize(numLocations);
        }
    }

    void addRoute(const string& u, const string& v, int distance) {
        if (locationIds.find(u) == locationIds.end() || locationIds.find(v) == locationIds.end()) {
            return;
        }
        int idU = locationIds[u];
        int idV = locationIds[v];
        adjList[idU].push_back({idV, distance});
        adjList[idV].push_back({idU, distance}); // Undirected graph
    }

    // Route Optimization using Dijkstra's Algorithm
    void findShortestPath(const string& start, const string& destination) {
        if (locationIds.find(start) == locationIds.end() || locationIds.find(destination) == locationIds.end()) {
            cout << "Invalid locations for route optimization.\n";
            return;
        }

        int startId = locationIds[start];
        int destId = locationIds[destination];

        vector<int> dist(numLocations, INT_MAX);
        vector<int> parent(numLocations, -1);
        dist[startId] = 0;

        // Min-heap: pair<distance, node_id>
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, startId});

        while (!pq.empty()) {
            int u = pq.top().second;
            int d = pq.top().first;
            pq.pop();

            if (d > dist[u]) continue;

            if (u == destId) break; // Reached target destination

            for (auto& edge : adjList[u]) {
                int v = edge.first;
                int weight = edge.second;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }

        if (dist[destId] == INT_MAX) {
            cout << "No route found between " << start << " and " << destination << ".\n";
        } else {
            cout << "Shortest route from " << start << " to " << destination << " is " << dist[destId] << " distance units.\n";
            cout << "Path: ";
            vector<int> path;
            for (int curr = destId; curr != -1; curr = parent[curr]) {
                path.push_back(curr);
            }
            for (int i = path.size() - 1; i >= 0; --i) {
                cout << locationNames[path[i]] << (i > 0 ? " -> " : "");
            }
            cout << "\n";
        }
    }

    // Connectivity Verification using BFS
    void checkConnectivity(const string& start) {
        if (locationIds.find(start) == locationIds.end()) {
            cout << "Invalid starting location for connectivity check.\n";
            return;
        }

        int startId = locationIds[start];
        vector<bool> visited(numLocations, false);
        queue<int> q;

        q.push(startId);
        visited[startId] = true;
        int reachableCount = 0;

        cout << "Reachable locations from " << start << ":\n";

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            reachableCount++;

            cout << " - " << locationNames[u] << "\n";

            for (auto& edge : adjList[u]) {
                int v = edge.first;
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }

        if (reachableCount < numLocations) {
            cout << "WARNING: The network is NOT fully connected! Unreachable regions:\n";
            for (int i = 0; i < numLocations; i++) {
                if (!visited[i]) {
                    cout << " - " << locationNames[i] << "\n";
                }
            }
        } else {
            cout << "STATUS: All regions are fully connected.\n";
        }
    }
};

// Drone Search using Binary Search
void searchDrone(const vector<Drone>& fleet, int searchId) {
    int left = 0;
    int right = fleet.size() - 1;
    bool found = false;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (fleet[mid].id == searchId) {
            cout << "Drone Found! ID: " << fleet[mid].id 
                 << " | Status: " << fleet[mid].status 
                 << " | Battery: " << fleet[mid].batteryLevel << "%\n";
            found = true;
            break;
        } else if (fleet[mid].id < searchId) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    if (!found) {
        cout << "Drone with ID " << searchId << " not found in the fleet.\n";
    }
}

int main() {
    cout << "=================================================\n";
    cout << "   International Cargo Drone Delivery Network    \n";
    cout << "=================================================\n\n";

    // ---------------------------------------------------------
    // 1. Delivery Request Handling Using Priority Queue
    // ---------------------------------------------------------
    cout << "--- 1. Processing Delivery Requests (Priority Queue) ---\n";
    priority_queue<DeliveryRequest> deliveryQueue;
    
    deliveryQueue.push({101, "Remote Village A", EXPRESS, "Vaccines"});
    deliveryQueue.push({102, "Distribution Center B", ROUTINE, "Office Supplies"});
    deliveryQueue.push({103, "Hospital C", EMERGENCY, "Blood Bags"});
    deliveryQueue.push({104, "Clinic D", EXPRESS, "Antibiotics"});

    while (!deliveryQueue.empty()) {
        DeliveryRequest req = deliveryQueue.top();
        deliveryQueue.pop();
        string prioStr = (req.priority == EMERGENCY) ? "EMERGENCY" : (req.priority == EXPRESS) ? "EXPRESS" : "ROUTINE";
        cout << "[Processing] ID: " << req.id << " | Priority: " << prioStr 
             << " | Item: " << req.item << " | Dest: " << req.destination << "\n";
    }
    cout << "\n";

    // ---------------------------------------------------------
    // 2. Drone Search Using Binary Search
    // ---------------------------------------------------------
    cout << "--- 2. Fleet Monitoring (Binary Search) ---\n";
    vector<Drone> fleet = {
        {1001, "Available", 95},
        {1002, "In Transit", 45},
        {1003, "Maintenance", 15},
        {1004, "Available", 100},
        {1005, "In Transit", 60}
    };
    
    // Sort fleet by ID as a requirement for Binary Search
    sort(fleet.begin(), fleet.end()); 
    
    int searchId1 = 1003;
    cout << "Searching for Drone ID " << searchId1 << "...\n";
    searchDrone(fleet, searchId1);
    
    int searchId2 = 2000;
    cout << "Searching for Drone ID " << searchId2 << "...\n";
    searchDrone(fleet, searchId2);
    cout << "\n";

    // ---------------------------------------------------------
    // 3 & 4. Route Optimization & Connectivity (Graph)
    // ---------------------------------------------------------
    DroneNetwork network;
    network.addLocation("Warehouse HQ");
    network.addLocation("City Alpha");
    network.addLocation("City Beta");
    network.addLocation("Hospital Gamma");
    network.addLocation("Remote Village Delta");
    network.addLocation("Isolated Region Epsilon"); // This node will be intentionally disconnected

    network.addRoute("Warehouse HQ", "City Alpha", 10);
    network.addRoute("Warehouse HQ", "City Beta", 15);
    network.addRoute("City Alpha", "Hospital Gamma", 5);
    network.addRoute("City Beta", "Hospital Gamma", 8);
    network.addRoute("City Beta", "Remote Village Delta", 20);
    network.addRoute("Hospital Gamma", "Remote Village Delta", 12);
    // Note: No routes are added for "Isolated Region Epsilon"

    // --- 3. Dijkstra's Algorithm ---
    cout << "--- 3. Route Optimization (Dijkstra's Algorithm) ---\n";
    network.findShortestPath("Warehouse HQ", "Remote Village Delta");
    cout << "\n";

    // --- 4. Breadth First Search (BFS) ---
    cout << "--- 4. Connectivity Verification (BFS) ---\n";
    network.checkConnectivity("Warehouse HQ");
    cout << "\n";

    return 0;
}
