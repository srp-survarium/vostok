////////////////////////////////////////////////////////////////////////////
//	Created 	: 17.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_ENGINE_LIBRARY_LINKAGE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_LIBRARY_LINKAGE_H_INCLUDED

#include <vostok/macro_library_name.h>

#	ifndef OPENGL
//#		error ooops!
//#		pragma comment(lib, VOSTOK_LIBRARY_NAME( render_engine_dx11, lib ) )
//#		pragma comment(lib, VOSTOK_LIBRARY_NAME( render_core_dx11, lib ) )
#	else // #ifndef OPENGL
//#		pragma comment(lib, VOSTOK_LIBRARY_NAME( render_engine_opengl, lib ) )
//#		pragma comment(lib, VOSTOK_LIBRARY_NAME( render_core_opengl, lib ) )
#	endif // #ifndef OPENGL

#endif // #ifndef VOSTOK_RENDER_ENGINE_LIBRARY_LINKAGE_H_INCLUDED