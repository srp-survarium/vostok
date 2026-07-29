////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_DECAL_SHADER_CONSTANTS_AND_GEOMETRY_H_INCLUDED
#define RENDER_DECAL_SHADER_CONSTANTS_AND_GEOMETRY_H_INCLUDED

/* INCLUDES */
class vostok::quasi_singleton<vostok::render::decal_shader_constants_and_geometry>;
class vostok::render::shader_constant_host;
typedef vostok::intrusive_ptr<vostok::render::res_geometry,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_geometry_ptr;
typedef vostok::intrusive_ptr<vostok::render::untyped_buffer,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::untyped_buffer_ptr;
class vostok::render::res_geometry;
class vostok::render::untyped_buffer;

/* FORWARD REFS */
class vostok::render::renderer_context;

namespace vostok {
namespace render {

class decal_shader_constants_and_geometry : public quasi_singleton< decal_shader_constants_and_geometry > {
public:
					decal_shader_constants_and_geometry	( );

			void	set									(
						renderer_context*		context,
						float4x4 const&			world_to_decal_matrix,
						float4x4 const&			decal_tangent_to_view_space_matrix,
						float					alpha_angle,
						float					clip_angle,
						float3 const&			decal_width_height_far_distance,
						float4x4 const&			decal_transform
					);

			void	set_geometry						( );
private:
			void	create_decal_geometry				( );
public:
	inline			~decal_shader_constants_and_geometry( ) { /* no source */ }

	/* 0x0000 */	/* quasi_singleton< decal_shader_constants_and_geometry > */
private:
	/* 0x0000 */	shader_constant_host*		m_world_to_decal_parameter;
	/* 0x0004 */	shader_constant_host*		m_eye_ray_corner_parameter;
	/* 0x0008 */	shader_constant_host*		m_decal_tangent_to_view_space_matrix_parameter;
	/* 0x000c */	shader_constant_host*		m_decal_angle_parameters;
	/* 0x0010 */	untyped_buffer_ptr			m_decal_vertex_buffer;
	/* 0x0014 */	untyped_buffer_ptr			m_decal_index_buffer;
	/* 0x0018 */	res_geometry_ptr			m_decal_geometry;
}; // class decal_shader_constants_and_geometry

STATIC_SIZE_ASSERT(decal_shader_constants_and_geometry, 0x1C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_DECAL_SHADER_CONSTANTS_AND_GEOMETRY_H_INCLUDED
