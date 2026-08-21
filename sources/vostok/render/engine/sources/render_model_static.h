#ifndef VOSTOK_RENDER_ENGINE_RENDER_MODEL_STATIC_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDER_MODEL_STATIC_H_INCLUDED

#include <vostok/configs_binary_config_value.h>
#include "render_model.h"
#include "render_model_instance_impl.h"
#include "render_surface.h"
#include "render_surface_instance.h"

namespace vostok {
namespace render {

struct batched_geometry_interface;
struct surface_stats;

class static_render_surface : public render_surface {
public:
	typedef render_surface super;

	explicit static_render_surface( bool colored );
	virtual ~static_render_surface( ) { }

	virtual void load( configs::binary_config_value const& properties, memory::chunk_reader& chunk );
	virtual bool is_dynamic( ) const
	{
		return false;
	}
	virtual void fill_lpv_vertex_color( batched_geometry_interface* in_out_geometry, float4x4 const& transform );
	virtual void add_shadow_vertices( batched_geometry_interface* in_out_geometry, float4x4 const& transform );

	void create_material_pass_geometry( ) { }
	void create_shadow_pass_geometry( pcbyte data, u32 const num_vertices, u32 const stride );
};

STATIC_SIZE_ASSERT( static_render_surface, 0x9C );

class static_render_model : public render_model {
	typedef render_model super;

public:
	static_render_model( ) { }
};

STATIC_SIZE_ASSERT( static_render_model, 0x138 );

typedef resources::resource_ptr<
	static_render_model,
	resources::unmanaged_intrusive_base
> static_render_model_ptr;

class static_render_model_instance : public render_model_instance_impl {
public:
	typedef render_model_instance_impl super;

	static_render_model_instance( );
	virtual ~static_render_model_instance( );

	virtual math::aabb get_aabb( )
	{
		return m_original->m_aabbox;
	}

	virtual void get_surfaces(
		float4x4 const*						mat_vp,
		float3 const*						view_pos,
		vector< render_surface_instance* >&	list,
		bool								visible_only,
		u8									lod_id,
		u32									surface_flags
	);

	void assign_original( static_render_model_ptr v );
	virtual bool get_locator( pcstr locator_name, model_locator_item& result ) const;

	virtual u32 get_surfaces_count( ) const
	{
		return m_instances_count;
	}

	virtual u32 get_surfaces_count( u32 lod_id ) const;
	virtual void get_surface_stats( u32 surface_id, surface_stats& stats ) const;
	void add_sectors_holder( configs::binary_config_value sectotrs_cfg );
	virtual void set_lod_params( u8 type, bool use_default, float p0, float p1, float p2 );

protected:
	u8 select_lod( float4x4 const& mat_vp, float3 const& view_pos );

	static_render_model_ptr		m_original;
	u8							m_instances_count;
	render_surface_instance*	m_surface_instances;
};

STATIC_SIZE_ASSERT( static_render_model_instance, 0x198 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDER_MODEL_STATIC_H_INCLUDED
