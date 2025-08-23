////////////////////////////////////////////////////////////////////////////
//	Created 	: 15.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/debug/extensions.h>
#include <vostok/debug/call_stack.h>

struct _EXCEPTION_POINTERS;

void vostok::debug::call_stack::iterate	( _EXCEPTION_POINTERS* pointers,  // can be NULL
											  void* call_stack[], // can be NULL
											  Callback const& callback,
											  bool			invert_order)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( pointers, call_stack, invert_order );
	VOSTOK_UNREFERENCED_PARAMETER		( callback );
}

void vostok::debug::call_stack::get_stack_trace ( void* stack_trace[], u32 const stack_trace_max, u32 const num_to_capture, u32* out_hash )
{
#pragma message( VOSTOK_TODO("PS3: iterate call stack") )
	VOSTOK_UNREFERENCED_PARAMETERS( stack_trace, stack_trace_max, num_to_capture, out_hash );
}

void vostok::debug::call_stack::finalize_symbols( )
{
}