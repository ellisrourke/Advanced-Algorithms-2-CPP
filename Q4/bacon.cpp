//
// Created by Ellis on 10/05/2021.
//
#include <unordered_map>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
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
    int count;
    unordered_map<string, Actor *> graph;

    Graph () : graph(){}
    Actor *retrieveNode(string name){
        if(graph.find(name) == graph.end()){
            return NULL;
        } else {
            return graph[name];
        }
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
        cout << "[?] Attempting to open file: " << filename << endl;

        file.open(filename);

        if(!file){
            cout << "[-] Unable to open file " << endl;
            exit(1);
        } else {
            cout << "[+] Reading data from file " << endl;
        }

        while(getline(file,line)){
            istringstream token(line);
            getline(token, actorName, '|');
            getline(token, movieName, '|');
            addNode(actorName, movieName);
            //cout << "The actor " << actorName << " was in the movie " << movieName << endl;
        }
        file.close();
        cout << "[+] Successfully read data from file " << endl;
        file.clear();
        file.seekg(0);

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

    Actor *breadthFirstSearch(Actor *start, Actor *target, bool findMax){
        if(start == NULL or target == NULL){
            cout << "[-] Unable to find key" << endl;
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
};

int main(int argc, char *argv[]){
    unordered_map<string, vector<string>> movie_actor;
    unordered_map<string, vector<string>> actor_movie;
    unordered_map<string, Actor> actor;


    if(argc < 4){
        cout << "Incorrect Arguments Passed" << endl;
        exit(1);
    }

    string filename=argv[1];
    string startActor = argv[2];
    string targetActor = argv[3];
    cout << "[?] Start Actor: " << startActor << endl;
    cout << "[?] Target Actor: " << targetActor << endl << endl;


    Graph imdb_a;
    imdb_a.readFile((string)filename);
    Actor *start = imdb_a.retrieveNode(startActor);
    Actor *target = imdb_a.retrieveNode(targetActor);
    imdb_a.breadthFirstSearch(start, target, false);
    cout << "- Part A -" << endl;
    imdb_a.displayPath(target);
    cout << " --------- " << endl;

/////////////////////////////////////////////////////////////////
    /*
    Graph imdb_b;
    imdb_b.readFile((string)filename);
    Actor *startb = imdb_b.retrieveNode(startActor);
    Actor *targetb = imdb_b.retrieveNode(targetActor);
    Actor *end = imdb_b.breadthFirstSearch(startb, targetb, true);
    cout << "- Part B -" << endl;
    imdb_b.displayPath(end);
     */
/////////////////////////////////////////////////////////////////

    Graph imdb_c;
    cout << "- Part C -" << endl;
    imdb_c.readFile(string(filename));
    Actor *start_c = imdb_c.retrieveNode("Gerald W. Abrams");
    Actor *end_c = imdb_c.retrieveNode("Don Ackerman (III)");
    imdb_c.breadthFirstSearch(start_c, end_c, false);
    imdb_c.displayPath(end_c);





    cout << "[+] Program End." << endl;

}