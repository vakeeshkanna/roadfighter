#include "soundmanager.h"
#include "log-class.h"
SoundManager* SoundManager::sManager = NULL;

SoundManager::SoundManager()
{
	int i;

	for(i = 0; i < MAX_SOUNDS; i++)
	{
		lpDSSOUNDS[i] = NULL;
	}
}

SoundManager::~SoundManager()
{

}

SoundManager* SoundManager::getInstance()
{
	if(sManager == NULL)
		sManager = new SoundManager();

	return sManager;
}

void SoundManager::addSound(int id, string fname)
{
	LPDIRECTSOUNDBUFFER soundBuffer = SE->createSoundBuffer(fname.c_str());
	lpDSSOUNDS[id] = soundBuffer;
}

void SoundManager::play(int id, Logical async, Logical looping)
{
	if(lpDSSOUNDS[id] != NULL)
	{
		if(looping && async)
		{
			lpDSSOUNDS[id]->Play(0, 0, DSBPLAY_LOOPING);
		}
		else if(!looping && async)
		{
			lpDSSOUNDS[id]->Play(0, 0, 0);
		}
		else if(!looping && !async)
		{
			HANDLE NotifyEvent;
			NotifyEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

			LPDIRECTSOUNDNOTIFY8 lpDsNotify;
			DSBPOSITIONNOTIFY PositionNotify;

			if (lpDSSOUNDS[id]->QueryInterface(IID_IDirectSoundNotify8,(LPVOID*)&lpDsNotify) == DS_OK)
			{
				PositionNotify.dwOffset = DSBPN_OFFSETSTOP;
				PositionNotify.hEventNotify = NotifyEvent;
				if(lpDsNotify->SetNotificationPositions(1, &PositionNotify) != DS_OK)
				{
					cout << "Error while setting up Notification Positions!" << endl;
				}
				else
				{
					lpDsNotify->Release();
				}
			}
			else
			{
				cout << "Notification settings failed!" << endl;
			}

			Logical playing = yes;
			lpDSSOUNDS[id]->Play(0, 0, 0);
			while(playing)
			{
				WaitForSingleObject(NotifyEvent, INFINITE);
				if(WAIT_OBJECT_0 == 0)
				{
					ResetEvent(NotifyEvent);
					playing = no;
				}
			}
		}
	}
}

void SoundManager::stop(int id)
{
	if(lpDSSOUNDS[id] != NULL)
	{
		lpDSSOUNDS[id]->Stop();
	}
}

Logical SoundManager::isPlaying(int id)
{
	DWORD status;

	if(lpDSSOUNDS[id] != NULL)
	{
		lpDSSOUNDS[id]->GetStatus(&status);
		if(status & DSBSTATUS_LOOPING | status & DSBSTATUS_PLAYING)
		{
			return yes;
		}
	}
	return no;
}

void SoundManager::stopAllSounds()
{
	int i;

	for(i = 0; i < MAX_SOUNDS; i++)
	{
		if(lpDSSOUNDS[i] != NULL)
		{
			lpDSSOUNDS[i]->Stop();
		}
	}
}
