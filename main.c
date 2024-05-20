#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <time.h>
#include <conio.h>

#include "algos/tsp_pso.c"
#include "algos/tsp_greedy.c"
#include "algos/tsp_bruteforce.c"
#include "algos/tsp_genetic.c"
#include "algos/tsp_bfs.c"

#ifndef MAX_NODE
    #define MAX_NODE 100
#endif
#define MAX_CITY_LEN 256
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

typedef struct Kota{
    char name[MAX_CITY_LEN];
    float latitude;
    float longitude;
} Kota;

float calcDistance(float lat1, float long1, float lat2, float long2) {
    // Haversine formula
    const float R = 6371; // Earth radius in kilometers
    float dLat = (lat2 - lat1) * M_PI / 180.0;
    float dLong = (long2 - long1) * M_PI / 180.0;
    float a = sin(dLat / 2) * sin(dLat / 2) + cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) * sin(dLong / 2) * sin(dLong / 2);
    float c = 2 * atan2(sqrt(a), sqrt(1 - a));
    float distance = R * c;
    return distance;
}

int main(){
    // Input File
    char nama_file[256];
    FILE *fp;

    // Array of Kota (nama, latitude, longitude)
    Kota arrKota[MAX_NODE];

    // Array of nama kota
    char** kotaName = (char **)malloc(MAX_NODE * sizeof(char *));
    for(int i=0;i<MAX_NODE;i++){
        kotaName[i] = (char *)malloc(MAX_CITY_LEN * sizeof(char));
    }

    // Adjacency Matrix
    float **adjMat;
    adjMat = (float **)malloc(MAX_NODE * sizeof(float *));

    for(int i=0;i<MAX_NODE;i++){
        adjMat[i] = (float *)malloc(MAX_NODE * sizeof(float));
    }

    int N = -1;
    printf("\033[2J\033[1;1H");
    while(N == -1){
        printf("Masukkan namafile: ");
        scanf(" %[^\n]s", nama_file);
        fp = fopen(nama_file, "r");
        while(fp == NULL){
            printf("Nama file tidak ditemukan!\n");
            printf("Masukkan namafile: ");
            scanf(" %[^\n]s", nama_file);
            fp = fopen(nama_file, "r");
        }

        // Parsing isi file ke adjMatrix, dll
        char buf[1005];
        char* token;
        int idx = 0;
        while(fgets(buf, 1000, fp)){
            Kota newKota;
            token = strtok(buf, ",");
            strcpy(newKota.name, token);
            strcpy(kotaName[idx], token);
            token = strtok(NULL, ",");
            newKota.latitude = atof(token);
            token = strtok(NULL, ",");
            newKota.longitude = atof(token);
            arrKota[idx] = newKota;
            idx++;
        }
        N = idx;
        // Check N sesuai batasan
        if(N < 6 || N > 15){
            if(N > MAX_NODE){
                printf("Jumlah kota terlalu banyak! Maksimal 100 kota\n");
                N = -1;
                continue;
            }

            printf("Jumlah kota direkomendasikan 6 <= N <= 15 !\n");
            printf("Jumlah kota sekarang: %d\n", N);
            char confirmInp[1000];
            printf("Apakah kamu yakin mau lanjut? (y/n): ");
            scanf(" %[^\n]s", confirmInp);
            while(strcmp(confirmInp, "y")!=0 && strcmp(confirmInp, "n")!=0){
                printf("Input tidak valid!\n");
                printf("Apakah kamu yakin mau lanjut? (y/n): ");
                scanf(" %[^\n]s", confirmInp);
            }
            if(strcmp(confirmInp,"y")==0){
                break;
            }else{
                N = -1;
            }
        }
    }

    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            double distance = calcDistance(arrKota[i].latitude, arrKota[i].longitude, arrKota[j].latitude, arrKota[j].longitude);
            adjMat[i][j] = distance;
        }
    }

    // Input Kota awal
    char kotaStart[MAX_CITY_LEN];
    int startNode = -1;
    printf("\033[2J\033[1;1H");
    while(startNode == -1){
        for(int i=0; i<N;i++){
            printf("%d. %s\n", i+1, arrKota[i].name);
        }
        printf("Masukkan kota awal: ");
        scanf(" %[^\n]s", kotaStart);
        for(int i=0; i<N; i++){
            if(strcmp(arrKota[i].name, kotaStart) == 0){
                startNode = i;
                break;
            }
        }
        printf("\033[2J\033[1;1H");
        if(startNode == -1){
            printf("Kota tidak ditemukan!\n");
        }
    }

    // UI Menu
    int inp;
    char menu[8][100] = {"Greedy", "Bruteforce", "Breadth First Search (BFS)", "Depth First Search (DFS)*", "Branch and Bound*", "Genetic", "Particle Swarm Optimization", "Exit"};
    clock_t now; double dt;
    while(1){
        printf("\033[2J\033[1;1H");
        printf("Kota Awal : %s\n", kotaStart);
        printf("Pilihan Algoritma:\n");
        for(int i=0;i<8;i++){
            printf("%d. %s\n", i+1, menu[i]);
        }
        printf("Pilih: ");
        scanf("%d", &inp);

        if(inp == 8){
            printf("\033[2J\033[1;1H");
            printf("Program Selesai. Terima kasih!");
            break;
        }

        printf("\033[2J\033[1;1H");
        now = clock();
        if(inp == 1){
            tspGreedy(N, adjMat, kotaName, startNode);
        }else if(inp == 2){
            tspBruteforce(N, adjMat, kotaName, startNode);
        }else if(inp == 3){
            tspBFS(N, adjMat, kotaName, startNode);
        }else if(inp == 4){
            // use algo4
        }else if(inp == 5){
            // use algo5
        }else if(inp == 6){
            tspGenetic(N, adjMat, kotaName, startNode);
        }else if(inp == 7){
            int max_iter = -1;
            while(!(1 <= max_iter && max_iter <= 10000000)){
                printf("Semakin besar jumlah iterasi, semakin akurat hasilnya, tetapi semakin lama prosesnya\n");
                printf("Masukkan jumlah iterasi (1-10000000): ");
                scanf("%d", &max_iter);
            }
            now = clock();
            tspPSO(N, adjMat, startNode, kotaName, max_iter);
        }else{
            printf("Input tidak valid!\n");
            continue;
        }
        dt = (double)(clock()-now)/CLOCKS_PER_SEC;
        printf("Waktu komputasi  : %f detik\n", dt);
        printf("Tekan apapun untuk lanjut\n");
        getch();
    }
}