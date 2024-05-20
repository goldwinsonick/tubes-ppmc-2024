#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>

#ifndef MAX_NODE
    #define MAX_NODE 100
#endif

// PSO Constants (Ini berdasar Eberhart & Shi, 2001)
#define W 0.729
#define C1 1.49445
#define C2 1.49445

// Particle struct
typedef struct Particle{
    float pos[MAX_NODE];
    int route[MAX_NODE];
    float dist;
    float vel[MAX_NODE];
    float pbest[MAX_NODE];
    float pbest_dist;
} Particle;

void debug(Particle **particles, Particle *gbest, int N){
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            printf("%d ", particles[i]->route[j]);
        }
        printf("\n");
    }
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            printf("%f ", particles[i]->pbest[j]);
        }
        printf("\n");
    }
    for(int i=0; i<N; i++){
        printf("%f\n", particles[i]->dist);
    }

    printf("Gbest:\n");
    for(int i=0; i<N; i++){
        printf("%d ", gbest->route[i]);
    }
    printf("\n");
    printf("%f\n", gbest->dist);
}

// @brief Update route dan dist berdasarkan pos (bisa untuk pos, pbest, dan gbest)
void updateRouteAndDist(float *pos, int *route, float *dist, float **adjMat, int N){
    // Reset route awal
    for(int i=0;i<N;i++){
        route[i] = i;
    }
    // Bubble sort route berdasar pos (Tanpa merubah pos)
    float tempPos[MAX_NODE];
    for(int i=0; i<N; i++){
        tempPos[i] = pos[i];
    }
    for(int i=0; i<N; i++){
        for(int j=0; j<N-1; j++){
            if(tempPos[j] > tempPos[j+1]){
                // Swap pos
                float temp = tempPos[j];
                tempPos[j] = tempPos[j+1];
                tempPos[j+1] = temp;
                // Swap route
                int tempRoute = route[j];
                route[j] = route[j+1];
                route[j+1] = tempRoute;
            }
        }
    }
    // Hitung dist
    *dist = 0;
    for(int i=0; i<N; i++){
        *dist += adjMat[route[i]][route[(i+1)%N]];
    }
}

void updatePbest(Particle **particles, int N){
    for (int i=0; i<N; i++){
        if (particles[i]->dist < particles[i]->pbest_dist){
            particles[i]->pbest_dist = particles[i]->dist;
            for (int j=0; j<N; j++){
                particles[i]->pbest[j] = particles[i]->pos[j];
            }
        }
    }
}

void updateGbest(Particle **particles, Particle* gbest, int N){
    for (int i=0; i<N; i++){
        if (particles[i]->dist < gbest->dist){
            gbest->dist = particles[i]->dist;
            for (int j=0; j<N; j++){
                gbest->pos[j] = particles[i]->pos[j];
                gbest->route[j] = particles[i]->route[j];
            }
        }
    }
}

Particle** initParticle(int N, Particle* gbest, float **adjMat){
    // Inisialisasi particles
    Particle **particles = (Particle **)malloc(N * sizeof(Particle *));

    for (int i=0; i<N; i++){
        particles[i] = (Particle *)malloc(sizeof(Particle));
        // Generate pos dan pbest awal
        // Ini random number 0-1 untuk setiap cell
        for (int j=0; j<N; j++){
            particles[i]->pos[j] = (float)rand() / (float)RAND_MAX;
            // Awalnya pos dan pbest sama
            particles[i]->pbest[j] = particles[i]->pos[j];
        }
        // Generate velocity awal
        for (int j=0; j<N; j++){
            particles[i]->vel[j] = 0.1;
        }
    }

    // Generate route dan dist untuk pos dan pbest
    for(int i=0; i<N; i++){
        updateRouteAndDist(particles[i]->pos, particles[i]->route, &particles[i]->dist, adjMat, N);
        particles[i]->pbest_dist = particles[i]->dist; // pbest dan dist masih sama
    }

    // Evaluasi gbest
    gbest->dist = particles[0]->dist;
    for (int i=0; i<N; i++){
        gbest->route[i] = particles[0]->route[i];
    }
    updateGbest(particles, gbest, N);
    return particles;
}

void updateVel(Particle **particles, Particle* gbest, int N){
    for (int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            // V(t+1) = W * V(t) + C1 * rand() * (pbest - x(t)) + C2 * rand() * (gbest - x(t))
            particles[i]->vel[j] = W * particles[i]->vel[j] + C1 * ((float)rand() / (float)RAND_MAX) * (particles[i]->pbest[j] - particles[i]->pos[j]) + C2 * ((float)rand() / (float)RAND_MAX) * (gbest->pos[j] - particles[i]->pbest[j]);
        }
    }
}
void updatePos(Particle **particles, int N){
    // Update Pos
    for (int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            particles[i]->pos[j] += particles[i]->vel[j];
        }
    }

    // Normalisasi pos agar nilainya 0-1
    for(int i=0; i<N; i++){
        // Cari max dan min nilai pos
        float maxPos = particles[i]->pos[0];
        float minPos = particles[i]->pos[0];
        for(int j=0; j<N; j++){
            if (particles[i]->pos[j] < minPos){
                minPos = particles[i]->pos[j];
            }
            if (particles[i]->pos[j] > maxPos){
                maxPos = particles[i]->pos[j];
            }
        }
        // Normalisasi dengan rumus (x - min) / (max - min)
        for(int j=0; j<N; j++){
            particles[i]->pos[j] = (particles[i]->pos[j] - minPos) / (maxPos - minPos);
        }
    }
}

void tspPSO(int N, float **adjMat, int startNode, char **kotaName, int max_iter) {

    // Inisialisasi gbest
    // Ini bagusnya sebenarnya bikin struct baru terdiri dari N(pos[N],vel[N]), N(pbest[N],pbest_vel[N]) dan Gbest
    // Tapi ini pakai struct particle aja gbest yang dipakai hanya pos, dist, dan route saja
    Particle* gbest = (Particle *)malloc(sizeof(Particle));
    // Inisiaisasi particles
    Particle **particles = initParticle(N, gbest, adjMat);
    // debug(particles, gbest, N);
    for(int i=0; i<max_iter; i++){
        updateVel(particles, gbest, N);
        updatePos(particles, N);
        // Update route dan dist dari pos
        for(int j=0; j<N; j++){
            updateRouteAndDist(particles[j]->pos, particles[j]->route, &particles[j]->dist, adjMat, N);
        }
        updatePbest(particles, N);
        updateGbest(particles, gbest, N);
        // debug(particles, gbest, N);
    }
    // debug(particles, gbest, N);
    int startIdx = 0;
    for(int i=0; i<N; i++){
        if(gbest->route[i] == startNode){
            startIdx = i;
            break;
        }
    }
    printf("Rute Ditemukan:\n");
    for(int i=0; i<N; i++){
        printf("%s -> ", kotaName[gbest->route[(startIdx+i)%N]]);
    }
    printf("%s\n", kotaName[gbest->route[startIdx]]);
    printf("Panjang Rute: %f\n", gbest->dist);
}