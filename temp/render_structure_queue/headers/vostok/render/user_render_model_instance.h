////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_USER_RENDER_MODEL_INSTANCE_H_INCLUDED
#define RENDER_USER_RENDER_MODEL_INSTANCE_H_INCLUDED

/* INCLUDES */
class vostok::render::render_model_instance_impl;
class vostok::render::user_render_surface;
struct vostok::render::render_surface_instance;
class vostok::math::aabb;

/* FORWARD REFS */
class vostok::render::surface_stats;
class vostok::render::vector<vostok::render::render_surface_instance *>;

namespace vostok {
namespace render {

class user_render_model_instance : public render_model_instance_impl {
public:
	virtual	math::aabb		get_aabb					( ) override { /* no source */ }

			void			assign_surface				( user_render_surface* surface );

	virtual	void			get_surfaces				(
								float4x4 const*							mat_vp,
								float3 const*							view_pos,
								vector< render_surface_instance* >&		dest,
								bool									visible_only,
								u8										__formal,
								u32										surface_flags
							) override;
	virtual	u32				get_surfaces_count			( ) const override { /* no source */ }
	virtual	u32				get_surfaces_count			( u32 arg_0 ) const override { /* no source */ }
	virtual	void			get_surface_stats			( u32 surface_id, surface_stats& __formal ) const override;

	inline					user_render_model_instance	( ) { /* no source */ }
	virtual					~user_render_model_instance	( ) { /* no source */ }

	/* 0x0000 */	/* render_model_instance_impl */
	/* 0x0188 */	user_render_surface*		m_surface;
	/* 0x018c */	render_surface_instance		m_surface_instance;
}; // class user_render_model_instance

STATIC_SIZE_ASSERT(user_render_model_instance, 0x1A8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_USER_RENDER_MODEL_INSTANCE_H_INCLUDED
