#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>

#define MAX_NODE 15

#define MAX_ITER 1000000
#define W 0.5
#define C1 1.0
#define C2 1.0

// Particle struct
typedef struct Particle{
    int route[MAX_NODE];
    float dist;
    float pbest[MAX_NODE];
    float vel[MAX_NODE];
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

void updateGbest(Particle **particles, Particle* gbest, int N){
    for (int i=0; i<N; i++){
        if (particles[i]->dist < gbest->dist){
            gbest->dist = particles[i]->dist;
            for (int j=0; j<N; j++){
                gbest->pbest[j] = particles[i]->pbest[j];
            }
        }
    }
}

void updateParticleRouteAndDist(Particle **particles, float **adjMat, int N){
    float tempPbest[MAX_NODE];
    for(int i=0; i<N; i++){
        // Route awal
        for(int j=0; j<N; j++){
            particles[i]->route[j] = j;
        }

        // Bubble sort berdasar tempPbest
        for(int j=0; j<N; j++){
            tempPbest[j] = particles[i]->pbest[j];
        }
        for(int j=0; j<N; j++){
            for(int k=j+1; k<N; k++){
                if(tempPbest[j] > tempPbest[k]){
                    // swap tempPbest
                    float temp = tempPbest[j];
                    tempPbest[j] = tempPbest[k];
                    tempPbest[k] = temp;
                    // swap route
                    int temp2 = particles[i]->route[j];
                    particles[i]->route[j] = particles[i]->route[k];
                    particles[i]->route[k] = temp2;
                }
            }
        }
        // Hitung dist routenya
        particles[i]->dist = 0;
        for (int j=0; j<N; j++){
            particles[i]->dist += adjMat[particles[i]->route[j]][particles[i]->route[(j+1)%N]];
        }
    }


}

// Generate particle awal
Particle** initParticle(int N, Particle* gbest, float **adjMat){
    // Inisialisasi particles (pbest)
    Particle **particles = (Particle **)malloc(N * sizeof(Particle *));

    for (int i=0; i<N; i++){
        particles[i] = (Particle *)malloc(sizeof(Particle));
        // Ini random number 0-1 untuk setiap cell
        for (int j=0; j<N; j++){
            particles[i]->pbest[j] = (float)rand() / (float)RAND_MAX;
        }


        // Generate velocity awal
        for (int j=0; j<N; j++){
            particles[i]->vel[j] = 0.1;
        }
    }

    // Generate route dan dist
    updateParticleRouteAndDist(particles, adjMat, N);

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
            particles[i]->vel[j] = W * particles[i]->vel[j] + C1 * ((float)rand() / (float)RAND_MAX) * (particles[i]->pbest[j] - particles[i]->pbest[j]) + C2 * ((float)rand() / (float)RAND_MAX) * (gbest->pbest[j] - particles[i]->pbest[j]);
        }
    }
}
void updatePbest(Particle **particles, int N){
    // Update pbest
    for (int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            particles[i]->pbest[j] += particles[i]->vel[j];
        }
    }
    // Normalisasi pbest
    for(int i=0; i<N; i++){
        float maxPbest = particles[i]->pbest[0];
        float minPbest = particles[i]->pbest[0];
        for(int j=0; j<N; j++){
            if (particles[i]->pbest[j] < minPbest){
                minPbest = particles[i]->pbest[j];
            }
            if (particles[i]->pbest[j] > maxPbest){
                maxPbest = particles[i]->pbest[j];
            }
        }
        for(int j=0; j<N; j++){
            particles[i]->pbest[j] = (particles[i]->pbest[j] - minPbest) / (maxPbest - minPbest);
        }
    }
}

void tspPSO(int N, float **adjMat, int startNode, char **kotaName) {
    // Inisialisasi gbest
    Particle* gbest = (Particle *)malloc(sizeof(Particle));
    // Generate random pbest dan route awal dan update gbest
    Particle **particles = initParticle(N, gbest, adjMat);
    // debug(particles, gbest, N);
    for(int i=0; i<MAX_ITER; i++){
        updateVel(particles, gbest, N);
        updatePbest(particles, N);
        updateGbest(particles, gbest, N);
        // printf("\n_____\n");
        // debug(particles, gbest, N);
    }
    debug(particles, gbest, N);
    int startIdx = 0;
    for(int i=0; i<N; i++){
        if(gbest->route[i] == startNode){
            startIdx = i;
            break;
        }
    }
    printf("Rute:\n");
    for(int i=0; i<N; i++){
        printf("%s -> ", kotaName[gbest->route[(startIdx+i)%N]]);
    }
    printf("%s\n", kotaName[gbest->route[startIdx]]);
    printf("Panjang rute: %f\n", gbest->dist);
}

// int main(){
//     srand(time(NULL));
//     // make this as matrix
//     // {0.000000 838.623474 311.299713 567.658386
//     // 838.623474 0.000000 533.906555 284.516510
//     // 311.299713 533.906555 0.000000 256.851563
//     // 567.658386 284.516510 256.851563 0.000000}
//     float adjMat[4][4] = {
//         {0.000000, 838.623474, 311.299713, 567.658386},
//         {838.623474, 0.000000, 533.906555, 284.516510},
//         {311.299713, 533.906555, 0.000000, 256.851563},
//         {567.658386, 284.516510, 256.851563, 0.000000}
//     };
//     // make the matrix array to double pointer
//     float **adjMatPtr = (float **)malloc(4 * sizeof(float *));
//     for(int i=0; i<4; i++){
//         adjMatPtr[i] = (float *)malloc(4 * sizeof(float));
//         for(int j=0; j<4; j++){
//             adjMatPtr[i][j] = adjMat[i][j];
//         }
//     }
//     tspPSO(4, adjMatPtr, 0);
// }