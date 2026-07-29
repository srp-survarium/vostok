////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RENDER_COLLISION_OBJECT_H_INCLUDED
#define RENDER_RENDER_COLLISION_OBJECT_H_INCLUDED

/* INCLUDES */
class vostok::collision::object;
class vostok::render::render_model_instance_impl;
class vostok::render::base_scene;

/* FORWARD REFS */
class fastdelegate::FastDelegate<bool __cdecl(vostok::collision::ray_triangle_result const &)>;
class vostok::math::aabb;
class vostok::math::cuboid;
class vostok::render::debug::renderer;
class vostok::vectora<vostok::collision::ray_triangle_result>;
class vostok::vectora<vostok::collision::triangle_result>;

namespace vostok {
namespace render {

class render_collision_object< render_model_instance_impl > : public collision::object {
public:
	inline	explicit						render_collision_object<vostok::render::render_model_instance_impl>( render_model_instance_impl* arg_0 ) { /* no source */ }

	virtual	void							render			( base_scene_ptr const& __formal, debug::renderer& __formal ) const override { /* no source */ }

	virtual	bool							aabb_query		( math::aabb const& __formal, vectora< collision::triangle_result >& __formal ) const override { /* no source */ }
	virtual	bool							cuboid_query	( math::cuboid const& arg_0, vectora< collision::triangle_result >& arg_1 ) const override { /* no source */ }
	virtual	bool							ray_query		(
												float3 const&		__formal,
												float3 const&		__formal,
												float				__formal,
												float&				__formal,
												vectora< collision::ray_triangle_result >&	__formal,
												fastdelegate::FastDelegate< bool( collision::ray_triangle_result const& ) > const&	__formal
											) const override { /* no source */ }

	virtual	bool							aabb_test		( math::aabb const& __formal ) const override { /* no source */ }
	virtual	bool							cuboid_test		( math::cuboid const& arg_0 ) const override { /* no source */ }
	virtual	bool							ray_test		(
												float3 const&		__formal,
												float3 const&		__formal,
												float				__formal,
												float&				__formal
											) const override { /* no source */ }

	virtual	void							add_triangles	( vectora< collision::triangle_result >& arg_0 ) const override { /* no source */ }

	virtual	math::aabb						update_aabb		( float4x4 const& arg_0 ) override { /* no source */ }

	inline	render_model_instance_impl*		owner			( ) const { /* no source */ }

	virtual									~render_collision_object<vostok::render::render_model_instance_impl>( ) { /* no source */ }

	/* 0x0000 */	/* collision::object */
private:
	/* 0x0030 */	render_model_instance_impl*		m_owner;
}; // class render_collision_object< render_model_instance_impl >

STATIC_SIZE_ASSERT(render_collision_object< render_model_instance_impl >, 0x34);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RENDER_COLLISION_OBJECT_H_INCLUDED
