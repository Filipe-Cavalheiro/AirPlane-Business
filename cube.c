#include <math.h>
#include <stdio.h>
#include <string.h>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
void usleep(__int64 usec){
  HANDLE timer;
  LARGE_INTEGER ft;

  ft.QuadPart = -(10 * usec); // Convert to 100 nanosecond interval, negative value indicates relative time

  timer = CreateWaitableTimer(NULL, TRUE, NULL);
  SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
  WaitForSingleObject(timer, INFINITE);
  CloseHandle(timer);
}
#endif

#define hideCursor() printf("\e[?25l")

float A, B, C;

int width = 60, height = 40;
float zBuffer[160 * 44];
char buffer[160 * 44];
float K1 = 40;

float incrementSpeed = 0.6;

float calculateX(int i, int j, int k) {
  return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
         j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

float calculateY(int i, int j, int k) {
  return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
         j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
         i * cos(B) * sin(C);
}

float calculateZ(int i, int j, int k) {
    return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

void calculateForSurface(float cubeX, float cubeY, float cubeZ, int ch, int horizontalOffset) {
    int distanceFromCam = 60;

    float x = calculateX(cubeX, cubeY, cubeZ);
    float y = calculateY(cubeX, cubeY, cubeZ);
    float z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;

    float ooz = 1 / z;

    int xp = (int)(width / 2 + horizontalOffset + K1 * ooz * x * 2);
    int yp = (int)(height / 2 + K1 * ooz * y);

    int idx = xp + yp * width;
    if (idx >= 0 && idx < width * height) {
        if (ooz > zBuffer[idx]) {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    float cubeHalfWidth;
    int backgroundASCIICode = ' ';
    float horizontalOffset;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    hideCursor();
    printf("\x1b[2J");
    while (1) {
        memset(buffer, backgroundASCIICode, width * height);
        memset(zBuffer, 0, width * height * 4);
        cubeHalfWidth = 12;
        horizontalOffset = 0;
        for (float cubeX = -cubeHalfWidth; cubeX < cubeHalfWidth; cubeX += incrementSpeed) {
            for (float cubeY = -cubeHalfWidth; cubeY < cubeHalfWidth; cubeY += incrementSpeed) {
                calculateForSurface(cubeX, cubeY, -cubeHalfWidth, '@', horizontalOffset);
                calculateForSurface(cubeHalfWidth, cubeY, cubeX, '$', horizontalOffset);
                calculateForSurface(-cubeHalfWidth, cubeY, -cubeX, '~', horizontalOffset);
                calculateForSurface(-cubeX, cubeY, cubeHalfWidth, '#', horizontalOffset);
                calculateForSurface(cubeX, -cubeHalfWidth, -cubeY, ';', horizontalOffset);
                calculateForSurface(cubeX, cubeHalfWidth, cubeY, '+', horizontalOffset);
            }
        }
        printf("\x1b[H");
        for (int k = 0; k < width * height; k++) {
            putchar(k % width ? buffer[k] : '\n');
        }

        A += 0.05;
        B += 0.05;
        C += 0.01;
        usleep(100);
    }
    return 0;
}