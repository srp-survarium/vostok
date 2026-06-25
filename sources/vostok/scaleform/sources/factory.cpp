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

// claude@NOTE: the remaining STUBs in this TU are parked on cross-module engine
// symbols / heavy SDK reconstruction, not reachability (the scaleform anchor pins
// every public method now):
//  - scaleform_engine::initialize references vostok::engine::scaleform_engine_alloc/
//    scaleform_engine_free/scaleform_log_output and the global g_log_output_ptr, none
//    of which are reconstructed in our sources yet (engine module owns them); cannot
//    even link extern refs until the engine TU defines them.
//  - xrSysAllocMalloc::Alloc/Free/Realloc are the X-Ray-era aligned-allocator math
//    over the m_mem_alloc_ptr/m_mem_free_ptr members (those members are populated by
//    the initialize() static-ctor path that depends on the engine symbols above).
//  - vostok_scaleform_log::LogMessageVarg forwards to Scaleform::Log::FormatLog then
//    dispatches by message-class to the engine global g_log_output_ptr (engine symbol).
//  - vostok_file_opener::OpenFile builds a Scaleform::MemoryFile from a member buffer
//    or forwards to FileOpener::OpenFile (member-state-dependent, low priority).
//  - flash_factory::flash_factory (18 stmts) / build_movie (14 stmts) construct the
//    GFx Loader state cone (ZlibSupport/FileOpener/FontProviderWin32/AS2Support/
//    AS3Support/ImageFileHandlerRegistry) and Loader::CreateMovie - heavy SDK glue
//    that also touches engine globals (0xA5C694 buffer-context pair).

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
