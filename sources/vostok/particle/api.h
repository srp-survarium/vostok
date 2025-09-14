////////////////////////////////////////////////////////////////////////////
//	Created		: 20.07.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_PARTICLE_API_H_INCLUDED
#define VOSTOK_PARTICLE_API_H_INCLUDED

#ifndef VOSTOK_PARTICLE_API
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_PARTICLE_API
#	else // #ifdef VOSTOK_STATIC_LIBRARIES
#		ifdef VOSTOK_PARTICLE_BUILDING
#			define VOSTOK_PARTICLE_API		VOSTOK_DLL_EXPORT
#		else // #ifdef VOSTOK_PARTICLE_BUILDING
#			ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_PARTICLE_API	VOSTOK_DLL_IMPORT
#			else // #ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_PARTICLE_API	VOSTOK_DLL_EXPORT
#			endif // #ifndef VOSTOK_ENGINE_BUILDING
#		endif // #ifdef VOSTOK_PARTICLE_BUILDING
#	endif // #ifdef VOSTOK_STATIC_LIBRARIES
#endif // #ifndef VOSTOK_PARTICLE_API

namespace vostok {
namespace particle {

VOSTOK_PARTICLE_API	void	initialize	( );
VOSTOK_PARTICLE_API	void	finalize	( );

} // namespace particle
} // namespace vostok

#endif // #ifndef VOSTOK_PARTICLE_API_H_INCLUDED
