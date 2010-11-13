#include "ogg-class.h"

using namespace std;

/*!
The constructor does not allocate any resources or load any data. It does not even ensure that the desired OGG file is present.
All this happens instead when calling CDirectSoundOgg::Play().

@param	&filename is the file you wish to stream once calling CDirectSoundOgg::Play()
@param	&pDirecSound is a reference to an already instantiated IDirectSound8 interface
*/
CDirectSoundOgg::CDirectSoundOgg(std::string &filename, LPDIRECTSOUND8 pDirectSound) :
	firstHalfPlaying(true),
	m_pBuffer(NULL),
	m_pPlayMarker(NULL),
	m_hFirstHalfEvent(0),
	m_hSecondHalfEvent(0),
	m_hPlayThread(0),
	m_bPlayThreadActive(false),
	m_strFilename(filename),
	m_hStopPlaybackEvent(0),
	m_bPlaybackDone(false),
	m_pDirectSound(pDirectSound)
{
	//----- create critical section to guard shared memory

	InitializeCriticalSection(&m_criticalSection);
}




/*!
Called internally to set up the DirectSound secondary buffer, check that the OGG file is present, that it contains Vorbis encoded audio and to set
up notification events for streaming the decoded audio into the DirectSound buffer.

CDirectSoundOgg::Play() starts up a play thread that calls this function.
*/
bool CDirectSoundOgg::Allocate() {
	//----- open OGG file

	//get a handle to the file

	FILE *fileHandle = fopen(GetFileName().c_str(), "rb");

	//test to make sure that the file was opened succesfully

	if(!fileHandle) return false;

	//open the file as an OGG file (allocates internal OGG buffers)

	if((ov_open(fileHandle, &m_vorbisFile, NULL, 0)) != 0) {
		//close the file

		fclose(fileHandle);
		//return error

		return false;
	}


	//----- Get OGG info

	//get information about OGG file

	vorbis_info *vorbisInfo = ov_info(&m_vorbisFile, -1);


	//----- setup buffer to hold OGG

	//get a wave format structure, which we will use later to create the DirectSound buffer description

	WAVEFORMATEX	    waveFormat;
	//clear memory

	memset(&waveFormat, 0, sizeof(waveFormat));
	//set up wave format structure

	waveFormat.cbSize			= sizeof(waveFormat);									//how big this structure is

	waveFormat.nChannels		= vorbisInfo->channels;									//how many channelse the OGG contains

	waveFormat.wBitsPerSample	= 16;													//always 16 in OGG

	waveFormat.nSamplesPerSec	= vorbisInfo->rate;										//sampling rate (11 Khz, 22 Khz, 44 KHz, etc.)

    waveFormat.nAvgBytesPerSec	= waveFormat.nSamplesPerSec * waveFormat.nChannels * 2;	//average bytes per second

    waveFormat.nBlockAlign		= 2 * waveFormat.nChannels;								//what block boundaries exist

    waveFormat.wFormatTag		= 1;													//always 1 in OGG



	//----- prepare DirectSound buffer description

	//get a buffer description

	DSBUFFERDESC dsBufferDescription;
	//clear memory

	memset(&dsBufferDescription, 0, sizeof(dsBufferDescription));
	//set up buffer description structure

	dsBufferDescription.dwSize			= sizeof(dsBufferDescription);		//how big this structure is

	dsBufferDescription.lpwfxFormat		= &waveFormat;						//information about the sound that the buffer will contain

	dsBufferDescription.dwBufferBytes	= BUFFER_HALF_SIZE * 2;				//total buffer size = 2 * half size

	dsBufferDescription.dwFlags			= DSBCAPS_CTRLPOSITIONNOTIFY;		//buffer must support notifications



	//----- create sound buffer

	//pointer to old interface, used to obtain pointer to new interface

	LPDIRECTSOUNDBUFFER pTempBuffer = NULL;
	//create old interface

	if(FAILED(m_pDirectSound->CreateSoundBuffer(&dsBufferDescription, &pTempBuffer, NULL))) {
		//return error

		return false;
	}

	//query for updated interface

	if(FAILED(pTempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&m_pBuffer))) {
		//return error

		return false;
	}

	//release old, temp interface

	SAFE_RELEASE(pTempBuffer);


	//----- set up the two half play markers

	//first get notification interface

	if(FAILED(m_pBuffer->QueryInterface(IID_IDirectSoundNotify8, (void**)&m_pPlayMarker))) {
		//return error

		return false;
	}

	//get two notification structures used to set up the play markers

	DSBPOSITIONNOTIFY notificationMarker[2];

	//create notification event objects

	m_hFirstHalfEvent = CreateEvent(NULL, false, false, NULL);
	m_hSecondHalfEvent = CreateEvent(NULL, false, false, NULL);

	//set up position for first event object to be fired

	notificationMarker[0].dwOffset = 0;
	notificationMarker[0].hEventNotify = m_hFirstHalfEvent;

	//set up position for second event object to be fired

	notificationMarker[1].dwOffset = BUFFER_HALF_SIZE;
	notificationMarker[1].hEventNotify = m_hSecondHalfEvent;

	//now actually set the notification points

	if(FAILED(m_pPlayMarker->SetNotificationPositions(2, notificationMarker))) {
		//return error

		return false;
	}

	//----- create notification event object used to signal when playback should stop

	m_hStopPlaybackEvent = CreateEvent(NULL, false, false, NULL);


	//----- Fill first half of buffer with initial data

	Fill(true);


	//----- success

	return true;
}

/*!
If the OGG is playing, it will be stopped and deallocated.
*/
CDirectSoundOgg::~CDirectSoundOgg() {
	if(IsPlaying()) Stop();

	//delete the critical section (safe to do now since the play thread is guaranteed dead)

	DeleteCriticalSection(&m_criticalSection);
}



/*!
Closes the OGG file, deallocates all DirectSound interfaces and closes all event handles.
*/
void CDirectSoundOgg::Cleanup() {
	//close OGG file

	ov_clear(&m_vorbisFile);

	//release player marker

	SAFE_RELEASE(m_pPlayMarker);
	//close first half event handle

	SAFE_CLOSE_HANDLE(m_hFirstHalfEvent);
	//close second half event handle

	SAFE_CLOSE_HANDLE(m_hSecondHalfEvent);

	//close stop playback event handle

	SAFE_CLOSE_HANDLE(m_hStopPlaybackEvent);

	//release DirectSound buffer

	SAFE_RELEASE(m_pBuffer);
}




/*!
Decompresses OGG data into either the first or the second half of the DirectSound secondary buffer. When the second half is entered (played), the
first half will be filled with the upcoming audio. Likewise, when the first half is entered, the second half will be filled with upcoming audio.

@param		firstHalf is true if the function should fill the first half of the buffer, or false if it should fill the second half of the buffer.
@returns	true if the sound was decoded without problems, otherwise false.
*/
bool CDirectSoundOgg::Fill(const bool firstHalf) {
	//----- initial allocations

	LPVOID	pFirstSegment;							//holds a pointer to the first segment that we lock

	DWORD	nFirstSegmentSize = 0;					//holds how big the first segment is

	LPVOID	pSecondSegment;							//holds a pointer to the second segment that we lock

	DWORD	nSecondSegmentSize = 0;					//holds how big the second segment is



    //----- lock DirectSound buffer half

	if(FAILED(m_pBuffer->Lock(
		(firstHalf ? 0 : BUFFER_HALF_SIZE),			//if we are locking the first half, lock from 0, otherwise lock from BUFFER_HALF_SIZE

		BUFFER_HALF_SIZE,							//how big a chunk of the buffer to block

		&pFirstSegment,								//pointer that will receive the locked segment start address

		&nFirstSegmentSize,							//will return how big the first segment is (should always be BUFFER_HALF_SIZE)

		&pSecondSegment,							//pointer that will receive the second locked segment start address (in case of wrapping)

		&nSecondSegmentSize,						//how big a chunk we wrapped with (in case of wrapping)

		0											//flags: no extra settings

		)))
	{
		//return error

		return false;
	}


	//----- debug safety: we should always have locked a complete segment of the size we requested

	assert(nFirstSegmentSize == BUFFER_HALF_SIZE);


	//----- decode OGG file into buffer

	unsigned int nBytesReadSoFar	= 0; //keep track of how many bytes we have read so far

	long nBytesReadThisTime			= 1; //keep track of how many bytes we read per ov_read invokation (1 to ensure that while loop is entered below)

	int nBitStream					= 0; //used to specify logical bitstream 0


	//decode vorbis file into buffer half (continue as long as the buffer hasn't been filled with something (=sound/silence)

	while(nBytesReadSoFar < BUFFER_HALF_SIZE) {
		//decode

		nBytesReadThisTime = ov_read(
			&m_vorbisFile,								//what file to read from

			(char*)pFirstSegment + nBytesReadSoFar,		//where to put the decoded data

			BUFFER_HALF_SIZE - nBytesReadSoFar,			//how much data to read

			0,											//0 specifies little endian decoding mode

			2,											//2 specifies 16-bit samples

			1,											//1 specifies signed data

			&nBitStream
		);

		//new position corresponds to the amount of data we just read

		nBytesReadSoFar += nBytesReadThisTime;


		//----- do special processing if we have reached end of the OGG file

		if(nBytesReadThisTime == 0) {
			//----- if looping we fill start of OGG, otherwise fill with silence

			if(m_bLooping) {
				//seek back to beginning of file

				ov_time_seek(&m_vorbisFile, 0);
			} else {
				//fill with silence

				for(unsigned int i = nBytesReadSoFar; i < BUFFER_HALF_SIZE; i++) {
					//silence = 0 in 16 bit sampled data (which OGG always is)

					*((char*)pFirstSegment + i) = 0;
				}

				//signal that playback is over

				m_bPlaybackDone = true;

				//and exit the reader loop

				nBytesReadSoFar = BUFFER_HALF_SIZE;
			}
		}
	}


	//----- unlock buffer

	m_pBuffer->Unlock(pFirstSegment, nFirstSegmentSize, pSecondSegment, nSecondSegmentSize);

	return true;
}




/*!
@returns true if the OGG is playing, otherwise false.
*/
bool CDirectSoundOgg::IsPlaying() {
	return GetPlayThreadActive();
}



/*!
Stops the OGG from playing. If the OGG was not playing, nothing happens.
*/
void CDirectSoundOgg::Stop() {
	//----- safeguard

	if(!IsPlaying()) return;


	//----- Signal the play thread to stop

	SetEvent(m_hStopPlaybackEvent);

	//wait for playing thread to exit

	if(WaitForSingleObject(m_hPlayThread, PLAY_THREAD_WAIT_PERIOD_MS) == WAIT_ABANDONED) {
		//the thread hasn't terminated as expected. kill it

		TerminateThread(m_hPlayThread, 1);

		//not playing any more

		SetPlayThreadActive(false);

		//since playing thread has not cleaned up, this thread will have to

		Cleanup();

		//TODO: You should report this error here somehow

	}


	//----- store that we are not playing any longer

	SetPlayThreadActive(false);
}



/*!
This function call is rather expensive. Do not use CDirectSoundOgg for short sound clips as a thread is allocated every time
CDirectSoundOgg::Play() is called.

@param looping is true if the sound file should loop (i.e. play until stopped), or otherwise false (i.e. play once)
*/
void CDirectSoundOgg::Play(const bool async, const bool looping) {
	//stop playback if already going on

	if(IsPlaying()) Stop();

	//mark that thread is now active

	SetPlayThreadActive(true);

	//store looping

	m_bLooping = looping;

	//----- Create play thread

	//temp storage for thread ID, which is not needed

	unsigned int nThreadID;
	//create thread and store handle to thread

	m_hPlayThread = (HANDLE)_beginthreadex(NULL, 0, CDirectSoundOgg::PlayingThread, this, 0, &nThreadID);

	if(!async)
	{
		while(IsPlaying())
		{

		}
	}
}




/*!
@returns	a string containing the filename of the OGG that will be played.
*/
std::string CDirectSoundOgg::GetFileName() {
	EnterCriticalSection(&m_criticalSection);
	std::string strFilename = m_strFilename;
	LeaveCriticalSection(&m_criticalSection);

	return strFilename;
}




/*!
This function is static so it can be used as a thread function. When CDirectSoundOgg::Play() is called, this function is started in a new thread.
This thread-function lies dormant most of the time, only reawakening when the sound buffer needs refilling.

This function calls CDirectSoundOgg::Allocate() once and then CDirectSoundOgg::Fill() repeatedly to play the OGG. Finally it calls
CDirectSoundOgg::Cleanup() to deallocate sound buffers and the OGG file itself.

@param		lpParam	contains a pointer to the actual instance of CDirectSoundOgg that started this function (used to gain access to non-static members)
@returns	an unsigned int, which is the return value of the thread (usually 0 if no problem, or 1 if a problem occured)
*/
unsigned int CDirectSoundOgg::PlayingThread(LPVOID lpParam) {
	//cast thread parameter to a CDirectSoundOgg pointer

	CDirectSoundOgg *oggInstance = static_cast<CDirectSoundOgg*>(lpParam);

	//allocate all resources

	if(!oggInstance->Allocate()) {
		oggInstance->Cleanup();
		return 1;
	}

	//----- allocations

	bool bErrorOccured = false;		//assume everything will go ok



	//----- indicate that we are playing

	oggInstance->m_bPlaybackDone = false;


	//----- start playing the buffer (looping because we are going to refill the buffer)

	oggInstance->m_pBuffer->Play(0, 0, DSBPLAY_LOOPING);


	//----- go into loop waiting on notification event objects

	//create tracker of what half we have are playing

	bool bPlayingFirstHalf = true;

	//create array of event objects

	vector<HANDLE> stlvEventObjects;
	//push event objects

	stlvEventObjects.push_back(oggInstance->m_hFirstHalfEvent);			//push event object for reaching first half

	stlvEventObjects.push_back(oggInstance->m_hSecondHalfEvent);		//push event object for reaching second half

	stlvEventObjects.push_back(oggInstance->m_hStopPlaybackEvent);		//push event object used to signal for playback stop


	//keep going in the loop until we need to stop

	bool	bContinuePlaying = true;		//used to keep track of when to stop the while loop

	bool	bPlaybackDoneProcessed = false;	//used ot only process m_bPlaybackDone once

	int		nStopAtNextHalf = 0;			//0 signals "do not stop", 1 signals "stop at first half", 2 signals "stop at second half"


	//enter refill loop

	while(bContinuePlaying && (!bErrorOccured)) {
		switch(WaitForMultipleObjects((DWORD)stlvEventObjects.size(), &(stlvEventObjects[0]), FALSE, INFINITE)) {

			//----- first half was reached

			case WAIT_OBJECT_0:
				//check if we should stop playing back

				if(nStopAtNextHalf == 1) {
					//stop playing

					bContinuePlaying = false;
					//leave and do not fill the next buffer half

					break;
				}

				//fill second half with sound

				if(!(oggInstance->Fill(false))) bErrorOccured = true;

				//if the last fill was the final fill, we should stop next time we reach this half (i.e. finish playing whatever audio we do have)

				if((oggInstance->m_bPlaybackDone) && (!bPlaybackDoneProcessed)) {
					//make the while loop stop after playing the next half of the buffer

					nStopAtNextHalf = 1;
					//indicate that we have already processed the playback done flag

					bPlaybackDoneProcessed = true;
				}

				break;

			//----- second half was reached

			case WAIT_OBJECT_0 + 1:
				//check if we should stop playing back

				if(nStopAtNextHalf == 2) {
					//stop playing

					bContinuePlaying = false;
					//leave and do not fill the next buffer half

					break;
				}

				//fill first half with sound

				if(!(oggInstance->Fill(true))) bErrorOccured = true;

				//if this last fill was the final fill, we should stop next time we reach this half (i.e. finish playing whatever audio we do have)

				if((oggInstance->m_bPlaybackDone) && (!bPlaybackDoneProcessed)) {
					//make the while loop stop after playing the next half of the buffer

					nStopAtNextHalf = 2;
					//indicate that we have already processed the playback done flag

					bPlaybackDoneProcessed = true;
				}

				break;

			//----- stop event has happened

			case WAIT_OBJECT_0 + 2:
				//exit the while loop

				bContinuePlaying = false;

				break;
		}
	}
	//----- stop the buffer from playing

	oggInstance->m_pBuffer->Stop();


	//----- perform cleanup

	oggInstance->Cleanup();


	//----- thread no longer active

	oggInstance->SetPlayThreadActive(false);


	//----- done

	return (bErrorOccured ? 1 : 0);

}



/*!
@returns	true if the playing thread is currently running, otherwise false
*/
bool CDirectSoundOgg::GetPlayThreadActive() {
	EnterCriticalSection(&m_criticalSection);
	bool bActive = m_bPlayThreadActive;
	LeaveCriticalSection(&m_criticalSection);

	return bActive;
}



/*!
@param	bActive is true if you want CDirectSoundOgg::GetPlayThreadActive() to return true, or false if you want it to return false
*/
void CDirectSoundOgg::SetPlayThreadActive(bool bActive) {
	EnterCriticalSection(&m_criticalSection);
	m_bPlayThreadActive = bActive;
	LeaveCriticalSection(&m_criticalSection);
}
