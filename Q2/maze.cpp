#include <iostream>
using namespace std;
#include <unordered_map>

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

    //void createSet(){
   //    for(int i=0; i<n; i++){
    //        parent[i] = i;
    //    }
   // }

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

struct Point{
    int x;
    int y;
};

void random_maze(DisjointSet &p){
    int rows = 55;
    int collumns = 88;
    int start = 0 * 0;
    int end = rows-1 * collumns-1;
    int numCells = rows * collumns;

    for(int i=1; i< rows; i++){
        for(int j=1; j<collumns; j++){
            p.createSet(i*j);
            //DisjointSet x;
            //x.createSet(i * j);
            //sets[i*j] = x;
        }
    }

    cout << p.findItem(1) << endl;
    cout << "test" << endl;
}

int main(){
        DisjointSet obj(5);
        ///obj.unionSet(0, 2);
       //obj.unionSet(4, 2);
        //obj.unionSet(3, 1);

        random_maze(obj);

        /*
        if (obj.findItem(4) == obj.findItem(0))
            cout << "Yes\n";
        else
            cout << "No\n";
        if (obj.findItem(1) == obj.findItem(0))
            cout << "Yes\n";
        else
            cout << "No\n";

        return 0;
        */
}
//https://betterprogramming.pub/using-disjoint-set-union-find-to-build-a-maze-generator-7462ea3b8632