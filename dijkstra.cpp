#include <stdlib.h>
#include <iostream>
#include <list>
#include <fstream>
#include <algorithm>

using namespace std;

int *noLocality() {
	int *arr = new int[10000];
	unsigned long seed = time(NULL);
	srand(seed);
	for (int i = 0; i < 10000; i++) {
		arr[i] = rand() % 100;
	}
	return arr;
}

int *workload80_20() {
	int *arr = new int[10000];
	unsigned long seed = time(NULL);
	srand(seed);
	for (int i = 0; i < 10000; i++) {
		int x = rand() % 100;
		if (x < 80) arr[i] = rand() % 20;
		else arr[i] = rand() % 80 + 20;
	}
	return arr;
}

int *looping() {
	int *arr = new int[10000], j = 0;
	unsigned long seed = time(NULL);
	for (int i = 0; i < 10000; i++) {
		if (j < 50) {
			arr[i] = j;
		} else {
			j = 0;
			arr[i] = j;
		}
		j++;
	}
	return arr;
}

float lru(int *arr, int data_size) {
	int cache[data_size], LRU[data_size];
	float hits = 0;
	for (int i = 0; i < data_size; i++) {
		cache[i] = -1;
		LRU[i] = -1;
	}
	for (int i = 0; i < 10000; i++) {
		bool inCache = false;
		for (int j = 0; j < data_size; j++) {
			if (cache[j] == arr[i]) {
				inCache = true;
				hits++;
				for (int k = 0; k < data_size; k++) {
					if (k != j && LRU[k] != -1 && LRU[k] < LRU[j]) LRU[k]++;
				}
				LRU[j] = 0;
				j = data_size;
			}
		}
		if (!inCache) {
			for (int j = 0; j < data_size; j++) {
				if (cache[j] == -1 && LRU[j] == -1) {
					cache[j] = arr[i];
					inCache = true;
					for (int k = 0; k < data_size; k++) {
						if (k != j && LRU[k] != -1) LRU[k]++;
					}
					LRU[j] = 0;
					j = data_size;
				}
			}
		}
		if (!inCache) {
			for (int j = 0; j < data_size; j++) {
				if (LRU[j] == data_size-1) {
					cache[j] = arr[i];
					for (int k = 0; k < data_size; k++) {
						if (k != j && LRU[k] != -1 && LRU[k] < LRU[j]) LRU[k]++;
					}
					LRU[j] = 0;
					j = data_size;
				}
			}
		}
	}
	return (hits/10000)*100;
}

float random(int *arr, int data_size) {
	int cache[data_size];
	float hits = 0;
	unsigned long seed = time(NULL);
	srand(seed);
	for (int i = 0; i < data_size; i++) cache[i] = -1;
	for (int i = 0; i < 10000; i++) {
		bool inCache = false;
		for (int j = 0; j < data_size; j++) {
			if (cache[j] == arr[i]) {
				inCache = true;
				hits++;
				j = data_size;
			}
		}
		if (!inCache) {
			for (int j = 0; j < data_size; j++) {
				if (cache[j] == -1) {
					cache[j] = arr[i];
					inCache = true;
					j = data_size;
				}
			}
		}
		if (!inCache) {
			int x = rand() % (data_size-1);
			cache[x] = arr[i];
		}
	}
	return (hits/10000)*100;
}

float clock(int *arr, int data_size) {
	if (data_size == 0) return 0;
	int cache[data_size], rec_used[data_size], tick = 0;
	float hits = 0;
	for (int i = 0; i < data_size; i++) {
		cache[i] = -1;
		rec_used[i] = -1;
	}
	for (int i = 0; i < 10000; i++) {
		bool inCache = false, skip = false;
		for (int j = 0; j < data_size; j++) {
			if (cache[j] == arr[i]) {
				inCache = true;
				hits++;
				rec_used[j] = 1;
				j = data_size;
			}
		}
		if (!inCache) {
			for (int j = 0; j < data_size; j++) {
				if (cache[j] == -1) {
					cache[j] = arr[i];
					inCache = true;
					rec_used[j] = 1;
					j = data_size;
				}
			}
		}
		if (!inCache) {
			for (int j = tick; j < data_size; j++) {
				if (rec_used[j] == 0) {
					cache[j] = arr[i];
					rec_used[j] = 1;
					tick = j;
					tick++;
					if (tick == data_size) tick = 0;
					skip = true;
					break;
				} else rec_used[j] = 0;
			}
			if (!skip) {
				for (int j = 0; j <= tick; j++) {
					if (rec_used[j] == 0) {
						cache[j] = arr[i];
						rec_used[j] = 1;
						tick = j;
						tick++;
						if (tick == data_size) tick = 0;
						skip = true;
						break;
					} else rec_used[j] = 0;
				}
			}
		}
	}
	return (hits/10000)*100;
}

float fifo(int *arr, int data_size) {
	if (data_size == 0) return 0;
	list <int> cache;
	float hits = 0;
	for (int i = 0; i < 10000; i++) {
		bool inCache = false;
		for (int j = 0; j < cache.size(); j++) {
			list <int>::iterator it = find(cache.begin(), cache.end(), arr[i]);
			if (it != cache.end()) {
				inCache = true;
				hits++;
				j = data_size;
			}
		}
		if (!inCache) {
			if (cache.size() < data_size) cache.push_back(arr[i]);
			else {
				cache.pop_front();
				cache.push_back(arr[i]);
			}
		}
	}
	return (hits/10000)*100;
}

float opt(int *arr, int data_size) {
	int cache[data_size];
	float hits = 0;
	list <int> l;
	for (int i = 0; i < data_size; i++) cache[i] = -1;
	for (int i = 0; i < 10000; i++) {
		bool inCache = false;
		for (int j = 0; j < data_size; j++) {
			if (cache[j] == arr[i]) {
				inCache = true;
				hits++;
				j = data_size;
			}
		}
		if (!inCache) {
			for (int j = 0; j < data_size; j++) {
				if (cache[j] == -1) {
					cache[j] = arr[i];
					inCache = true;
					j = data_size;
				}
			}
		}
		if (!inCache) {
			l.clear();
			for (int j = 0; j < data_size; j++) l.push_back(cache[j]);
			for (int j = i+1; j < 10000; j++) {
				if (l.size() != 1) l.remove(arr[j]);
				else break;
			}
			for (int j = 0; j < data_size; j++) {
				if (cache[j] == l.front()) {
					cache[j] = arr[i];
					break;
				}
			}
		}
	}
	return (hits/10000)*100;
}

int main(int argc, char **argv) {
	ofstream fnoLoc, f8020, fLoop;
	fnoLoc.open("no-locality.csv");
	f8020.open("80-20.csv");
	fLoop.open("looping.csv");
	int *arr1 = noLocality();
	int *arr2 = workload80_20();
	int *arr3 = looping();
	fnoLoc << "#cache,OPT,LRU,FIFO,RAND,CLOCK," << endl;
	f8020 << "#cache,OPT,LRU,FIFO,RAND,CLOCK," << endl;
	fLoop << "#cache,OPT,LRU,FIFO,RAND,CLOCK," << endl;
	for (int i = 0; i <= 100; i+=5) {
		fnoLoc << i << "," << opt(arr1, i) << "," << lru(arr1, i) << "," << fifo(arr1, i) << "," << random(arr1, i) << "," << clock(arr1, i) << "," << endl;
		f8020 << i << "," << opt(arr2, i) << "," << lru(arr2, i) << "," << fifo(arr2, i) << "," << random(arr2, i) << "," << clock(arr2, i) << "," << endl;
		fLoop << i << "," << opt(arr3, i) << "," << lru(arr3, i) << "," << fifo(arr3, i) << "," << random(arr3, i) << "," << clock(arr3, i) << "," << endl;
	}
	fnoLoc.close();
	f8020.close();
	fLoop.close();
	return 0;
}
