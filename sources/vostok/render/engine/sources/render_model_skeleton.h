#ifndef VOSTOK_RENDER_ENGINE_RENDER_MODEL_SKELETON_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDER_MODEL_SKELETON_H_INCLUDED

#include <vostok/render/core/memory.h>
#include "render_model.h"
#include "render_model_instance_impl.h"
#include "render_surface.h"
#include "render_surface_instance.h"

namespace vostok {

namespace memory {

class reader;

} // namespace memory

namespace render {

class shader_constant_host;
struct surface_stats;

class skeleton_render_surface : public render_surface {
public:
	skeleton_render_surface( );
	virtual void update( vector< float4x4 > const& bones ) = 0;
	virtual bool is_dynamic( ) const { return true; }
	virtual ~skeleton_render_surface( ) { }
};

STATIC_SIZE_ASSERT( skeleton_render_surface, 0x9C );

class skeleton_render_model : public render_model {
public:
	skeleton_render_model( );
	virtual ~skeleton_render_model( ) { }

	void load_bones( memory::reader& bones_chunk );
	void update( vector< float4x4 > const& bones );
	void get_bind_pose( float4x4* matrices, u32 count ) const;

	shader_constant_host*	m_bones_matrices_shader_constant;
	shader_constant_host*	m_prev_bones_matrices_shader_constant;
	vector< float4x4 >		m_inverted_bones_matrices_in_bind_pose;
};

STATIC_SIZE_ASSERT( skeleton_render_model, 0x150 );

typedef resources::resource_ptr<
	skeleton_render_model,
	resources::unmanaged_intrusive_base
> skeleton_render_model_ptr;

class skeleton_render_model_instance : public render_model_instance_impl {
public:
	typedef render_model_instance_impl super;

	skeleton_render_model_instance( );
	virtual ~skeleton_render_model_instance( );

	virtual void get_surfaces(
		float4x4 const*						mat_vp,
		float3 const*						view_pos,
		vector< render_surface_instance* >&	list,
		bool								visible_only,
		u8									lod_id,
		u32									surface_flags
	);

	virtual void update( );
	virtual void set_constants( );

	virtual math::aabb get_aabb( )
	{
		return math::create_identity_aabb( ) * 2;
	}

	void assign_original( skeleton_render_model_ptr v );
	void update_render_matrices( float4x4 const* matrices, u32 count );
	virtual bool get_locator( pcstr locator_name, model_locator_item& result ) const;

	virtual u32 get_surfaces_count( ) const
	{
		return m_instances_count;
	}

	virtual u32 get_surfaces_count( u32 ) const;
	virtual void get_surface_stats( u32 surface_id, surface_stats& stats ) const;
	virtual void get_bind_pose( float4x4* matrices, u32 count ) const;

	vector< float4x4 >			m_prev_bones_matrices;
	vector< float4x4 >			m_bones_matrices;
	skeleton_render_model_ptr	m_original;
	u8							m_instances_count;
	render_surface_instance*	m_surface_instances;
};

STATIC_SIZE_ASSERT( skeleton_render_model_instance, 0x1B0 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDER_MODEL_SKELETON_H_INCLUDED
