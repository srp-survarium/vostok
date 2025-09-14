////////////////////////////////////////////////////////////////////////////
//	Created		: 24.12.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_ENGINE_API_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_API_H_INCLUDED

#ifndef VOSTOK_RENDER_ENGINE_API
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_RENDER_ENGINE_API
#	else // #ifdef VOSTOK_STATIC_LIBRARIES
#		ifdef VOSTOK_RENDER_BUILDING
#			define VOSTOK_RENDER_ENGINE_API		VOSTOK_DLL_EXPORT
#		else // #ifdef VOSTOK_RENDER_BUILDING
#			ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_RENDER_ENGINE_API	VOSTOK_DLL_IMPORT
#			else // #ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_RENDER_ENGINE_API	VOSTOK_DLL_EXPORT
#			endif // #ifndef VOSTOK_ENGINE_BUILDING
#		endif // #ifdef VOSTOK_RENDER_BUILDING
#	endif // #ifdef VOSTOK_STATIC_LIBRARIES
#endif // #ifndef VOSTOK_RENDER_ENGINE_API

namespace vostok {
namespace render {
namespace engine {

class world;

VOSTOK_RENDER_ENGINE_API world*	create_world	( );
VOSTOK_RENDER_ENGINE_API void		destroy			( world*& instance );

} // namespace engine
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_API_H_INCLUDED