////////////////////////////////////////////////////////////////////////////
//	Created 	: 21.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "engine_world.h"
#include "apc.h"
#include <vostok/input/world.h>
#include <vostok/editor/world/world.h>
#include <vostok/render/facade/engine_renderer.h>
#include <vostok/core/core.h>
#include <vostok/fs_utils.h>
#include <vostok/resources_fs.h>
#include <vostok/fs/fs_native_path_string.h>
#include <vostok/fs/fs_path_string_utils_inline.h>
#include <vostok/render/api.h>

#include <vostok/os_preinclude.h>
#undef NOUSER
#undef NOMSG
#undef NOWINSTYLES
#undef NOWINMESSAGES
#undef NOSYSMETRICS
#undef NOSHOWWINDOW
#undef NOWINOFFSETS
#define _WIN32_WINNT				0x0500
#include <vostok/os_include.h>

#include <objbase.h>				// for COINIT_MULTITHREADED
#pragma comment( lib, "delayimp.lib" )

// <shlobj.h> drags in prsht.h/commctrl.h, which need the GDI/USER types this
// TU strips via os_preinclude (NOGDI/NOUSER); declare the one shell import +
// CSIDL we use directly instead.
#define CSIDL_PERSONAL				0x0005
extern "C" __declspec(dllimport) HRESULT __stdcall SHGetFolderPathA( HWND hwnd, int csidl, HANDLE hToken, DWORD dwFlags, LPSTR pszPath );
#pragma comment( lib, "shell32.lib" )


using vostok::engine::engine_world;

static pcstr s_window_id					= VOSTOK_ENGINE_ID " DX10 Renderer Window";
static pcstr s_window_class_id				= VOSTOK_ENGINE_ID " DX10 Renderer Window Class ID";

vostok::command_line::key	s_no_log_file		("no_log_file", "no_log_file", "logging", "disables writing log to a file", "<no arguments>");

static WNDCLASSEX s_window_class;

static vostok::engine::engine_world*			s_world	= 0;

static LRESULT APIENTRY message_processor	( HWND window_handle, UINT message_id, WPARAM w_param, LPARAM l_param )
{
	switch ( message_id ) {
		case WM_CLOSE : {
			if ( s_world->is_destruction_started() )
				break;

			return			1;
		}
		case WM_DESTROY: {
			PostQuitMessage	( 0 );

			return			( 0 );
		}
		case WM_ACTIVATE: {

			u16 fActive						= LOWORD(w_param);
			BOOL fMinimized					= (BOOL) HIWORD(w_param);
			bool const active				= ((fActive!=WA_INACTIVE) && (!fMinimized)) ? true : false;

			if (active)
			{
				s_world->on_application_activate( );
				while (	ShowCursor( FALSE ) >= 0 );
			}else
			{
				s_world->on_application_deactivate( );
				while (	ShowCursor( TRUE ) < 0 );
			}
			//if ( (w_param == WA_ACTIVE) || (w_param == WA_CLICKACTIVE) ) {
			//	if ( !s_world->editor_world() )
			//		while (	ShowCursor( FALSE ) >= 0 );

			//	s_world->on_application_activate( );
			//
			//	break;
			//}

			//ASSERT			( w_param == WA_INACTIVE );
			//if ( !s_world->editor_world() )
			//	while (	ShowCursor( TRUE ) < 0 );

			break;
		}
	}

	return					( DefWindowProc( window_handle, message_id, w_param, l_param ) );
}

HWND new_window			( )
{
	WNDCLASSEX const temp	=
	{
		sizeof( WNDCLASSEX ),
		CS_CLASSDC,
		&message_processor,
		0L,
		0L,
		GetModuleHandle( 0 ),
		NULL,
		NULL,
		NULL,
		NULL,
		s_window_class_id,
		NULL
	};

	s_window_class			= temp;
	RegisterClassEx			( &s_window_class );

	u32 const screen_size_x	= GetSystemMetrics( SM_CXSCREEN );
	u32 const screen_size_y	= GetSystemMetrics( SM_CYSCREEN );

	DWORD const	window_style = WS_OVERLAPPED;// | WS_CAPTION;

	u32 window_size_x		= 0;
	u32 window_size_y		= 0;

	u32 const window_sizes_x []	= { 1024, 800, 640 };
	u32 const window_sizes_y []	= { 768, 600, 480 };
	for ( u32 i=0; i<vostok::array_size(window_sizes_x); ++i ) {
		if ( window_sizes_x[i] < screen_size_x &&
			 window_sizes_y[i] < screen_size_y )
		{
			window_size_x	= window_sizes_x[i];
			window_size_y	= window_sizes_y[i];
			break;
		}
	}

	R_ASSERT				(window_size_x);

	RECT window_size		= { 0, 0, window_size_x, window_size_y };
	AdjustWindowRect		( &window_size, window_style, false );

	HWND const result		=
		CreateWindow (
			s_window_class_id,
			s_window_id,
			window_style,
			( screen_size_x - window_size.right ) / 2,
			( screen_size_y - window_size.bottom ) / 2,
			window_size.right - window_size.left,
			window_size.bottom - window_size.top,
			GetDesktopWindow( ),
			0,
			s_window_class.hInstance,
			0
		);
	R_ASSERT				( result );
	return					result;
}

vostok::logging::log_file_usage_enum	engine_world::log_file_usage ( ) const
{
	return									logging::create_log;
}

void engine_world::initialize_core	( )
{
	pcstr debug_thread_id	= 0;
	if ( threading::g_debug_single_thread )
		debug_thread_id		= "main";
	else if ( threading::core_count() == 1 ) {
		if ( command_line_editor() )
			debug_thread_id	= "editor + logic + render";
		else
			debug_thread_id	= "logic + render";
	}
	else
		debug_thread_id		= "render";

	pstr lua_config_device_folder_to_save_to = 0;
	STR_JOINA				( lua_config_device_folder_to_save_to, get_resources_path(), "/sources" );
	if ( !command_line_editor() )
		core::initialize	( lua_config_device_folder_to_save_to, debug_thread_id, core::perform_debug_initialization );
	else
		core::initialize	( lua_config_device_folder_to_save_to, debug_thread_id, core::delay_debug_initialization );

	initialize_scaleform	( );
}

void engine_world::create_render	( vostok::configs::binary_config_ptr const& in_config, bool is_editor )
{
	s_world					= this;

	m_render_world			= render::create_world (
		m_engine_user_module_proxy.allocator(),
		is_editor ? &m_editor_allocator : 0,
		in_config,
		is_editor
	);
	R_ASSERT				( m_render_world );
}

void engine_world::destroy_render	( )
{
	render::destroy_world	( m_render_world );

	ShowWindow				( m_main_window_handle, SW_HIDE );

	ASSERT					( s_world );
	s_world					= 0;
}

void engine_world::show_window		( bool call_application_activate )
{
	if ( m_editor ) {
		SetForegroundWindow	( m_editor->main_handle( ) );
		return;
	}

	ShowWindow				( m_main_window_handle, SW_SHOW );
	SetForegroundWindow		( m_main_window_handle );

	if ( call_application_activate )
		on_application_activate	( );
}

bool engine_world::process_messages( )
{
	ASSERT					( !m_editor );

	MSG						message;
	while ( PeekMessage( &message, 0, 0, 0, PM_REMOVE ) ) {
		TranslateMessage	( &message );
		DispatchMessage		( &message );

		if ( message.message == WM_QUIT )
			return			( false );
	}

	return					( true );
}

void engine_world::run				( )
{
	if ( m_early_destruction_started )
		return;

//	m_timer.start			( );

	if ( m_editor && (threading::core_count( ) == 1) ) {
		m_editor->run		( );

		if ( !m_destruction_started )
			exit			( 0 );

		return;
	}

	for (;;) {
		if ( !m_editor && !process_messages() )
			return;

		if ( m_destruction_started )
			return;

		tick				( );
	}
}

pcstr engine_world::get_resources_path	( ) const
{
	return								"../../resources";
}

pcstr engine_world::get_mounts_path		( ) const
{
	return								"../../mounts";
}

// sushi@TODO: structure matches the target (8 stmts, same shape/locals); byte
// residual is inline-vs-call - the target out-lines append_relative_path<native,
// native> (a real call, ~0x1f bytes) while our base inlines it (~0x67).
pcstr engine_world::get_user_data_directory	( ) const
{
	using namespace vostok::fs_new;

	static native_path_string			s_user_data_directory;
	static bool							s_initialized	= false;
	if ( !s_initialized ) {
		char							path[MAX_PATH];
		SHGetFolderPathA				( NULL, CSIDL_PERSONAL, NULL, 0, path );
		_strlwr_s						( path, sizeof(path) );
		s_user_data_directory			= path;
		append_relative_path			( &s_user_data_directory, native_path_string::convert("survarium") );
		s_initialized					= true;
	}

	return								s_user_data_directory.c_str();
}

void engine_world::enable_game						( bool value )
{
	enable_game_impl	( value );
}

vostok::math::float2 engine_world::get_render_window_size( ) const
{
	RECT			rect;
	if ( GetClientRect( m_render_window_handle, &rect) )
		return		math::float2(float(rect.right-rect.left), float(rect.bottom-rect.top) );

	return			math::float2( 10, 10 );
}
