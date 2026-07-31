/* 
INPUT  - Enter map edges (supports both char/string labels 'A', 'B' and numeric '0', '1') 
OUTPUT - Generates shortest path, DOT file, and PNG visualization
*/

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <fstream>   // File handling
#include <cstdlib>   // system()
#include <unordered_map>
#include <climits>
#include <string>

using namespace std;

const string DOT_FILE = "graph.dot";
const string PNG_FILE = "graph.png";

class Edge {
public:
    int v;
    int wt;

    Edge(int v, int wt) {
        this->v = v;
        this->wt = wt;
    }
};

// Map manager to seamlessly handle string-to-int and int-to-string conversions
class NodeManager {
    unordered_map<string, int> nameToIndex;
    vector<string> indexToName;

public:
    int getOrAddIndex(const string& name) {
        if (nameToIndex.find(name) == nameToIndex.end()) {
            nameToIndex[name] = indexToName.size();
            indexToName.push_back(name);
        }
        return nameToIndex[name];
    }

    int getIndex(const string& name) const {
        if (nameToIndex.find(name) != nameToIndex.end()) {
            return nameToIndex.at(name);
        }
        return -1; // Not found
    }

    string getName(int index) const {
        if (index >= 0 && index < indexToName.size()) {
            return indexToName[index];
        }
        return "";
    }

    int size() const {
        return indexToName.size();
    }
};

void graph_generate(const vector<vector<Edge>>& graph, int source, int dest, const vector<int>& path, const NodeManager& nodes) {
    ofstream file(DOT_FILE);

    file << "graph G{\n";
    file << "  ratio = fill;\n";
    file << "  size = \"10,8\";\n";
    file << "  nodesep = 0.8;\n";
    file << "  ranksep = 1.2;\n";
    file << "  pad = \"1.0\";\n";
    
    file << "  node [\n";
    file << "    shape=circle,\n";
    file << "    style=filled,\n";
    file << "    fillcolor=lightblue,\n";
    file << "    fontsize=30,\n";
    file << "    penwidth=4\n";
    file << "  ];\n";

    file << "  edge [\n";
    file << "    fontsize=20,\n";
    file << "    penwidth=4\n";
    file << "  ];\n";

    vector<bool> isPath(graph.size(), false);
    for (int node : path) {
        isPath[node] = true;
    }

    for (int u = 0; u < graph.size(); u++) {
        string uName = nodes.getName(u);

        if (u == source) {
            file << "  \"" << uName << "\" [fillcolor=green];\n";
        } else if (u == dest) {
            file << "  \"" << uName << "\" [fillcolor=red];\n";
        } else if (isPath[u]) {
            file << "  \"" << uName << "\" [fillcolor=lightyellow];\n";
        }

        for (const auto &edge : graph[u]) {
            if (u < edge.v) {
                bool highlight = false;

                for (size_t i = 0; i < path.size() - 1; i++) {
                    if ((u == path[i] && edge.v == path[i + 1]) ||
                        (u == path[i + 1] && edge.v == path[i])) {
                        highlight = true;
                        break;
                    }
                }

                string vName = nodes.getName(edge.v);
                file << "  \"" << uName << "\" -- \"" << vName << "\"";

                if (highlight) {
                    file << " [label=\"" << edge.wt << "\", color=orange, penwidth=5]";
                } else {
                    file << " [label=\"" << edge.wt << "\"]";
                }
                file << ";\n";
            }
        }
    }
    file << "}\n";
    file.close();
}

void path(int source, int dest, const vector<vector<Edge>>& graph, int V, const NodeManager& nodes) {
    vector<int> dis(V, INT_MAX);
    vector<vector<int>> stpath(V);
    
    dis[source] = 0;
    stpath[source].push_back(source);
    
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dis[u]) continue;

        for (const Edge &e : graph[u]) {
            if (dis[e.v] > dis[u] + e.wt) {
                dis[e.v] = dis[u] + e.wt;
                stpath[e.v] = stpath[u];
                stpath[e.v].push_back(e.v);
                pq.push({dis[e.v], e.v});
            }
        }
    }

    cout << "\n--- Distances from " << nodes.getName(source) << " ---" << endl;
    for (int i = 0; i < V; i++) {
        nodes.getName(source)
        if (dis[i] == INT_MAX) cout << "INF\n";
        else cout << dis[i] << "\n";
    }

    if (dis[dest] == INT_MAX) {
        cout << "\nNo path exists from " << nodes.getName(source) << " to " << nodes.getName(dest) << endl;
        graph_generate(graph, source, dest, {}, nodes);     //MNM
        return;
    }

    string topath = "";
    for (size_t i = 0; i < stpath[dest].size(); i++) {
        topath += nodes.getName(stpath[dest][i]);
        if (i != stpath[dest].size() - 1) {
            topath += " -> ";
        }
    }

    graph_generate(graph, source, dest, stpath[dest], nodes);

    cout << "\nShortest path from " << nodes.getName(source) 
         << " to " << nodes.getName(dest) 
         << " is: " << topath 
         << " with a distance of " << dis[dest] << endl;
}

int main() {
    NodeManager nodes;
    struct InputEdge { string u, v; int wt; };
    vector<InputEdge> inputEdges;

    cout << "Enter the connected places with distance between them." << endl;
    cout << "Enter node 'exit' as first node to stop." << endl;
    cout << "Format: [Place_A] [Place_B] [Distance]" << endl << endl;

    while (true) {
        string uStr, vStr;
        int weight;

        cout << "a -> b c: ";
        cin >> uStr;
        if (uStr == "exit" || uStr == "-1") break;

        cin >> vStr >> weight;
        inputEdges.push_back({uStr, vStr, weight});

        // Register nodes in NodeManager
        nodes.getOrAddIndex(uStr);
        nodes.getOrAddIndex(vStr);
    }

    int V = nodes.size();
    if (V == 0) {
        cout << "No graph data entered." << endl;
        return 0;
    }

    vector<vector<Edge>> graph(V);
    for (const auto& edge : inputEdges) {
        int u = nodes.getIndex(edge.u);
        int v = nodes.getIndex(edge.v);
        graph[u].push_back(Edge(v, edge.wt));
        graph[v].push_back(Edge(u, edge.wt));
    }

    string srcStr, destStr;
    cout << "\nEnter starting and destination points:" << endl;
    
    int source = -1, dest = -1;
    while (source == -1) {
        cout << "Start: ";
        cin >> srcStr;
        source = nodes.getIndex(srcStr);
        if (source == -1) cout << "Node not found! Try again.\n";
    }

    while (dest == -1) {
        cout << "Destination: ";
        cin >> destStr;
        dest = nodes.getIndex(destStr);
        if (dest == -1) cout << "Node not found! Try again.\n";
    }

    path(source, dest, graph, V, nodes);

    // Graphviz render commands
    string command = "dot -Tpng " + DOT_FILE + " -o " + PNG_FILE;
    system(command.c_str());

    #ifdef _WIN32
        string open = "start " + PNG_FILE;
    #else
        string open = "xdg-open " + PNG_FILE;
    #endif
    system(open.c_str());

    return 0;
}
