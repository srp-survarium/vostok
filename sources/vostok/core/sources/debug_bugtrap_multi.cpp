////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "debug_bugtrap.h"
#include "debug.h"
#include <vostok/core/core.h>

bool vostok::core::debug::bugtrap::initialized	( )
{
	return		( true );
}

void vostok::core::debug::bugtrap::initialize		( )
{
}

void vostok::core::debug::bugtrap::finalize		( )
{
}

bool vostok::core::debug::bugtrap::on_thread_spawn( )
{
	return		( true );
}

void vostok::core::debug::bugtrap::add_file		( pcstr full_path_file_name )
{
	VOSTOK_UNREFERENCED_PARAMETER	( full_path_file_name );
}

void vostok::core::debug::bugtrap::change_usage	( error_mode error_mode, bugtrap_usage bugtrap_usage )
{
	VOSTOK_UNREFERENCED_PARAMETERS( error_mode, bugtrap_usage );
}

void vostok::core::debug::on_error				( pcstr message )
{
	VOSTOK_UNREFERENCED_PARAMETER	( message );
}