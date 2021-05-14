#include <iostream>
using namespace std;
#include <unordered_map>
#include <vector>
#include <ctime>

class DisjointSet{
    int n;
    int *parent;
    int *rank;
public:
    DisjointSet(int n){
        this->n = n;
        parent = new int[n];
        rank = new int[n];
        createSet(0);
    }

    void createSet(int v){
        parent[v] = v;
    }

    int findItem(int v){
        if(parent[v] != v){
            parent[v] = findItem(parent[v]);
        }
        return parent[v];
    }

    void unionSet(int a, int b){
        int aSet = findItem(a);
        int bSet = findItem(b);

        if(aSet == bSet){ return; }

        if(rank[aSet] < rank[bSet]){
            parent[aSet] = bSet;
        } else if(rank[aSet] > rank[bSet]){
            parent[bSet] = aSet;
        } else {
            parent[bSet] = aSet;
            rank[aSet] = rank[aSet] + 1;
        }
    }
};

int main(){
    int rows = 10;
    int collumns = 10;
    //vector<vector<DisjointSet>> maze;
    vector<DisjointSet> maze;
    int start = 1;
    int end = rows * collumns;
    int numCells = rows * collumns;

    for(int i=0; i< rows*collumns; i++){
        DisjointSet x(i);
        maze.push_back(x);
    }

    maze.unionSet(0, 2);
    //obj.unionSet(4, 2);
    //obj.unionSet(3, 1);
    /*
    srand (time(NULL));
    int count = 100;
    while(count > 0){
        int x = (rand() % rows) + 1 ;
        int y = (rand() % collumns) + 1 ;
        cout << "x " << x << " y " << y << endl;
        count--;
        //int setNumber = x * y
    }
    */
    cout << "End" << endl;


}
//https://betterprogramming.pub/using-disjoint-set-union-find-to-build-a-maze-generator-7462ea3b8632