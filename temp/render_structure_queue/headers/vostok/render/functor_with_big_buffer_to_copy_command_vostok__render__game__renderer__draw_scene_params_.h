////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_FUNCTOR_WITH_BIG_BUFFER_TO_COPY_COMMAND_H_INCLUDED
#define RENDER_FUNCTOR_WITH_BIG_BUFFER_TO_COPY_COMMAND_H_INCLUDED

/* INCLUDES */
class boost::function<void __cdecl(vostok::render::base_command &)>;
class boost::function<void __cdecl(vostok::render::game::renderer::draw_scene_params const &)>;
class vostok::render::base_command;
struct vostok::render::game::renderer::draw_scene_params;

namespace vostok {
namespace render {

class functor_with_big_buffer_to_copy_command< game::renderer::draw_scene_params > : public base_command , public boost::noncopyable {
public:
	inline			functor_with_big_buffer_to_copy_command<vostok::render::game::renderer::draw_scene_params>(
						boost::function< void( game::renderer::draw_scene_params const& ) > const&	on_execute,
						game::renderer::draw_scene_params const&	data,
						boost::function< void( base_command& ) > const&	on_defer_execution
					) { /* no source */ }

	virtual	void	execute			( ) override { /* no source */ }

	virtual	void	defer_execution	( ) override { /* no source */ }

	virtual			~functor_with_big_buffer_to_copy_command<vostok::render::game::renderer::draw_scene_params>( ) { /* no source */ }

	/* 0x0000 */	/* base_command */
	/* 0x0054 */	/* boost::noncopyable */
private:
	/* 0x0058 */	boost::function< void( game::renderer::draw_scene_params const& ) >	m_on_execute;
	/* 0x0078 */	boost::function< void( base_command& ) >	m_on_defer_execution;
	/* 0x0098 */	const game::renderer::draw_scene_params	m_data;
}; // class functor_with_big_buffer_to_copy_command< game::renderer::draw_scene_params >

STATIC_SIZE_ASSERT(functor_with_big_buffer_to_copy_command< game::renderer::draw_scene_params >, 0xB8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_FUNCTOR_WITH_BIG_BUFFER_TO_COPY_COMMAND_H_INCLUDED
