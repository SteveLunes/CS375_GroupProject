#include <stdlib.h>
#include <iostream>
#include <limits.h>
#include <vector>
#include "dijkstra.h"

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
	for (int i = 0; i < numVertices; i++) if (dist[i] == INT_MAX) dist[i] = -1;
	for (int i = 0; i < numVertices; i++) cout << "Distance from start vertex to " << i << ": " << dist[i] << endl;
}

int main(int argc, char **argv) {
	vector <vector <int> > vec;
	for (int i = 0; i < 7; i++) {
		vector <int> newVec;
		vec.push_back(newVec);
	}
	int s0[7] = {0, 1, 0, 0, 0, 0, 0};
	int a1[7] = {1, 0, 2, 3, 5, 0, 0};
	int b2[7] = {0, 2, 0, 4, 0, 0, 0};
	int c3[7] = {0, 3, 4, 0, 0, 0, 0};
	int d4[7] = {0, 5, 0, 0, 0, 6, 7};
	int e5[7] = {0, 0, 0, 0, 6, 0, 0};
	int f6[7] = {0, 0, 0, 0, 7, 0, 0};
	for (int i = 0; i < 7; i++) vec[0].push_back(s0[i]);
	for (int i = 0; i < 7; i++) vec[1].push_back(a1[i]);
	for (int i = 0; i < 7; i++) vec[2].push_back(b2[i]);
	for (int i = 0; i < 7; i++) vec[3].push_back(c3[i]);
	for (int i = 0; i < 7; i++) vec[4].push_back(d4[i]);
	for (int i = 0; i < 7; i++) vec[5].push_back(e5[i]);
	for (int i = 0; i < 7; i++) vec[6].push_back(f6[i]);
	dijkstra(vec, 0, 7);
	return 0;
}
