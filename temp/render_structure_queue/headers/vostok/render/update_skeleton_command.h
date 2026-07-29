////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_UPDATE_SKELETON_COMMAND_H_INCLUDED
#define RENDER_UPDATE_SKELETON_COMMAND_H_INCLUDED

/* INCLUDES */
class vostok::render::base_command;
class vostok::render::engine::world;
typedef vostok::resources::resource_ptr<vostok::render::render_model_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::render_model_instance_ptr;
class vostok::render::render_model_instance;

namespace vostok {
namespace render {

class update_skeleton_command : public base_command , public boost::noncopyable {
public:
					update_skeleton_command	(
						engine::world&					world,
						render_model_instance_ptr		instance,
						float4x4 const*					matrices,
						u32								count
					);

	virtual	void	execute					( ) override;

	virtual			~update_skeleton_command( ) { /* no source */ }

	/* 0x0000 */	/* base_command */
	/* 0x0054 */	/* boost::noncopyable */
private:
	/* 0x0054 */	float4x4						m_matrices[80];
	/* 0x1454 */	render_model_instance_ptr		m_model_instance;
	/* 0x1458 */	engine::world&					m_world;
	/* 0x145c */	u32								m_matrices_count;
}; // class update_skeleton_command

STATIC_SIZE_ASSERT(update_skeleton_command, 0x1460);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_UPDATE_SKELETON_COMMAND_H_INCLUDED
