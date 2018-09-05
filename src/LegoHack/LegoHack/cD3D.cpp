#include <hD3D.h>

IDirect3DDevice9Ex* pDevice;
IDirect3D9Ex* pInstance;
D3DPRESENT_PARAMETERS pParams;

ID3DXFont* pFontTahomaLarge;
ID3DXFont* pFontTahomaSmall;

ID3DXFont* pFontConsolasLarge;
ID3DXFont* pFontConsolasSmall;

ID3DXFont* pFontTerminalLarge;
ID3DXFont* pFontTerminalSmall;

ID3DXLine* pLine;

D3DXVECTOR3 vLastPos;

char buf[128];

/*WorldToScreen Function from GuidedHacking.com*/
bool WorldToScreen(D3DXVECTOR3 pos, D3DXVECTOR2 &screen, D3DXMATRIX matrix, int windowWidth, int windowHeight)
{

	//D3DXMatrixTranspose(&matrix, %matrix);
	//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
	D3DXVECTOR4 clipCoords;
	clipCoords.x = pos.x*matrix._11 + pos.y*matrix._12 + pos.z*matrix._13 + matrix._14;
	clipCoords.y = pos.x*matrix._21 + pos.y*matrix._22 + pos.z*matrix._23 + matrix._24;
	clipCoords.z = pos.x*matrix._31 + pos.y*matrix._32 + pos.z*matrix._33 + matrix._34;
	clipCoords.w = pos.x*matrix._41 + pos.y*matrix._42 + pos.z*matrix._43 + matrix._44;

	if (clipCoords.w < 0.1f)
		return false;

	//perspective division, dividing by clip.W = Normalized Device Coordinates
	D3DXVECTOR3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (rWidth / 2 * NDC.x) + (NDC.x + rWidth / 2);
	screen.y = -(rHeight / 2 * NDC.y) + (NDC.y + rHeight / 2);
	return true;
}

BOOL D3DInitialize(HWND hWnd) {
	
	Direct3DCreate9Ex(D3D_SDK_VERSION, &pInstance);

	pParams.Windowed = TRUE;
	pParams.BackBufferFormat = D3DFMT_A8R8G8B8;
	pParams.BackBufferHeight = rHeight;
	pParams.BackBufferWidth = rWidth;
	pParams.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	pParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pParams.EnableAutoDepthStencil = TRUE;
	pParams.AutoDepthStencilFormat = D3DFMT_D16;

	pInstance->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &pParams, 0, &pDevice);

	if (pDevice == NULL) { return FALSE; }

	D3DXCreateFont(pDevice, 14, 0, 0, 0, false, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Tahoma", &pFontTahomaLarge);
	D3DXCreateFont(pDevice, 12, 0, 0, 0, false, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Tahoma", &pFontTahomaSmall);

	D3DXCreateFont(pDevice, 18, 0, 0, 0, false, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Consolas", &pFontConsolasLarge);
	D3DXCreateFont(pDevice, 12, 0, 0, 0, false, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Consolas", &pFontConsolasSmall);

	D3DXCreateFont(pDevice, 18, 0, 0, 0, false, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &pFontTerminalLarge);
	D3DXCreateFont(pDevice, 9, 0, 0, 0, false, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &pFontTerminalSmall);

	D3DXCreateLine(pDevice, &pLine);

	return TRUE;
}

void D3DRender() {
	pParams.BackBufferHeight = rHeight;
	pParams.BackBufferWidth = rWidth;
	pDevice->Reset(&pParams);

	pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

	pDevice->BeginScene();

	if (tWnd == GetForegroundWindow() || tWnd == GetActiveWindow()) {

		DrawStringOutline("LegoHack", 15, 7, 197, 66, 244, 255, pFontTahomaLarge);
		DrawStringOutline("DREAM666", 15, 20, 132, 47, 47, 255, pFontTahomaLarge);

		//sprintf((char*)buf, "Width: %d \nHeight: %d", rWidth, rHeight);
		//DrawStringOutline((char *)buf, 8, 30, 132, 47, 47, 255, pFontTahomaLarge);

		D3DXVECTOR3 position = { 50.5f,50.5f,35.5f };

		D3DXVECTOR2 screenHead = {};
		D3DXVECTOR2 screenTorso = {};
		D3DXVECTOR2 screenFeet = {};

		D3DXVECTOR2 screenJoint = {};
		
		float mass = 0.0f;
		DWORD baseentaddy;

		//Entity Loop
		for (int i = 0; i < 0; i++) {
			if (vBaseEntity_Position[i] == D3DXVECTOR3{ 0.0f, 0.0f, 0.0f })
				continue;

			//DEBUG Uncomment this line to draw dots on all entities found and stored by first scan
			//if (WorldToScreen(dwBaseEntity_Position[i], screenXY, vMatrix, rWidth, rHeight))
				//DrawStringOutline("o", screenXY.x, screenXY.y, 224, 196, 13, 255, pFontTahomaSmall);
			
		}

		//Player Loop
		ReadProcessMemory(hGame, (void*)dwVMATRIX, &vMatrix, sizeof(D3DXMATRIX), NULL);
		for (int i = 0; i < 2000; i++) {

			if (dwBaseHumanoid[i] == 0x0)
				continue;

			if (vBasePlayer_Position[i][0] == D3DXVECTOR3{ 0.0f, 0.0f, 0.0f })
				continue;

			if (iBasePlayer_Alive[i] == -1)
				continue;

			if (WorldToScreen(vBasePlayer_Position[i][0], screenTorso, vMatrix, rWidth, rHeight)) {
				WorldToScreen(D3DXVECTOR3{ vBasePlayer_Position[i][0].x, vBasePlayer_Position[i][0].y + 2.5f, vBasePlayer_Position[i][0].z }, screenHead, vMatrix, rWidth, rHeight);
				WorldToScreen(D3DXVECTOR3{ vBasePlayer_Position[i][0].x, vBasePlayer_Position[i][0].y - 3.0f, vBasePlayer_Position[i][0].z }, screenFeet, vMatrix, rWidth, rHeight);
				//DrawStringOutline("o", screenTorso.x, screenTorso.y, 224, 196, 13, 255, pFontTahomaSmall);
				//DrawStringOutline("o", screenFeet.x, screenFeet.y, 224, 196, 13, 255, pFontTahomaSmall);

				RECT rPosition;

				rPosition.left = screenTorso.x - (screenHead.y - screenTorso.y) / 2.1f;
				rPosition.top = screenHead.y;
				rPosition.bottom = screenFeet.y;
				rPosition.right = screenTorso.x + (screenHead.y - screenTorso.y) / 2.1f;

				DrawRectOutlined(rPosition, 22, 163, 43, 255);

				sprintf((char*)buf, "base: 0x%02X", dwBaseHumanoid[i]);
				DrawStringOutline(cPlayerNames[i], (screenTorso.x + 5.0f) - (screenHead.y - screenTorso.y) / 2.1f, screenFeet.y + (screenHead.y - screenFeet.y), 255, 255, 255, 255, pFontTahomaSmall);
				//screen.x - (screenhead.y - screen.y) / 2.6f
				//screen.x + (screenhead.y - screen.y) / 2.6f

				//DrawStringOutline("player", (screenTorso.x + 5.0f) - (screenHead.y - screenTorso.y) / 2.1f, screenFeet.y + (screenHead.y - screenFeet.y), 255, 255, 255, 255, pFontTahomaSmall);
				//for (int jointid = 1; jointid < 20; jointid++) {
					//WorldToScreen(D3DXVECTOR3{ dwBasePlayer_Position[i][2].x, dwBasePlayer_Position[i][2].y, dwBasePlayer_Position[i][2].z }, screenJoint, vMatrix, rWidth, rHeight);
					//DrawStringOutline("o", screenJoint.x, screenJoint.y, 255, 255, 255, 255, pFontTahomaSmall);
				//}
			}

		}

		//Draw Crosshair
		DrawCrosshair(50, 151, 188, 1.5f);
	}


	pDevice->EndScene();
	pDevice->PresentEx(0, 0, 0, 0, 0);
}

void DrawCrosshair(float r, float g, float b, float size) {
	DrawLine(((rWidth / 2) - 10.0f * size) - 1.0f, (rHeight / 2) + 0.006f, ((rWidth / 2) + 10.5f * size) + 1.0f, (rHeight / 2) + 0.006f, 1, 1, 1, 255, 3.0f);
	DrawLine((rWidth / 2) - 0.006f, ((rHeight / 2) - 10.0f * size) - 1.0f, (rWidth / 2) - 0.006f, ((rHeight / 2) + 11.0f * size) + 0.006f, 1, 1, 1, 255, 3.0f);
	DrawLine(((rWidth / 2) - 10 * size), rHeight / 2, ((rWidth / 2) + 10.5f * size), rHeight / 2, r, g, b, 255, 1.0f);
	DrawLine(rWidth / 2, ((rHeight / 2) - 10.0f * size), rWidth / 2, ((rHeight / 2) + 11.0f * size), r, g, b, 255, 1.0f);
}

void DrawString(char* string, float x, float y, int r, int g, int b, int a, ID3DXFont* pFont) {
	RECT rPosition;
	rPosition.left = x;
	rPosition.top = y;

	pFont->DrawTextA(0, string, strlen(string), &rPosition, DT_NOCLIP, D3DCOLOR_RGBA(r, g, b, a));
}

void DrawStringOutline(char* string, float x, float y, int r, int g, int b, int a, ID3DXFont* pFont) {
	RECT rPosition;

	rPosition.left = x + 1.0f;
	rPosition.top = y;

	pFont->DrawTextA(0, string, strlen(string), &rPosition, DT_NOCLIP, D3DCOLOR_RGBA(1, 1, 1, a));

	rPosition.left = x - 1.0f;
	rPosition.top = y;

	pFont->DrawTextA(0, string, strlen(string), &rPosition, DT_NOCLIP, D3DCOLOR_RGBA(1, 1, 1, a));

	rPosition.left = x;
	rPosition.top = y + 1.0f;

	pFont->DrawTextA(0, string, strlen(string), &rPosition, DT_NOCLIP, D3DCOLOR_RGBA(1, 1, 1, a));

	rPosition.left = x;
	rPosition.top = y - 1.0f;

	pFont->DrawTextA(0, string, strlen(string), &rPosition, DT_NOCLIP, D3DCOLOR_RGBA(1, 1, 1, a));

	rPosition.left = x;
	rPosition.top = y;

	pFont->DrawTextA(0, string, strlen(string), &rPosition, DT_NOCLIP, D3DCOLOR_RGBA(r, g, b, a));
}

void DrawRect(RECT drawRect, int r, int g, int b, int a) {
	D3DXVECTOR2 points[5];
	pLine->SetWidth(1);

	float x = drawRect.left;
	float y = drawRect.top;
	float x2 = drawRect.right;
	float y2 = drawRect.bottom;

	points[0] = D3DXVECTOR2(x, y);
	points[1] = D3DXVECTOR2(x2, y);
	points[2] = D3DXVECTOR2(x2, y2);
	points[3] = D3DXVECTOR2(x, y2);
	points[4] = D3DXVECTOR2(x, y);

	pLine->Begin();
	pLine->Draw(points, 5, D3DCOLOR_RGBA(r, g, b, a));
	pLine->End();
}

void DrawLine(float x, float y, float x2, float y2, int r, int g, int b, int a, float w) {
	D3DXVECTOR2 points[2];
	pLine->SetWidth(w);

	points[0] = D3DXVECTOR2(x, y);
	points[1] = D3DXVECTOR2(x2, y2);

	pLine->Begin();
	pLine->Draw(points, 2, D3DCOLOR_RGBA(r, g, b, a));
	pLine->End();
}

void DrawRectOutlined(RECT drawRect, int r, int g, int b, int a) {
	D3DXVECTOR2 points[5];

	float x = drawRect.left;
	float y = drawRect.top;
	float x2 = drawRect.right;
	float y2 = drawRect.bottom;

	points[0] = D3DXVECTOR2(x + 0.25f, y + 0.25f);
	points[1] = D3DXVECTOR2(x2 + 0.25f, y + 0.25f);
	points[2] = D3DXVECTOR2(x2 + 0.25f, y2 + 0.25f);
	points[3] = D3DXVECTOR2(x + 0.25f, y2 + 0.25f);
	points[4] = D3DXVECTOR2((x + 0.25f), y + 0.25f);

	pLine->SetWidth(3.5f);
	pLine->Draw(points, 5, D3DCOLOR_RGBA(1, 1, 1, a));

	points[0] = D3DXVECTOR2(x, y);
	points[1] = D3DXVECTOR2(x2, y);
	points[2] = D3DXVECTOR2(x2, y2);
	points[3] = D3DXVECTOR2(x, y2);
	points[4] = D3DXVECTOR2(x, y);

	pLine->SetWidth(1);
	pLine->Begin();
	pLine->Draw(points, 5, D3DCOLOR_RGBA(r, g, b, a));
	pLine->End();
}

void FillRect(RECT drawRect, int r, int g, int b, int a) {
	D3DRECT RectDraw = { drawRect.left, drawRect.top, drawRect.right, drawRect.bottom };

	pDevice->Clear(1, &RectDraw, D3DCLEAR_TARGET | D3DCLEAR_TARGET, D3DCOLOR_RGBA(r, g, b, a), 0, 0);
}

void FillRectXY(float x, float y, float x2, float y2, int r, int g, int b, int a) {
	D3DXVECTOR2 points[2];

	int h = x2 - x;
	int w = y2 - y;

	pLine->SetWidth(h);

	points[0].x = x; points[0].y = y + (h >> 1);
	points[1].x = x + w; points[1].y = y + (h >> 1);

	pLine->Begin();
	pLine->Draw(points, 2, D3DCOLOR_RGBA(r, g, b, a));
	pLine->End();
}