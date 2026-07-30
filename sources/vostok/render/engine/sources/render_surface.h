#ifndef VOSTOK_RENDER_ENGINE_RENDER_SURFACE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDER_SURFACE_H_INCLUDED

#include <vostok/math_aabb.h>
#include <vostok/math_sphere.h>
#include <vostok/render/facade/vertex_input_type.h>
#include "material_effects_instance.h"
#include "render_geometry.h"

namespace vostok {

namespace configs {

class binary_config_value;

} // namespace configs

namespace memory {

class chunk_reader;

} // namespace memory

namespace render {

struct batched_geometry_interface;
struct material_effects;

class render_surface : public boost::noncopyable {
public:
	render_surface( ) :
		m_aabbox( math::create_zero_aabb( ) )
	{
		// FUNCTION BODY[0x6f360]
	}
	virtual ~render_surface( );

	enum_vertex_input_type get_vertex_input_type( ) { return m_vertex_input_type; }

	virtual void load( configs::binary_config_value const& properties, memory::chunk_reader& chunk );
	void set_material_effects( material_effects_instance_ptr mtl_instance_ptr, pcstr material_name );
	void set_default_material( );
	virtual bool is_dynamic( ) const = 0;

	virtual void fill_lpv_vertex_color( batched_geometry_interface*, float4x4 const& ) { }
	virtual void add_shadow_vertices( batched_geometry_interface*, float4x4 const& ) { }

	material_effects& get_material_effects( );

	enum_vertex_input_type			m_vertex_input_type;
	math::aabb						m_aabbox;
	math::sphere					m_bounding_sphere;
	render_geometry					m_render_geometry;
	material_effects_instance_ptr	m_materail_effects_instance;
	float							m_streaming_texture_factor;
};

STATIC_SIZE_ASSERT( render_surface, 0x9C );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDER_SURFACE_H_INCLUDED
