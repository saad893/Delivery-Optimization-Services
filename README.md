# Delivery Service Optimization using Minimum Spanning Tree

A console-based C++ application that optimizes parcel delivery routes across major Pakistani cities using **Minimum Spanning Tree (MST)** algorithms — both **Kruskal's** and **Prim's** — to minimize total road distance and reduce delivery costs.

---

## Features

- Add road connections between any two cities with distance in kilometers
- View all registered routes in a formatted table
- Compute the optimal delivery network using **Prim's Algorithm** (min-heap, vertex-based)
- Compute the optimal delivery network using **Kruskal's Algorithm** (edge-sorted, Union-Find)
- Save routes to disk (`saved_routes.txt`) and reload them across sessions
- Color-coded console output (success in green, errors in red, menu in yellow)
- Animated loading dots while processing MST

---

## Cities Covered

| ID | City        |
|----|-------------|
| 0  | Lahore      |
| 1  | Islamabad   |
| 2  | Karachi     |
| 3  | Faisalabad  |
| 4  | Multan      |
| 5  | Peshawar    |

---

## Project Structure

```
Delivery Service Optimization using Minimum Spanning Tree/
├── main.cpp                  # Console application (CLI) — primary entry point
├── delivery_optimization.cpp # Standalone MST logic module
├── RouteOptimizerGUI.cpp     # Windows GUI version (Win32 API)
├── saved_routes.txt          # Auto-generated route save file
├── main.exe                  # Compiled CLI executable
├── delivery_optimization.exe # Compiled GUI executable
├── report.docx               # Project report
└── .vscode/
    └── tasks.json            # VSCode build tasks
```

---

## Core Classes & Structures

### `Connection`
Represents a road between two cities:
```cpp
struct Connection {
    int from;        // Origin city ID
    int to;          // Destination city ID
    int kilometers;  // Road distance
};
```

### `UnionFind`
Used by Kruskal's algorithm for cycle detection. Implements **path compression** and **union by rank** for near-constant time operations.

### `RouteManager`
The main application class. Manages the route list and exposes all menu operations:

| Method           | Description                                      |
|------------------|--------------------------------------------------|
| `addRoute()`     | Prompts user to add a new city-to-city connection |
| `showRoutes()`   | Prints all routes in a formatted table            |
| `runPrimMST()`   | Runs Prim's algorithm and prints the MST          |
| `runKruskalMST()`| Runs Kruskal's algorithm and prints the MST       |
| `writeToFile()`  | Saves all routes to `saved_routes.txt`            |
| `readFromFile()` | Loads routes from `saved_routes.txt`              |

---

## Algorithms

### Prim's Algorithm
Builds the MST by starting at city 0 and greedily expanding via the cheapest available edge at each step, using a **min-heap priority queue**. Tracks the best known cost (`cheapest[]`) and parent (`connectedTo[]`) for each unvisited city.

### Kruskal's Algorithm
Sorts all edges by distance ascending, then greedily adds each edge if it does not form a cycle — determined using the **Union-Find** data structure. Produces the globally minimum spanning tree.

Both algorithms output the selected edges, total original distance, optimized MST cost, and distance saved.

---

## Sample Output

```
===== KRUSKAL'S MST RESULT =====
Lahore --> Islamabad (100 km)
Islamabad --> Karachi (300 km)

Total Route Distance : 400 km
Optimized MST Cost   : 400 km
Distance Saved       : 0 km
```

---

## How to Build

### Requirements
- Windows OS (uses `<windows.h>` for console colors and `Sleep()`)
- MinGW / GCC (g++) or MSVC
- VSCode (optional — build tasks provided in `.vscode/tasks.json`)

### Compile CLI version
```bash
g++ main.cpp -o main.exe
```

### Compile GUI version
```bash
g++ RouteOptimizerGUI.cpp -o delivery_optimization.exe -lcomctl32 -mwindows
```

### Run
```bash
./main.exe
```

---

## Menu Options

```
  PARCEL ROUTE OPTIMIZATION TOOL

 1. Add a New Route
 2. View All Routes
 3. Run Prim's Algorithm (MST)
 4. Run Kruskal's Algorithm (MST)
 5. Save Routes to File
 6. Load Routes from File
 7. Exit
```

---

## Save File Format (`saved_routes.txt`)

Routes are stored one per line as:
```
<from_id> <to_id> <distance_km>
```

Example:
```
0 1 100
1 2 300
```

---

## Authors
;
- **Syed Muhammad Saad Hasham**
- **Muhammad Mamoon**

---

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.
