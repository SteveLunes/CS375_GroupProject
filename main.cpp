#include <stdlib.h>
#include <iostream>
#include <vector>
#include <time.h> // used to generate random edge weights between 1-50

using namespace std;

// generates a graph with edges weights between 1-50
vector<vector<int>> createGraph(int numNodes, int numEdges, bool directed);

// helper function for generating graphs
// checks if a node is already in the the subgraph
bool contains(vector<int> selectedNodes, int i);
void printGraph(vector<vector<int>> graph);

int main(int argc, char const *argv[]) {

  // parameters: createGraph(int numNodes, int numEdges, bool directed)
  vector<vector<int>> graph1 = createGraph(5,5,false); // undirected graph with 5 nodes and 5 edges
  vector<vector<int>> graph2 = createGraph(5,12,true); // directed graph with 10 nodes and 12 edges

  printGraph(graph1);
  printGraph(graph2);

  return 0;
}

vector<vector<int>> createGraph(int numNodes, int numEdges, bool directed) {
  srand(time(NULL));
  // generate random number from 1 to 50
  // rand() % 50 + 1

  vector<vector<int>> graph(numNodes, vector<int>(numNodes, 0) );
  if(directed) {
    if(numEdges > numNodes*numNodes || numEdges < numNodes - 1) {
      cout << "Invalid number of edges for directed graph." << endl;
      return graph;
    }
  } else { // undirected
    if(numEdges >  numNodes*(numNodes-1)/2 || numEdges < numNodes - 1) {
      cout << "Invalid number of edges for undirected graph." << endl;
      return graph;
    }
  }

  vector<int> selectedNodes; // the nodes currently in the subgraph
  selectedNodes.push_back(rand() % numNodes);
  //cout << "selectedNodes: " << selectedNodes[0] << endl;

  int node1;
  int node2;

  int count = numEdges;

  while(count > 0) {

    bool found = false;

    while(!found) {
      node1 = rand() % numNodes;
      node2 = selectedNodes[rand()%selectedNodes.size()];

      //cout << "node1: " << node1 << endl;
      //cout << "node2: " << node2 << endl;

      if(node1!=node2 && graph[node1][node2] == 0) {
         found = true;
         //cout << "created an edge between " << node1 << " and " << node2 << endl;
      }
    }

    if(!contains(selectedNodes, node1)) selectedNodes.push_back(node1);

    graph[node1][node2] = rand() % 50 + 1; // random weight from 1-50
    if(!directed) graph[node2][node1] = graph[node1][node2]; // undirected graphs are symmentric
    //printGraph(graph);
    count--;

  }

  return graph;
}

void printGraph(vector<vector<int>> graph) {
  for(int i=0; i<graph[1].size(); i++) {
    for(int j=0; j<graph[1].size(); j++) {
      cout << graph[i][j] << ' ';
    }
    cout << endl;
  }
  cout << endl;
}

bool contains(vector<int> selectedNodes, int num) {
  bool contains = false;

  for(int i=0; i<selectedNodes.size(); i++) {
    if(selectedNodes[i] == num) contains = true;
  }

  return contains;
}
