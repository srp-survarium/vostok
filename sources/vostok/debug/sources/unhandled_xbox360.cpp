////////////////////////////////////////////////////////////////////////////
//	Created 	: 07.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "platform.h"

void vostok::debug::platform::format_message			( )
{
}

void vostok::debug::platform::save_minidump				( _SYSTEMTIME const& date_time, _EXCEPTION_POINTERS* exception_information )
{
	VOSTOK_UNREFERENCED_PARAMETER	( date_time );
	VOSTOK_UNREFERENCED_PARAMETER	( exception_information );
}

void vostok::debug::platform::prologue_dump_call_stack	( _EXCEPTION_POINTERS* exception_information )
{
	VOSTOK_UNREFERENCED_PARAMETER	( exception_information );
}

void vostok::debug::set_thread_stack_guarantee			( )
{
}