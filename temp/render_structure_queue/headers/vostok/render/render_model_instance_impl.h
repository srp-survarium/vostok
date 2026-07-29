////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RENDER_MODEL_INSTANCE_IMPL_H_INCLUDED
#define RENDER_RENDER_MODEL_INSTANCE_IMPL_H_INCLUDED

/* INCLUDES */
class vostok::render::render_collision_object<vostok::render::render_model_instance_impl>;
class vostok::render::render_model_instance;
class vostok::math::aabb;

/* FORWARD REFS */
class vostok::render::vector<vostok::render::render_surface_instance *>;

namespace vostok {
namespace render {

class render_model_instance_impl : public render_model_instance {
public:
								render_model_instance_impl	( );

	virtual	void				set_constants				( ) { /* no source */ }

	virtual	math::aabb			get_aabb					( ) = 0;
	virtual	void				get_surfaces				(
									float4x4 const*							__formal,
									float3 const*							__formal,
									vector< render_surface_instance* >&		__formal,
									bool									__formal,
									u8										__formal,
									u32										surface_flags
								) { /* no source */ }

	virtual	void				set_transform				( float4x4 const& transform ) { /* no source */ }

	inline	float4x4 const&		transform					( ) const { /* no source */ }

	virtual						~render_model_instance_impl	( ) { /* no source */ }

	/* 0x0000 */	/* render_model_instance */
	/* 0x0110 */	render_collision_object< render_model_instance_impl >	m_collision_object;
protected:
	/* 0x0144 */	float4x4	m_transform;
}; // class render_model_instance_impl

STATIC_SIZE_ASSERT(render_model_instance_impl, 0x188);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RENDER_MODEL_INSTANCE_IMPL_H_INCLUDED
