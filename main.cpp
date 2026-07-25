/* 
INPUT - Enter the map [a graph of roads] and a staring point with a ending destination 
OUTPUT - This will give the shorrtest path for the journey
*/

#include <iostream>
#include <vector>
#include <list>
#include <queue>
using namespace std;

class Edge{
    public:
    int v;
    int wt;

    Edge(int v, int wt){
        this->v = v;
        this->wt = wt;
    }
};

void path(int src, int end, vector<vector<Edge>>&g, int V){
    vector<int>dis(V, INT_MAX);
    vector<vector<int>>stpath(V);
    dis[src] = 0;
    stpath[src].push_back(src);
    priority_queue<pair<int, int> , vector<pair<int, int>>, greater<pair<int, int>> > pq;
    pq.push({0,src});


    while(pq.size() > 0){
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if(d > dis[u])
            continue;

        for(Edge e: g[u]){
            if(dis[e.v] >= dis[u] + e.wt){
                dis[e.v] = dis[u] + e.wt;
                stpath[e.v] = stpath[u];
                stpath[e.v].push_back(e.v);
                pq.push({dis[e.v], e.v});
            }
        }
    }

    cout << "Distances from " << src << endl;
    for(int i=0; i<V; i++){
        if(dis[i]==INT_MAX)
            cout<<"INF ";
        else
            cout<<dis[i]<<" ";
        }
    
        cout << endl;

    cout << "Paths from " << src << " to" << endl;

    for(int i=0; i<V; i++){
        cout<< i << " : ";
        for(int j=0; j<stpath[i].size(); j++){
            cout << stpath[i][j]<< " ";
        }
        cout << endl;
    }

    string topath = "";

    for(int i =0; i<stpath[end].size(); i++){
        topath += to_string(stpath[end][i]) ; 
        if(i != stpath[end].size()-1){
            topath += " -> ";
        }
    }
    cout << "Shortest path from " << src << " to " << end << " is " << topath << " with the distance of " << dis[end];

}

void entry_graph(vector<pair<pair<int, int>, int>>& p, vector<vector<Edge>>& g){
    for(int i=0; i<p.size(); i++){
        g[p[i].first.first].push_back(Edge(p[i].first.second, p[i].second));            // u -> v
        g[p[i].first.second].push_back(Edge(p[i].first.first, p[i].second));            // v -> u
    }
}

void inputs(int a, int b, int c, vector<pair<pair<int, int>, int>>& p){

    p.push_back({{a,b},c});
    
}

int main(){
    int V=6;
    vector<vector<Edge>> g(V);
    // g[0].push_back(Edge(1,2));
    // g[0].push_back(Edge(2,4));

    // g[1].push_back(Edge(2,1));
    // g[1].push_back(Edge(3,7));

    // g[2].push_back(Edge(4,3));

    // g[3].push_back(Edge(5,1));

    // g[4].push_back(Edge(3,2));
    // g[4].push_back(Edge(5,5));

    
    // vector<pair<pair<int, int>, int>> p = {{{0,1},2}, {{0,2},4}, {{1,2},1}, {{1,3},7}, {{2,4},3}, {{3,5},1}, {{4,3},2}, {{4,5},5}};
    
    vector<pair<pair<int, int>, int>> p ;

    cout << "Enter the linked palces with distance between them to create a map." << endl;
    cout << "Enter first place(a) and second palce(b) with diatance(c) between them (a->b)  [-1 to stop] " << endl;

    int a=0, b=0, c=0;
    while(true){
        cout << "a->b c: ";
        cin >> a >> b >> c ;

        if(a == -1) break;

        p.push_back({{a,b},c});
    }

    entry_graph(p, g);
    
    
    int srt, end;

    cout << "Enter starting and finish point of the journey to find shortest path and its distance" << endl;

    cout << "start: ";
    cin >> srt;
    cout << "end: ";
    cin >> end;


    path(srt, end, g, V);
}



// improvements - make and shows graph when we enter the entries