#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h> // used to generate random edge weights between 1-50
#include <ctime> // timer
#include <chrono>

using namespace std;
using namespace std::chrono;

// generates a graph with edges weights between 1-50
vector<vector<int>> createGraph(int numNodes, int numEdges, bool directed, bool connected);

// Dijkstra's Algorithm
int minDist(int *dist, bool *inTree, int numVertices);
void dijkstra(vector <vector <int> > graph, int start, int numVertices);

// helper function for generating graphs
// checks if a node is already in the the subgraph
bool contains(vector<int> selectedNodes, int i);
void printGraph(vector<vector<int>> graph);

int main(int argc, char const *argv[]) {

  // parameters: createGraph(int numNodes, int numEdges, bool directed)
  // vector<vector<int>> graph1 = createGraph(5,5,false,true);
  // vector<vector<int>> graph2 = createGraph(5,5,true,true);
  // vector<vector<int>> graph3 = createGraph(5,7,true);
  // vector<vector<int>> graph4 = createGraph(5,8,true);

  //printGraph(graph2);
  //
  // for(int i=0;i<5; i++) {
  //   for(int j=0; j<5; j++) {
  //     if (graph2[i][j] != 0) {
  //     graph2[j][i] = graph2[i][j];
  //   } else if (graph2[j][i] != 0) {
  //     graph2[i][j] = graph2[j][i];
  //   }
  //   }
  // }

  //printGraph(graph2);
  // // dijkstra(graph1, 0, 5);
  // // cout << endl;
  //printGraph(graph2);
  // printGraph(graph3);
  // printGraph(graph4);
  // dijkstra(graph2, 0, 5);
  //

  // write to output file
  // ofstream myfile;
  // myfile.open("data.csv");

  //double duration;
  auto duration = 0;
  typedef chrono::milliseconds ms;
  high_resolution_clock::time_point start;
  high_resolution_clock::time_point end;
  // dense-sparse graphs
  //myfile << "Number of Edges,Time" << endl;

  // for(unsigned long i=99; i<4450; i++) {
  //   vector<vector<int>> graph = createGraph(100,i,true,true);
  //   //clock_t start = clock();
  //   start = high_resolution_clock::now();
  //   dijkstra(graph, 0, 100);
  //   //duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
  //   end = high_resolution_clock::now();
  //   duration += duration_cast<ms>(end - start).count();
  //   cout << "number of edges: " << i << endl;
  //   cout << "time: " << duration << endl;
  //   //myfile << i << ',' << duration << endl;
  // }

  // undirected vs directed
  int x = 1000; // numNodes
  for(int k=0; k<5000; k++) {
    cout << k << endl;
    vector<vector<int>> graph = createGraph(x,1500,true,true);
    //clock_t start = clock();
    start = high_resolution_clock::now();
    dijkstra(graph, 0, x);
    //duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
    end = high_resolution_clock::now();

    duration += duration_cast<ms>(end - start).count();
    cout << "directed time: " << duration << endl;

    duration = 0;

    //printGraph(graph);
  //  int temp;
    bool ud[x];
    for (int i = 0; i < x; i++) ud[i] = false;
    for(int i=0;i<x; i++) {
      for(int j=0; j<x; j++) {
        if (graph[i][j] != 0 && graph[j][i] == 0) {
        graph[j][i] = graph[i][j];
        ud[j] = true;
      } else if (graph[j][i] != 0 && graph[i][j] == 0) {
        graph[i][j] = graph[j][i];
        ud[i] = true;
      } else if (graph[i][j] != 0 && graph[j][i] != 0 && ud[j] == false && ud[i] == false) {
    int   temp = graph[i][j] + graph[j][i];

        graph[i][j] = temp;
        graph[j][i] = temp;
      }
    }
  }

    start = high_resolution_clock::now();
    //start = clock();
    dijkstra(graph, 0, x);
    end = high_resolution_clock::now();
    //duration = duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
    duration += duration_cast<ms>(end - start).count();
    cout << "undirected time: " << duration << endl;
    //printGraph(graph);

    duration = 0;
  }

  // myfile.close();
  return 0;
}

vector<vector<int>> createGraph(int numNodes, int numEdges, bool directed, bool connected) {
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
      if(connected) node2 = selectedNodes[rand()%selectedNodes.size()];
      else node2 = rand() % numNodes;

      //cout << "node1: " << node1 << endl;
      //cout << "node2: " << node2 << endl;

      if(node1!=node2 && graph[node1][node2] == 0) {
         found = true;
         //cout << "created an edge between " << node1 << " and " << node2 << endl;
      }
    }

    if(connected) if(!contains(selectedNodes, node1)) selectedNodes.push_back(node1);

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

/*
  Dijkstra's Algorithm
*/
int minDist(int *dist, bool *inTree, int numVertices) {
	int minDist = INT_MAX, minIndex;
	for (int i = 0; i < numVertices; i++) {
		if (!(inTree[i]) && dist[i] <= minDist) {
			minIndex = i;
			minDist = dist[i];
		}
	}
	return minIndex;
}

void dijkstra(vector <vector <int> > graph, int start, int numVertices) {
	int dist[numVertices];
	bool inTree[numVertices];
	for (int i = 0; i < numVertices; i++) {
		dist[i] = INT_MAX;
		inTree[i] = false;
	}
	dist[start] = 0;
	for (int i = 0; i < numVertices-1; i++) {
		int u = minDist(dist, inTree, numVertices);
		inTree[u] = true;
		for (int v = 0; v < numVertices; v++)
			if (!(inTree[v]) && dist[u] != INT_MAX && graph[u][v] && dist[u] + graph[u][v] < dist[v])
				dist[v] = dist[u] + graph[u][v];
	}

  for (int i=0; i<numVertices; i++) if(dist[i] == INT_MAX) dist[i] = -1;
	//for (int i = 0; i < numVertices; i++) cout << "Distance from start vertex to " << i << ": " << dist[i] << endl;
}
