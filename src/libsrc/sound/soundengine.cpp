#include "soundengine.h"

SoundEngine* SoundEngine::sEngine = NULL;

SoundEngine::SoundEngine()
{
	lpDS= NULL;
	//lpDSSOUNDS = NULL;
}

SoundEngine::~SoundEngine()
{
	cleanUp();
}

void SoundEngine::cleanUp()
{
	//SafeRelease(lpDS);
}

SoundEngine* SoundEngine::getInstance()
{
	if(sEngine == NULL)
		sEngine = new SoundEngine();
	return sEngine;
}

Logical SoundEngine::init(HWND hwnd)
{
	if(DirectSoundCreate8(NULL, &lpDS, NULL) != DS_OK)
	{
		return no;
	}

	if(lpDS->SetCooperativeLevel(hwnd, DSSCL_NORMAL) != DS_OK)
	{
		return no;
	}

	return yes;
}

LPDIRECTSOUNDBUFFER SoundEngine::createSoundBuffer(string fname)
{
	WaveClass wave;
	LPDIRECTSOUNDBUFFER lpSoundBuffer;

	wave.create(fname.c_str());

	DWORD dataLen = wave.getDataLen();
	WAVEFORMATEX waveformat;
	wave.getFormat(waveformat);

	DSBUFFERDESC bufDesc;
	ZeroMemory(&bufDesc, sizeof(DSBUFFERDESC));
	bufDesc.dwSize = sizeof(DSBUFFERDESC);
	bufDesc.dwFlags= DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_STATIC;
	bufDesc.dwBufferBytes = dataLen;
	bufDesc.lpwfxFormat = &waveformat;

	if(lpDS->CreateSoundBuffer(&bufDesc, &lpSoundBuffer, NULL) != DS_OK)
	{
		return NULL;
	}

	BYTE* dsBufData;
	if(lpSoundBuffer->Lock(0, dataLen, (void**)&dsBufData, &dataLen, NULL, 0, 0) != DS_OK)
	{
		return NULL;
	}
	dataLen = wave.getData(dsBufData, dataLen);

	if(lpSoundBuffer->Unlock(dsBufData, dataLen, NULL, 0) != DS_OK)
	{
		return NULL;
	}

	return lpSoundBuffer;
}

LPDIRECTSOUND8 SoundEngine::getDirectSound()
{
	return lpDS;
}
