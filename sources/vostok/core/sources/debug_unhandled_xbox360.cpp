////////////////////////////////////////////////////////////////////////////
//	Created 	: 07.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "debug_platform.h"

void vostok::core::debug::platform::format_message			( )
{
}

void vostok::core::debug::platform::save_minidump				( _SYSTEMTIME const& date_time, _EXCEPTION_POINTERS* exception_information )
{
	VOSTOK_UNREFERENCED_PARAMETER	( date_time );
	VOSTOK_UNREFERENCED_PARAMETER	( exception_information );
}

void vostok::core::debug::platform::prologue_dump_call_stack	( _EXCEPTION_POINTERS* exception_information )
{
	VOSTOK_UNREFERENCED_PARAMETER	( exception_information );
}

void vostok::core::debug::set_thread_stack_guarantee			( )
{
}