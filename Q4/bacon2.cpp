#include <iostream>
#include <queue>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>
#include <set>
#include <queue>
using namespace std;

class Actor{
public:
    string name;
    vector<Actor *> connections;
    Actor * parent;
    bool isActor;

    Actor(string name, bool isActor) : name(name), connections(){
        parent = nullptr;
        isActor = isActor;
    }

    void addConnection(Actor *connection){
        connections.push_back(connection);
        connection->connections.push_back(this);
    };
};

class Graph{
public:
    unordered_map<string, Actor *> g;
    Graph () : g(){}

    Actor *returnNode(string name){
        if(g.find(name) == g.end()){
            return NULL;
        }
        return g[name];
    }


    void addConnection(string name, string movie){
        if(returnNode(name) == NULL){
            Actor * a = new Actor(name, true);
            g[name] = a;
        }
        if(returnNode(movie) == NULL){
            Actor * m = new Actor(movie, true);
            g[movie] = m;
        }
        g[name]->addConnection(g[movie]);
    }

    void readData(){
        ifstream file;
        string line;
        string name;
        string movie;
        int count;
        file.open("../data/bacon3.txt");

        while(getline(file, line)){
            istringstream token(line);
            getline(token, name, '|');
            getline(token, movie, '|');
            addConnection(name, movie);
            count++;
        }
        file.close();
        cout << count << " lines" << endl;


    }
};

Actor *Breadth_first_search(Graph &graph, Actor *start, Actor *end, bool findMax){
    if(start == NULL || end == NULL){
        cout << "Does not exist" << endl;
        return 0;
    }

    queue<Actor *> q;
    set<Actor *> s;
    Actor *last;

    q.push(start);
    s.insert(start);

    while(!q.empty()){
        Actor *current = q.front();
        //last = current->isActor ? current : NULL;
        last = current;
        q.pop();

        if(current->name == end->name && !findMax){
            return current;
        }
        for(int i=0; i< current->connections.size(); i++){
            if(s.count(current->connections[i]) == 0){
                s.insert(current->connections[i]);
                q.push(current->connections[i]);
                current->connections[i]->parent = current;
            }
        }
    }
    return last;
}

void bacon_number(Graph g, Actor *target){
    int bacon = 0;
    Actor *node = target;
    while(node != NULL){
        cout << " -> " << node->name;
        node = node->parent;
        bacon++;
    }

    cout << endl << "Bacon Number: " << bacon / 2 << endl;
}

int main(){

    Graph imdb_a;
    imdb_a.readData();
    cout << "Part A:" << endl;
    cout << "-------------------------------------" << endl;
    Actor *a_a = imdb_a.returnNode("Carmen Villa (II)");
    Actor *a_b = imdb_a.returnNode("Pete Smith (I)");
    Breadth_first_search(imdb_a, a_a, a_b, false);
    bacon_number(imdb_a, a_b);
    cout << "-------------------------------------" << endl;


    Graph imdb_b;
    cout << "Part B:" << endl;
    cout << "-------------------------------------" << endl;
    imdb_b.readData();
    cout << "data size " << imdb_b.g.size() << endl;
    Actor *b_a = imdb_b.returnNode("Carmen Villa (II)");
    Actor *b_b = imdb_b.returnNode("Pete Smith (I)");
    Actor *longest = Breadth_first_search(imdb_b, b_a, b_b, true);
    cout << "Longest " << longest->name << endl;
    bacon_number(imdb_b, longest);
    cout << "-------------------------------------" << endl;


    cout << "Part C:" << endl;
    cout << "-------------------------------------" << endl;
    Graph imdb_c;
    imdb_c.readData();
    Actor *c_a = imdb_c.returnNode("Carmen Villa (II)");
    Actor *c_b = imdb_c.returnNode("Pete Smith (I)");
    Breadth_first_search(imdb_c, c_a, c_b, false);
    bacon_number(imdb_c, c_b);
    cout << "-------------------------------------" << endl;



    return 0;
}