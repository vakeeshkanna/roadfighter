#define WIN32_LEAN_AND_MEAN
#include<windows.h>
#define INITGUID
#include<ddraw.h>
#include<stdio.h>
#include<stdlib.h>
#include<mmsystem.h>
#include "renderingengine.h"
#include "soundengine.h"
#include "soundmanager.h"
#include "roadfighter.h"
#include "stage.h"
#include "viewport.h"
#include "playercar.h"
#include "car.h"
#include "control.h"
#include "log-class.h"
#include "mpydll.h"
#include "framerate.h"

//#define SafeRelease(x) if(x) {x->Release();x=NULL;}
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?1:0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?0:1)

bool Exclusive = true;
double FPS = 50.0;
#ifndef WINAPI
#define WINAPI
#endif

HWND hwnd;

long WINAPI WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);
static BOOL Init(HINSTANCE hInstance,int cCmdShow);

int WINAPI WinMain(HINSTANCE hinstance,HINSTANCE hPrevInstance,PSTR szCmdLine,int iCmdShow)
{
	MSG msg;
	BOOL notDone = true;
	int frame = 0;
	//RE->setScreenDimensions(640,480);

	if(!Init(hinstance,iCmdShow))
	{
//		Cleanup();
		return false;
	}

	//MapSetVal(MPY_8BITTOPINK, 0);
	//MapLoad ("mymap.fmp", RE->DDraw);
	//MapSetDrawArea (0, 0, 256, 224);
	//Initialization stuff comes here
	RoadFighter *rf = new RoadFighter();

	//set the current viewable area, i.e the clipper for the main buffer (viewport)
	rf->setViewableArea(0,0,VIEWPORT_WIDTH,VIEWPORT_HEIGHT);
	rf->init();
	//AllocateMemory
	//LoadFiles
	//BuildTables
	//FR->init(40);
	while(notDone)
	{
		//MapDrawBG (VP->buffer, 0, 0);
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				notDone = false;
				//if(rf != NULL)
					//delete rf;
				RE->cleanUp();
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			PlayerCar *player = rf->getPlayer();
			if(!rf->isStageLoaded())
			{
				rf->reinit();
				rf->prepareStage(ROADFIGHTER_STAGE_3);
				rf->setStageLoaded(yes);
			}
			else if(player->isStageCompleted())
			{
				int currentStage = rf->getCurrentStage();

				if(currentStage == ROADFIGHTER_STAGE_4)
				{
					//show credits
					exit(0);
				}
				if(currentStage < NUM_STAGES)
				{
					rf->prepareStage(currentStage + 1);
				}
			}

			if(rf->isSkipCurrentFrame())
			{
				rf->setSkipCurrentFrame(no);
			}
			else
			{
				rf->processNextFrame();

				/*//Control
				rf->processUserInput(0.0);

				//Game Logic
				rf->applyGameLogic(0.0);

				//Game AI
				rf->applyGameAI(0.0);

				//Collision Detection
				rf->applyCollisionDetection();

				//Game Physics
				rf->applyGamePhysics(0.0);

				//Process Player State
				rf->processPlayerState();
				*/
			}

			//RenderFrame
			//rf->renderFrame();
			RE->flipBuffers(hwnd);
			if(rf->isStartingFirstStage())
			{
				rf->startFirstStage();
			}
		}
	}
	return msg.wParam;
}

long WINAPI WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
//	PAINTSTRUCT ps;
	switch(message)
	{
		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_ESCAPE:
					DestroyWindow(hwnd);
					break;

				default:
					break;
			}
			return 0;

				case WM_DESTROY:
//				Cleanup();
				lprintf("Exiting ...\n");
				PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hwnd,message,wParam,lParam);
}

static BOOL Init(HINSTANCE hInstance,int cCmdShow)
{
	static char szAppName[]="RoadFighter";
	WNDCLASS WindowClass;
	WindowClass.style =CS_HREDRAW|CS_VREDRAW;
	WindowClass.lpfnWndProc =WndProc;
	WindowClass.cbClsExtra =0;
	WindowClass.cbWndExtra =0;
	WindowClass.hInstance=hInstance;
	WindowClass.hIcon=LoadIcon(0,IDI_APPLICATION);
	WindowClass.hCursor=LoadCursor(0,IDC_ARROW);
	WindowClass.hbrBackground =static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	WindowClass.lpszMenuName =0;
	WindowClass.lpszClassName =szAppName;
	RegisterClass(&WindowClass);
    int Screen_Width=GetSystemMetrics(SM_CXSCREEN);
	int Screen_Height=GetSystemMetrics(SM_CYSCREEN);

	hwnd = CreateWindow(szAppName, szAppName, WS_VISIBLE | WS_POPUP | WS_OVERLAPPED, 0, 0, Screen_Width, Screen_Height, 0, 0, hInstance, 0);

	ShowWindow(hwnd,1);
	UpdateWindow(hwnd);

	//init RenderingEngine
	if(Exclusive)
	{
		if(!RE->init(hwnd))
		{
			return false;
		}
	}
	else
	{
		if(!RE->initWindowed(hwnd))
		{
			return false;
		}
	}

	//init SoundEngine

	if(!SE->init(hwnd))
	{
		return false;
	}

	ShowCursor(FALSE);
	return true;
}


//void Cleanup(void)
//{
//	for(int i = 0;i < 3;i++)
//		SafeRelease(back_surf[i]);
//	SafeRelease(lpDDSBack);
//	SafeRelease(lpDDSPrimary);
//	SafeRelease(lpDD);
//}


