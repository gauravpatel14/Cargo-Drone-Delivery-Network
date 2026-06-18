# International Cargo Drone Delivery Network 🚁

## 1. Project Title
**International Cargo Drone Delivery Network**

## 2. Problem Statement
In remote regions where traditional transportation infrastructure is limited, delivering medical supplies, emergency equipment, and essential goods is challenging. There is an urgent need for an intelligent logistics platform capable of coordinating a global fleet of autonomous delivery drones to ensure rapid, reliable, and cost-effective deliveries while managing priorities and verifying network connectivity.

## 3. Objectives
- Establish an intelligent drone logistics platform to support emergency deliveries.
- Implement dynamic route optimization to minimize delivery times.
- Provide fleet monitoring dashboards to track drone availability and health.
- Ensure real-time decision-making for network connectivity verification.

## 4. System Overview / Architecture
The system acts as a central control hub for a global drone fleet. It receives delivery requests and processes them based on urgency. It actively tracks the status and battery levels of thousands of drones. When a delivery is dispatched, the system calculates the absolute shortest flight path across a mapped network of warehouses, cities, and remote villages. It also continuously scans the grid to identify any disconnected or unreachable regions.

## 5. Data Structures and Algorithms Used
- **Priority Queue (Max-Heap):** Used for Delivery Request Handling.
- **Binary Search:** Used for Fleet Monitoring and Drone Search.
- **Graph (Adjacency List):** Used to model the logistics network.
- **Dijkstra's Algorithm:** Used for Route Optimization.
- **Breadth-First Search (BFS):** Used for Connectivity Verification.

## 6. Implementation Approach
- **Priority Queue:** Implemented using `std::priority_queue` with a custom comparator to ensure `EMERGENCY` requests bubble to the top.
- **Binary Search:** The fleet data is stored in a `std::vector` and sorted by ID. A standard iterative binary search `O(log N)` is used to quickly locate specific drones.
- **Dijkstra's Algorithm:** A Min-Heap (`std::priority_queue<greater>`) paired with an Adjacency List calculates the shortest distance between a source and destination.
- **BFS:** A `std::queue` is used to traverse the network layer-by-layer starting from the Headquarters to verify that all nodes have at least one connecting route.

## 7. Time and Space Complexity Analysis
| Algorithm/Data Structure | Time Complexity (Worst Case) | Space Complexity |
|--------------------------|------------------------------|------------------|
| **Priority Queue (Insert/Extract)** | O(log N) | O(N) |
| **Binary Search** | O(log N) | O(1) |
| **Dijkstra's Algorithm** | O((V + E) log V) | O(V) |
| **Breadth-First Search (BFS)** | O(V + E) | O(V) |
*(Where V = Vertices/Locations, E = Edges/Routes, N = Number of Items)*

## 8. Execution Steps
1. Navigate to the project directory: `cd "DSa viva prep"`
2. Compile the C++ source code: `g++ src/CargoDroneNetwork.cpp -o CargoDroneNetwork`
3. Execute the program: `./CargoDroneNetwork`

## 9. Sample Inputs and Outputs
**Sample Scenario:**
- Emergency blood bags, express antibiotics, and routine office supplies are requested.
- Drones #1003 and #2000 are searched for maintenance.
- A route is requested from Warehouse HQ to Remote Village Delta.
- Network connectivity is verified from Warehouse HQ.

**Sample Output:**
*See the [output/sample_output.txt](./output/sample_output.txt) file for the complete execution trace.*

## 10. Screenshots
> **Note to evaluator:** Screenshots of the program execution in the terminal can be found in the [screenshots/terminal.png](./screenshots/terminal.png) directory.

## 11. Results and Observations
- The Priority Queue successfully bypassed standard FIFO logic to serve critical medical supplies instantly.
- Binary search drastically reduced lookup times for drone maintenance checks compared to linear scanning.
- Dijkstra's algorithm effectively identified the 27-unit distance path, avoiding longer alternative routes.
- BFS successfully identified "Isolated Region Epsilon" as an unreachable node, proving its value as a diagnostic tool.

## 12. Conclusion
The "International Cargo Drone Delivery Network" demonstrates the highly practical, real-world application of foundational Data Structures and Algorithms. By matching the right tool to the right problem (e.g., using a Max-Heap for emergencies), the system achieves maximum operational efficiency, improves delivery speeds, and guarantees accessibility to remote regions.
