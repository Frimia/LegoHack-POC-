#include <hMain.h>

char WINNAME[19] = " ";
char TARGETNAME[64] = "Roblox";

int rWidth = 1024;
int rHeight = 768;

int aimx = 0;
int aimy = 0;
int op = 0;

DWORD procID;

WNDCLASSEX wndClass;

MARGINS MARGIN = { 0, 0, rWidth, rHeight };
HWND tWnd;
HWND hWnd;
HANDLE hGame;

BOOL UNLOADING = FALSE;
BOOL NOTFOUND = FALSE;

D3DXMATRIX vMatrix;

DWORD dwVMATRIX = 0x0; //HARDCODED ADDRESS OF VMATRIX NEEDS GETTING THIS ADDRESS NEEDS TO BE REPLACED WITH EITHER A POINTER CHAIN TO THE ADDRESS OR SIG SCAN!!!

SYSTEM_INFO sysinfo;

DWORD dwPage[2048];
DWORD dwPageSize[2048];


int iBaseEntity_Alive[50000];
D3DXVECTOR3 vBaseEntity_Position[50000];
char cPlayerNames[2000][64];

DWORD dwBaseEntity[50000];
DWORD dwBaseHumanoid[2000];
DWORD dwBasePlayer[1000];

int iBasePlayer_Alive[1000];
D3DXVECTOR3 vBasePlayer_Position[1000][20];

char membuffer[0x3fffffff];

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	//Create Console Window
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	//Initialization
	while (!tWnd) {
		tWnd = FindWindow(NULL, TARGETNAME);
		GetWindowThreadProcessId(tWnd, &procID);
	}

	while (!hGame) {
		hGame = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
	}

	if (!tWnd && !UNLOADING) { MessageBox(NULL, "Unable to locate game window!", "Error", MB_OK | MB_ICONERROR); 	
	NOTFOUND = TRUE; }

	HWND hWnd = InitializeWin((HINSTANCE)hInst);
	MSG uMessage;

	if (hWnd == NULL) { exit(1); }

	ShowWindow(hWnd, SW_SHOW);

	//Get System Info
	GetSystemInfo(&sysinfo);

	//Create threads
	//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Thread::CheatUpdate, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Thread::ReadUpdate, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Thread::EntityUpdate, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Thread::UpdateWinPosition, 0, 0, 0);

	while (true) {
		if (PeekMessage(&uMessage, hWnd, 0, 0, PM_REMOVE)) {
			DispatchMessage(&uMessage);
			TranslateMessage(&uMessage);
		}
		
	}
	
	//Cleanup and unload our module (left over from internal overlay) - D66
	DestroyWindow(hWnd);
	UnregisterClass(WINNAME, (HINSTANCE)hInst);
	FreeLibraryAndExitThread((HMODULE)hInst, 0);
	return 2;
}

namespace Thread {

	// Roblox Sig Based Entity Scanner
	void EntityUpdate() {
		while (true) {
			//Min and max address space
			uintptr_t min_address = (uintptr_t)sysinfo.lpMinimumApplicationAddress;
			uintptr_t max_address = (uintptr_t)sysinfo.lpMaximumApplicationAddress;

			MEMORY_BASIC_INFORMATION mbi;

			DWORD lastpage = 0x0;
			DWORD lastpagesize = 0x0;

			//Get target process mem pages
			int pageindex = 0;
			while (min_address < max_address) {
				VirtualQueryEx(hGame, (void *)min_address, &mbi, sizeof(MEMORY_BASIC_INFORMATION));

				if (mbi.State == MEM_COMMIT && mbi.Protect == PAGE_READWRITE && mbi.Type != MEM_IMAGE && mbi.Type == MEM_PRIVATE) {
					if ((DWORD)mbi.BaseAddress > (DWORD)(lastpage + lastpagesize) || pageindex == 0) {
						dwPage[pageindex] = (DWORD)mbi.BaseAddress;
						dwPageSize[pageindex] = (DWORD)mbi.RegionSize;

						std::cout << "Page found: " << std::hex << dwPage[pageindex] << std::endl;

						pageindex++;
					}
					lastpage = (DWORD)mbi.BaseAddress;
					lastpagesize = (DWORD)mbi.RegionSize;
				}

				min_address += mbi.RegionSize;
			}

			//Get all entities
			int entindex = 0, pentindex = 0;
			for (int i = 0; i <= pageindex; i++) {
				ReadProcessMemory(hGame, (void *)(dwPage[i]), &membuffer, dwPage[i] + dwPageSize[i], NULL);
				bool foundsig = false;
				for (int ii = 0; ii < dwPageSize[i]; ii++) {

					// Get All Base Entities
					for (int iii = 0; iii <= 5; iii++) {
						break; //Uncomment this line to store all entities in dwBaseEntity list
						char sig[6] = "\x33\x33\x44\x43";

						if (membuffer[ii + iii] != sig[iii])
							break;

						if (iii >= 5) {
							dwBaseEntity[entindex] = (dwPage[i] + ii) - 0xA0;

							iBaseEntity_Alive[entindex] = *(int*)(&membuffer[ii - 0xA0 + 0x18]);
							vBaseEntity_Position[entindex] = *(D3DXVECTOR3*)(&membuffer[ii - 0xA0 + 0xD8]);
							std::cout << "Found entity sig at: 0x" << std::hex << dwBaseEntity[entindex] << " Alive: " << std::dec << iBaseEntity_Alive[entindex] << " X: "
							<< vBaseEntity_Position[entindex].x << " Y: " << vBaseEntity_Position[entindex].y << " Z: " << vBaseEntity_Position[entindex].z << std::endl;
							entindex++;

							foundsig = true;
						}
					}

					// Get All Humanoids
					for (int iii = 0; iii <= 23; iii++) {
						char sig[25] = "\x20\x41\x00\x00\xC0\x3F\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x3F\x00\x00";
						char mask[25] = "xxxxxxx???xxxxxxxxxx";
						if (membuffer[ii + iii] != sig[iii] && mask[iii] == 'x')
							break;

						if (iii >= 23) {
							dwBaseHumanoid[pentindex] = (dwPage[i] + ii) - 0x126;
							//iBasePlayer_Alive[pentindex] = *(int*)(&membuffer[ii - 0x126 + 0x5c]);

							std::cout << "Found base humanoid at: 0x" << std::hex << (DWORD)(dwPage[i] + ii) << std::dec << " Alive: "<< iBasePlayer_Alive[pentindex] << std::endl;
							pentindex++;

							foundsig = true;
						}
					}
				}
				//Sleep(1);
			}
			//std::cout << "Players found: " << std::dec << pentindex << std::endl;

			//std::cout << "Finished Pass." << std::endl;


			Sleep(1048);
		}
	}

	void CheatUpdate() {
		while (true) {
			//Read MVP Matrix

			//Sleep(1);
		}
	}

	void ReadUpdate() {
		while (true) {
			//Update Player Positions
			for (int i = 0; i < 2000; i++) {

				if (dwBaseHumanoid[i] == 0x0)
					continue;

				//Deref to BasePlayer
				DWORD dwBaseEntAddress = 0x0;
				ReadProcessMemory(hGame, (void*)(dwBaseHumanoid[i] + 0x54), &dwBaseEntAddress, sizeof(DWORD), NULL);
				ReadProcessMemory(hGame, (void*)(dwBaseEntAddress + 0x5c), &dwBaseEntAddress, sizeof(DWORD), NULL);

				//Store player base address
				dwBasePlayer[i] = dwBaseEntAddress;

				//Store Position
				ReadProcessMemory(hGame, (void*)(dwBaseEntAddress + 0xD8), &vBasePlayer_Position[i][0], sizeof(D3DXVECTOR3), NULL);

				//Store Alive State
				ReadProcessMemory(hGame, (void*)(dwBaseHumanoid[i] + 0x5C), &iBasePlayer_Alive[i], sizeof(int), NULL);

				//Store Player Name
				DWORD temp = 0x0;
				ReadProcessMemory(hGame, (void*)(dwBaseHumanoid[i] + 0x34), &temp, sizeof(DWORD), NULL);
				ReadProcessMemory(hGame, (void*)(temp + 0x28), &temp, sizeof(DWORD), NULL);
				ReadProcessMemory(hGame, (void*)(temp), &cPlayerNames[i], 32, NULL);

				ReadProcessMemory(hGame, (void*)(temp), &temp, sizeof(DWORD), NULL);

				char buffer[64] = "";
				if (ReadProcessMemory(hGame, (void*)(temp), &buffer, 32, NULL))
					strcpy((char*)cPlayerNames[i], (char*)buffer);
			}

			Sleep(16);
		}
	}
}

HWND WINAPI InitializeWin(HINSTANCE hInst) {

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.cbClsExtra = NULL;
	wndClass.cbWndExtra = NULL;
	wndClass.hCursor = LoadCursor(0, IDC_ARROW);
	wndClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	wndClass.hIconSm = LoadIcon(0, IDI_APPLICATION);
	wndClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	wndClass.hInstance = hInst; 
	wndClass.lpfnWndProc = WindowProc;
	wndClass.lpszClassName = WINNAME;
	wndClass.lpszMenuName = WINNAME;
	wndClass.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wndClass)) {
		exit(1);
	}

	hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, WINNAME, WINNAME, WS_POPUP, 1, 1, rWidth, rHeight, 0, 0, 0, 0); 
	SetLayeredWindowAttributes(hWnd, 0, 1.0f, LWA_ALPHA);
	SetLayeredWindowAttributes(hWnd, 0, RGB(0, 0, 0), LWA_COLORKEY);

	if (!hWnd)
		exit(1337);

	if (!D3DInitialize(hWnd))
		exit(1338);

	return hWnd;
}

namespace Thread {

	void UpdateWinPosition() {
		while (true) {

			// Clean Entity List
			for (int ii = 0; ii < 2000; ii++) {
				for (int pid = 0; pid < 2000; pid++) {
					if (dwBaseHumanoid[ii] == dwBaseHumanoid[pid] && pid != ii) {
						dwBaseHumanoid[pid] = 0x0;
						//dwBasePlayer_Position[pid][0] = D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
					}
				}
			}

			UpdateSurface(hWnd);

			Sleep(2000);
		}

		return;
	}
}

void WINAPI UpdateSurface(HWND hWnd) {
		RECT wSize;
		HWND tWnd;

		tWnd = FindWindow(NULL, TARGETNAME);

		if (!tWnd && hWnd) { ShowWindow(hWnd, SW_HIDE); exit(0); }

		//if (tWnd) {
			GetWindowRect(tWnd, &wSize);
			rWidth = wSize.right - wSize.left;
			rHeight = wSize.bottom - wSize.top;

			DWORD dwStyle = GetWindowLong(tWnd, GWL_STYLE);
			if (dwStyle & WS_BORDER)
			{
				wSize.top += 23; rHeight -= 23;
				//wSize.left += 10; rWidth -= 10;
			}

			if (hWnd) {
				MoveWindow(hWnd, wSize.left, wSize.top, rWidth, rHeight, true);
			}
	//	}

	return;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
	switch (uMessage) {
		case WM_CREATE:
			DwmExtendFrameIntoClientArea(hWnd, &MARGIN);
			break;

		case WM_PAINT:
			D3DRender();
			Sleep(16);
			break;

		case WM_DESTROY:
			DeleteObject(wndClass.hbrBackground);
			DeleteObject(wndClass.hIcon);
			PostQuitMessage(1);
			break;

		default:
			return DefWindowProc(hWnd, uMessage, wParam, lParam);
			break;
	}

	return 0;
}