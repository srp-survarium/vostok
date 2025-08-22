////////////////////////////////////////////////////////////////////////////
//	Created 	: 25.09.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/linkage_helper.h>

#include <vostok/os_preinclude.h>
#undef NOCLIPBOARD
#undef NOUSER
#include <vostok/os_include.h>

VOSTOK_DECLARE_LINKAGE_ID(core_os_functions_win)

using namespace vostok;

bool vostok::os::is_clipboard_empty				( )
{
	if ( !OpenClipboard(0) )
		return					false;

	HGLOBAL const handle		= GetClipboardData( CF_TEXT );
	if ( !handle ) {
		CloseClipboard			( );
		return					false;
	}

	pcstr const data			= static_cast<pcstr>( GlobalLock(handle) );
	u32 const string_length		= strings::length(data);

	CloseClipboard				( );
	return						( !string_length );
}

void vostok::os::copy_to_clipboard				( pcstr buffer )
{
	if ( !OpenClipboard(0) )
		return;

	u32 const handle_size = ( strings::length(buffer) + 1 ) * sizeof(char);
	HGLOBAL const handle = GlobalAlloc( GHND, handle_size );
	if ( !handle ) {
		CloseClipboard			();
		return;
	}

	char* const memory			= static_cast<char*>( GlobalLock(handle) );
	strings::copy					( memory, handle_size, buffer );
	GlobalUnlock				( handle );
	EmptyClipboard				( );
	SetClipboardData			( CF_TEXT, handle );
	CloseClipboard				( );
}

pstr vostok::os::paste_from_clipboard				( vostok::memory::base_allocator& allocator )
{
	if ( !OpenClipboard(0) )
		return					0;

	HGLOBAL const handle		= GetClipboardData( CF_TEXT );
	if ( !handle )
		return					0;

	pcstr const data			= static_cast<pcstr>( GlobalLock(handle) );
	u32 const string_length		= strings::length(data);
	if ( !string_length ) {
		CloseClipboard			( );
		return					0;
	}

	u32 const buffer_size		= string_length + 1;
	pstr const result			= (pstr)VOSTOK_MALLOC_IMPL( allocator, buffer_size*sizeof(char), "buffer for pasting from clipboard");
	strings::copy				( result, buffer_size, data );
	result[buffer_size-1]		= 0;
	GlobalUnlock				( handle );
	CloseClipboard				( );
	return						result;
}

void vostok::os::update_clipboard					( pcstr const string )
{
	if ( !OpenClipboard(0) )
		return;

	HGLOBAL const handle		= GetClipboardData(CF_TEXT);
	if (!handle) {
		CloseClipboard			( );
		copy_to_clipboard		( string );
		return;
	}

	LPSTR const memory			= static_cast<pstr>( GlobalLock(handle) );
	u32 const memory_length		= strings::length(memory);
	u32 const string_length		= strings::length(string);
	u32 const buffer_size		= (memory_length + string_length + 1) * sizeof(char);
	LPSTR const buffer			= (LPSTR)ALLOCA( buffer_size );
	strings::copy				( buffer, buffer_size, memory );
	GlobalUnlock				( handle );

	strings::append				( buffer, buffer_size, string );
	CloseClipboard				( );
	copy_to_clipboard			( buffer );
}