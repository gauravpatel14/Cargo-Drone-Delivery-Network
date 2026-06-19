#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <climits>

using namespace std;

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

    bool operator<(const DeliveryRequest& other) const {
        return priority < other.priority;
    }
};

struct Drone {
    int id;
    string status;
    int batteryLevel;

    bool operator<(const Drone& other) const {
        return id < other.id;
    }
};

class DroneNetwork {
private:
    int numLocations;
    vector<string> locationNames;
    unordered_map<string, int> locationIds;
    vector<vector<pair<int, int>>> adjList;

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
        adjList[idV].push_back({idU, distance});
    }

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

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, startId});

        while (!pq.empty()) {
            int u = pq.top().second;
            int d = pq.top().first;
            pq.pop();

            if (d > dist[u]) continue;
            if (u == destId) break;

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

    cout << "--- 1. Processing Delivery Requests (Priority Queue) ---\n";
    priority_queue<DeliveryRequest> deliveryQueue;
    
    deliveryQueue.push({101, "Matheran Hills Outpost", EXPRESS, "Vaccines"});
    deliveryQueue.push({102, "Kharghar Sector-10", ROUTINE, "Office Supplies"});
    deliveryQueue.push({103, "Apollo Hospital Navi Mumbai", EMERGENCY, "Blood Bags"});
    deliveryQueue.push({104, "Ambernath", EXPRESS, "Antibiotics"});

    while (!deliveryQueue.empty()) {
        DeliveryRequest req = deliveryQueue.top();
        deliveryQueue.pop();
        string prioStr = (req.priority == EMERGENCY) ? "EMERGENCY" : (req.priority == EXPRESS) ? "EXPRESS" : "ROUTINE";
        cout << "[Processing] ID: " << req.id << " | Priority: " << prioStr 
             << " | Item: " << req.item << " | Dest: " << req.destination << "\n";
    }
    cout << "\n";

    cout << "--- 2. Fleet Monitoring (Binary Search) ---\n";
    vector<Drone> fleet = {
        {1001, "Available", 95},
        {1002, "In Transit", 45},
        {1003, "Maintenance", 15},
        {1004, "Available", 100},
        {1005, "In Transit", 60}
    };
    
    sort(fleet.begin(), fleet.end()); 
    
    int searchId1 = 1003;
    cout << "Searching for Drone ID " << searchId1 << "...\n";
    searchDrone(fleet, searchId1);
    
    int searchId2 = 2000;
    cout << "Searching for Drone ID " << searchId2 << "...\n";
    searchDrone(fleet, searchId2);
    cout << "\n";

    DroneNetwork network;

    network.addLocation("Panvel Central Warehouse");
    network.addLocation("Ambernath");
    network.addLocation("Kharghar Sector-10");
    network.addLocation("Apollo Hospital Navi Mumbai");
    network.addLocation("Matheran Hills Outpost");
    network.addLocation("Sanjay Gandhi National Park Jungle");

    network.addRoute("Panvel Central Warehouse", "Ambernath", 10);
    network.addRoute("Panvel Central Warehouse", "Kharghar Sector-10", 15);
    network.addRoute("Ambernath", "Apollo Hospital Navi Mumbai", 5);
    network.addRoute("Ambernath", "Kharghar Sector-10", 15);
    network.addRoute("Kharghar Sector-10", "Apollo Hospital Navi Mumbai", 8);
    network.addRoute("Kharghar Sector-10", "Matheran Hills Outpost", 20);
    network.addRoute("Apollo Hospital Navi Mumbai", "Matheran Hills Outpost", 12);

    cout << "--- 3. Route Optimization (Dijkstra's Algorithm) ---\n";
    network.findShortestPath("Panvel Central Warehouse", "Matheran Hills Outpost");
    cout << "\n";

    cout << "--- 4. Connectivity Verification (BFS) ---\n";
    network.checkConnectivity("Panvel Central Warehouse");
    cout << "\n";

    return 0;
}