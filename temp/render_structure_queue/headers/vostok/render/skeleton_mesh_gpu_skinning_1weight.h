////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SKELETON_MESH_GPU_SKINNING_1WEIGHT_H_INCLUDED
#define RENDER_SKELETON_MESH_GPU_SKINNING_1WEIGHT_H_INCLUDED

/* INCLUDES */
class vostok::render::skeleton_render_surface;
typedef vostok::intrusive_ptr<vostok::render::untyped_buffer,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::untyped_buffer_ptr;
class vostok::render::untyped_buffer;

/* FORWARD REFS */
class vostok::configs::binary_config_value;
class vostok::memory::chunk_reader;
class vostok::render::vector<vostok::math::float4x4>;

namespace vostok {
namespace render {

class skeleton_mesh_gpu_skinning_1weight : public skeleton_render_surface {
public:
					skeleton_mesh_gpu_skinning_1weight	( );
	virtual			~skeleton_mesh_gpu_skinning_1weight	( );

	virtual	void	load								( configs::binary_config_value const& properties, memory::chunk_reader& r ) override;

	virtual	void	update								( vector< float4x4 > const& bones ) override;

	/* 0x0000 */	/* skeleton_render_surface */
private:
	/* 0x009c */	untyped_buffer_ptr		m_vertex_buffer;
}; // class skeleton_mesh_gpu_skinning_1weight

STATIC_SIZE_ASSERT(skeleton_mesh_gpu_skinning_1weight, 0xA0);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SKELETON_MESH_GPU_SKINNING_1WEIGHT_H_INCLUDED
