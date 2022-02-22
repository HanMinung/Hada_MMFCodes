// ShareServer.cpp : Defines the entry point for the console application.
//

#ifndef SM_SERVER_
#define SM_SERVER_

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>
#include <tchar.h>

#define RUNTIME                       (double)             (100.0)      //[sec]
#define FREQ                          (double)             (1.0)      //[Hz]
#define N_data                        (unsigned int)       (RUNTIME*FREQ)

double iniTime = 0.0;
double simTime = 0.0;
double curTime = 0.0;
double delTime = 0.0;
double Ts = 1 / FREQ;
int    simcnt = 0;

typedef struct
{
    double idx;
    float x;
    float y;
    float z;
} MYSTRUCT;

HANDLE hMemoryMap = NULL;
LPBYTE pMemoryMap = NULL;

double GetWindowTime();
void CheckTime();

int main() {

    hMemoryMap = OpenFileMapping(FILE_MAP_READ, FALSE, L"mmf_server");
    if (!hMemoryMap)
    {
        MessageBox(NULL, L"Failed CreateFileMapping", L"Error", MB_OK);
        return FALSE;
    }

    pMemoryMap = (BYTE*)MapViewOfFile(hMemoryMap, FILE_MAP_READ, 0, 0, 0);
    if (!pMemoryMap)
    {
        CloseHandle(hMemoryMap);
        MessageBox(NULL, L"Failed MapViewOfFile", L"Error", MB_OK);
        return FALSE;
    }

    MYSTRUCT* pStruct = (MYSTRUCT*)pMemoryMap;

    iniTime = GetWindowTime() * 0.001;

    do {
        printf("pStruct->idx : %f\n", pStruct->idx);
        printf("pStruct->x : %f\n", pStruct->x);
        printf("pStruct->y : %f\n", pStruct->y);
        printf("pStruct->z : %f\n", pStruct->z);

        CheckTime();
    } while (simTime < RUNTIME);


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


double GetWindowTime(void) {
    LARGE_INTEGER   liCount, liFreq;
    QueryPerformanceCounter(&liCount);        // 시간 함수 밀리 세컨드 단위로 측정이 가능하다
    QueryPerformanceFrequency(&liFreq);       // 진동수/[sec]
    return((liCount.QuadPart / ((double)(liFreq.QuadPart))) * 1000.0);
};

void CheckTime()
{

    while (1) {
        curTime = GetWindowTime() * 0.001;   // [ms]
        delTime = curTime - iniTime - simTime;

        if (delTime >= Ts) {
            break;
        }
    }
    simTime = ((double)simcnt + 1.0) * Ts;
    simcnt = simcnt + 1;
}

#endif