#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <time.h>
#include <conio.h>

#include "algos/tsp_ilp.c"
#include "algos/tsp_greedy.c"

#define MAX_NODE 15
#define MAX_CITY_LEN 256

typedef struct Kota{
    char name[MAX_CITY_LEN];
    float latitude;
    float longitude;
}Kota;

float calcDistance(float lat1, float long1, float lat2, float long2){
    // Haversine
    float r = 6371;
    return 2*r*asin(sin((lat2-lat1)/2)*sin((lat2-lat1)/2) + cos(lat1)*cos(lat2)*sin((long2-long1)/2)*sin((long2-long1)/2));
}

int main(){
    // Input File
    char nama_file[256];
    // printf("\033[2J\033[1;1H");
    printf("Masukkan namafile: ");
    scanf("%[^\n]s", nama_file);
    FILE* fp = fopen(nama_file, "r");
    while(fp == NULL){
        printf("\033[2J\033[1;1H");
        printf("Nama file tidak ditemukan!\n");
        printf("Masukkan namafile: ");
        scanf(" %[^\n]s", nama_file);
        fp = fopen(nama_file, "r");
    }

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
    int N = idx;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            adjMat[i][j] = calcDistance(arrKota[i].latitude, arrKota[i].longitude, arrKota[j].latitude, arrKota[j].longitude);
        }
    }
    // Debug
    // for(int i=0;i<N;i++){
    //     for(int j=0;j<N;j++){
    //         printf("%f ", adjMat[i][j]);
    //     }
    //     printf("\n");
    // }

    // Input Kota awal
    printf("\033[2J\033[1;1H");
    char kotaStart[MAX_CITY_LEN];
    int startNode = -1;
    while(startNode == -1){
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
    char menu[8][100] = {"Algoritma Greedy", "Algo2", "Algo3", "Algo4", "Algo5", "Algo6", "Algoritma Integer Linear Programming", "Exit"};
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
            // use algo2
        }else if(inp == 3){
            // use algo3
        }else if(inp == 4){
            // use algo4
        }else if(inp == 5){
            // use algo5
        }else if(inp == 6){
            // use algo6
        }else if(inp == 7){
            tspILP(N, adjMat, kotaName);
        }
        dt = (double)(clock()-now)/CLOCKS_PER_SEC;
        printf("Waktu komputasi  : %f detik\n", dt);
        printf("Tekan apapun untuk lanjut\n");
        getch();
    }
}