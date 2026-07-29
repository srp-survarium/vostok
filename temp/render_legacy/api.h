////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_API_H_INCLUDED
#define VOSTOK_RENDER_API_H_INCLUDED

#include <vostok/configs_binary_config.h>

#ifndef VOSTOK_RENDER_API
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_RENDER_API
#	else
#		ifdef VOSTOK_RENDER_BUILDING
#			define VOSTOK_RENDER_API				VOSTOK_DLL_EXPORT
#		else
#			ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_RENDER_API			VOSTOK_DLL_IMPORT
#			else
#				define VOSTOK_RENDER_API			VOSTOK_DLL_EXPORT
#			endif
#		endif
#	endif
#endif

namespace vostok {
namespace particle {
	struct world;
}

namespace render {

class world;

typedef memory::base_allocator					logic_allocator_type;
typedef memory::base_allocator					editor_allocator_type;
typedef memory::doug_lea_allocator_type			render_allocator_type;

VOSTOK_RENDER_API	void	set_memory_allocator	(
							render_allocator_type&				render_allocator,
							logic_allocator_type&				logic_allocator,
							editor_allocator_type&				editor_allocator
						);

VOSTOK_RENDER_API	world*	create_world			(
							memory::base_allocator&				logic_allocator,
							memory::base_allocator*				editor_allocator,
							configs::binary_config_ptr const&	in_config,
							bool								is_editor
						);

VOSTOK_RENDER_API	void	destroy_world			( world*& world );

} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_API_H_INCLUDED
