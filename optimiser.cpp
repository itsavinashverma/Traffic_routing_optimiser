#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <limits>
#include <cmath>
#include <functional>
#include <iomanip>
#include <tuple>

using namespace std;

 int networkDelayTime(int n, vector<vector<int>>& times,int k) {
        // Create adjacency list 1-based 
        vector<vector<pair<int, int>>> adj(n + 1);
        for (auto& time : times) {
            adj[time[0]].push_back({time[1], time[2]});
        }

        // Min-heap priority queue: {time, node}
        queue<pair<int, int>> pq;
        vector<int> dist(n + 1, INT_MAX);
        dist[k] = 0;
        pq.push({0, k});

        while (!pq.empty()) {
            auto [time, node] = pq.front();
            pq.pop();

            for (auto& [neigh, wt] : adj[node]) {
                if (time + wt < dist[neigh]) {
                    dist[neigh] = time + wt;
                    pq.push({dist[neigh], neigh});
                }
            }
        }

        int maxTime = 0;
        for (int i = 1; i <= n; ++i) {
            if (dist[i] == INT_MAX) return -1;
            maxTime = max(maxTime, dist[i]);
        }
        return maxTime;
    }

int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
        //make the adj list first;
        vector<vector<pair<int,int>>> adj(n) ;
        for (auto & flight : flights){
            adj[flight[0]].push_back({flight[1], flight[2]});
        }

        //using modified dijkstra's algorith
        vector<int> mincost(n, INT_MAX);
        mincost[src] = 0;
        queue<tuple<int,int,int>>q; //using q instead of pq to save memory because we know the stops will += 1;
        q.push({0 , src , 0}); //stops , node  , price;
        while(!q.empty()){
            auto [stop , node , price] = q.front() ;
            q.pop();
            //if stops excedded k
            if (stop >= k+1) continue; //no need to go adj cities

            
            //traverse neighbours
            for (auto it : adj[node]){
                int adjnode = it.first;
                int edw = it.second;
                if (price + edw < mincost[adjnode] && stop <= k){
                mincost[adjnode] = price + edw;
                q.push({stop+1 , adjnode, price + edw});

                }
            }
        }

        if (mincost[dst] == INT_MAX) return -1;
        return mincost[dst];
    }

 int minimumEffortPath(vector<vector<int>>& heights) {
        int R = heights.size();
        int C = heights[0].size();
        vector<vector<int>> dist( R , vector<int> (C , INT_MAX));
        priority_queue<tuple<int,int,int> , vector<tuple<int,int,int>> , greater<>> q;//min heap;
        q.push({0,0,0}); // dist , x , y;
        dist[0][0] = 0 ;
        while(!q.empty()){
            auto [e,x,y] = q.top(); q.pop();
            //if reached;
            if (x == R-1 && y == C-1){
                return e ; 
            }
            int dx[] = {0,1,0,-1};
            int dy[] = {-1,0,1,0};
            for (int i = 0 ;i < 4; i++){
                int nx = x + dx[i];
                int ny = y + dy[i];
                if (nx >= 0 && nx < R && ny >= 0 && ny < C ){
                    int effort = max(abs(heights[x][y] - heights[nx][ny]) , e); 
                    if (effort < dist[nx][ny]){
                        dist[nx][ny] = effort;
                        q.push({effort , nx , ny});

                    }
 
                }
            }

        }

        return 0;
    }

double maxProbability(int n, vector<vector<int>>& edges, vector<double>& succProb, int start_node, int end_node) {
        vector<vector<pair<int,double>>> graph(n);
        for (int i = 0; i < edges.size(); i++) {
            int u = edges[i][0], v = edges[i][1];
            double prob = succProb[i];
            graph[u].push_back({v, prob});
            graph[v].push_back({u, prob});
        }

        // max-heap priority queue: {probability, node}
        priority_queue<pair<double,int>> pq;
        vector<double> best(n, 0.0);

        best[start_node] = 1.0;
        pq.push({1.0, start_node});

        while (!pq.empty()) {
            auto [currProb, node] = pq.top();
            pq.pop();

            // if reached end_node, return
            if (node == end_node) return currProb;

            // relax neighbors
            for (auto& [nbr, prob] : graph[node]) {
                double newProb = currProb * prob;
                if (newProb > best[nbr]) {
                    best[nbr] = newProb;
                    pq.push({newProb, nbr});
                }
            }
        }
        return 0.0;
    }


// --- Input Handlers and Solvers ---

void solveNetworkDelay() {
    int n, k, num_edges;
    std::cout << "Enter number of nodes (n): ";
    std::cin >> n;
    std::cout << "Enter the starting node (k): ";
    std::cin >> k;
    std::cout << "Enter the number of edges: ";
    std::cin >> num_edges;
    std::vector<std::vector<int>> times;
    std::cout << "Enter " << num_edges << " edges in the format 'source destination time':\n";
    for (int i = 0; i < num_edges; ++i) {
        int u, v, w;
        std::cin >> u >> v >> w;
        times.push_back({u, v, w});
    }
    int result = networkDelayTime(n, times, k);
    std::cout << "\nResult: Minimum time for signal to reach all nodes is " << result << ".\n";
}

void solveCheapestPrice() {
    int n, src, dst, k, num_flights;
    std::cout << "Enter number of cities (n): ";
    std::cin >> n;
    std::cout << "Enter source city, destination city, and max stops (src dst k): ";
    std::cin >> src >> dst >> k;
    std::cout << "Enter number of flights: ";
    std::cin >> num_flights;
    std::vector<std::vector<int>> flights;
    std::cout << "Enter " << num_flights << " flights in the format 'from to price':\n";
    for (int i = 0; i < num_flights; ++i) {
        int u, v, p;
        std::cin >> u >> v >> p;
        flights.push_back({u, v, p});
    }
    int result = findCheapestPrice(n, flights, src, dst, k);
    std::cout << "\nResult: The cheapest price is " << result << ".\n";
}

void solveMinimumEffort() {
    int rows, cols;
    std::cout << "Enter number of rows and columns for the terrain grid: ";
    std::cin >> rows >> cols;
    std::vector<std::vector<int>> heights(rows, std::vector<int>(cols));
    std::cout << "Enter the height for each cell (row by row):\n";
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cin >> heights[i][j];
        }
    }
    int result = minimumEffortPath(heights);
    std::cout << "\nResult: The minimum effort required is " << result << ".\n";
}

void solveMaxProbability() {
    int n, start, end, num_edges;
    std::cout << "Enter number of nodes (n): ";
    std::cin >> n;
    std::cout << "Enter the start node and end node: ";
    std::cin >> start >> end;
    std::cout << "Enter number of edges: ";
    std::cin >> num_edges;
    std::vector<std::vector<int>> edges;
    std::vector<double> succProb;
    std::cout << "Enter " << num_edges << " edges in the format 'node1 node2 probability':\n";
    for (int i = 0; i < num_edges; ++i) {
        int u, v;
        double p;
        std::cin >> u >> v >> p;
        edges.push_back({u, v});
        succProb.push_back(p);
    }
    double result = maxProbability(n, edges, succProb, start, end);
    std::cout << "\nResult: The maximum probability is " << std::fixed << std::setprecision(5) << result << ".\n";
}


void displayMenu() {
    std::cout << "\n--- Traffic Routing Optimizer: Graph Algorithms ---\n";
    std::cout << "Select an optimization problem to solve:\n";
    std::cout << "1. Signal Propagation Time (Dijkstra's)\n";
    std::cout << "2. Least-Cost Route with Maximum Stops (Modified Dijkstra's)\n";
    std::cout << "3. Minimum Effort Terrain Traversal (Dijkstra on a Grid)\n";
    std::cout << "4. Most Reliable Route (Dijkstra with Probabilities)\n";
    std::cout << "5. Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    int choice;
    do {
        displayMenu();
        std::cin >> choice;
        std::cout << "--------------------------------------------------\n";

        switch (choice) {
            case 1: solveNetworkDelay(); break;
            case 2: solveCheapestPrice(); break;
            case 3: solveMinimumEffort(); break;
            case 4: solveMaxProbability(); break;
            case 5: std::cout << "Exiting program.\n"; break;
            default: std::cout << "Invalid choice. Please try again.\n"; break;
        }
         std::cout << "--------------------------------------------------\n";

    } while (choice != 5);

    return 0;
}

//test cases ; 
// 1. Network Delay Time
// Input: n = 4, times = [[2,1,1],[2,3,1],[3,4,1]], k = 2
// Output: 2    
// 2. Cheapest Flights Within K Stops
// Input: n = 3, flights = [[0,1,100],[1,2,100],[0,2,500]], src = 0, dst = 2, k = 1
// Output: 200
// 3. Path With Minimum Effort
// Input: heights = [[1,2,2],[3,8,2],[5,3,5]]
// Output: 2
// 4. Path with Maximum Probability
// Input: n = 3, edges = [[0,1],[1,2],[0,2]], succProb = [0.5,0.5,0.2], start = 0, end = 2
// Output: 0.25000
// TypeError: list indices must be integers or slices, not float
// --- End of File ---
