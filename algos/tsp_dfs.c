#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

#define MAX_NODE 100
#define MAX_CITY_LEN 256

void dfs(int start, int current, int N, float **adjMat, int *visited, float currentCost, float *minCost, int *path, int *bestPath, int level) {
    visited[current] = 1;
    path[level] = current;

    if (level == N - 1) {
        float totalCost = currentCost + adjMat[current][start];
        if (totalCost < *minCost) {
            *minCost = totalCost;
            memcpy(bestPath, path, N * sizeof(int));
        }
    } else {
        for (int i = 0; i < N; i++) {
            if (!visited[i]) {
                dfs(start, i, N, adjMat, visited, currentCost + adjMat[current][i], minCost, path, bestPath, level + 1);
            }
        }
    }

    visited[current] = 0;
}

void tspDFS(int N, float **adjMat, char **kotaName, int startNode) {
    int visited[MAX_NODE] = {0};
    int path[MAX_NODE];
    int bestPath[MAX_NODE];
    float minCost = FLT_MAX;

    dfs(startNode, startNode, N, adjMat, visited, 0, &minCost, path, bestPath, 0);

    printf("\nBest Route Found: ");
    for (int i = 0; i < N; i++) {
        printf("%s -> ", kotaName[bestPath[i]]);
    }
    printf("%s\n", kotaName[startNode]);
    printf("Best route distance: %f\n", minCost);
}
