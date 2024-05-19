#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <limits.h>

#ifndef MAX_NODE
    #define MAX_NODE 15
#endif

// Fungsi swap elemen untuk kegunaan permutasi
void swap(int* x, int* y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

// Fungsi untuk menghitung jarak sebuah path dan membandingkan mana yang lebih minimum
void calculateDistance(int n, float **adjMat, int* path, float* minDistance, int* bestPath) {
    float distance = 0.0;
    for (int i = 0; i < n - 1; i++) {
        distance += adjMat[path[i]][path[i + 1]];
    }

    distance += adjMat[path[n - 1]][path[0]]; 

    if (distance < *minDistance) {
        *minDistance = distance;
        for (int i = 0; i < n; i++) {
            bestPath[i] = path[i];
        }
    }
}

// Fungsi untuk melakukan permutasi untuk menelusuri seluruh kemungkinan path
void permute(int n, int* path,  float** adjMat, float* minDistance, int* bestPath, int startNode) {
    int status[MAX_NODE] = {0};

    for (int i = 0; i < n; i++) {
        path[i] = i;
    }

    calculateDistance(n, adjMat, path, minDistance, bestPath);

    int i = 1;
    while (i < n) {
        if (status[i] < i) {
            if (i % 2 == 0) {
                swap(&path[1], &path[i]);
            } else {
                swap(&path[status[i]], &path[i]);
            }
            if (path[0] != startNode) {
                for (int i = 0; i < n; i++) {
                    if (path[i] == startNode) {
                        swap(&path[0], &path[i]);
                        break;
                    }
                }
            }
            calculateDistance(n, adjMat, path, minDistance, bestPath);
            status[i]++;
            i = 1; 
        } else {
            status[i] = 0;
            i++;
        }
    }
}

// Fungsi inisialisasi array, memanggil fungsi permute, dan menampilkan output
void tspBruteforce(int n, float **adjMat, char** kotaName, int startNode) {
    int path[MAX_NODE];
    int bestPath[MAX_NODE];
    float minDistance = FLT_MAX;

    permute(n, path, adjMat, &minDistance, bestPath, startNode);

    printf("Jalur terpendek yang ditemukan:\n");
    for (int i = 0; i < n; i++) {
        printf("%s -> ", kotaName[bestPath[i]]);
    }
    printf("%s\n", kotaName[bestPath[0]]);
    printf("Panjang jalur terbaik: %f\n", minDistance);
}