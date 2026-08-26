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
#include "scaleform_render_command_queue_impl.h"
#include "vostok_file_opener.h"
#include "vostok_scaleform_log.h"

#include "GFx.h"
#include "GFx/AS3/AS3_Global.h"
#include "GFx_FontProvider_Win32.h"
#include "GFx_Render.h"

namespace survarium {

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

void* scaleform_engine::xrSysAllocMalloc::Realloc( void* oldPtr, u32 oldSize, u32 newSize, u32 align )
{
	void*	new_ptr	= Alloc( newSize, align );
	if ( new_ptr )
	{
		memcpy( new_ptr, oldPtr, newSize < oldSize ? newSize : oldSize );
		Free( oldPtr, oldSize, align );
	}
	return new_ptr;
}

Scaleform::File* vostok_file_opener::OpenFile( pcstr purl, s32 flags, s32 mode )
{
	if ( cached_file.raw_data )
		return SF_NEW Scaleform::MemoryFile( purl, ( const Scaleform::UByte* )cached_file.raw_data, cached_file.raw_data_size );

	return Scaleform::GFx::FileOpener::OpenFile( purl, flags, mode );
}

void vostok_scaleform_log::LogMessageVarg( Scaleform::LogMessageId message_id, pcstr fmt, va_list args )
{
	Scaleform::LogMessageType const scaleform_message_type = message_id.GetMessageType( );
	char buffer[ Scaleform::Log::MaxLogBufferMessageSize ];
	Scaleform::Log::FormatLog( buffer, sizeof( buffer ), message_id, fmt, args );

	if ( scaleform_message_type == Scaleform::LogMessage_Text )
	{
		g_log_output_ptr( 1, buffer );
		return;
	}

	if ( scaleform_message_type == Scaleform::LogMessage_Warning )
	{
		g_log_output_ptr( 2, buffer );
		return;
	}

	if ( scaleform_message_type == Scaleform::LogMessage_Error )
	{
		g_log_output_ptr( 3, buffer );
		return;
	}

	g_log_output_ptr( 1, buffer );
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

flash_movie* flash_factory::build_movie( void* raw_data, u32 raw_data_size, pcstr movie_name )
{
	static bool created_image_creator;

	if ( !created_image_creator )
	{
		created_image_creator	= true;
		Scaleform::Ptr<Scaleform::GFx::ImageCreator>	image_creator	=
			*SF_NEW Scaleform::GFx::ImageCreator( m_render_thread_queue->impl->pHAL->GetTextureManager( ) );
		m_gfx_loader->SetImageCreator( image_creator );
	}

	g_file_opener.cached_file.raw_data		= raw_data;
	g_file_opener.cached_file.raw_data_size	= raw_data_size;

	flash_movie*	movie	= new flash_movie;
	movie->m_movie_def	= m_gfx_loader->CreateMovie( movie_name, 0, 0 );

	g_file_opener.cached_file.raw_data		= NULL;
	g_file_opener.cached_file.raw_data_size	= 0;

	movie->m_movie	= movie->m_movie_def->CreateInstance(
		Scaleform::GFx::MemoryParams( ),
		true,
		NULL,
		m_render_thread_queue->impl
	);
	movie->m_movie->SetUserData( movie );
	movie->m_movie->SetMouseCursorCount( 1 );
	movie->m_movie->SetControllerCount( 1 );
	movie->m_handle	= &movie->m_movie->GetDisplayHandle( );

	return movie;
}

void scaleform_engine::initialize(
		void* ( *alloc_function )( u32 ),
		void ( *free_function )( void* ),
		void ( *log_function )( u8, pcstr )
	)
{
	static xrSysAllocMalloc scaleform_alloc( alloc_function, free_function );
	Scaleform::GFx::System::Init( &scaleform_alloc );
	g_log_output_ptr = log_function;
}

flash_text_manager* flash_factory::create_text_manager( )
{
	return new flash_text_manager( m_gfx_loader );
}

flash_factory::flash_factory( scaleform_game_engine& game_engine )
{
	m_gfx_loader	= new Scaleform::GFx::Loader;
	m_gfx_loader->SetFileOpener( &g_file_opener );
	m_gfx_loader->SetLog( &g_vostok_logger );

	Scaleform::Ptr<Scaleform::GFx::FontProviderWin32>	font_provider	= *SF_NEW Scaleform::GFx::FontProviderWin32( GetDC( NULL ) );
	m_gfx_loader->SetFontProvider( font_provider );

	Scaleform::Ptr<Scaleform::GFx::ASSupport>	as2_support	= *SF_NEW Scaleform::GFx::AS2Support;
	m_gfx_loader->SetAS2Support( as2_support );

	Scaleform::Ptr<Scaleform::GFx::ASSupport>	as3_support	= *SF_NEW Scaleform::GFx::AS3Support;
	m_gfx_loader->SetAS3Support( as3_support );

	Scaleform::Ptr<Scaleform::GFx::ImageFileHandlerRegistry>	image_handlers	= *SF_NEW Scaleform::GFx::ImageFileHandlerRegistry;
	image_handlers->AddHandler( &Scaleform::Render::JPEG::FileReader::Instance );
	image_handlers->AddHandler( &Scaleform::Render::PNG::FileReader::Instance );
	image_handlers->AddHandler( &Scaleform::Render::TGA::FileReader::Instance );
	image_handlers->AddHandler( &Scaleform::Render::DDS::FileReader::Instance );
	m_gfx_loader->SetImageFileHandlerRegistry( image_handlers );

	m_render_thread_queue	= new scaleform_render_command_queue( game_engine );
}

} // namespace survarium
