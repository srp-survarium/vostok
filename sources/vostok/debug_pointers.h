////////////////////////////////////////////////////////////////////////////
//	Created 	: 16.12.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_DEBUG_POINTERS_H_INCLUDED
#define VOSTOK_DEBUG_POINTERS_H_INCLUDED

#ifdef DEBUG
#	define VOSTOK_USE_DEBUG_POINTERS	1
#else // #ifdef DEBUG
#	define VOSTOK_USE_DEBUG_POINTERS	0
#endif // #ifdef DEBUG

#include <vostok/raw_ptr.h>
#include <vostok/non_null_ptr.h>
#include <vostok/loose_ptr.h>

#endif // #ifndef VOSTOK_DEBUG_POINTERS_H_INCLUDED