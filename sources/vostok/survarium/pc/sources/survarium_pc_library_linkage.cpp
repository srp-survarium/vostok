////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#ifdef OPENGL
#	include <vostok/engine/pc/opengl/library_linkage.h>
#else // #ifdef OPENGL
#	include <vostok/engine/pc/dx11/library_linkage.h>
#endif // #ifdef OPENGL

#include <vostok/game/library_linkage.h>
#include <vostok/game_core/library_linkage.h>

#include <vostok/network_core/library_linkage.h>

#include <vostok/physics/library_linkage.h>

#ifdef VOSTOK_STATIC_LIBRARIES
#	include <vostok/engine/library_linkage.h>
#	include <vostok/engine/library_dependencies.h>

#	include <ogg/library_linkage.h>
#	include <vorbis/library_linkage.h>
#	pragma comment( lib, "x3daudio.lib" )
#	include <ode/library_linkage.h>
#	include <vostok/render/engine/library_linkage.h>

#	include <vostok/linkage_helper.h>
	VOSTOK_INCLUDE_TO_LINKAGE(sound_library_linkage)
#endif // #ifdef VOSTOK_STATIC_LIBRARIES
// Resolve the zlib inflate half from zlibN.lib (1.2.3) BEFORE the Scaleform
// prebuilt's defaultlib can satisfy it with its bundled 1.2.7 copy: this TU is
// an explicit link object, so its undefineds hit the command-line lib list
// first. The retail exe carries inflate 1.2.3 (the game zlib), not 1.2.7.
extern "C" int inflate( void* strm, int flush );
extern "C" int inflateInit2_( void* strm, int windowBits, char const* version, int stream_size );
extern "C" unsigned long adler32( unsigned long adler, unsigned char const* buf, unsigned int len );

// a namespace-scope object's dynamic initializer is a /OPT:REF root (.CRT$XCU)
static struct zlib_inflate_pin {
	zlib_inflate_pin( )
	{
		static void* volatile s_zlib_pin = 0;
		s_zlib_pin = ( void* )&inflate;
		s_zlib_pin = ( void* )&inflateInit2_;
		s_zlib_pin = ( void* )&adler32;
	}
} s_zlib_inflate_pin;
