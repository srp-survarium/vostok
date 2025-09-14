////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.08.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef PCH_H_INCLUDED
#define PCH_H_INCLUDED

#define VOSTOK_CORE_BUILDING	1

#ifndef VOSTOK_STATIC_LIBRARIES
#	define VOSTOK_ENGINE_BUILDING
#endif // #ifndef VOSTOK_STATIC_LIBRARIES

#define VOSTOK_IN_PCH			1
#include <vostok/extensions.h>
#undef VOSTOK_IN_PCH

#endif // #ifndef PCH_H_INCLUDED