#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <fstream>
#include <string>
#include <iomanip>
#include <climits>
#include <functional>
#include <windows.h>

using namespace std;


// ─── Forward Declarations ──────────────────────────────────────────────────

void printSuccess(string message);
void printError(string message);


// ─── Road Connection Structure ─────────────────────────────────────────────

struct Connection {
    int from;
    int to;
    int kilometers;
};


// ─── Union-Find for Cycle Detection ────────────────────────────────────────

class UnionFind {
private:
    vector<int> root;
    vector<int> treeRank;

public:
    UnionFind(int totalNodes) {
        root.resize(totalNodes);
        treeRank.resize(totalNodes, 0);
        for (int i = 0; i < totalNodes; i++)
            root[i] = i;
    }

    int getRoot(int node) {
        if (root[node] != node)
            root[node] = getRoot(root[node]);   // path compression
        return root[node];
    }

    void merge(int nodeA, int nodeB) {
        int rootA = getRoot(nodeA);
        int rootB = getRoot(nodeB);

        if (rootA == rootB)
            return;

        if (treeRank[rootA] < treeRank[rootB])
            root[rootA] = rootB;
        else if (treeRank[rootA] > treeRank[rootB])
            root[rootB] = rootA;
        else {
            root[rootB] = rootA;
            treeRank[rootA]++;
        }
    }
};


// ─── Route Management System ────────────────────────────────────────────────

class RouteManager {
private:
    vector<Connection> routeList;

    vector<string> cityNames = {
        "Lahore",
        "Islamabad",
        "Karachi",
        "Faisalabad",
        "Multan",
        "Peshawar"
    };

    int totalCities = 6;

    int getTotalDistance() {
        int sum = 0;
        for (auto& r : routeList)
            sum += r.kilometers;
        return sum;
    }

public:

    // ── Add New Route ──────────────────────────────────────────────────────

    void addRoute() {
        int cityA, cityB, dist;

        cout << "\nCity List:\n";
        for (int i = 0; i < (int)cityNames.size(); i++)
            cout << i << ". " << cityNames[i] << "\n";

        cout << "\nSelect Origin City: ";
        cin >> cityA;

        cout << "Select Destination City: ";
        cin >> cityB;

        if (cityA < 0 || cityA >= totalCities || cityB < 0 || cityB >= totalCities) {
            cout << "\nCity number out of range!\n";
            return;
        }

        cout << "Enter Distance in km: ";
        cin >> dist;

        Connection newRoute;
        newRoute.from = cityA;
        newRoute.to = cityB;
        newRoute.kilometers = dist;

        routeList.push_back(newRoute);
        cout << "\nRoute Recorded Successfully!\n";
    }


    // ── Show All Routes ────────────────────────────────────────────────────

    void showRoutes() {
        if (routeList.empty()) {
            cout << "\nRoute list is empty.\n";
            return;
        }

        cout << "\n========== ROUTE REGISTRY ==========\n\n";

        cout << left
            << setw(20) << "FROM"
            << setw(20) << "TO"
            << setw(15) << "KM"
            << "\n";

        cout << "---------------------------------------------\n";

        for (auto& r : routeList) {
            cout << left
                << setw(20) << cityNames[r.from]
                << setw(20) << cityNames[r.to]
                << setw(15) << (to_string(r.kilometers) + " km")
                << "\n";
        }
    }


    // ── Write Routes to Disk ───────────────────────────────────────────────

    void writeToFile() {
        ofstream outFile("saved_routes.txt");

        if (!outFile) {
            cout << "\nFailed to create file!\n";
            return;
        }

        for (auto& r : routeList)
            outFile << r.from << " " << r.to << " " << r.kilometers << "\n";

        outFile.close();
        printSuccess("Routes saved to file.");
    }


    // ── Read Routes from Disk ──────────────────────────────────────────────

    void readFromFile() {
        ifstream inFile("saved_routes.txt");

        if (!inFile) {
            cout << "\nSave file not found!\n";
            return;
        }

        routeList.clear();

        Connection temp;
        while (inFile >> temp.from >> temp.to >> temp.kilometers)
            routeList.push_back(temp);

        inFile.close();

        printSuccess("Routes loaded from file.");
        showRoutes();
    }


    // ── Prim's Minimum Spanning Tree ───────────────────────────────────────

    void runPrimMST() {
        if (routeList.empty()) {
            cout << "\nNo routes to process!\n";
            return;
        }

        // Build adjacency list
        vector<vector<pair<int, int>>> graph(totalCities);
        for (auto& r : routeList) {
            graph[r.from].push_back({ r.to,   r.kilometers });
            graph[r.to].push_back({ r.from, r.kilometers });
        }

        // Min-heap: {cost, city}
        priority_queue<
            pair<int, int>,
            vector<pair<int, int>>,
            greater<pair<int, int>>
        > minHeap;

        vector<int> cheapest(totalCities, INT_MAX);
        vector<int> connectedTo(totalCities, -1);
        vector<bool> visited(totalCities, false);

        cheapest[0] = 0;
        minHeap.push({ 0, 0 });

        while (!minHeap.empty()) {
            int current = minHeap.top().second;
            minHeap.pop();

            if (visited[current]) continue;
            visited[current] = true;

            for (auto& neighbor : graph[current]) {
                int nextCity = neighbor.first;
                int edgeCost = neighbor.second;

                if (!visited[nextCity] && edgeCost < cheapest[nextCity]) {
                    cheapest[nextCity] = edgeCost;
                    connectedTo[nextCity] = current;
                    minHeap.push({ cheapest[nextCity], nextCity });
                }
            }
        }

        int mstTotal = 0;
        cout << "\n===== PRIM'S MST RESULT =====\n";

        for (int i = 1; i < totalCities; i++) {
            if (connectedTo[i] != -1) {
                cout << cityNames[connectedTo[i]]
                    << " --> " << cityNames[i]
                    << " (" << cheapest[i] << " km)\n";
                mstTotal += cheapest[i];
            }
        }

        cout << "\nTotal Route Distance : " << getTotalDistance() << " km\n";
        cout << "Optimized MST Cost   : " << mstTotal << " km\n";
        cout << "Distance Saved       : " << getTotalDistance() - mstTotal << " km\n";
    }


    // ── Kruskal's Minimum Spanning Tree ───────────────────────────────────

    void runKruskalMST() {
        if (routeList.empty()) {
            cout << "\nNo routes to process!\n";
            return;
        }

        // Sort all edges by distance
        vector<Connection> ranked = routeList;
        sort(ranked.begin(), ranked.end(),
            [](const Connection& a, const Connection& b) {
                return a.kilometers < b.kilometers;
            });

        UnionFind uf(totalCities);

        int mstTotal = 0;
        cout << "\n===== KRUSKAL'S MST RESULT =====\n";

        for (auto& edge : ranked) {
            if (uf.getRoot(edge.from) != uf.getRoot(edge.to)) {
                uf.merge(edge.from, edge.to);

                cout << cityNames[edge.from]
                    << " --> " << cityNames[edge.to]
                    << " (" << edge.kilometers << " km)\n";

                mstTotal += edge.kilometers;
            }
        }

        cout << "\nTotal Route Distance : " << getTotalDistance() << " km\n";
        cout << "Optimized MST Cost   : " << mstTotal << " km\n";
        cout << "Distance Saved       : " << getTotalDistance() - mstTotal << " km\n";
    }
};


// ─── Console Utilities ─────────────────────────────────────────────────────

void applyColor(int colorCode) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
}

void showLoadingDots() {
    cout << "\nProcessing";
    for (int i = 0; i < 5; i++) {
        cout << ".";
        Sleep(300);
    }
    cout << "\n";
}

void showBanner() {
    applyColor(11);
    cout << "\n";
    cout << "  PARCEL ROUTE OPTIMIZATION TOOL\n";
    applyColor(15);
}

void printSuccess(string message) {
    applyColor(10);
    cout << "\n[OK] " << message << "\n";
    applyColor(15);
}

void printError(string message) {
    applyColor(12);
    cout << "\n[FAIL] " << message << "\n";
    applyColor(15);
}


// ─── Program Entry Point ───────────────────────────────────────────────────

int main() {
    RouteManager manager;
    int option;

    do {
        system("cls");
        showBanner();

        applyColor(14);
        cout << " 1. Add a New Route\n";
        cout << " 2. View All Routes\n";
        cout << " 3. Run Prim's Algorithm (MST)\n";
        cout << " 4. Run Kruskal's Algorithm (MST)\n";
        cout << " 5. Save Routes to File\n";
        cout << " 6. Load Routes from File\n";
        cout << " 7. Exit\n";
        applyColor(15);

        cout << "\nYour Choice: ";
        cin >> option;

        switch (option) {
        case 1:
            manager.addRoute();
            break;

        case 2:
            manager.showRoutes();
            break;

        case 3:
            showLoadingDots();
            manager.runPrimMST();
            break;

        case 4:
            showLoadingDots();
            manager.runKruskalMST();
            break;

        case 5:
            manager.writeToFile();
            break;

        case 6:
            manager.readFromFile();
            break;

        case 7:
            applyColor(10);
            cout << "\n  Thanks for using the Route Optimizer. Goodbye!\n";
            applyColor(15);
            break;

        default:
            cout << "\nPlease enter a valid option (1-7).\n";
        }

        if (option != 7) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }

    } while (option != 7);

    return 0;
}