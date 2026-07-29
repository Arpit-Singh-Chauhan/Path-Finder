/* 
INPUT - Enter the map [a graph of roads] and a staring point with a ending destination 
OUTPUT - This will give the shorrtest path for the journey
*/

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <fstream>   // File handling
#include <cstdlib>   // system()
using namespace std;

const string DOT_FILE = "graph.dot";
const string PNG_FILE = "graph.png";

class Edge{
    public:
    int v;
    int wt;

    Edge(int v, int wt){
        this->v = v;
        this->wt = wt;
    }
};


void graph_generate(const vector<vector<Edge>>& graph, int source, int dest, vector<int>& path){

    ofstream file(DOT_FILE);

    file << "graph G{\n";
        
        file<< "ratio = fill \n";
        file<<  "size = \"10,8\" \n";
        file<<  "nodesep = 0.8 \n";          
        file<<  "ranksep = 1.2 \n";

        file << "node [ \n";
        file<<  "shape=circle \n";
        file<<  "style=filled \n";
        file<<  "fillcolor=lightblue \n";
        file<<  "fontsize=40 \n";
        file<<  "penwidth=4 \n] \n";
             

        file << "edge [ \n";
        file<<  "fontsize=50 \n";
        file<<  "penwidth = 4 \n] \n";

    vector<bool> isPath(graph.size(), false);

        for(int node : path){
            isPath[node] = true;
        }

        for(int u = 0; u < graph.size(); u++){
            if(u == source){
                file << u << " [fillcolor=green];\n";
            }
            else if(u == dest){
                file << u << " [fillcolor=red];\n";
            }
            else if(isPath[u]){
                file << u << " [fillcolor=lightyellow];\n";
            }

            for(const auto &edge : graph[u]){
                if(u < edge.v){
                    bool highlight = false;

                    for(int i = 0; i < path.size() - 1; i++){
                        if((u == path[i] && edge.v == path[i + 1]) ||
                        (u == path[i + 1] && edge.v == path[i])){
                            highlight = true;
                            break;
                        }
                    }

                    file << u << " -- " << edge.v;

                    if(highlight){
                        file << " [label=\"" << edge.wt
                            << "\", color=orange, penwidth=5]";
                    }
                    else{
                        file << " [label=\"" << edge.wt << "\"]";
                    }

                    file << ";\n";
                }
            }
        }
    file<<"}";

    file.close();
}

void path(int source, int dest, vector<vector<Edge>>&graph, int V){
    vector<int>dis(V, INT_MAX);
    vector<vector<int>>stpath(V);
    dis[source] = 0;
    stpath[source].push_back(source);
    priority_queue<pair<int, int> , vector<pair<int, int>>, greater<pair<int, int>> > pq;
    pq.push({0,source});


    while(pq.size() > 0){
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if(d > dis[u])
            continue;

        for(const Edge &e: graph[u]){
            if(dis[e.v] > dis[u] + e.wt){
                dis[e.v] = dis[u] + e.wt;
                stpath[e.v] = stpath[u];
                stpath[e.v].push_back(e.v);
                pq.push({dis[e.v], e.v});
            }
        }
    }

cout << "Distances from " << source << endl;
    for(int i=0; i<V; i++){
        if(dis[i]==INT_MAX)
            cout<<"INF ";
        else
            cout<<dis[i]<<" ";
        }
    
        cout << endl;

cout << "Paths from " << source << " to" << endl;
    for(int i=0; i<V; i++){
        cout<< i << " : ";
        for(int j=0; j<stpath[i].size(); j++){
            cout << stpath[i][j]<< " ";
        }
        cout << endl;
    }

    string topath = "";

    for(int i =0; i<stpath[dest].size(); i++){
        topath += to_string(stpath[dest][i]) ; 
        if(i != stpath[dest].size()-1){
            topath += " -> ";
        }
    }

    graph_generate(graph, source, dest, stpath[dest]);

    if(dis[dest]==INT_MAX){
    cout<<"No path exists from " << source << " to " << dest;
    return;
    }

    cout << "Shortest path from " << source << " to " << dest << " is " << topath << " with the distance of " << dis[dest];

}

void entry_graph(vector<pair<pair<int, int>, int>>& edges, vector<vector<Edge>>& graph){
    for(int i=0; i<edges.size(); i++){
        graph[edges[i].first.first].push_back(Edge(edges[i].first.second, edges[i].second));            // u -> v
        graph[edges[i].first.second].push_back(Edge(edges[i].first.first, edges[i].second));            // v -> u
    }
}


int main(){

    int V;
    cout << "Enter number of places (node): " ;
    cin >> V ;

    vector<vector<Edge>> graph(V);

    // graph[0].push_back(Edge(1,2));
    // graph[0].push_back(Edge(2,4));

    // graph[1].push_back(Edge(2,1));
    // graph[1].push_back(Edge(3,7));

    // graph[2].push_back(Edge(4,3));

    // graph[3].push_back(Edge(5,1));

    // graph[4].push_back(Edge(3,2));
    // graph[4].push_back(Edge(5,5));

    
    vector<pair<pair<int, int>, int>> edges = {{{0,1},2}, {{0,2},4}, {{1,2},1}, {{1,3},7}, {{2,4},3}, {{3,5},1}, {{4,3},2}, {{4,5},5}};
    

    // vector<pair<pair<int, int>, int>> edges ;

    // cout << "Enter the linked palces with distance between them to create a map." << endl;
    // cout << "Enter first place(a) and second palce(b) with diatance(c) between them (a->b)  [-1 to stop] " << endl;

    // int a=0, b=0, c=0;
    // while(true){
    //     cout << "a->b c: ";
    //     cin >> a;
    //     if(a == -1) break;

    //     cin >> b >> c;

    //     if(a<0 || a>=V || b<0 || b>=V){
    //     cout<<"Invalid node\n";
    //     continue;
    //     }

    //     edges.push_back({{a,b},c});
    // }

    entry_graph(edges, graph);
    
    
    int source, dest;

    cout << "Enter starting and finish point of the journey to find shortest path and its distance" << endl;

    cout << "Start: ";
    cin >> source;
    cout << "Destination: ";
    cin >> dest;


    path(source, dest, graph, V);

    // graph_generate(graph, source, dest);

    string command = "dot -Tpng " + DOT_FILE + " -o " + PNG_FILE;
    system(command.c_str());

    string open = "start " + PNG_FILE;
    system(open.c_str());
}
