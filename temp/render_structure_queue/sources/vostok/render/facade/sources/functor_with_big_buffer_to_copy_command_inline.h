////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\RENDER\FACADE\SOURCES\FUNCTOR_WITH_BIG_BUFFER_TO_COPY_COMMAND_INLINE_H_INCLUDED
#define VOSTOK\RENDER\FACADE\SOURCES\FUNCTOR_WITH_BIG_BUFFER_TO_COPY_COMMAND_INLINE_H_INCLUDED

namespace vostok {
namespace render {

// STATE[STUB]
inline  functor_with_big_buffer_to_copy_command<vostok::render::game::renderer::draw_scene_params>::functor_with_big_buffer_to_copy_command<vostok::render::game::renderer::draw_scene_params>(
	boost::function< void( game::renderer::draw_scene_params const& ) > const&	on_execute,
	game::renderer::draw_scene_params const&	data,
	boost::function< void( base_command& ) > const&	on_defer_execution
)
{
	// FUNCTION BODY[0x15e90]: 1
	// <0>
	// ******
}

// STATE[STUB]
inline void functor_with_big_buffer_to_copy_command<vostok::render::game::renderer::draw_scene_params>::execute( )
{
	// FUNCTION BODY[0x1a2f0]: 1
	// <0x1a2f0>|0x000|+0x00f:'30'
	// ******
}

// STATE[STUB]
inline void functor_with_big_buffer_to_copy_command<vostok::render::game::renderer::draw_scene_params>::defer_execution( )
{
	// FUNCTION BODY[0x1a5f0]: 1
	// <0x1a5f0>|0x000|+0x009:'36'
	// ******
}

	// TYPEDEFS
	// typedef
	// 	boost::function< void( vostok::render::game::renderer::draw_scene_params const& ) >
	// 	on_execute_type;

	// ******

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK\RENDER\FACADE\SOURCES\FUNCTOR_WITH_BIG_BUFFER_TO_COPY_COMMAND_INLINE_H_INCLUDED
