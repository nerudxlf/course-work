#include <windows.h>
#include <stdio.h>
#define NUMBER_OF_MAN 4

#define TIME 5000 //ms
#define INF INFINITE

WORD NUMBER_OF_PIECES_OF_CAKE = 20;
CRITICAL_SECTION csec;
HANDLE hMutex;
//1) 4 человека
//2) отрезать пирог может только один
//3) всего 20 кусков
//4) при нажатии на торт он обновляется

void GoToXY(const int X,const int Y){
    HANDLE OutPutHandle;
    CONSOLE_SCREEN_BUFFER_INFO ScreenBufInfo;
    OutPutHandle=GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(OutPutHandle,&ScreenBufInfo);
    ScreenBufInfo.dwCursorPosition.X=X;
    ScreenBufInfo.dwCursorPosition.Y=Y;
    SetConsoleCursorPosition(OutPutHandle,ScreenBufInfo.dwCursorPosition);
}


void thing(int id){
    //GoToXY(id*40, i);
    //printf("The %d is thinking now\n", id);
    Sleep(TIME*id/10);
}
void knife(int id){
    WaitForSingleObject(hMutex, INF);
    printf("Knife is bloking\n");
    //GoToXY(id*40, i);
    printf("The %d cuts the pie\n", id);
    Sleep(1000);
    printf("Knife is unbloking\n");
    ReleaseMutex(hMutex);
}
void eating(int id){
    //GoToXY(id*40, i);
    printf("The %d is eatnig now\n", id);
    NUMBER_OF_PIECES_OF_CAKE--;
    printf("%d\n", NUMBER_OF_PIECES_OF_CAKE);
    Sleep(TIME*id/10);
} 
void dining(void* lpHDining){
    DWORD id = (int)lpHDining;
    int mOne, mTwo, mThree, mFour;
    mOne = 0;
    mTwo = 0;
    mThree = 0;
    mFour = 0;
    while(NUMBER_OF_PIECES_OF_CAKE>=0){
        thing(id);
        knife(id);
        eating(id);
    }
    ExitThread(0);
}
int main(){
    system("cls");
    InitializeCriticalSection(&csec);
    HANDLE hPeopleDining[NUMBER_OF_MAN];
    register short int i;
    hMutex = CreateMutex(NULL, FALSE, NULL);
    if(NULL == hMutex){
        printf("Error Mutex\n");
        return 0;
    }
    for(i=0;i<NUMBER_OF_MAN;i++){
        hPeopleDining[i] = (HANDLE) _beginthreadex(NULL, 4096, &dining, (void*)(i+1), 0, NULL);
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
