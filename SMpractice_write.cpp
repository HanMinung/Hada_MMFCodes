#include <windows.h>
#include <assert.h>
#include <conio.h>
#include <tchar.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>   
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define UNIT_PI                 (3.14)
#define RUNTIME (double)        (300.0)
#define FREQ    (double)        (1.0)
#define N_data  (unsigned_int)  (RUNTIME*FREQ)

double i = 0.0;

double GetWindowTime(void) {
    LARGE_INTEGER   liCount, liFreq;
    QueryPerformanceCounter(&liCount); 
    QueryPerformanceFrequency(&liFreq);                                     // 진동수/[sec]
    return((liCount.QuadPart / ((double)(liFreq.QuadPart))) * 1000.0);
};
// 공유 데이터 형식
typedef struct
{
    double idx ;
    float x;
    float y;
    float z;
} MYSTRUCT;

double    iniTime = GetWindowTime();
double    curTime = 0.0;
double    delTime = 0.0;
double    simTime = 0.0;
double    Ts = 1 / FREQ;

int main()
{

    HANDLE hMemoryMap = NULL;
    LPBYTE pMemoryMap = NULL;

    hMemoryMap = CreateFileMapping((HANDLE)0xffffffff, NULL, PAGE_READWRITE, 0, sizeof(MYSTRUCT), L"mmf_server");
    if (!hMemoryMap)
    {
        MessageBox(NULL, L"Failed CreateFileMapping", L"Error", MB_OK);
        return FALSE;
    }

    pMemoryMap = (BYTE*)MapViewOfFile(hMemoryMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (!pMemoryMap)
    {
        CloseHandle(hMemoryMap);
        MessageBox(NULL, L"Failed MapViewOfFile", L"Error", MB_OK);

        return FALSE;
    }

    MYSTRUCT* pStruct = (MYSTRUCT*)pMemoryMap;

    while (1) {
        
        curTime = GetWindowTime();
        simTime = curTime - iniTime;

        pStruct->idx = i + simTime ;    
        pStruct->x = 20.3f ;
        pStruct->y = 10.4f ;
        pStruct->z = 40.5f ;

    }


    // key 감지 시 break
    while (1)
    {
        if (_kbhit() != 0)
        {
            break;
        }
    }


    // 메모리 해제
    if (pMemoryMap)
    {
        UnmapViewOfFile(pMemoryMap);
    }

    if (hMemoryMap)
    {
        CloseHandle(hMemoryMap);
    }

    return 0;
}