////////////////////////////////////////////////////////////////////////////
//	Created 	: 26.08.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "survarium_pc_application.h"
#include "resource.h"
#include <vostok/engine/api.h>

#include <vostok/os_preinclude.h>
#undef NOUSER
#undef NOMSG
#undef NOWINMESSAGES
#undef NOCTLMGR
#undef NOGDI
#undef NOMB
#include <vostok/os_include.h>

using survarium::application;

static HWND	s_splash_screen			= 0;

static HANDLE	s_presence_mutex		= 0;

static vostok::fixed_string512		s_finger_print;

static bool load_splash_screen		( HANDLE const file_handle, pbyte const buffer, u32 const buffer_size )
{
	vostok::threading::event_tasks_unaware	event;

	OVERLAPPED overlapped;
	overlapped.Offset				= 0;
	overlapped.OffsetHigh			= 0;
	overlapped.hEvent				= event.get_handle( );

	BOOL const result				= ReadFile( file_handle, buffer, buffer_size, 0, &overlapped );
	if ( !result ) {
		DWORD const last_error		= GetLastError( );
		if ( last_error != ERROR_IO_PENDING )
			return					false;
	}

	event.wait						( vostok::threading::wait_time_infinite );

	DWORD bytes_done;
	if ( !GetOverlappedResult(file_handle, &overlapped, &bytes_done, FALSE) )
		return						false;

	return							true;
}

class raii_buffer : private boost::noncopyable {
public:
	raii_buffer( pbyte const buffer, bool dynamic_allocation ) :
		m_buffer					( buffer ),
		m_dynamic_allocation		( dynamic_allocation )
	{
	}

	~raii_buffer( )
	{
		if ( !m_dynamic_allocation )
			return;

		HeapFree					( GetProcessHeap(), 0, m_buffer );
	}

private:
	pbyte const						m_buffer;
	bool							m_dynamic_allocation;
};

static pbyte allocate				( u32 const size )
{
	vostok::memory::lock_process_heap		( );
	pbyte const result					= static_cast<pbyte>( HeapAlloc(GetProcessHeap(), 0, size) );
	vostok::memory::unlock_process_heap	( );
	return								result;
}

#if VOSTOK_PLATFORM_WINDOWS_32
#	define RETURN_TYPE	BOOL
#elif VOSTOK_PLATFORM_WINDOWS_64 // #ifdef VOSTOK_PLATFORM_WINDOWS_32
#	define RETURN_TYPE	INT_PTR
#else // #elif VOSTOK_PLATFORM_WINDOWS_64 // #ifdef VOSTOK_PLATFORM_WINDOWS_32
#	error do not use this file in non-Windows platforms
#endif // #elif VOSTOK_PLATFORM_WINDOWS_64 // #ifdef VOSTOK_PLATFORM_WINDOWS_32

static RETURN_TYPE CALLBACK window_procedure ( HWND window_handle, UINT msg, WPARAM wp, LPARAM lp)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( lp, window_handle );

	switch( msg ){
		case WM_DESTROY:
			PostQuitMessage	( 0 );
			return					( FALSE );

		case WM_CTLCOLORSTATIC:
			SetTextColor			( (HDC)wp, RGB( 255, 255, 255 ) );
			SetBkMode				( (HDC)wp, TRANSPARENT );
			return					( (RETURN_TYPE)GetStockObject( NULL_BRUSH ) );
	}

	return							( FALSE );
}

#undef RETURN_TYPE

#pragma warning(push)
#pragma warning(disable:4509)
static void splash_screen_main		( )
{
	vostok::fixed_string<260> file_name	= "../../resources/sources/splash_screens/";
	file_name.append				( vostok::engine::command_line_editor() ? "development_editor" : "development_engine" );
	file_name.append				( ".bmp" );
	HANDLE const file_handle		=
		CreateFile (
			file_name.c_str(),
			GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING,
			0
		);

	if ( file_handle == INVALID_HANDLE_VALUE ) {
		LOG_ERROR					( "cannot open splash screen from file \"%s\" ", file_name.c_str() );
		return;
	}

	u32 const file_size				= GetFileSize(file_handle, 0);
	u32 const sector_size			= 512;
	u32 const buffer_size			= vostok::math::align_up( file_size, sector_size );

	pbyte buffer					= 0;
	bool dynamic_allocation			= true;
	if ( buffer_size < 900*1024 ) {
		__try {
			buffer					= static_cast<pbyte>( ALLOCA(buffer_size) );
			dynamic_allocation		= false;
		}
		__except(GetExceptionCode() == STATUS_STACK_OVERFLOW)
		{
			int						errcode = _resetstkoflw();
			R_ASSERT_U				(errcode, "Could not reset the stack after \"Stack overflow\" exception!");
			buffer					= allocate( buffer_size );
		};
	}
	else {
		buffer						= allocate( buffer_size );
	}

	raii_buffer const				guard( buffer, dynamic_allocation );

	if ( !load_splash_screen(file_handle, buffer, buffer_size) ) {
		LOG_ERROR					( "cannot load splash screen from file \"%s\" ", file_name.c_str() );
		return;
	}

	CloseHandle						( file_handle );

	vostok::memory::lock_process_heap	( );

	BITMAPFILEHEADER* const file_header = vostok::pointer_cast<BITMAPFILEHEADER*>(buffer);
	BITMAPINFOHEADER* const info_header	= vostok::pointer_cast<BITMAPINFOHEADER*>(buffer + sizeof(BITMAPFILEHEADER));
	pbyte const pixel_buffer		= buffer + file_header->bfOffBits;
	u32 const bytes_per_line		= vostok::math::align_up( u32(info_header->biBitCount*info_header->biWidth), u32(32) )/8;
	pbyte const line				= (pbyte)ALLOCA(bytes_per_line);
	for ( int i=0, j=int(info_header->biHeight)-1; i<j; ++i, --j ) {
		vostok::memory::copy			( line, bytes_per_line, pixel_buffer + i*bytes_per_line, bytes_per_line );
		vostok::memory::copy			( pixel_buffer + i*bytes_per_line, bytes_per_line, pixel_buffer + j*bytes_per_line, bytes_per_line );
		vostok::memory::copy			( pixel_buffer + j*bytes_per_line, bytes_per_line, line, bytes_per_line );
	}

	HBITMAP const bitmap_handle		= 
		CreateBitmap(
			info_header->biWidth,
			info_header->biHeight,
			info_header->biPlanes,
			info_header->biBitCount,
			buffer + file_header->bfOffBits
		);
	if ( !bitmap_handle ) {
		LOG_ERROR					( "cannot create bitmap for splash screen from file \"%s\" ", file_name.c_str() );
		return;
	}

	HMODULE const module			= GetModuleHandle( 0 );
	s_splash_screen					= CreateDialog( module, MAKEINTRESOURCE( IDD_SPLASH_SCREEN ), 0, window_procedure );
	SetDlgItemText					( s_splash_screen, IDC_FINGER_PRINT, s_finger_print.c_str() );

	HWND const picture_control		= GetDlgItem( s_splash_screen, IDC_SPLASH_SCREEN );
	if ( !picture_control ) {
		PostMessage					( s_splash_screen, WM_DESTROY, 0, 0 );
		LOG_ERROR					( "cannot find picture control in splash screen dialog" );
		return;
	}

	// ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/shellcc/platform/commctls/staticcontrols/staticcontrolreference/staticcontrolmessages/stm_setimage.htm
	HBITMAP const previous_bitmap	= (HBITMAP)SendMessage( picture_control, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bitmap_handle );
	ASSERT							( !previous_bitmap, "do not assign image to splash screen in design mode" );
	if ( previous_bitmap )
		DeleteObject				( previous_bitmap );

	RECT							screen_rectangle;
	GetWindowRect					( GetDesktopWindow( ), &screen_rectangle );

	SetWindowPos					(
		s_splash_screen,
#ifndef MASTER_GOLD
		HWND_NOTOPMOST,
#else // #ifndef MASTER_GOLD
		HWND_TOPMOST,
#endif // #ifndef MASTER_GOLD
		(screen_rectangle.right - screen_rectangle.left - info_header->biWidth)/2,
		(screen_rectangle.bottom - screen_rectangle.top - info_header->biHeight)/2,
		info_header->biWidth,
		info_header->biHeight,
		SWP_SHOWWINDOW
	);

	SetActiveWindow					( s_splash_screen );
	SetForegroundWindow				( s_splash_screen );

	MSG								message;
	for ( ;; ) {
		bool const result			= !!GetMessage( &message, 0, 0, 0);
		if ( !result )
			break;

		TranslateMessage			( &message );
		DispatchMessage				( &message );

		//if ( !vostok::debug::is_debugger_present() && (GetForegroundWindow() != s_splash_screen) )
		//	SetForegroundWindow		( s_splash_screen );
	}

	// in some cases this could be done after STM_SETIMAGE
	// but it is more safe to do this here always
	DeleteObject					( bitmap_handle );

	vostok::memory::unlock_process_heap ( );
}
#pragma warning(pop)

void application::preinitialize					( )
{
	vostok::engine::preinitialize		( m_game_proxy, GetCommandLine( ), "survarium", __DATE__ );

	char temp[64];
	void   decode_finger_print		(char (&)[64]);
	decode_finger_print				( temp );
	s_finger_print					=	temp;

	if ( !vostok::engine::command_line_no_splash_screen() )
		vostok::threading::spawn		( &splash_screen_main, "splash screen", "splash", 0, 0, vostok::threading::tasks_unaware );
}

void application::postinitialize				( )
{
	PostMessage						( s_splash_screen, WM_DESTROY, 0, 0 );
}

// claude@NOTE: optimized-target wall - structure MATCHes (8/8 stmts). Byte residual
// is the `return s_presence_mutex != 0;` codegen: target does setne al on the live
// CreateMutexA result (eax) right after storing, base materialises the bool via
// xor ecx/setne cl/mov al,cl. Not source-steerable from /Od source.
bool check_presence_mutex						( )
{
	s_presence_mutex				= OpenMutexA( READ_CONTROL, FALSE, "survarium_already_running" );
	if ( !s_presence_mutex ) {
		s_presence_mutex			= CreateMutexA( 0, FALSE, "survarium_already_running" );
		return						s_presence_mutex != 0;
	}

	CloseHandle						( s_presence_mutex );
	MessageBoxA						( 0, "Survarium application already running", "Survarium", MB_ICONERROR );
	TerminateProcess				( GetCurrentProcess(), 1 );
	return							false;
}

void destroy_presence_mutex						( )
{
	CloseHandle						( s_presence_mutex );
}
