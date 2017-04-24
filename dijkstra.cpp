#include <stdlib.h>
#include <iostream>
#include <limits.h>
#include <vector>
#include <time.h>

using namespace std;

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
			if (!(inTree[v]) && graph[u][v] && dist[u] + graph[u][v] < dist[v]) 
				dist[v] = dist[u] + graph[u][v];
	}
	for (int i = 0; i < numVertices; i++) cout << "Distance from start vertex to " << i << ": " << dist[i] << endl;
}

int main(int argc, char **argv) {
	vector <vector <int> > vec;
	unsigned long seed = time(NULL);
	srand(seed);
	for (int i = 0; i < 50; i++) {
		vector <int> newVec;
		vec.push_back(newVec);
		for (int j = 0; j < 50; j++) {
			int x = rand() % 100;
			vec[i].push_back(x);
		}
	}
	dijkstra(vec, 0, 50);
	return 0;
}
