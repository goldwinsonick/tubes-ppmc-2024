#include <stdio.h>
void tspGreedy(int N, float **adjMat, char **arrKotaName, int startNode){
    // Inisialisasi
    int visited[N];
    for(int i=0;i<N;i++){
        visited[i] = 0;
    }
    visited[startNode] = 1;
    int currNode = startNode;
    float totalDist = 0;
    printf("Rute: %s", arrKotaName[startNode]);
    for(int i=1;i<N;i++){
        float minDist = 1e9;
        int nextNode = -1;
        for(int j=0;j<N;j++){
            if(visited[j] == 0 && adjMat[currNode][j] < minDist){
                minDist = adjMat[currNode][j];
                nextNode = j;
            }
        }
        visited[nextNode] = 1;
        totalDist += minDist;
        printf(" -> %s", arrKotaName[nextNode]);
        currNode = nextNode;
    }
    printf("\nTotal Jarak: %f\n", totalDist);
}