////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "platform.h"

void vostok::debug::platform::setup_storage_access_handler	( error_mode error_mode, void (*storage_access_handler) ( int ) )
{
	VOSTOK_UNREFERENCED_PARAMETER			( storage_access_handler );
	setup_unhandled_exception_handler	( error_mode );
}

void vostok::debug::platform::change_storage_access_handler	( error_mode error_mode, 
																  void (*storage_access_handler) ( int ) )
{
	VOSTOK_UNREFERENCED_PARAMETER			( storage_access_handler );
	change_unhandled_exception_handler	( error_mode );
}