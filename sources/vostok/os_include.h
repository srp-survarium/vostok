////////////////////////////////////////////////////////////////////////////
//	Created 	: 25.09.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_OS_INCLUDE_H_INCLUDED
#define VOSTOK_OS_INCLUDE_H_INCLUDED

#include <vostok/os_preinclude.h>


#	pragma warning ( push )
#	pragma warning ( disable : 4005 )		// because of the DELETE macro
#	include			<windows.h>
#	pragma warning ( pop )

#	undef DELETE
#	undef max
#	undef min


	COMPILE_ASSERT	(((HANDLE)(LONG_PTR)INVALID_HANDLE_VALUE_fake_value) == INVALID_HANDLE_VALUE, HANDLE_definition_in_windows_h_differs_from_what_is_expected_See_os_include_h);
	COMPILE_ASSERT	(sizeof(HMODULE_fake_type) == sizeof(HMODULE), HMODULE_definition_in_windows_h_differs_from_what_is_expected_See_os_include_h);

#endif // #ifndef VOSTOK_OS_INCLUDE_H_INCLUDED