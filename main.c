#include <windows.h>
#include <stdio.h>
#define NUMBER_OF_MAN 4
#define NUMBER_OF_PIECES_OF_CAKE 20
#define TIME 5000 //ms
#define INF INFINITE

HANDLE hMutex;

void thing(int id){
    printf("The %d is thinking now\n", id);
    Sleep(TIME*id/30);
}

void eating(int id){
    WaitForSingleObject(hMutex, INF);
    printf("The %d is eatnig now\n", id);
    ReleaseMutex(hMutex);
    Sleep(TIME*id/10);
} 

void dining(void* lpHDining){
    DWORD id = (int)lpHDining;;
    while(TRUE){
        thing(id);
        eating(id);
    }
    ExitThread(0);
}

int main(){
    HANDLE hPeopleDining[NUMBER_OF_MAN];
    register short int i;

    hMutex = CreateMutex(NULL, FALSE, NULL);

    if(NULL == hMutex){
        printf("Error Mutex\n");
        return 0;
    }
    for(i=0;i<NUMBER_OF_MAN;i++){
        hPeopleDining[i] = CreateThread(NULL, 0, &dining, (void*)(i+1), 0, NULL);
        if(NULL == hPeopleDining[i]){
            printf("Error Thread:%d\n", hPeopleDining[i]);
            return 0;
        }
    }

    WaitForMultipleObjects(NUMBER_OF_MAN, hPeopleDining, TRUE, INF);

    for(i=0; i<NUMBER_OF_MAN;i++){
        CloseHandle(hPeopleDining[i]);
    }
    CloseHandle(hMutex);
    return 0;
}
