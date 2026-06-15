////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "flash_factory.h"
#include "flash_movie.h"
#include "flash_text_manager.h"
#include "scaleform_engine.h"
#include "scaleform_game_engine.h"
#include "scaleform_render_command_queue.h"
#include "vostok_file_opener.h"
#include "vostok_scaleform_log.h"

#include "GFx.h"

namespace survarium {

// the rich index shows dynamic initializers + atexit destructors for both
// (survarium::`dynamic initializer for 'g_file_opener'' etc.)
static vostok_file_opener		g_file_opener;
static vostok_scaleform_log		g_vostok_logger;

// STATE[STUB]
void* scaleform_engine::xrSysAllocMalloc::Alloc( u32 size, u32 align )
{
	// FUNCTION BODY[0x0ae120]
	VOSTOK_UNREFERENCED_PARAMETERS	( size, align );
	return NULL;
}

// STATE[STUB]
void scaleform_engine::xrSysAllocMalloc::Free( void* ptr, u32 size, u32 align )
{
	// FUNCTION BODY[0x0ae160]
	VOSTOK_UNREFERENCED_PARAMETERS	( ptr, size, align );
}

// STATE[STUB]
void* scaleform_engine::xrSysAllocMalloc::Realloc( void* old_ptr, u32 old_size, u32 new_size, u32 align )
{
	// FUNCTION BODY[0x0ae180]
	VOSTOK_UNREFERENCED_PARAMETERS	( old_ptr, old_size, new_size, align );
	return NULL;
}

// STATE[STUB]
Scaleform::File* vostok_file_opener::OpenFile( pcstr url, s32 flags, s32 mode )
{
	// FUNCTION BODY[0x0ae1e0]
	VOSTOK_UNREFERENCED_PARAMETERS	( url, flags, mode );
	return NULL;
}

// STATE[STUB]
void vostok_scaleform_log::LogMessageVarg( Scaleform::LogMessageId message_id, pcstr fmt, va_list args )
{
	// FUNCTION BODY[0x0ae240]
	VOSTOK_UNREFERENCED_PARAMETERS	( message_id, fmt, args );
}

// STATE[STUB]
void flash_factory::tick( )
{
	// FUNCTION BODY[0x5bb9c0]
}

// STATE[STUB]
void flash_factory::destroy_movie( flash_movie* movie )
{
	// FUNCTION BODY[0x5bb9d0]
	VOSTOK_UNREFERENCED_PARAMETER	( movie );
}

// STATE[STUB]
flash_factory::~flash_factory( )
{
	// FUNCTION BODY[0x5bb9f0]
}

// STATE[STUB]
void scaleform_engine::destroy( )
{
	// FUNCTION BODY[0x5bba30]
}

// STATE[STUB]
flash_movie* flash_factory::build_movie( void* buffer, u32 buffer_size, pcstr file_name )
{
	// FUNCTION BODY[0x5bba40]
	VOSTOK_UNREFERENCED_PARAMETERS	( buffer, buffer_size, file_name );
	return NULL;
}

// STATE[STUB]
void scaleform_engine::initialize(
		void* ( *alloc )( u32 ),
		void ( *free )( void* ),
		void ( *log )( u8, pcstr )
	)
{
	// FUNCTION BODY[0x5bbbb0]
	// the rich index shows a `dynamic atexit destructor for 'scaleform_alloc''
	// inside this function - a local `static xrSysAllocMalloc scaleform_alloc`
	VOSTOK_UNREFERENCED_PARAMETERS	( alloc, free, log );
}

// STATE[STUB]
flash_text_manager* flash_factory::create_text_manager( )
{
	// FUNCTION BODY[0x5bbc10]
	return NULL;
}

// STATE[STUB]
flash_factory::flash_factory( scaleform_game_engine& engine )
{
	// FUNCTION BODY[0x5bbc40]
	VOSTOK_UNREFERENCED_PARAMETER	( engine );
}

} // namespace survarium
