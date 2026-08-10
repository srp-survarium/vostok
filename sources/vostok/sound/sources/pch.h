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

#	include <xaudio2.h>
#	undef XAUDIO2_DEFAULT_SAMPLERATE
#	define XAUDIO2_DEFAULT_SAMPLERATE 44100
#	include <x3daudio.h>
#	include <xapo.h>
#	include <xapobase.h>
#	include <xapofx.h>

#include <vorbis/vorbisfile.h>
#include <vorbis/vorbisenc.h>

#include "sound_memory.h"

#endif // #ifndef PCH_H_INCLUDED
