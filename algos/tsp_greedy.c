#include <stdio.h>
#include <stdbool.h>
#include <float.h>

// Fungsi untuk mencari kota terdekat yang belum dikunjungi
int findNearestCity(int currentCity, bool visited[], float **adjMat, int N) {
    int nearestCity = -1;
    float minDistance = FLT_MAX;
    
    for (int i = 0; i < N; i++) {
        if (!visited[i] && adjMat[currentCity][i] < minDistance && currentCity != i) {
            minDistance = adjMat[currentCity][i];
            nearestCity = i;
        }
    }
    
    return nearestCity;
}

// Fungsi Algoritma Greedy TSP
void tspGreedy(int N, float **adjMat, char **arrKotaName, int startNode) {
    bool visited[N];
    for (int i = 0; i < N; i++) {
        visited[i] = false;
    }

    int currentCity = startNode;
    visited[currentCity] = true;

    printf("Rute yang ditemukan (Greedy): \n%s", arrKotaName[currentCity]);

    float totalDistance = 0;
    int nextCity;

    for (int i = 0; i < N - 1; i++) {
        nextCity = findNearestCity(currentCity, visited, adjMat, N);
        if (nextCity == -1) {
            break;
        }

        totalDistance += adjMat[currentCity][nextCity];
        currentCity = nextCity;
        visited[currentCity] = true;
        
        printf(" -> %s", arrKotaName[currentCity]);
    }

    // Kembali ke kota awal
    totalDistance += adjMat[currentCity][startNode];
    printf(" -> %s\n", arrKotaName[startNode]);
    printf("Total jarak: %.2f km\n", totalDistance);
}
