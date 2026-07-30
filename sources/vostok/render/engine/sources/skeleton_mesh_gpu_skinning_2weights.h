#ifndef VOSTOK_RENDER_ENGINE_SKELETON_MESH_GPU_SKINNING_2WEIGHTS_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SKELETON_MESH_GPU_SKINNING_2WEIGHTS_H_INCLUDED

#include <vostok/render/core/untyped_buffer.h>
#include "render_model_skeleton.h"

namespace vostok {
namespace render {

class skeleton_mesh_gpu_skinning_2weights : public skeleton_render_surface {
public:
	skeleton_mesh_gpu_skinning_2weights( );
	virtual ~skeleton_mesh_gpu_skinning_2weights( );

	virtual void load( configs::binary_config_value const& properties, memory::chunk_reader& r );
	virtual void update( vector< float4x4 > const& bones );

private:
	untyped_buffer_ptr m_vertex_buffer;
};

STATIC_SIZE_ASSERT( skeleton_mesh_gpu_skinning_2weights, 0xA0 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SKELETON_MESH_GPU_SKINNING_2WEIGHTS_H_INCLUDED
