#include <Windows.h>
#include <stdio.h>
#define NUMBER_OF_MAN 4
#define TIME 5000 //ms
#define INF INFINITE


short int NUMBER_OF_PIECES_OF_CAKE = 19;
short int STOP_INT = 0;

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
    "|        This is a pie      |",
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
        "| pieces left |",
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

void editNum(int arg){
    WaitForSingleObject(hMutexNum, INF);
    if(arg==0) NUMBER_OF_PIECES_OF_CAKE--;
    else NUMBER_OF_PIECES_OF_CAKE=20;
    ReleaseMutex(hMutexNum);
}

void thing(int id){
    WaitForSingleObject(hSPrintThink, INF);
    if(id==1){
        GoToXY(40, 7);//верхний
        printf("(~.~)");
    }
    else if(id==2){
        GoToXY(18, 15);//левый
        printf("(~.~)");
    }
    else if(id==3){
        GoToXY(40, 25);//нижний
        printf("(~.~)");
    }
    else if(id==4){
        GoToXY(62, 15);//правый
        printf("(~.~)");
    }
    ReleaseSemaphore(hSPrintThink, 1, NULL);
    Sleep(TIME*id/3);
}

void knife(int id){
    WaitForSingleObject(hMutex, INF);
    Sleep(2000);
    ReleaseMutex(hMutex);
}

void eating(int id){
    WaitForSingleObject(hSPrintEat, INF);
    if(id==1){
        GoToXY(40, 7);
        printf("(^O^)");
    }
    else if(id==2){
        GoToXY(18, 15);
        printf("(^O^)");
    }
    else if(id==3){
        GoToXY(40, 25);
        printf("(^O^)");
    }
    else if(id==4){
        GoToXY(62, 15);
        printf("(^O^)");
    }
    ReleaseSemaphore(hSPrintEat, 1, NULL);
    Sleep(TIME*id/3);
} 

void stop(int id){
    while(TRUE){
        scanf("%d", &STOP_INT);
        if(STOP_INT)
    }
}

void dining(void* lpHDining){
    DWORD id = (int)lpHDining;
    while(NUMBER_OF_PIECES_OF_CAKE>1){
        thing(id);
        knife(id);
        WaitForSingleObject(sPrintPie, INF);
        GoToXY(40, 15);
        if(NUMBER_OF_PIECES_OF_CAKE>9) printf("%d", NUMBER_OF_PIECES_OF_CAKE);
        else printf(" %d", NUMBER_OF_PIECES_OF_CAKE);
        editNum(0);
        ReleaseSemaphore(sPrintPie, 1, NULL);
        eating(id);
    }
    thing(id);
    WaitForSingleObject(sPrintPie, INF);
    GoToXY(40, 15);
    ReleaseSemaphore(sPrintPie, 1, NULL);
    ExitThread(0);
}

void displayMouseClick(HANDLE hStdout, int x, int y){
    char value = 0;
    COORD pos;
    DWORD len;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    pos.X = x;
    pos.Y = y;
    ReadConsoleOutputCharacter(hStdout, &value, 1, pos, &len);
    if((x>=34 && x<=48) && (y>=12 && y<= 18)){
        editNum(1);
    }
}

void startMouse(void* lpHDining){
    INPUT_RECORD inbuf;
    COORD coord;
    DWORD len;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD fdwMode;
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE),
        hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    fdwMode = ENABLE_EXTENDED_FLAGS;
    SetConsoleMode(hStdin, fdwMode);
    fdwMode = ENABLE_MOUSE_INPUT;
    if (!SetConsoleMode(hStdin, fdwMode)) printf("Failed to set console mode\n");
    GetConsoleScreenBufferInfo(hStdout, &csbi);
    while(1){
        ReadConsoleInput(hStdin, &inbuf, 1, &len);
        if(inbuf.EventType == MOUSE_EVENT){
            if(inbuf.Event.MouseEvent.dwButtonState==FROM_LEFT_1ST_BUTTON_PRESSED){
                displayMouseClick(hStdout,
                    inbuf.Event.MouseEvent.dwMousePosition.X,
                    inbuf.Event.MouseEvent.dwMousePosition.Y);
            }
        }
    }
    CloseHandle(hStdin);
    CloseHandle(hStdout);
    ExitThread(0);
}

int main(){
    system("cls");
    HANDLE hPeopleDining[NUMBER_OF_MAN];
    HANDLE hMouseClick;
    register short int i;
    printf("(^O^) - he is eating    (~.~) - he is thinking");
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
    hMouseClick = (HANDLE) _beginthreadex(NULL, 8192, &startMouse, (void*)1, 0, NULL);
    if(NULL == hMouseClick){
        printf("Error Thread:%d\n", hMouseClick);
        return 0;
    }
    WaitForMultipleObjects(NUMBER_OF_MAN, hPeopleDining, TRUE, INF);
    for(i=0; i<NUMBER_OF_MAN;i++){
        CloseHandle(hPeopleDining[i]);
    }
    CloseHandle(hMouseClick);
    CloseHandle(hSPrintEat);
    CloseHandle(hSPrintThink);
    CloseHandle(hMutexNum);
    CloseHandle(sPrintPie);
    CloseHandle(hMutex);
    return 0;
}
