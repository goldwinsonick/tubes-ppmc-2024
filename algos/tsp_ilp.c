#include <stdio.h>
#include <string.h>

// @param N Jumlah Kota
// @param adjMat Matriks Adjacency
// @param arrKotaName nama Kota
// @brief Menyelesaikan TSP dengan ILP
void tspILP(int N, float **adjMat, char **arrKotaName){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            printf("%f ", adjMat[i][j]);
        }
        printf("\n");
    }
    for(int i=0;i<N;i++){
        printf("%s\n", arrKotaName[i]);
    }
}