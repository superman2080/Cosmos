#include "Sound.h"
#include "DXUT.h"
#include "DXUTenum.h"
#include "DXUTmisc.h"
#include "SDKwavefile.h"
#include <xaudio2.h>
#include <shellapi.h>
#include <mmsystem.h>
#include <conio.h>

extern GAME_STATE g_GameState;

IXAudio2* pXAudio = NULL;

IXAudio2MasteringVoice* pMasterVoice = NULL;

IXAudio2SourceVoice* pSourceVoice = NULL;

XAUDIO2_BUFFER buffer = { 0, };

CWaveFile wav;

VOID SoundInit()
{
	XAudio2Create(&pXAudio, 0, XAUDIO2_DEFAULT_PROCESSOR);
	pXAudio->CreateMasteringVoice(&pMasterVoice);

	wav.Open(L"\Resource/Electro_1.wav", NULL, WAVEFILE_READ);
	PWAVEFORMATEX pwfx = wav.GetFormat();
	DWORD cbSize = wav.GetSize();

	BYTE* wavByte = new BYTE[cbSize];
	wav.Read(wavByte, cbSize, &cbSize);

	
	pXAudio->CreateSourceVoice(&pSourceVoice, pwfx);

	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.pAudioData = wavByte;
	buffer.AudioBytes = cbSize;
	buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

	pSourceVoice->SubmitSourceBuffer(&buffer);

	pSourceVoice->SetVolume(0.5f);
}

VOID SoundUpdate()
{
	switch (g_GameState)
	{
	case LOADING:
		break;
	case READY:
		pSourceVoice->Stop(0);
		break;
	case STAGE1:
		pSourceVoice->Start(0);
		break;
	case STAGE2:
		break;
	default:
		break;
	}
}

VOID SoundRelease()
{
	if (pMasterVoice != NULL)
		pMasterVoice->DestroyVoice();

	if (pSourceVoice != NULL)
		pSourceVoice->DestroyVoice();

	if (pXAudio != NULL)
		pXAudio->Release();
}
