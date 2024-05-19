// algos/tsp_genetic.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>

#define POPULATION_SIZE 100
#define MUTATION_RATE 0.01
#define GENERATIONS 1000

// Fungsi utilitas untuk menghitung jarak antara dua titik
float calculateDistance(float** adjMat, int* route, int N) {
    float totalDistance = 0.0;
    for (int i = 0; i < N - 1; i++) {
        totalDistance += adjMat[route[i]][route[i+1]];
    }
    totalDistance += adjMat[route[N-1]][route[0]]; // kembali ke titik awal
    return totalDistance;
}

// Inisialisasi populasi awal
void initializePopulation(int** population, int N) {
    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < N; j++) {
            population[i][j] = j;
        }
        // Acak urutan kota
        for (int j = 0; j < N; j++) {
            int r = rand() % N;
            int temp = population[i][j];
            population[i][j] = population[i][r];
            population[i][r] = temp;
        }
    }
}

// Seleksi individu terbaik
void selectBest(int** population, float* fitness, int* bestRoute, int N) {
    int bestIndex = 0;
    for (int i = 1; i < POPULATION_SIZE; i++) {
        if (fitness[i] < fitness[bestIndex]) {
            bestIndex = i;
        }
    }
    for (int i = 0; i < N; i++) {
        bestRoute[i] = population[bestIndex][i];
    }
}

// Mutasi individu
void mutate(int* route, int N) {
    for (int i = 0; i < N; i++) {
        if ((float)rand() / RAND_MAX < MUTATION_RATE) {
            int r = rand() % N;
            int temp = route[i];
            route[i] = route[r];
            route[r] = temp;
        }
    }
}

// Algoritma Genetika untuk TSP
void tspGenetic(int N, float** adjMat, char** kotaName, int startNode) {
    int** population = (int**)malloc(POPULATION_SIZE * sizeof(int*));
    for (int i = 0; i < POPULATION_SIZE; i++) {
        population[i] = (int*)malloc(N * sizeof(int));
    }
    float* fitness = (float*)malloc(POPULATION_SIZE * sizeof(float));
    int* bestRoute = (int*)malloc(N * sizeof(int));

    initializePopulation(population, N);

    for (int gen = 0; gen < GENERATIONS; gen++) {
        for (int i = 0; i < POPULATION_SIZE; i++) {
            fitness[i] = calculateDistance(adjMat, population[i], N);
        }

        selectBest(population, fitness, bestRoute, N);

        // Pembaruan populasi (crossover, mutasi, dll. bisa ditambahkan)
        for (int i = 0; i < POPULATION_SIZE; i++) {
            mutate(population[i], N);
        }
    }

    // Output hasil
    printf("Best route: ");
    for (int i = 0; i < N; i++) {
        printf("%s ", kotaName[bestRoute[i]]);
    }
    printf("\nBest route distance: %f km\n", calculateDistance(adjMat, bestRoute, N));

    // Dealokasi memori
    for (int i = 0; i < POPULATION_SIZE; i++) {
        free(population[i]);
    }
    free(population);
    free(fitness);
    free(bestRoute);
}
