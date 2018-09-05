#pragma once

#include <Windows.h>
#include <Process.h>

#include <iostream>
#include <sstream>

#include <dwmapi.h>

#pragma comment(lib, "dwmapi.lib")

#include <d3d9.h>
#include <d3dx9.h>
#include <hD3D.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

extern int rWidth;
extern int rHeight;
extern HWND tWnd;
extern HWND hWnd;
extern D3DXMATRIX vMatrix;
extern DWORD dwVMATRIX;
extern HANDLE hGame;
extern DWORD dwBaseEntity[50000];
extern int iBaseEntity_Alive[50000];
extern D3DXVECTOR3 vBaseEntity_Position[50000];
extern char cPlayerNames[2000][64];
extern DWORD dwBaseHumanoid[2000];
extern DWORD dwBasePlayer[1000];
extern int iBasePlayer_Alive[1000];
extern D3DXVECTOR3 vBasePlayer_Position[1000][20];
extern int aimx; 
extern int aimy;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
void WINAPI UpdateSurface(HWND hWnd);
HWND WINAPI InitializeWin(HINSTANCE hInst);

namespace Thread {
	void UpdateWinPosition();
	void EntityUpdate();
	void CheatUpdate();
	void ReadUpdate();
}
