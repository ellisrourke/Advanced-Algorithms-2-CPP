#include <iostream>
#include <queue>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>
//#include "stdc++.h"

using namespace std;

class ActorNode {
	public:
		// True = Actor, False = Movie
		ActorNode * parent;
		bool actor;
		string node_name;
		vector<ActorNode *> neighbors;

		ActorNode(string input, bool type) : node_name(input), neighbors(){
			parent = nullptr;
			actor = type;
		}

		void addEdge(ActorNode *edge){
			neighbors.push_back(edge);
			edge->neighbors.push_back(this);
		}
};

class Graph {
	public:
		int number_ent;
		unordered_map<string, ActorNode *> um;

		Graph () : um(){}

		ActorNode *getNodePointer(string node){
			// if node if not found
			if(um.find(node) == um.end()){
				return NULL;
			}
			return um[node];
		}

		void addNode(string name, string movie){
			// If actor does not exist
			if(getNodePointer(name) == NULL){
				ActorNode * n = new ActorNode(name, true);
				um[name] = n;
			}
			if(getNodePointer(movie) == NULL){
				ActorNode * n = new ActorNode(movie, true);
				um[movie] = n;
			}
			um[name]->addEdge(um[movie]);
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

ActorNode *BFS(Graph &graph, ActorNode *start, ActorNode *stop, bool highest){
	if(start == NULL || stop == NULL){
		cout << "Does not exist" << endl;
		return 0;
	}
	queue<ActorNode *> que;
	set<ActorNode *> s;
	ActorNode *last_node;

	/* Que is a FIFO and the set is
	   to make sure a neighbor is only
	   visited once.
	   */
	que.push(start);
	s.insert(start);
	while(!que.empty()){
		ActorNode *n = que.front();
		last_node = n->actor ? n : NULL;
		que.pop();

		/* Find neighbors and if we are finding a start and stop
		   between two actors one being Kevin Bacon then Highest
		   is equal to false. If looking for highest bacon number
		   then its set to true.
		   */
		if(n->node_name == stop->node_name && highest == false)
			return n;
		for(int i=0; i < n->neighbors.size(); i++){
			if(s.count(n->neighbors[i]) == 0){
				s.insert(n->neighbors[i]);
				que.push(n->neighbors[i]);
				n->neighbors[i]->parent = n;
			}
		}
	}
	return last_node;
}

// Print the Graph
void printPath(Graph g, ActorNode *stop){
	int bacon_number = 0;
	ActorNode *ptr = stop;
	while(ptr != NULL){
		cout << "\t" << ptr->node_name << endl;
		ptr = ptr->parent;
		bacon_number += 1;
	}
	cout << "\tBacon number = " << bacon_number/2 << endl;
}

int main(){
	// PART A
	Graph graph;

	graph.populate();
	cout << "Part A:" << endl;
	ActorNode *kevin = graph.getNodePointer("Kevin Bacon (I)");
	ActorNode *stop = graph.getNodePointer("Denzel Washington");

	BFS(graph, kevin, stop, false);
	printPath(graph, stop);

	//Part B
	Graph graph_two;

	graph_two.populate();
	cout << "Part B:" << endl;
	ActorNode *kevin_two = graph_two.getNodePointer("Kevin Bacon (I)");

	ActorNode *end = BFS(graph_two, kevin, stop, true);
	printPath(graph_two, end);



	// Part C
	Graph graph_three;
	graph_three.populate();
	cout << "Part C:" << endl;
	ActorNode *chris = graph_three.getNodePointer("Christian Slater");
	ActorNode *zeta = graph_three.getNodePointer("Catherine Zeta-Jones");

	BFS(graph_three, chris, zeta, false);
	printPath(graph_three, zeta);

	return 0;
}
