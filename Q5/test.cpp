// 3805ICT Advanced Algorithms, Assignment #3.
// Identifies minimal vertex covers within the complement graph of a supplied graph file.
// Graph file (.clq) is supplied as a command-line argument.
// Jack Kearsley (s5007230). October, 2018.

//////////////////////////////////// Includes ///////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <set>
#include <string>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>
#include <queue>
#include <list>
#include <array>
#include <climits>
#include <ctime>

using namespace std;

//////////////////////////////// Structs & Objects /////////////////////////////////////
class Vertex {
public:
    int id;
    // Adjacency list.
    set<int> adj;
    int degree;
    double pnFactor = 1.0;
    double score = 0.0;
};
// Filepath to graph file.
char const * GRAPH_FILE;
// Name of graph.
char * GRAPH_NAME;

// Stores the best search parameters for each graph in the DIMACS-C test set.
class Parameter {
public:
    int numSearches;
};
clock_t clk_start;


/////////////////////////////// Function Prototypes ////////////////////////////////////
void readComplementClqFile(const char * file, unordered_map<int, Vertex> &g, int * V, int * E);
void greedySearch(unordered_map<int, Vertex> &g, set<int> &cover);

bool scoreCmp(pair<int, Vertex> a, pair<int, Vertex> b);
bool degreeCmp(pair<int, Vertex> a, pair<int, Vertex> b);
bool degreeCmpRev(pair<int, Vertex> a, pair<int, Vertex> b);
void dfs(unordered_map<int, Vertex> &g, set<int> &cover);


////////////////////////////// Function Implementations ///////////////////////////////

int main(int argc, char * argv[]) {
    if (argc < 2) {
        printf("Usage: s5007230_ass3.exe graph.clq");
        return 1;
    }
    srand(time(NULL));
    GRAPH_FILE = argv[1];

    // The graph file is specified at the command-line as argv[1].
    // Read in the complement graph of the specified graph file.
    unordered_map<int, Vertex> graph;
    int V = 0, E = 0;
    readComplementClqFile(GRAPH_FILE, graph, &V, &E);
    clk_start = clock();

    // Search for vertex cover.
    set<int> cover;
    //greedySearch(graph, cover);
    dfs(graph, cover);

    // Print statistics.
    double t_elapsed = (double)(clock() - clk_start) / CLOCKS_PER_SEC;
    printf("\n" "Vertex cover size: %d/%d\n", cover.size(), graph.size());
    printf("Time elapsed: %.4lfs\n", t_elapsed);
    // Write vertex cover to a CSV file.
    return 0;
}

void recursiveUtil(unordered_map<int, Vertex> &g, set<int> &cover, int remove, set<int> &bestCover, int * bestCoverSize, int level, set<int> &ignore, unsigned long long * op) {
    // Add adjacent vertices to 'ignore' list.
    cover.erase(remove);//
    ignore.insert(remove);//
    auto remV = g.find(remove);//
    for (auto adj : remV->second.adj) {//
        ignore.insert(adj);//
    }//

    set<int> covercopy;
    int numSearches = level * log(level + 3) + 5;
    if (level == 1) numSearches = 20;

    // Create an set of candidate vertices to be searched.
    // If a vertex is in the 'ignore' set it can't be part of 'candidates'.
    set<int> candidates = (cover);
    for (auto ig : ignore) {
        candidates.erase(ig);
    }

    // If less candidates than allowed searches, just search all the candidates and skip selection process.
    if (candidates.size() <= numSearches) {
        covercopy = (candidates);
    }
    else {
        int nm = 0;
        int numTrials = 3;
        while (nm < numSearches) {
            double minDegree = (double)INT_MAX;
            int minDegreeVertex = 0;
            for (int i = 0; i < numTrials; i++) {
                int roll = rand() % (candidates.size() - 1);
                auto it = candidates.begin();
                std::advance(it, roll);
                auto v = g.find(*it);
                // Update the score for 'v' based on penalty factor.
                v->second.score = (double)(v->second.degree) * v->second.pnFactor;
                if (v->second.score < minDegree) {
                    minDegree = v->second.score;
                    minDegreeVertex = *it;
                }
            }
            covercopy.insert(minDegreeVertex);
            candidates.erase(minDegreeVertex);
            nm++;
        }
    }

    // For each vertex in 'cover'.
    for (int c : covercopy) {

        auto vpnt = g.find(c);
        bool canRemove = true;

        // See if all adjacent vertices of 'c' are in cover already
        for (auto adj : vpnt->second.adj) {
            // If an adajcent vertex isn't in cover, cant remove vertex 'c'.
            if (cover.find(adj) == cover.end()) {
                canRemove = false;
                break;
            }
        }

        // Remove the vertex and call function recursively.
        // Create 'cover' because the for loop is iterating over 'cover'.
        if (canRemove) {
            *op++;
            recursiveUtil(g, cover, c, bestCover, bestCoverSize, level + 1, ignore, op);
            //printf("finished recursiveUtil for level %d, removed %d, bestcoversize: %d\n", level, remove, *bestCoverSize);
        }
        if (level == 1) printf("Vertex %d completed, bestcoversize: %d\n", c, *bestCoverSize);

    } /////////// for (all vertices in cover) ////////////

    // Keep track of smallest cover found so far.
    if (cover.size() < *bestCoverSize) {
        bestCover = cover;
        *bestCoverSize = cover.size();
        printf("New best found, bestCoverSize: %d\n", *bestCoverSize);
        double t_elapsed = (double)(clock() - clk_start) / CLOCKS_PER_SEC;
        printf("Time elapsed: %.4lfs\n", t_elapsed);
    }

    // Insert the removed vertex back into 'cover' before returning.
    cover.insert(remove);
    ignore.erase(remove);
    auto remU = g.find(remove);
    for (auto adj : remU->second.adj) {
        ignore.erase(adj);
    }

    // Reset penalties every so often.
    if (*op % 50 == 0) {
        for (auto c : cover) {
            auto V = g.find(c);
            V->second.pnFactor = 1.0;
        }

    }
    ////////////// for (vertex in covercopy) //////////////////

}


void dfs(unordered_map<int, Vertex> &g, set<int> &cover) {
    printf("\nStarting DFS...\n");
    set<int> bestCover;
    int bestCoverSize = INT_MAX;
    // Set the cover as all the vertices in 'g'.
    for (auto element : g) {
        cover.insert(element.first);
    }
    set<int> ignore;
    int minVertex = 0;
    double minVertexScore = (double)INT_MAX;

    // Pick smallest degree of a number of trials.
    int trials = 3;
    for (int i = 0; i < trials; i++) {
        int roll = rand() % (cover.size() - 1);
        auto it = cover.begin();
        advance(it, roll);
        auto V = g.find(*it);
        if (V->second.score < minVertexScore) {
            minVertexScore = V->second.score;
            minVertex = *it;
        }
    }

    // Begin recursive depth-first search.
    unsigned long long op = 0;
    recursiveUtil(g, cover, minVertex, bestCover, &bestCoverSize, 1, ignore, &op);
    cover = bestCover;
    printf("...DFS completed.\n");
}









////////////////////////////// std::sort() comparison functions /////////////////////////////

////////////////////////////////// Utility Functions ////////////////////////////////////////
// Read in complement graph from .clq file.
void readComplementClqFile(const char * file, unordered_map<int, Vertex> &g, int * V, int * E) {
    printf("Reading complement graph of '%s' file... ", file);
    FILE * f = fopen(file, "r");
    if (!f) {
        printf("Error opening '%s' file.\n", file);
        exit(1);
    }
    // Extract filename from path.
    char * gfileCpy = strdup(file);
    char * tok = strtok(gfileCpy, "/\\");
    char * prev = strdup(tok);
    while (tok != NULL) {
        prev = strdup(tok);
        tok = strtok(NULL, "/\\");
    }
    GRAPH_NAME = prev;

    // Read in file one line at a time.
    char line[200];
    while (fgets(line, sizeof(line), f)) {
        // Skip commented lines.
        if (line[0] == 'c') {
            continue;
        }
            // 'p' line contains the number of vertices (V) and edges (E).
        else if (line[0] == 'p') {
            // Skip the first two words.
            char * tok;
            tok = strtok(line, " ");
            tok = strtok(NULL, " ");
            // Get number of vertices and edges.
            *V = atoi(strtok(NULL, " "));
            *E = atoi(strtok(NULL, " "));
            // Make the graph a complete graph (all possible edges between all vertices).
            for (int i = 1; i <= *V; i++) {
                Vertex new_vert = Vertex();
                new_vert.id = i;
                for (int j = 1; j < *V; j++) {
                    new_vert.adj.insert(j);
                }
                new_vert.adj.erase(i);
                pair<int, Vertex> pr = { i, new_vert };
                g.insert(pr);
            }
        }
            // All other 'e' lines are edges between two vertices.
        else if (line[0] == 'e') {
            char * tok;
            tok = strtok(line, " ");
            int src = atoi(strtok(NULL, " "));
            int dst = atoi(strtok(NULL, " "));
            // Remove the edge from the complete graph, to create the complement graph.
            auto vertex = g.find(src);
            if (vertex != g.end()) {
                vertex->second.adj.erase(dst);
            }
            vertex = g.find(dst);
            if (vertex != g.end()) {
                vertex->second.adj.erase(src);
            }
        }
    }/////// while (reading file) //////////

    for (auto i = g.begin(); i != g.end(); i++) {
        i->second.degree = i->second.adj.size();
        i->second.score = (double)(i->second.degree);
        i->second.pnFactor = 1.0;
    }

    printf("...file read successfully.\n");
    fclose(f);
}

