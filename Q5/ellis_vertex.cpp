#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <set>
#include <cmath>
#include <vector>
#include <queue>
#include <list>
#include <array>
#include <ctime>
#include <bits/stdc++.h>

using namespace std;

class Node {
public:
    int i;
    set<int> list;
    int degree;
    double pnFactor = 1.0;
    double score = 0.0;
    bool remove = true;
};

void recursive_depthFirstSearch(unordered_map<int, Node> &graph, set<int> &current_cover, int min_node, set<int> &best_cover, set<int> &exclude, int *min_cover_size, int depth, unsigned long long *reset_penalty) {
    current_cover.erase(min_node);
    exclude.insert(min_node);
    auto remove_node = graph.find(min_node);
    for (auto list : remove_node->second.list) {
        exclude.insert(list);
    }

    int branches = depth * log(depth + 3) + 5;
    if (depth == 1) {
        branches = 20;
    }

    set<int> options = (current_cover);
    for (auto it : exclude) {
        options.erase(it);
    }

    set<int> duplicate_cover;

    if (options.size() < branches) {
        duplicate_cover = (options);
    } else {
        int n = 100;
        for (int i = 0; i < branches; i++) {
            int min_node = 0;
            double min_deg = (double) INT_MAX;
            for (int j = 0; j < n; j++) {
                int randomNode = (rand() % options.size()) - 1;
                auto it = options.begin();
                advance(it, randomNode);
                auto node = graph.find(*it);
                node->second.score = node->second.degree * node->second.pnFactor * 1.0;
                if (node->second.score < min_deg) {
                    min_deg = node->second.score;
                    min_node = *it;
                }
            }
            duplicate_cover.insert(min_node);
            options.erase(min_node);
        }
    }

    for (int it : duplicate_cover) {
        auto vector = graph.find(it);
        for (auto v : vector->second.list) {
            if (current_cover.find(v) == current_cover.end()) {
                vector->second.remove = false;
                break;
            }
        }
        if (vector->second.remove == true) {
            *reset_penalty++;
            recursive_depthFirstSearch(graph, current_cover, it, best_cover, exclude, min_cover_size, depth + 1,
                                       reset_penalty);
        }

        if (*min_cover_size > current_cover.size()) {
            best_cover = current_cover;
            *min_cover_size = current_cover.size();
            cout << "Found new min vertex cover: " << *min_cover_size << endl;
        }


        current_cover.insert(min_node);
        exclude.erase(min_node);
        remove_node = graph.find(min_node);
        for (auto it : remove_node->second.list) {
            exclude.erase(it);
        }

        if(*reset_penalty % 50 == 0){
            for(auto it : current_cover){
                auto node = graph.find(it);
                node->second.pnFactor = 1.0;
            }
        }


    }
}//END

void depthFirstSearch(unordered_map<int, Node> &graph){
    cout << "[?] Starting Depth First Search" << endl;
    set<int> best_cover;
    set<int> current_cover;
    set<int> exclude_cover;

    int min_cover_size = graph.size();
    cout << "Start min vertex cover: " << min_cover_size << endl;

    int min_node = 0;
    double min_node_score = INT_MAX;

    for(auto e : graph){
        current_cover.insert(e.first);
    }

    for(int i=0; i<10; i++){
        int randomNode = (rand() % current_cover.size()) - 1;
        auto it = current_cover.begin();
        advance(it, randomNode);
        auto node = graph.find(*it);
        if(node->second.score < min_node_score){
            cout << "better random is " << min_node << endl;
            min_node_score = node->second.score;
            min_node = *it;
        }
    }

    unsigned long long reset_penalty = 0;
    recursive_depthFirstSearch(graph, current_cover, min_node, best_cover, exclude_cover, &min_cover_size, 1, &reset_penalty);
}


int main(int argc, char * argv[]){
    srand(time(NULL));
    unordered_map<int, Node> graph;
    int V;
    int E;
    int lines = 0;
    string filename = "../data/" + (string)argv[1];
    //read file
    std::ifstream file;
    std::string line;
    cout << "[?] Opening file " << filename << endl;
    file.open(filename);
    while(getline(file, line)){
        lines++;
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

            V = numVert;
            E = numEdges;

            for(int i=0; i<V; i++){
                Node n = Node();
                n.i = i+1;
                for(int j=0; j<V; j++){
                    n.list.insert((j+1));
                }
                n.list.erase(i+1);
                pair<int, Node> piar = std::make_pair(i, n);
                graph.insert(piar);
            }

        }
        if(line[0] == 'e'){
            string u;
            string v;
            istringstream token(line);
            getline(token, u, ' ');
            getline(token, u, ' ');
            getline(token, v, ' ');

            auto vx = graph.find(stoi(u));
            if(vx != graph.end()){
                vx->second.list.erase(stoi(v));
            }
            vx = graph.find(stoi(v));
            if(vx != graph.end()){
                vx->second.list.erase(stoi(u));
            }
        }
    }
    for(auto it = graph.begin(); it != graph.end(); it++){
        it->second.degree = it->second.list.size();
        it->second.pnFactor = 1.0;
        it->second.score = (double)(it->second.degree);
    }
    cout << "[+] Sucessfully read from file" << endl;
    cout << "[?] Read " << lines << " lines" << endl;
    cout << "[?] Number of edges " << E << endl;
    cout << "[?] Number of vertices " << V << endl;
    file.close();
    cout << "[?] Closed file" << endl;

    depthFirstSearch(graph);
}