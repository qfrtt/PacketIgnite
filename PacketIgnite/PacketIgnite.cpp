// PacketIgnite.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

ConnectToServer_t ConnectToServer;

CD3DApplication * g_pD3DApplication;
pPlayer* g_pPlayer;

typedef HRESULT(WINAPI* EndScene_t)(LPDIRECT3DDEVICE9 pDevice);
EndScene_t pEndScene;

HRESULT WINAPI nEndScene(LPDIRECT3DDEVICE9 pDevice);

LRESULT __stdcall nWndProc(HWND hwnd, unsigned int msg, WPARAM WParam, LPARAM LParam);
WNDPROC pWndProc;

void* DbNet;
void* ClientNet;
bool Connected = false;
bool EnableEQ = false;
bool static IsAsal = false;


void AllocateConsole()
{
	FILE* fp;
	AllocConsole();
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);
}

void PlaceHooks()
{
	DWORD Base = reinterpret_cast<DWORD>(GetModuleHandle(nullptr));

	g_pD3DApplication = reinterpret_cast<CD3DApplication*>(Base + 0x95EDE0); //"about:blank"

	DWORD* pdwNewDevice = (DWORD*)g_pD3DApplication->GetD3DDevice();
	pdwNewDevice = (DWORD*)pdwNewDevice[0];
	pEndScene = reinterpret_cast<EndScene_t>(DetourFunction((PBYTE)pdwNewDevice[42], (PBYTE)nEndScene));

	pWndProc = reinterpret_cast<WNDPROC>(SetWindowLongW(FindWindowA(nullptr, "Fly For Fun (Classic)"), GWL_WNDPROC, static_cast<LONG>(reinterpret_cast<LONG_PTR>(nWndProc))));

	ConnectToServer = reinterpret_cast<ConnectToServer_t>(Base + 0x271CF0); //E8 ? ? ? ? 85 C0 75 31 6A 00 6A 00 68 ? ? ? ? B9 ? ? ? ? E8 ? ? ? ? 50 B9 ? ? ? ? E8 ? ? ? ? 6A 01 E8 ? ? ? ? 8B C8 E8 ? ? ? ? E9 ? ? ? ? 6A 00 E8 ? ? ? ? 8B C8 E8 ? ? ? ? 6A 66 
	Send = reinterpret_cast<Send_t>(Base + 0x1451F0); //55 8B EC 51 89 4D FC 8B 45 FC 83 78 04 00 75 04 33 C0 EB 17
}

LRESULT __stdcall nWndProc(HWND hwnd, unsigned int msg, WPARAM WParam, LPARAM LParam)
{
	if (TwEventWin(hwnd, msg, WParam, LParam))
		return 0;

	return CallWindowProc(pWndProc, hwnd, msg, WParam, LParam);
}

HRESULT WINAPI nEndScene(LPDIRECT3DDEVICE9 pDevice)
{	
	TwDraw();
	return pEndScene(pDevice);
}

void SendKeyPress(DWORD Key)
{
	PostMessage(FindWindowA(nullptr, "Fly For Fun (Classic)"), WM_KEYDOWN, Key, NULL);
	Sleep(30);
	PostMessage(FindWindowA(nullptr, "Fly For Fun (Classic)"), WM_KEYUP, Key, NULL);
	Sleep(30);
}

DWORD WINAPI SwitchEQ()
{
	while (true)
	{
		if (GetAsyncKeyState(VK_NUMPAD1) & 0x1 && EnableEQ && !IsAsal)
		{
			//GO TO ASAL BUILD
			SendKeyPress('5');
			SendKeyPress(VK_F1);
			SendKeyPress(VK_F2);
			SendKeyPress(VK_F3);
			SendKeyPress(VK_F4);
			SendKeyPress(VK_F5);
			SendKeyPress(VK_F6);
			SendKeyPress(VK_F7);
			SendKeyPress(VK_F8);
			SendKeyPress(VK_F9);
			SendKeyPress('6');
			SendKeyPress(VK_F1);
			SendKeyPress(VK_F2);
			SendKeyPress(VK_F3);
			SendKeyPress(VK_F4);
			SendKeyPress(VK_F5);
			SendKeyPress(VK_F6);
			SendKeyPress(VK_F7);
			SendKeyPress(VK_F8);
			SendKeyPress(VK_F9);
			SendKeyPress('7');
			SendKeyPress(VK_F1);
			SendKeyPress(VK_F2);
			SendKeyPress(VK_F3);
			SendKeyPress(VK_F4);
			SendKeyPress(VK_F5);
			SendKeyPress(VK_F6);
			Sleep(50);
			SendKeyPress(VK_F7);
			SendKeyPress('1');
			IsAsal = true;
		}

		if (GetAsyncKeyState(VK_NUMPAD2) & 0x1 && EnableEQ && IsAsal)
		{
			//GO TO HP BUILD
			SendKeyPress('2');
			SendKeyPress(VK_F1);
			SendKeyPress(VK_F2);
			SendKeyPress(VK_F3);
			SendKeyPress(VK_F4);
			SendKeyPress(VK_F5);
			SendKeyPress(VK_F6);
			SendKeyPress(VK_F7);
			SendKeyPress(VK_F8);
			SendKeyPress(VK_F9);
			SendKeyPress('3');
			SendKeyPress(VK_F1);
			SendKeyPress(VK_F2);
			SendKeyPress(VK_F3);
			SendKeyPress(VK_F4);
			SendKeyPress(VK_F5);
			SendKeyPress(VK_F6);
			SendKeyPress(VK_F7);
			SendKeyPress(VK_F8);
			SendKeyPress(VK_F9);
			SendKeyPress('4');
			SendKeyPress(VK_F1);
			SendKeyPress(VK_F2);
			SendKeyPress(VK_F3);
			SendKeyPress(VK_F4);
			SendKeyPress(VK_F5);
			SendKeyPress(VK_F6);
			Sleep(50);
			SendKeyPress(VK_F7);
			SendKeyPress('1');
			IsAsal = false;
		}
	}

	return 0;
}

void SpinBot()
{
	g_pPlayer = *(pPlayer**)((reinterpret_cast<DWORD>(GetModuleHandle(nullptr)) + 0x95EA04));
	
	for (int i = 0; i < 360; i++)
	{
		float angle = *(float*)(g_pPlayer + 0x18);
		*(float*)(g_pPlayer + 0x18) = i + angle;
	}
}

void DoingTheStuff()
{
	while (true)
	{	
		DbNet = (void*)((reinterpret_cast<DWORD>(GetModuleHandle(nullptr)) + 0x953948)); //23000
		ClientNet = (void*)((reinterpret_cast<DWORD>(GetModuleHandle(nullptr)) + 0x52BC50));

		if (GetAsyncKeyState(VK_F10) & 0x1)
		{
			if (ConnectToServer(DbNet, "139.99.16.79", 24000, TRUE, 0, 10000))
			{
				printf("Connected to FlyFF db ! \n");
				Connected = true;
			}
		}

		if (GetAsyncKeyState(VK_F11) & 0x1)
		{
			SpawnItem(DbNet, 901, 21043, 1, 1215958945719254784);
			//SendQueryAddCoupleExperience(DbNet, 495, 100000);
			//SendTest(ClientNet);
			//SendQueryGetSealCharConm(DbNet, 453);
		}

		/*if (GetAsyncKeyState(VK_F9) & 0x1)
		{
			SpinBot();
		}*/
	}
}