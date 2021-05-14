// Program to print Vertex Cover of a given undirected graph
#include<iostream>
#include <list>
#include <string>
#include <bits/stdc++.h>

using namespace std;

#include<bits/stdc++.h>
using namespace std;
#define max 1000
//array to store graph
bool arr[max][max];
//check if minimum vertex cover exists
bool check_cover(int V, int k, int E) {
    int set = (1 << k) - 1;
    int limit = (1 << V);
    //to mark the edges of size 'k'
    bool vis[max][max];
    while (set < limit) {
        //resetting the vertex cover for each iteration
        memset(vis, 0, sizeof vis);
        int count = 0;
        //checking the values which has a high value
        for (int j = 1, v = 1 ; j < limit ; j = j << 1, v++) {
            if (set & j) {
                //marking the edges visited
                for (int k = 1 ; k <= V ; k++) {
                    if (arr[v][k] && !vis[v][k]) {
                        vis[v][k] = 1;
                        vis[k][v] = 1;
                        count++;
                    }
                }
            }
        }
        //if all the edges are covered
        if (count == E)
            return true;
        int c = set & -set;
        int r = set + c;
        set = (((r^set) >> 2) / c) | r;
    }
    return false;
}
//to find minimum vertex cover
int find_cover(int n, int m) {
    //performing binary search
    int left = 1, right = n;
    while (right > left){
        int mid = (left + right) >> 1;
        if (!check_cover(n, mid, m))
            left = mid + 1;
        else
            right = mid;
    }
    return left;
}
//inserting edge in the graph
void add_edge(int u, int v) {
    arr[u][v] = 1;
    arr[v][u] = 1;
}

// Driver program to test methods of graph class
int main()
{

    //Graph g(1);
    std::ifstream file;
    std::string line;
    file.open("../data/brock800_1.clq");
    while(getline(file, line)){
        if(line[0] == 'p'){
            string v;
            string e;
            istringstream token(line);
            getline(token, e, ' ');
            getline(token, e, ' ');
            getline(token, v, ' ');
            getline(token, e, ' ');

            int numEdges = stoi(e);
            int numVert = stoi(v);

            cout << "Number of e " << numEdges << endl;
            cout << "Number of v " << numVert << endl;

        }
        if(line[0] == 'e'){
            string u;
            string v;
            istringstream token(line);
            getline(token, u, ' ');
            getline(token, u, ' ');
            getline(token, v, ' ');
            add_edge(stoi(u),stoi(v));
            //cout << u << " -> " << v << endl;
            //g.addEdge(stoi(u),stoi(v));
        }
    }

    // Create a graph given in the above diagram
    //g.printVertexCover();
    cout << find_cover(V, E);
    cout << "End" << endl;
    return 0;
}