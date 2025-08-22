////////////////////////////////////////////////////////////////////////////
//	Created 	: 22.12.2009
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef PCH_H_INCLUDED
#define PCH_H_INCLUDED

#define VOSTOK_SOUND_BUILDING

#ifndef VOSTOK_STATIC_LIBRARIES
#	define VOSTOK_ENGINE_BUILDING
#endif // #ifndef VOSTOK_STATIC_LIBRARIES

#define VOSTOK_LOG_MODULE_INITIATOR	"sound"
#include <vostok/extensions.h>

#include <vostok/os_preinclude.h>
#undef NOUSER
#undef NOMSG
#include <vostok/os_include.h>

#ifndef SN_TARGET_PS3
#	include <xaudio2.h>
#	include <x3daudio.h>
#	include <xapo.h>
#	include <xapobase.h>
#	include <xapofx.h>
#else // #ifndef SN_TARGET_PS3
	struct IXAudio2SourceVoice;
	struct IXAudio2;
	struct IXAudio2MasteringVoice;
	struct IXAudio2SubmixVoice;
	struct CXAPOBase { };
	struct IXAudio2VoiceCallback { };
	struct WAVEFORMATEX { };
	struct X3DAUDIO_HANDLE { };
	struct X3DAUDIO_LISTENER { };
	struct X3DAUDIO_DISTANCE_CURVE { };
	struct X3DAUDIO_DISTANCE_CURVE_POINT { };
	struct XAUDIO2_VOICE_STATE { };
	typedef u32 UINT32;
	typedef long HRESULT;
#endif // #ifndef SN_TARGET_PS3

#include <vorbis/vorbisfile.h>
#include <vorbis/vorbisenc.h>

#include "sound_memory.h"

#endif // #ifndef PCH_H_INCLUDED
