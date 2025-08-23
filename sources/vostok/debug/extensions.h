////////////////////////////////////////////////////////////////////////////
//	Created 	: 24.09.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_DEBUG_EXTENSIONS_H_INCLUDED
#define VOSTOK_DEBUG_EXTENSIONS_H_INCLUDED

#include <vostok/debug/debug.h>
#include <vostok/debug/macros.h>
#include <vostok/debug/signalling_bool.h>
#include <vostok/debug/check_invariants.h>

// Disable specific Code Analysis warning:
// "_alloca indicates failure by raising a stack overflow exception. Consider using _malloca instead"
#pragma warning( disable : 6255 )

#endif // #ifndef VOSTOK_DEBUG_EXTENSIONS_H_INCLUDED