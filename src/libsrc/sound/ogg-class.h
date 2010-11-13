#ifndef OGG_CLASS
#define OGG_CLASS
#pragma once
#include	<string>						//for std::string
#include	<vector>						//for std::vecto
#include	<windows.h>						//for critical section/threading support
#include	<process.h>						//for threading support
#include	<assert.h>						//for assertion support
#include	<dsound.h>						//DirectSound headers
#include    <vorbis/codec.h>				//for OGG vorbis codec
#include    <vorbis/vorbisfile.h>			//for OGG vorbis file information


//macros

#define SAFE_DELETE(p)			{ if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p)			{ if(p) { (p)->Release(); (p)=NULL; } }
#define SAFE_CLOSE_HANDLE(h)	{ if(h) { CloseHandle(h); (h)=0; } }

//!Holds how large each half of the DirectSound buffer is

const	int BUFFER_HALF_SIZE = 15 * 1024;			//15 kb, which is roughly one second of OGG sound


//!Holds how long the main thread should wait for the player thread to shut down (ms)

const	int PLAY_THREAD_WAIT_PERIOD_MS = 500;

/*!
\brief Class that can stream and decompress an Ogg Vorbis file into a memory buffer and then play that buffer.

Use this class to stream long sound clips using little memory. To compress a wave file into Ogg Vorbis format, download a free encoder from
http://www.vorbis.org

CDirectSoundOgg creates a thread per object instance being played. Although the threads lie dormant 99.99% of the time while
streaming, they do consume resources. Accordingly, you should take care as to how many CDirectSoundOgg instances you have playing at the
same time. This class has some latency before playing, as it need to set up a fair bit of details before playback can commence.

If you call CDirectSoundOgg::Play() while the OGG is already playing, the OGG will stop and start from the beginning.

\note A thread is only active when the instance is being played. It will not consume resources having many CDirectSoundOgg instances that are
not being played.

\note When playing on a 2.8 GHz HT Pentium 4, the play thread run by this class consumes roughly 1% CPU.

This class is covered by the following license:

=== LICENSE BEGIN ===

© 2004, Bjørn Toft Madsen, http://www.sunbeam60.net
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
* The name Bjørn Toft Madsen or the website http://www.sunbeam60.net may not be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=== LICENSE END ===

This class uses Xiph.Org Foundation software, covered under the license below.

=== LICENSE BEGIN ===

© 2004, Xiph.Org Foundation

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
* Neither the name of the Xiph.org Foundation nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

This software is provided by the copyright holders and contributors “as is” and any express or implied warranties, including, but not limited to, the implied warranties of merchantability and fitness for a particular purpose are disclaimed. In no event shall the foundation or contributors be liable for any direct, indirect, incidental, special, exemplary, or consequential damages (including, but not limited to, procurement of substitute goods or services; loss of use, data, or profits; or business interruption) however caused and on any theory of liability, whether in contract, strict liability, or tort (including negligence or otherwise) arising in any way out of the use of this software, even if advised of the possibility of such damage.

=== LICENSE END===
*/

#include "vorbis/vorbisfile.h"

class EXPORT CDirectSoundOgg {
	private:
		//!Holds the filename to play

		std::string m_strFilename;

		//!Holds if the file should loop (i.e. play until stopped)

		bool m_bLooping;

		//!Holds if playback is done (only interesting if we aren't looping)

		bool m_bPlaybackDone;

		//!True if the play thread is active

		bool m_bPlayThreadActive;

		//!Holds a pointer to the DirectSound pointer

		LPDIRECTSOUND8 m_pDirectSound;
	protected:
		//!Returns if the play thread is active

		bool GetPlayThreadActive();
		//!Set if the play thread is active

		void SetPlayThreadActive(bool bActive);

		//!Holds a handle to a OGG Vorbis file

		OggVorbis_File m_vorbisFile;

		//!Holds a DirectSound buffer

		LPDIRECTSOUNDBUFFER8 m_pBuffer;

		//!Pointer to critical section that protects shared memory

		CRITICAL_SECTION m_criticalSection;

		//!A handle for the play thread

		HANDLE m_hPlayThread;

		//!True if the first half of the buffer is playing

		bool firstHalfPlaying;

		//!Holds a pointer to a marker that will be used to detect when a point is reached in the play buffer

		LPDIRECTSOUNDNOTIFY8 m_pPlayMarker;
		//!Holds a handle for the event that happens when first half of play buffer is reached

		HANDLE m_hFirstHalfEvent;
		//!Holds a handle for the event that happens when second half of play buffer is reached

		HANDLE m_hSecondHalfEvent;

		//!Holds a handle to an event object used to signal that playback should stop

		HANDLE m_hStopPlaybackEvent;

		//!Allocates all internal structures and prefills data into first buffer half

		bool Allocate();

		//!Handles all internal cleanup

		void Cleanup();

		//!Fills one of the two soundbuffer halves with decoded data

		bool Fill(const bool firstHalf);

		//!This is the playing thread that is launched when Play() is called.

		static unsigned int WINAPI PlayingThread(LPVOID lpParam);

	public:
		//!Default constructor that takes an OGG file to load

		CDirectSoundOgg(std::string &filename, LPDIRECTSOUND8 pDirectSound);

		//!Default destructor

		virtual ~CDirectSoundOgg();

		//!Returns the file to play

		std::string GetFileName();

		//!Returns true if the OGG is playing, otherwise false

		bool IsPlaying();

		//!Stops the OGG

		void Stop();

		//!Starts streaming the OGG

		void Play(const bool async = true, const bool looping = false);
};
#endif
