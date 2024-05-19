#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define MAX_NODE 20

void tspBFS(int N, float **adjMat, char **arrKotaName, int startNode) {
    bool visited[MAX_NODE] = {false};
    int queue[MAX_NODE];
    double totalDistance = 0;
    int front = 0, rear = -1;
    int currentCityIndex, nextCityIndex;

    printf("Best route found:\n");

    // Start from the given startNode
    visited[startNode] = true;
    queue[++rear] = startNode;

    while (front <= rear) {
        currentCityIndex = queue[front++]; // Dequeue the current city
        printf("%s -> ", arrKotaName[currentCityIndex]);

        // Find the next unvisited city with shortest distance
        float minDistance = INFINITY;
        nextCityIndex = -1;
        for (int i = 0; i < N; i++) {
            if (!visited[i] && adjMat[currentCityIndex][i] < minDistance) {
                minDistance = adjMat[currentCityIndex][i];
                nextCityIndex = i;
            }
        }
        if (nextCityIndex != -1) {
            queue[++rear] = nextCityIndex;
            totalDistance += minDistance;
            visited[nextCityIndex] = true;
        }
    }

    // Return to the start city
    printf("%s\n", arrKotaName[startNode]);
    totalDistance += adjMat[queue[rear]][startNode]; // Add distance to return to start city
    printf("Best route distance: %f km\n", totalDistance);
}