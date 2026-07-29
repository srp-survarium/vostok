////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_UPDATE_MODEL_VERTEX_BUFFER_COMMAND_H_INCLUDED
#define RENDER_UPDATE_MODEL_VERTEX_BUFFER_COMMAND_H_INCLUDED

/* INCLUDES */
class vostok::memory::base_allocator;
class vostok::render::base_command;
class vostok::render::engine::world;
class vostok::vectora<vostok::render::buffer_fragment>;
typedef vostok::resources::resource_ptr<vostok::render::render_model_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::render_model_instance_ptr;
class vostok::render::render_model_instance;

namespace vostok {
namespace render {

class update_model_vertex_buffer_command : public base_command , public boost::noncopyable {
public:
	inline			update_model_vertex_buffer_command	(
						engine::world&							arg_0,
						render_model_instance_ptr				arg_1,
						vectora< buffer_fragment > const&		arg_2,
						memory::base_allocator&					arg_3
					) { /* no source */ }
	virtual			~update_model_vertex_buffer_command	( );

	virtual	void	execute								( ) override;

	/* 0x0000 */	/* base_command */
	/* 0x0054 */	/* boost::noncopyable */
private:
	/* 0x0054 */	vectora< buffer_fragment >		m_fragments;
	/* 0x0064 */	render_model_instance_ptr		m_object;
	/* 0x0068 */	engine::world&					m_world;
	/* 0x006c */	memory::base_allocator&			m_allocator;
}; // class update_model_vertex_buffer_command

STATIC_SIZE_ASSERT(update_model_vertex_buffer_command, 0x70);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_UPDATE_MODEL_VERTEX_BUFFER_COMMAND_H_INCLUDED
