////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SKELETON_RENDER_MODEL_INSTANCE_H_INCLUDED
#define RENDER_SKELETON_RENDER_MODEL_INSTANCE_H_INCLUDED

/* INCLUDES */
class vostok::render::render_model_instance_impl;
class vostok::render::vector<vostok::math::float4x4>;
struct vostok::render::render_surface_instance;
class vostok::math::aabb;
typedef vostok::resources::resource_ptr<vostok::render::skeleton_render_model,vostok::resources::unmanaged_intrusive_base>
	vostok::render::skeleton_render_model_ptr;
class vostok::render::skeleton_render_model;

/* FORWARD REFS */
class vostok::render::model_locator_item;
class vostok::render::surface_stats;
class vostok::render::vector<vostok::render::render_surface_instance *>;

namespace vostok {
namespace render {

class skeleton_render_model_instance : public render_model_instance_impl {
public:
							skeleton_render_model_instance	( );
	virtual					~skeleton_render_model_instance	( );

	virtual	void			get_surfaces					(
								float4x4 const*							__formal,
								float3 const*							__formal,
								vector< render_surface_instance* >&		list,
								bool									visible_only,
								u8										__formal,
								u32										surface_flags
							) override;

	virtual	void			update							( ) override;

	virtual	void			set_constants					( ) override;

	virtual	math::aabb		get_aabb						( ) override { /* no source */ }

			void			assign_original					( skeleton_render_model_ptr v );

			void			update_render_matrices			( float4x4 const* matrices, u32 count );

	virtual	bool			get_locator						( pcstr locator_name, model_locator_item& result ) const override;
	virtual	u32				get_surfaces_count				( ) const override { /* no source */ }
	virtual	u32				get_surfaces_count				( u32 __formal ) const override;
	virtual	void			get_surface_stats				( u32 surface_id, surface_stats& __formal ) const override;
	virtual	void			get_bind_pose					( float4x4* matrices, u32 count ) const override;

	/* 0x0000 */	/* render_model_instance_impl */
	/* 0x0188 */	vector< float4x4 >				m_prev_bones_matrices;
	/* 0x0194 */	vector< float4x4 >				m_bones_matrices;
	/* 0x01a0 */	skeleton_render_model_ptr		m_original;
	/* 0x01a4 */	u8								m_instances_count;
	/* 0x01a8 */	render_surface_instance*		m_surface_instances;
}; // class skeleton_render_model_instance

STATIC_SIZE_ASSERT(skeleton_render_model_instance, 0x1B0);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SKELETON_RENDER_MODEL_INSTANCE_H_INCLUDED
