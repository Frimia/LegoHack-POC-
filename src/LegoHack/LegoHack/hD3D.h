#pragma once

#include <hMain.h>

//DX Stuff
void D3DRender();
BOOL D3DInitialize(HWND hWnd);

//Drawing Functions
void DrawString(char* string, float x, float y, int r, int g, int b, int a, ID3DXFont* pFont);
void DrawStringOutline(char* string, float x, float y, int r, int g, int b, int a, ID3DXFont* pFont);
void DrawRect(RECT drawRect, int r, int g, int b, int a);
void DrawRectOutlined(RECT drawRect, int r, int g, int b, int a);
void DrawLine(float x, float y, float x2, float y2, int r, int g, int b, int a, float w);
void DrawCrosshair(float r, float g, float b, float size);
void FillRect(RECT drawRect, int r, int g, int b, int a);
void FillRectXY(float x, float y, float x2, float y2, int r, int g, int b, int a);
