#include <bits/stdc++.h>
#include "constants.h"
using namespace std;

class Router {
public:
    int routerId;
    vector<pair<Router*, int>> neighbours;
    vector<int> routingTableDistances;
    vector<int> routingTable;

    Router(int id, int n) { // Initialize Routing Table
        this->routerId = id;
        routingTableDistances.resize(n + 1, INF);
        routingTable.resize(n + 1, -1);
    }

    void addNeighbour(Router* neighbour, int weight) {
        neighbours.push_back(make_pair(neighbour, weight));
    }

    void updateRoutingTable() {
        routingTableDistances[this->routerId] = 0;
        routingTable[this->routerId] = routerId;
        multiset<pair<int, Router*>> priQueue;

        priQueue.insert(make_pair(0, this));

        while (!priQueue.empty()) {
            pair<int, Router*> vertex = *(priQueue.begin());
            priQueue.erase(priQueue.begin());
            int distance = vertex.first;
            Router* router = vertex.second;

            for (auto& child : router->neighbours) {
                if (routingTableDistances[(child.first)->routerId] > distance + child.second) {
                    routingTable[(child.first)->routerId] = router->routerId;
                    routingTableDistances[(child.first)->routerId] = distance + child.second;
                    priQueue.insert(make_pair(distance + child.second, child.first));
                }
            }
        }
    }

    void printRoutingTable() const {
        int n = routingTable.size();
        for (int i = 1; i < n; i++) {
            if (i == routerId) continue; // Skip the router itself
            int next_hop = routingTable[i];
            if (next_hop == -1) {
                cout << OUTPUT_DESTINATION_ROUTER << i << " "
                    << NO_ROUTE_AVAILABLE << endl;
                continue;
            }
            // Trace the path from the current router to the destination router
            while (routingTable[next_hop] != routerId && next_hop != -1) {
                next_hop = routingTable[next_hop];
            }
            // Print the correct outgoing link
            cout << OUTPUT_DESTINATION_ROUTER << i << " "
                << OUTPUT_TAKES_OUTGOING_LINK << routerId << "," << next_hop << endl;
        }
    }
};

int main() {
    cout << PROMPT_NUM_NODES_EDGES << endl;

    int n, m;
    cin >> n >> m;

    vector<Router*> routers(n + 1);

    for (int i = 1; i <= n; i++) {
        routers[i] = new Router(i, n);
    }

    cout << PROMPT_ENTER_EDGES << endl;

    for (int i = 0; i < m; i++) {
        int node1, node2, weight;
        cin >> node1 >> node2 >> weight;

        routers[node1]->addNeighbour(routers[node2], weight);
        routers[node2]->addNeighbour(routers[node1], weight);
    }

    for (int i = 1; i <= n; i++) {
        routers[i]->updateRoutingTable();
    }

    while (true) {
        cout << PROMPT_EXIT << endl;

        string s;
        cin >> s;

        if (s == RESPONSE_EXIT) {
            break;
        }

        cout << PROMPT_ENTER_NODE << endl;

        int node;
        cin >> node;

        routers[node]->printRoutingTable();
    }

    // Cleanup memory
    for (int i = 1; i <= n; i++) {
        delete routers[i];
    }

    return 0;
}
