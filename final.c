#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//global variable
int M = -1;
int N = -1;
int K = -1;
int T = -1;


void printData(){
    printf("M is %d, N is %d, K is %d, T is %d \n", M,N,K,T);
}


int main(int argc, char *argv[]){
    char fileName[256];

    if(argc == 3){
        strcpy(fileName,argv[1]);
    }
    else{
        printf("Need provide a valid data file and the number of data set");
        exit(0);
    }
    printf("File name is %s \n", fileName);
    
    int i;
    int count = 0;
    FILE *f;
    f = fopen("data.txt","r");
    
    if(f){
        while(count < atoi(argv[2]) && !feof(f)){
            char data[256];
            
            int d[4];
            char *tmp;

            fgets(data,256, f);
            printf("Data is %s in this round\n", data);

            tmp = strtok(data,",");
            d[0]= atoi(tmp);
            for( i = 1; i < 4; i++){
                tmp = strtok(NULL, ",");
                d[i]= atoi(tmp);
            }
            
            // get data
            M = d[0];
            N = d[1];
            K = d[2];
            T = d[3];
            printData();

            // create four threads








            count++;
        }
    }
    else{
        printf("Error, can't open file!");
    }
}



