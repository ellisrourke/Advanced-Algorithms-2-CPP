//
// Created by Ellis on 10/05/2021.
//
#include <unordered_map>
#include <string>
#include <vector>
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
            cout << "The actor " << actorName << " was in the movie " << movieName << endl;
        }
        file.close();
        cout << "[+] Successfully read data from file " << endl;

    }
};

int main(int argc, char *argv[]){
    unordered_map<string, vector<string>> movie_actor;
    unordered_map<string, vector<string>> actor_movie;
    unordered_map<string, Actor> actor;


    if(argc < 3){
        cout << "Incorrect Aguments Passed" << endl;
        exit(1);
    }

    string filename=argv[1];
    string key = argv[2];
    cout << "[?] Specified key: " << key << endl;


    Graph imdb;
    imdb.readFile((string)filename);
    //loadDataFromFile(file, movie_actor, actor_movie, actor, key);
    //breadthFirstSearch(movie_actor, actor_movie, actor, key);

    cout << "[+] Program End." << endl;

}