#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <semaphore.h>
#include <time.h>

#define max_ride 5 // the max rides a car can do
#define max_refill 100 // the max car the gas station can refill

//global variable
int M = -1; // Vistor num
int N = -1; // car num
int K = -1; // car hold time
int T = -1; // gas pumps
int vNum = -1; // vistor num
bool finish = false; // finish or not

typedef struct{
    int rideCount;
    int rideTime;
    bool available;
}car;

// sem_t and mutex_t
sem_t sem_arrive; //vistor arrive or not
sem_t sem_car;// car available or not
sem_t sem_pump; // pump available or not
sem_t sem_fillGas; // truck fill gas or not
pthread_mutex_t mutex;

// pthreads
void *masterP();
void *vistor();
void *carP();
void *gas();


void *gas(){

}

void *carP(){
    int i;
    double time = 0;
    int carNum = N;
    car *cars; 
    cars =(car *)malloc(N);

    for(i = 0; i < N; i++){
        cars[i].rideCount = 0;
        cars[i].available = true;
        cars[i].rideTime = -1;
    }

    do{ 
        if(M <= 0){
            pthread_mutex_lock(&mutex);
            finish = true;
            pthread_mutex_unlock(&mutex);
        }
        else{
        sem_wait(&sem_arrive);
        bool find = false;
        for(i = 0; i < carNum; i++){
            if(cars[i].available && !find && cars[i].rideCount < max_ride){
                printf("Car %d is available for ride!\n", i+1);
                sem_post(&sem_car);
                cars[i].rideCount = cars[i].rideCount + 1;
                cars[i].available = false;
                cars[i].rideTime = time;
                
                pthread_mutex_lock(&mutex);
                N = N - 1;

                pthread_mutex_unlock(&mutex);
                find = true;
            }
            else if(!cars[i].available && (time == cars[i].rideTime + K) ){
                printf("Time is %f, ridetime is %d, k is %d\n", time,cars[i].rideTime,K);
                printf("Car %d finish its task!, it is available now!\n",i+1);
                cars[i].available = true;
                
                pthread_mutex_lock(&mutex);
                N = N + 1;
                pthread_mutex_unlock(&mutex);
            }

        }
        if(!find){
            printf("No car available now!");
        }
        sleep(1);
        time = time + 1;
        //printf("Time is %f",time);
        }
    }while(!finish);

    pthread_exit(NULL);

}

void *vistor(){
    
    printf("Vistor thread start!\n");
    
    do{
        if(M <= 0){
            pthread_mutex_lock(&mutex);
            finish = true;
            pthread_mutex_unlock(&mutex);
        }
        else{
        
        printf("Customer %d 's turn! \n", (vNum - M + 1));
        sem_post(&sem_arrive);
        //wait for car
        sleep(1);
        sem_wait(&sem_car);
        printf("WOw customer %d get in to car!\n",(vNum - M + 1));
        
        pthread_mutex_lock(&mutex);
        M = M - 1;
        pthread_mutex_unlock(&mutex);
        sleep(1);
        
        }
    }while(!finish);

    pthread_exit(NULL);
}

void *masterP(){
    //create other three threads here
    pthread_t v;//vistor
    pthread_t c;//car
    pthread_t g;//gas
    
    printf("SD zoo Open!");
    // start other three program
    pthread_create(&v,NULL,vistor,NULL);
    pthread_create(&c,NULL,carP,NULL);
    pthread_create(&g,NULL,gas,NULL);

    do{
        //printData();
        if(M <= 0){
            pthread_mutex_lock(&mutex);
            finish = true;
            pthread_mutex_unlock(&mutex);
        }
        sleep(1);
    }while(!finish);
    // finish
    printf("All finished! Bye bye, see you tomorrow!\n");
    pthread_join(v,NULL);
    pthread_join(c,NULL);
    pthread_join(g,NULL);
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
            vNum = d[0];
            printData();

            // Initial 
            pthread_t master;

            sem_init(&sem_arrive,0,1);
            sem_init(&sem_car,0,1);
            sem_init(&sem_pump,0,1);
            sem_init(&sem_fillGas,0,1);
            pthread_mutex_init(&mutex,NULL);

            // start master program
            pthread_create(&master,NULL,masterP,NULL);

            // finish master program
            pthread_join(master,NULL);
            count++;
        }
    }
    else{
        printf("Error, can't open file!");
    }
}

void printData(){
    printf("Vistor: %d, Car: %d, Ride time: %d, pump: %d \n", M,N,K,T);
}

