#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main() {
    HWND hwnd = GetDesktopWindow();
    HDC hdc = GetWindowDC(hwnd);

    RECT rect;
    GetWindowRect(hwnd, &rect);

    int width, height, nrChannel;
    unsigned char *img = stbi_load("./images/diddy.png", &width, &height, &nrChannel, 0);

    if (!img) {
        printf("Failed to load image\n");
        return 1;
    }

    int result = MessageBox(NULL, "Accept your invite to the diddy party?", "Diddy", MB_YESNO | MB_ICONQUESTION);


    srand((unsigned int)time(NULL)); 

    while (1) {
        POINT mousePOS;
        GetCursorPos(&mousePOS);
        printf("%d\n", rect.right);
        printf("%d\n", rect.bottom);

        int deltaX = rand() % 11 - 5; 
        int deltaY = rand() % 11 - 5; 

        BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hdc, 0, 0, NOTSRCCOPY);

        int startX = rand() % rect.right; 
        int startY = rand() % rect.bottom;

        startX = (startX + deltaX + rect.right) % rect.right; 
        startY = (startY + deltaY + rect.bottom) % rect.bottom; 

        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                unsigned char *offset = img + (4 * (y * width + x));

                if (offset[3] == 0) {
                    continue;
                }

                int screenX = (startX + x) % rect.right; 
                int screenY = (startY + y) % rect.bottom; 

                if (screenX >= 0 && screenX < rect.right && screenY >= 0 && screenY < rect.bottom) {
                    SetPixel(hdc, screenX, screenY, RGB(offset[0], offset[1], offset[2]));
                }
            }
        }

        InvalidateRect(hwnd, NULL, TRUE);
    }

    ReleaseDC(hwnd, hdc);
    stbi_image_free(img);
    return 0;
}

// gcc diddy.c -o diddy.exe -lgdi32
// ./diddy.exe