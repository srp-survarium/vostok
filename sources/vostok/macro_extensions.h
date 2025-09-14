////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.08.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_MACRO_EXTENSIONS_H_INCLUDED
#define VOSTOK_MACRO_EXTENSIONS_H_INCLUDED

#if defined( _CPPUNWIND ) && !defined( _MANAGED )
#	error please disable exceptions
#endif // #if defined( _CPPUNWIND ) && !defined( _MANAGED )

#ifndef NDEBUG
#	define DEBUG
#endif // #ifdef NDEBUG

#include <vostok/macro_compile_assert.h>
#include <vostok/macro_strings.h>
#include <vostok/macro_platform.h>
#include <vostok/macro_compiler.h>
#include <vostok/macro_building.h>
#include <vostok/macro_unreferenced_parameter.h>
#include <vostok/macro_todo.h>

#define VOSTOK_ENGINE_COMPANY_ID	"GSC Game World"
#define VOSTOK_ENGINE_VERSION		2
#define VOSTOK_ENGINE_SUBVERSION	0
#define VOSTOK_ENGINE_ID			"X-Ray Engine v" VOSTOK_MAKE_STRING(VOSTOK_ENGINE_VERSION) "." VOSTOK_MAKE_STRING(VOSTOK_ENGINE_SUBVERSION)

#define IMPLICIT

#endif // #ifndef VOSTOK_MACRO_EXTENSIONS_H_INCLUDED