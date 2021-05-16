#include <unordered_map>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include <bits/stdc++.h>

using namespace std;

class Actor{
public:
    string name;
    Actor * parentActor;
    vector<Actor *> connections;
    bool isActor;

    Actor(string name, bool isActor) : name(name), connections(){
        parentActor = nullptr ;
        isActor = isActor;
    }

    void addConnection(Actor *connection) {
        connections.push_back(connection);
        connection->connections.push_back(this);
    }

};

class Graph{
public:
    int count=0;
    unordered_map<string, Actor *> graph;

    Graph () : graph(){}
    Actor *retrieveNode(string name){
        if(graph.find(name) == graph.end()){
            return NULL;
        }
        return graph[name];
    }

    void addNode(string actor, string movie){
        if(retrieveNode(actor) == NULL){
            Actor * newNode = new Actor(actor, true);
            graph[actor] = newNode;
        }
        if(retrieveNode(movie) == NULL){
            Actor * newNode = new Actor(movie, true);
            graph[movie] = newNode;
        }
        graph[actor]->addConnection(graph[movie]);
    }

    void readFile(string filename){
        ifstream file;
        string movieName;
        string actorName;
        string line;
        int count =0;
        cout << "[?] Attempting to open file: " << filename << endl;

        file.open(filename);

        if(!file){
            cout << "[-] Unable to open file " << endl;
            exit(1);
        } else {
            cout << "[+] Reading data from file " << endl;
        }

        if(file.is_open()){
            while(getline(file,line)){
                istringstream token(line);
                getline(token, actorName, '|');
                getline(token, movieName, '|');
                addNode(actorName, movieName);
                count++;
                //cout << "The actor " << actorName << " was in the movie " << movieName << endl;
            }
        }
        file.close();
        cout << "[+] Successfully read data from file " << endl;
        cout << "[+] Read " << count << " lines from file " << endl;
    }

    void displayPath(Actor *target){
        int count = 0;
        Actor *current = target;
        while(current != NULL){
            cout << current->name << endl;
            current = current->parentActor;
            count+=1;
        }
        cout << "Bacon Number: " << count / 2 << endl;
    }

    void populate(){
        ifstream myReadFile;
        string output, name, movie;
        myReadFile.open("../data/bacon1.txt");

        if(myReadFile.is_open()){
            while(getline(myReadFile, output)){
                istringstream token(output);
                getline(token, name, '|');
                getline(token, movie, '|');
                addNode(name, movie);
            }
        }
        myReadFile.close();
    }


};

Actor *breadthFirstSearch(Actor *start, Actor *target, bool findMax){
    if(start == NULL or target == NULL){
        cout << "[-] Unable to find key" << endl;
        return 0;
    }

    queue<Actor *>q;
    set<Actor *> s;
    Actor *last;

    q.push(start);
    s.insert(start);
    while(!q.empty()){
        Actor *current = q.front();
        last = current->isActor ? current : NULL;
        q.pop();

        if(current->name == target->name && !findMax){
            return current;
        }


        for(int i=0; i<current->connections.size(); i++){
            if(s.count(current->connections[i]) == 0){
                s.insert(current->connections[i]);
                q.push(current->connections[i]);
                current->connections[i]->parentActor = current;
            }
        }
    }
    return last;
}

int main(int argc, char *argv[]){
    //if(argc < 4){
   //     cout << "Incorrect Arguments Passed" << endl;
    //    exit(1);
   // }

    string filename=argv[1];
    //string startActor = argv[2];
    //string targetActor = argv[3];
    string startActor = "Kevin Bacon (I)";
    string targetActor = "Denzel Washington";

    cout << "[?] Start Actor: " << startActor << endl;
    cout << "[?] Target Actor: " << targetActor << endl << endl;

/*
    Graph imdb_a;
    imdb_a.readFile((string)filename);
    Actor *start = imdb_a.retrieveNode(startActor);
    Actor *target = imdb_a.retrieveNode(targetActor);
    imdb_a.breadthFirstSearch(start, target, false);
    cout << "- Part A -" << endl;
    imdb_a.displayPath(target);
    cout << " --------- " << endl;

/////////////////////////////////////////////////////////////////

    Graph imdb_b;
    cout << "- Part B -" << endl;
    imdb_b.readFile((string)filename);
    Actor *startb = imdb_b.retrieveNode("Kevin Bacon (I)");
    Actor *targetb = imdb_b.retrieveNode(targetActor);
    Actor *end = imdb_b.breadthFirstSearch(startb, targetb, true);
    imdb_b.displayPath(end);
    cout << " --------- " << endl;
*/
/////////////////////////////////////////////////////////////////

    Graph imdb_c;
    cout << "- Part C -" << endl;
    imdb_c.readFile((string)filename);
    Actor *start_c = imdb_c.retrieveNode("Christian Slate");
    Actor *end_c = imdb_c.retrieveNode("Catherine Zeta-Jones");

    for (auto const& name : imdb_c.graph)
    {
        std::cout << name.first << endl;
    }


    cout << "start " << start_c->name << endl;
    cout << "end " << end_c->name << endl;

    breadthFirstSearch(start_c, end_c, false);
    imdb_c.displayPath(start_c);
    cout << " --------- " << endl;




    cout << "[+] Program End." << endl;

}