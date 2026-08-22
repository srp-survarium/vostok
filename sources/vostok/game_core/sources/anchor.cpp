#include "pch.h"
#include "anchor.h"

// keep the zlib inflate half alive under /OPT:REF - its real caller (the
// zlib-backed compressor/unpack path) is not reconstructed yet. inflate pulls
// inflate_fast/inflate_table/updatewindow/adler32 with it.
extern "C" int inflate( void* strm, int flush );
extern "C" int inflateInit2_( void* strm, int windowBits, char const* version, int stream_size );
extern "C" unsigned long adler32( unsigned long adler, unsigned char const* buf, unsigned int len );

namespace survarium
{

IncludeAll::IncludeAll()
{
	vostok::anchor_game_core( );
	vostok::anchor_physics( );
	vostok::anchor_collision( );
	vostok::anchor_ai_navigation( );
	vostok::anchor_game( );
	vostok::anchor_scaleform( );
	vostok::render::anchor_render_facade( );

	// runtime volatile stores - a statically initialized pin array gets its
	// data section (and the relocs keeping these alive) stripped by /OPT:REF
	static void* volatile s_zlib_pin = 0;
	s_zlib_pin = ( void* )&inflate;
	s_zlib_pin = ( void* )&inflateInit2_;
	s_zlib_pin = ( void* )&adler32;
}


}
