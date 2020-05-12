#include <Windows.h>
#include <stdio.h>
#define NUMBER_OF_MAN 4
#define TIME 5000 //ms
#define INF INFINITE


short int NUMBER_OF_PIECES_OF_CAKE = 17;

HANDLE hMutex, 
    hSPrintThink, 
    hSPrintEat, 
    sPrintPie,
    hMutexNum;

//1) 4 человека
//2) отрезать пирог может только один
//3) всего 20 кусков
//4) при нажатии на торт он обновляется

const char arrTable[14][30] = {
    " ___________________________ ",
    "/                           \\",
    "|                           |",
    "|                           |",
    "|                           |",
    "|                           |",
    "|                           |",
    "|                           |",
    "|                           |",
    "|                           |",
    "|                           |",
    "|                           |",
    "|                           |",
    "\\___________________________/",

};


const char arr[7][15] = {
        "_______________",
        "|             |",
        "|             |",
        "|             |",
        "|             |",
        "|             |",
        "|_____________|"
    };


void GoToXY(const int X,const int Y){
    HANDLE OutPutHandle;
    CONSOLE_SCREEN_BUFFER_INFO ScreenBufInfo;
    OutPutHandle=GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(OutPutHandle,&ScreenBufInfo);
    ScreenBufInfo.dwCursorPosition.X=X;
    ScreenBufInfo.dwCursorPosition.Y=Y;
    SetConsoleCursorPosition(OutPutHandle,ScreenBufInfo.dwCursorPosition);
}

void printPie(){
    for(register short int i = 0; i<7; i++){
        for(register short int j = 0; j<15; j++){
            GoToXY(j+34, i+12);
            printf("%c\n", arr[i][j]);
        }
    }
}

void printTable(){
    for(register short int i = 0; i<14; i++){
        for(register short int j = 0; j<30; j++){
            GoToXY(j+27, i+9);
            printf("%c\n", arrTable[i][j]);
        }
    }
}

void editNum(){
    WaitForSingleObject(hMutexNum, INF);
    NUMBER_OF_PIECES_OF_CAKE--;
    ReleaseMutex(hMutexNum);
}

void thing(int id){
    WaitForSingleObject(hSPrintThink, INF);
    if(id==1){
        GoToXY(40, 5);//верхний
        printf("(~.~)");
    }
    else if(id==2){
        GoToXY(15, 15);//левый
        printf("(~.~)");
    }
    else if(id==3){
        GoToXY(40, 25);//нижний
        printf("(~.~)");
    }
    else if(id==4){
        GoToXY(65, 15);//правый
        printf("(~.~)");
    }
    ReleaseSemaphore(hSPrintThink, 1, NULL);
    Sleep(TIME*id/2);
}

void knife(int id){
    WaitForSingleObject(hMutex, INF);
    Sleep(2000);
    ReleaseMutex(hMutex);
}

void eating(int id){
    WaitForSingleObject(hSPrintEat, INF);
    if(id==1){
        GoToXY(40, 5);
        printf("(^O^)");
    }
    else if(id==2){
        GoToXY(15, 15);
        printf("(^O^)");
    }
    else if(id==3){
        GoToXY(40, 25);
        printf("(^O^)");
    }
    else if(id==4){
        GoToXY(65, 15);
        printf("(^O^)");
    }
    ReleaseSemaphore(hSPrintEat, 1, NULL);
    Sleep(TIME*id/3);
} 


void dining(void* lpHDining){
    DWORD id = (int)lpHDining;
    while(NUMBER_OF_PIECES_OF_CAKE>=0){
        thing(id);
        knife(id);

        WaitForSingleObject(sPrintPie, INF);
        GoToXY(40, 15);
        if(NUMBER_OF_PIECES_OF_CAKE+2>9) printf("%d", NUMBER_OF_PIECES_OF_CAKE+2);
        else printf(" %d", NUMBER_OF_PIECES_OF_CAKE+2);
        editNum();
        ReleaseSemaphore(sPrintPie, 1, NULL);

        eating(id);
    }
    thing(id);
    WaitForSingleObject(sPrintPie, INF);
    GoToXY(40, 15);
    //printf(" %d", NUMBER_OF_PIECES_OF_CAKE+2);
    ReleaseSemaphore(sPrintPie, 1, NULL);
    ExitThread(0);
}

int main(){
    system("cls");
    HANDLE hPeopleDining[NUMBER_OF_MAN];
    register short int i;
    hMutex = CreateMutex(NULL, FALSE, NULL);
    hSPrintEat = CreateSemaphore(NULL, 1, 1, NULL);
    hSPrintThink = CreateSemaphore(NULL, 1, 1, NULL);
    sPrintPie = CreateSemaphore(NULL, 1, 1, NULL);
    hMutexNum = CreateMutex(NULL, FALSE, NULL);
    if(NULL == hMutex){
        printf("Error Mutex\n");
        return 0;
    }
    if(NULL == hSPrintEat){
        printf("Error Mutex\n");
        return 0;
    }
    if(NULL == hSPrintThink){
        printf("Error Mutex\n");
        return 0;
    }
    if(NULL == sPrintPie){
        printf("Error Mutex\n");
        return 0;
    }
    if(NULL == hMutexNum){
        printf("Error Mutex\n");
        return 0;
    }
    printTable();
    printPie();
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
    CloseHandle(hSPrintEat);
    CloseHandle(hSPrintThink);
    CloseHandle(hMutexNum);
    CloseHandle(sPrintPie);
    CloseHandle(hMutex);
    return 0;
}
