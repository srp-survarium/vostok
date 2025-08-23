////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/debug/extensions.h>
#include "bugtrap.h"
#include "utils.h"

bool vostok::debug::bugtrap::initialized	( )
{
	return		( true );
}

void vostok::debug::bugtrap::initialize		( )
{
}

void vostok::debug::bugtrap::finalize		( )
{
}

bool vostok::debug::bugtrap::on_thread_spawn( )
{
	return		( true );
}

void vostok::debug::bugtrap::add_file		( pcstr full_path_file_name )
{
	VOSTOK_UNREFERENCED_PARAMETER	( full_path_file_name );
}

void vostok::debug::bugtrap::change_usage	( error_mode error_mode, bugtrap_usage bugtrap_usage )
{
	VOSTOK_UNREFERENCED_PARAMETERS( error_mode, bugtrap_usage );
}

void vostok::debug::on_error				( pcstr message )
{
	VOSTOK_UNREFERENCED_PARAMETER	( message );
}