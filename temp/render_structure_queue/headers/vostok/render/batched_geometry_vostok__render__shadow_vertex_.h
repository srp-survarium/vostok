////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_BATCHED_GEOMETRY_H_INCLUDED
#define RENDER_BATCHED_GEOMETRY_H_INCLUDED

/* INCLUDES */
class vostok::math::aabb;
class vostok::render::vector<unsigned short>;
class vostok::render::vector<vostok::render::geometry_batch>;
class vostok::render::vector<vostok::render::shadow_vertex>;
struct vostok::render::batched_geometry_interface;
typedef vostok::intrusive_ptr<vostok::render::res_declaration,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_declaration_ptr;
typedef vostok::resources::resource_ptr<vostok::render::material_effects_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::material_effects_instance_ptr;
class vostok::render::material_effects_instance;
class vostok::render::res_declaration;

/* FORWARD REFS */
class boost::function<void __cdecl(vostok::render::geometry_batch const &)>;
class vostok::render::batched_vertex_source;
class vostok::render::renderer_context;

namespace vostok {
namespace render {

struct batched_geometry< shadow_vertex > : public batched_geometry_interface {
	inline	void	for_each_batch_render	(
						renderer_context*		arg_0,
						boost::function< void( geometry_batch const& ) > const&	arg_1,
						boost::function< void( geometry_batch const& ) > const&	arg_2
					) { /* no source */ }
	inline	void	for_each_batch_visible_render_first_and_remove(
						boost::function< void( geometry_batch const& ) > const&	arg_0,
						boost::function< void( geometry_batch const& ) > const&	arg_1,
						const u32		arg_2,
						const u32		arg_3
					) { /* no source */ }

	inline	u32		prepare_visible_batches	(
						renderer_context*		arg_0,
						float3 const&			arg_1,
						bool					arg_2,
						const u32				arg_3
					) { /* no source */ }
	inline	u32		get_num_visible_batches	( const u32 arg_0 ) const { /* no source */ }

	inline			batched_geometry<vostok::render::shadow_vertex>( ) { /* no source */ }
protected:
	inline			batched_geometry<vostok::render::shadow_vertex>(
						D3D11_INPUT_ELEMENT_DESC const*		layout,
						const u32							num_elements,
						const u32							in_batched_geometry_max_vertices_count
					) { /* no source */ }
	inline			~batched_geometry<vostok::render::shadow_vertex>( ) { /* no source */ }

	virtual	void	add_vertex				( batched_vertex_source const& arg_0, float3 const& arg_1 ) = 0;
	virtual	void	add_data				(
						batched_vertex_source const*	arg_0,
						const u32						arg_1,
						u16 const*						arg_2,
						const u32						arg_3,
						float4x4 const&					arg_4,
						material_effects_instance_ptr const&	arg_5
					) override { /* no source */ }

	inline	void	finalize_batch			( ) { /* no source */ }

	inline	void	invalidate				( ) { /* no source */ }

	/* 0x0000 */	/* batched_geometry_interface */
	/* 0x0004 */	vector< geometry_batch >			m_geometry_batches;
	/* 0x0010 */	vector< geometry_batch >			m_visible_geometry_batches[8];
	/* 0x0070 */	res_declaration_ptr					m_layout;
	/* 0x0074 */	u32									m_batched_geometry_max_vertices_count;
	/* 0x0078 */	u32									m_num_visible_batches[8];
	/* 0x0098 */	vector< shadow_vertex >				m_vertices;
	/* 0x00a4 */	vector< u16 >						m_indices;
	/* 0x00b0 */	math::aabb							m_bbox;
	/* 0x00c8 */	material_effects_instance_ptr		m_materail_effects_instance;
}; // struct batched_geometry< shadow_vertex >

STATIC_SIZE_ASSERT(batched_geometry< shadow_vertex >, 0xCC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_BATCHED_GEOMETRY_H_INCLUDED
