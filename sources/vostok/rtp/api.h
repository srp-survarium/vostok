////////////////////////////////////////////////////////////////////////////
//	Created		: 23.04.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RTP_API_H_INCLUDED
#define VOSTOK_RTP_API_H_INCLUDED

#include <vostok/render/engine/base_classes.h>

#ifndef VOSTOK_RTP_API
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_RTP_API
#	else // #ifdef VOSTOK_STATIC_LIBRARIES
#		ifdef VOSTOK_RTP_BUILDING
#			define VOSTOK_RTP_API						VOSTOK_DLL_EXPORT
#		else // #ifdef VOSTOK_RTP_BUILDING
#			ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_RTP_API					VOSTOK_DLL_IMPORT
#			else // #ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_RTP_API					VOSTOK_DLL_EXPORT
#			endif // #ifndef VOSTOK_ENGINE_BUILDING
#		endif // #ifdef VOSTOK_RTP_BUILDING
#	endif // #ifdef VOSTOK_STATIC_LIBRARIES
#endif // #ifndef VOSTOK_RTP_API

#if !VOSTOK_PLATFORM_XBOX_360 && !VOSTOK_PLATFORM_PS3 && (!defined(MASTER_GOLD) )
#		define	VOSTOK_RTP_LEARNING	1
#	if (!VOSTOK_DEBUG_ALLOCATOR)
#		error	rtp uses debug allocator for learning
#	endif
#else
#		define	VOSTOK_RTP_LEARNING	0
#endif

namespace vostok {

namespace animation {
	struct world;
} // namespace animation

namespace render {
namespace game {
	class renderer;
} // namespace game
} // namespace render

namespace rtp {

struct engine;
struct world;

typedef memory::base_allocator				allocator_type;

VOSTOK_RTP_API	world*	create_world		(
												engine& engine,
												animation::world& animation,
												render::scene_ptr const& scene,
												render::game::renderer* renderer,
												pcstr const resources_vostok_path
											);
VOSTOK_RTP_API	void	destroy_world		( world*& world );
VOSTOK_RTP_API	void	set_memory_allocator( memory::base_allocator& allocator );

} // namespace rtp
} // namespace vostok

#endif // #ifndef VOSTOK_RTP_API_H_INCLUDED