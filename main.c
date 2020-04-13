#include <Windows.h>
#include <stdio.h>
#define NUMBER_OF_MAN 4

HANDLE hMutex;

void dining(void* lpHDining){
    int id = (int)lpHDining;
    printf("%d\n", id);
    while(TRUE){
        thing(id);
        eating(id);
    }
}

void thing(int id){

}

void eating(int id){

}

int main(){
    HANDLE hPeopleDining[NUMBER_OF_MAN];
    unsigned short int ID[NUMBER_OF_MAN];
    register int i;

    hMutex = CreateMutex(NULL, FALSE, NULL);

    if(NULL == hMutex){
        printf("Error Mutex\n");
        return 0;
    }
    for(i=0; i<NUMBER_OF_MAN;i++){
        hPeopleDining[i] = CreateThread(NULL, 0, &dining, (void*)ID[i], 0, NULL);
        if(NULL == hPeopleDining[i]){
            printf("Error Thread:%d\n", hPeopleDining[i]);
            return 0;
        }
    }
    return 0;
}
