////////////////////////////////////////////////////////////////////////////
//	Created		: 09.06.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SOUND_WORLD_PARAMETERS_H_INCLUDED
#define SOUND_WORLD_PARAMETERS_H_INCLUDED

namespace vostok {
namespace sound {

static vostok::command_line::key	s_debug_audio				( "debug_audio", "", "sound engine", "running debug XAudio2 engine" );
static vostok::command_line::key	s_speed_of_sound			( "speed_of_sound", "", "sound engine", "speed of sound" );

// voices
#define VOSTOK_MONO_VOICES_COUNT		64
#define VOSTOK_STEREO_VOICES_COUNT	64

static vostok::command_line::key	s_xaudio_mono_voices_count	( "xaudio_mono_voices_count", "xaudio_mono_voices", "sound engine", "count of mono xaudio voices in voice_factory, default is " VOSTOK_MAKE_STRING( VOSTOK_MONO_VOICES_COUNT ) "." );
static vostok::command_line::key	s_xaudio_stereo_voices_count( "xaudio_stereo_voices_count", "xaudio_stereo_voices", "sound engine", "count of stereo xaudio voices in voice_factory, default is " VOSTOK_MAKE_STRING( VOSTOK_STEREO_VOICES_COUNT ) "." );

// no default-value enum here in retail (its record lacks the values) -
// the defaults reach sound_world.cpp as the macros themselves.

// propagators
#define VOSTOK_PROPAGATORS_COUNT 64

static vostok::command_line::key	s_propagators_count	( "propagators_count", "", "sound engine", "count of sound_instance_propagator's, default is " VOSTOK_MAKE_STRING( VOSTOK_PROPAGATORS_COUNT ) "." );

enum
{
	propagators_count_default_value		= VOSTOK_PROPAGATORS_COUNT,
};

#undef VOSTOK_PROPAGATORS_COUNT

} // namespace sound
} // namespace vostok

#endif // #ifndef SOUND_WORLD_PARAMETERS_H_INCLUDED
