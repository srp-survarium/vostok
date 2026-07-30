#ifndef VOSTOK_RENDER_FACADE_SOURCES_UPDATE_SKELETON_COMMAND_H_INCLUDED
#define VOSTOK_RENDER_FACADE_SOURCES_UPDATE_SKELETON_COMMAND_H_INCLUDED

#include <boost/noncopyable.hpp>
#include <vostok/render/facade/base_command.h>
#include <vostok/render/facade/model.h>
#include <vostok/math_float4x4.h>

namespace vostok {
namespace render {

namespace engine {
class world;
}

class update_skeleton_command : public base_command, private boost::noncopyable {
public:
	update_skeleton_command(
		engine::world& world,
		render_model_instance_ptr instance,
		float4x4 const* matrices,
		u32 count
	);

	virtual void execute( );
	virtual ~update_skeleton_command( ) { /* no source */ }

private:
	float4x4 m_matrices[80];
	render_model_instance_ptr m_model_instance;
	engine::world& m_world;
	u32 m_matrices_count;
};

STATIC_SIZE_ASSERT( update_skeleton_command, 0x1460 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_SOURCES_UPDATE_SKELETON_COMMAND_H_INCLUDED
