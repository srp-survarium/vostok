////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STATIC_RENDER_MODEL_INSTANCE_H_INCLUDED
#define RENDER_STATIC_RENDER_MODEL_INSTANCE_H_INCLUDED

/* INCLUDES */
class vostok::render::render_model_instance_impl;
struct vostok::render::render_surface_instance;
class vostok::configs::binary_config_value;
class vostok::math::aabb;
typedef vostok::resources::resource_ptr<vostok::render::static_render_model,vostok::resources::unmanaged_intrusive_base>
	vostok::render::static_render_model_ptr;
class vostok::render::static_render_model;

/* FORWARD REFS */
class vostok::render::model_locator_item;
class vostok::render::surface_stats;
class vostok::render::vector<vostok::render::render_surface_instance *>;

namespace vostok {
namespace render {

class static_render_model_instance : public render_model_instance_impl {
public:
							static_render_model_instance	( );
	virtual					~static_render_model_instance	( );

	virtual	math::aabb		get_aabb						( ) override { /* no source */ }
	virtual	void			get_surfaces					(
								float4x4 const*							mat_vp,
								float3 const*							view_pos,
								vector< render_surface_instance* >&		list,
								bool									visible_only,
								u8										lod_id,
								u32										surface_flags
							) override;

			void			assign_original					( static_render_model_ptr v );

	virtual	bool			get_locator						( pcstr locator_name, model_locator_item& result ) const override;
	virtual	u32				get_surfaces_count				( ) const override { /* no source */ }
	virtual	u32				get_surfaces_count				( u32 lod_id ) const override;
	virtual	void			get_surface_stats				( u32 surface_id, surface_stats& stats ) const override;

			void			add_sectors_holder				( configs::binary_config_value sectotrs_cfg );

	virtual	void			set_lod_params					(
								u8			type,
								bool		use_default,
								float		p0,
								float		p1,
								float		p2
							) override;

protected:
			u8				select_lod						( float4x4 const& mat_vp, float3 const& view_pos );

	/* 0x0000 */	/* render_model_instance_impl */
	/* 0x0188 */	static_render_model_ptr		m_original;
	/* 0x018c */	u8							m_instances_count;
	/* 0x0190 */	render_surface_instance*	m_surface_instances;
}; // class static_render_model_instance

STATIC_SIZE_ASSERT(static_render_model_instance, 0x198);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STATIC_RENDER_MODEL_INSTANCE_H_INCLUDED
