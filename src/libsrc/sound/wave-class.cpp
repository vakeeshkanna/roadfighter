#include "wave-class.h"

WaveClass::WaveClass(): imageLen(0), resource(no), imageData(NULL)
{

}

WaveClass::WaveClass(const char* fileName) : imageLen(0), resource(no), imageData(NULL)
{
	create(fileName);
}

WaveClass::WaveClass(UINT uiResID, HMODULE hmod) : imageLen(0), resource(yes), imageData(NULL)
{
	create(uiResID, hmod);
}

WaveClass::~WaveClass()
{
	free();
}

Logical WaveClass::create(const char* fileName)
{
	free();
	resource = no;

	FILE *sndFile = fopen(fileName, "r");
	fseek(sndFile, 0, SEEK_END);
	imageLen = ftell(sndFile);
	fseek(sndFile, 0, SEEK_SET);
	imageData = new BYTE[imageLen];
	if(!imageData)
	{
		return no;
	}

	fread(imageData, imageLen, 1, sndFile);
	fclose(sndFile);

	return yes;
}

Logical WaveClass::create(UINT uiResID, HMODULE hmod)
{
	free();

	resource = yes;
	HRSRC hresinfo;
	hresinfo = FindResource(hmod, MAKEINTRESOURCE(uiResID), "Wave");

	if(!hresinfo)
	{
		return no;
	}

	HGLOBAL memwave = LoadResource(hmod, hresinfo);

	if(memwave)
	{
		imageData = (BYTE*) LockResource(memwave);
		imageLen = SizeofResource(hmod, hresinfo);
	}

	return (imageData ? yes : no);
}

Logical WaveClass::isValid() const
{
	return imageData ? yes : no;
}

Logical WaveClass::free()
{
	if(imageData)
	{
		if(resource)
		{
			HGLOBAL memWave = GlobalHandle(imageData);

			if(memWave)
			{
				FreeResource(memWave);
			}
		}
		else
		{
			delete []imageData;
		}

		imageData = NULL;
		imageLen = 0;
		return yes;
	}
	return no;
}

Logical WaveClass::play(Logical async, Logical looped) const
{
	if(!isValid())
	{
		return no;
	}

	return PlaySound((LPCSTR)imageData, NULL, SND_MEMORY | SND_NODEFAULT | (async ? SND_ASYNC :  SND_SYNC) | (looped ? (SND_LOOP | SND_ASYNC) : 0));
}

Logical WaveClass::getFormat(WAVEFORMATEX& format) const
{
	if(!isValid())
	{
		return no;
	}

	CMMMemoryIOInfo mmioInfo((HPSTR) imageData, imageLen);
	CMMIO mmio(mmioInfo);

	CMMTypeChunk mmckParent('W', 'A', 'V', 'E');
	mmio.Descend(mmckParent, MMIO_FINDRIFF);

	CMMIdChunk mmckSubChunk('f', 'm', 't', ' ');

	mmio.Descend(mmckSubChunk, mmckParent, MMIO_FINDCHUNK);
	mmio.Read((HPSTR)&format, sizeof(WAVEFORMATEX));
	mmio.Ascend(mmckSubChunk);

	return yes;
}

DWORD WaveClass::getDataLen() const
{
	if(!isValid())
	{
		return (DWORD)0;
	}

	CMMMemoryIOInfo mmioInfo((HPSTR) imageData, imageLen);
	CMMIO mmio(mmioInfo);

	CMMTypeChunk mmckParent('W', 'A', 'V', 'E');
	mmio.Descend(mmckParent, MMIO_FINDRIFF);

	CMMIdChunk mmckSubChunk('d', 'a', 't', 'a');

	mmio.Descend(mmckSubChunk, mmckParent, MMIO_FINDCHUNK);
	return mmckSubChunk.cksize;
}

DWORD WaveClass::getData(BYTE*& waveData, DWORD maxLen) const
{
	if(!isValid())
	{
		return (DWORD)0;
	}

	CMMMemoryIOInfo mmioInfo((HPSTR) imageData, imageLen);
	CMMIO mmio(mmioInfo);

	CMMTypeChunk mmckParent('W', 'A', 'V', 'E');
	mmio.Descend(mmckParent, MMIO_FINDRIFF);

	CMMIdChunk mmckSubChunk('d', 'a', 't', 'a');
	mmio.Descend(mmckSubChunk, mmckParent, MMIO_FINDCHUNK);

	DWORD lenToCopy = mmckSubChunk.cksize;

	if(waveData == NULL)
	{
		waveData = (BYTE*)GlobalLock(GlobalAlloc(GMEM_MOVEABLE, lenToCopy));
	}
	else if(maxLen < lenToCopy)
	{
		lenToCopy = maxLen;
	}

	if(waveData)
	{
		mmio.Read((HPSTR)waveData, lenToCopy);
	}

	return lenToCopy;
}
