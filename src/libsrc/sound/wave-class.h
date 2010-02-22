#ifndef WAVE_CLASS_H
#define WAVE_CLASS_H

#include "basedefs.h"
#include <fstream>
#include <windows.h>
#include <MMSystem.h>
#include "mmio.h"
#include <dsound.h>

using namespace std;
class EXPORT WaveClass
{
public:
	WaveClass();
	WaveClass(const char* fileName);
	WaveClass(UINT uiResID, HMODULE hmod);
	virtual ~WaveClass();

	Logical getFormat(WAVEFORMATEX& format) const;
	Logical create(const char* fileName);
	Logical create(UINT uiResID, HMODULE hmod);
	Logical isValid() const;
	Logical play(Logical async = yes, Logical looped = no) const;
	DWORD getDataLen() const;
	DWORD getData(BYTE*& waveData, DWORD dwMaxToCopy) const;
	Logical free();

private:
	BYTE* imageData;
	DWORD imageLen;
	Logical resource;
};
#endif
