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
static void ( *g_log_output_ptr )( u8, pcstr );

void* scaleform_engine::xrSysAllocMalloc::Alloc( u32 size, u32 align )
{
	u8*	raw		= ( u8* )m_mem_alloc_ptr( size + align );
	u8*	aligned	= NULL;
	if ( raw )
	{
		aligned	= ( u8* )( ( ( u32 )raw + align - 1 ) & ~( align - 1 ) );
		if ( aligned == raw )
			aligned	+= align;

		( ( u32* )aligned )[ -1 ]	= aligned - raw;
	}
	return aligned;
}

void scaleform_engine::xrSysAllocMalloc::Free( void* ptr, u32 size, u32 align )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( size, align );
	// the aligned block stores the byte distance back to the raw allocation in the
	// u32 just before it; recover the raw pointer and hand it to the X-Ray free hook.
	m_mem_free_ptr( ( u8* )ptr - ( ( u32* )ptr )[ -1 ] );
}

void* scaleform_engine::xrSysAllocMalloc::Realloc( void* old_ptr, u32 old_size, u32 new_size, u32 align )
{
	void*	new_ptr	= Alloc( new_size, align );
	if ( new_ptr )
	{
		memcpy( new_ptr, old_ptr, new_size < old_size ? new_size : old_size );
		Free( old_ptr, old_size, align );
	}
	return new_ptr;
}

Scaleform::File* vostok_file_opener::OpenFile( pcstr url, s32 flags, s32 mode )
{
	if ( cached_file.raw_data )
		return SF_NEW Scaleform::MemoryFile( url, ( const Scaleform::UByte* )cached_file.raw_data, cached_file.raw_data_size );

	return Scaleform::GFx::FileOpener::OpenFile( url, flags, mode );
}

// STATE[STUB]
void vostok_scaleform_log::LogMessageVarg( Scaleform::LogMessageId message_id, pcstr fmt, va_list args )
{
	// FUNCTION BODY[0x0ae240]
	VOSTOK_UNREFERENCED_PARAMETERS	( message_id, fmt, args );
}

void flash_factory::tick( )
{
}

void flash_factory::destroy_movie( flash_movie* movie )
{
	delete movie;
}

flash_factory::~flash_factory( )
{
	delete m_gfx_loader;
	delete m_render_thread_queue;
}

void scaleform_engine::destroy( )
{
	Scaleform::GFx::System::Destroy( );
}

// STATE[STUB]
flash_movie* flash_factory::build_movie( void* buffer, u32 buffer_size, pcstr file_name )
{
	// FUNCTION BODY[0x5bba40]
	VOSTOK_UNREFERENCED_PARAMETERS	( buffer, buffer_size, file_name );
	return NULL;
}

void scaleform_engine::initialize(
		void* ( *alloc )( u32 ),
		void ( *free )( void* ),
		void ( *log )( u8, pcstr )
	)
{
	static xrSysAllocMalloc scaleform_alloc( alloc, free );
	Scaleform::GFx::System::Init( &scaleform_alloc );
	g_log_output_ptr = log;
}

flash_text_manager* flash_factory::create_text_manager( )
{
	return new flash_text_manager( m_gfx_loader );
}

// STATE[STUB]
flash_factory::flash_factory( scaleform_game_engine& engine )
{
	// FUNCTION BODY[0x5bbc40]
	VOSTOK_UNREFERENCED_PARAMETER	( engine );
}

} // namespace survarium
