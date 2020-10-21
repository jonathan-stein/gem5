/* Benchmark Application in C.
 * The error injection function is sssp.
 * 
 * Single-source Shortest Path in a directed graph.
 * Utilize Bellman-Ford algorithm, since Dijkstra's algorithm needs a priority queue.
 * https://en.wikipedia.org/wiki/Bellman%E2%80%93Ford_algorithm
 * 
 * Author: Ziang Wan, Jonathan Stein
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


typedef struct GraphNode {
  char *name;
  // Directed weighted edges. Assume no more than 10 neighbors.
  int neighborCount;
  struct GraphNode *neighbors[10];
  int edgeWeights[10];
  // Store the result distance.
  int distance;
} GraphNode_t;


// The main function: sssp using Bellman-Ford.
// Assume source is the first item in graph.
void sssp(GraphNode_t *graph, const int n) {
  graph[0].distance = 0;
  for (int repetition = 0; repetition < n - 1; repetition += 1) {
    // Iterate over all directed edges.
    for (int node = 0; node < n; node += 1) {
      GraphNode_t *edgeSource = &(graph[node]);
      for (int neighborIdx = 0; neighborIdx < graph[node].neighborCount; neighborIdx += 1) {
        GraphNode_t *edgeDestination = graph[node].neighbors[neighborIdx];
        const int weight = graph[node].edgeWeights[neighborIdx];
        // Relax edges.
        if (edgeSource->distance + weight < edgeDestination->distance) {
          edgeDestination->distance = edgeSource->distance + weight;
        }
      }
    }
  }
}


int main(int argc, char *argv[]) {
  // Unfortunate manual construction of the graph.
  // Adjacency list.
  const int n = 6;
  GraphNode_t *graph = malloc(n * sizeof(GraphNode_t));

  graph[0].name = "source";
  graph[0].neighborCount = 2;
  graph[0].neighbors[0] = &(graph[1]);
  graph[0].edgeWeights[0] = 5;
  graph[0].neighbors[1] = &(graph[3]);
  graph[0].edgeWeights[1] = -2;
  graph[0].distance = INT_MAX;

  graph[1].name = "A";
  graph[1].neighborCount = 1;
  graph[1].neighbors[0] = &(graph[2]);
  graph[1].edgeWeights[0] = 1;
  graph[1].distance = INT_MAX;

  graph[2].name = "B";
  graph[2].neighborCount = 2;
  graph[2].neighbors[0] = &(graph[5]);
  graph[2].edgeWeights[0] = 3;
  graph[2].neighbors[1] = &(graph[4]);
  graph[2].edgeWeights[1] = -1;
  graph[2].distance = INT_MAX;

  graph[3].name = "C";
  graph[3].neighborCount = 3;
  graph[3].neighbors[0] = &(graph[1]);
  graph[3].edgeWeights[0] = 2;
  graph[3].neighbors[1] = &(graph[2]);
  graph[3].edgeWeights[1] = 4;
  graph[3].neighbors[2] = &(graph[4]);
  graph[3].edgeWeights[2] = 4;
  graph[3].distance = INT_MAX;

  graph[4].name = "D";
  graph[4].neighborCount = 1;
  graph[4].neighbors[0] = &(graph[5]);
  graph[4].edgeWeights[0] = 1;
  graph[4].distance = INT_MAX;

  graph[5].name = "destination";
  graph[5].neighborCount = 0;
  graph[5].distance = INT_MAX;

  sssp(graph, n);

  // Expected answer: source:0 A:0 B:1 C:-2 D:0 destination:1.
  // See the graph.png in this directory.
  printf("Result: ");
  for (int i = 0; i < n - 1; i += 1) {
    printf("%s:%d ", graph[i].name, graph[i].distance);
  }
  printf("%s:%d\n", graph[n - 1].name, graph[n - 1].distance);

  free(graph);
  return 0;
}