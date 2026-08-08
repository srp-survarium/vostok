////////////////////////////////////////////////////////////////////////////
//	Created 	: 15.03.2012
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "engine_world.h"

#include <vostok/scaleform/sources/scaleform_engine.h>

#pragma comment( lib, "libgfx.lib" )
#pragma comment( lib, "libgfx_zlib.lib" )
#pragma comment( lib, "libgfx_libpng.lib" )
#pragma comment( lib, "libgfx_as2.lib" )
#pragma comment( lib, "libgfx_as3.lib" )
#pragma comment( lib, "libgfx_libjpeg.lib" )
#pragma comment( lib, "libgfxexpat.lib" )
#pragma comment( lib, "pcre.lib" )

#undef VOSTOK_LOG_MODULE_INITIATOR
#define VOSTOK_LOG_MODULE_INITIATOR "scaleform"

namespace vostok {
namespace engine {

static void* scaleform_engine_alloc( u32 size )
{
	return MT_MALLOC( size, "scaleform" );
}

static void scaleform_engine_free( void* ptr )
{
	MT_FREE( ptr );
}

static void scaleform_log_output( u8 message_type, pcstr fmt )
{
	fixed_string2048 buff;
	buff.append( fmt );

	switch ( message_type )
	{
		case 1:
			LOG_INFO( "%s", buff );
			break;
		case 2:
			LOG_WARNING( "%s", buff );
			break;
		case 3:
			LOG_ERROR( "%s", buff );
			break;
	}
}

void engine_world::initialize_scaleform( )
{
	survarium::scaleform_engine::initialize(
		&scaleform_engine_alloc,
		&scaleform_engine_free,
		&scaleform_log_output
	);
}

void engine_world::destroy_scaleform( )
{
	survarium::scaleform_engine::destroy( );
}

} // namespace engine
} // namespace vostok
