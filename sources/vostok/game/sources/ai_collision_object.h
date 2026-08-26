////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef AI_COLLISION_OBJECT_H_INCLUDED
#define AI_COLLISION_OBJECT_H_INCLUDED

#include <vostok/collision/object.h>
#include <vostok/collision/geometry_instance.h>
#include <vostok/ai/collision_object.h>

namespace survarium {

class ai_collision_object : public collision::object , public ai::collision_object , private boost::noncopyable {
public:
	// ref member; the geometry overload builds an instance in the legacy code -
	// a matcher supplies the real source when a TU instantiates this
	inline											ai_collision_object	(
														collision::geometry*	arg_0,
														float4x4 const&			arg_1,
														ai::game_object&		arg_2,
														u32						arg_3,
														void*					arg_4
													) :
		m_geometry_instance( *( collision::geometry_instance* )NULL ),	// buildability placeholder
		m_ai_object( arg_2 ) { /* no source */ }
	inline											ai_collision_object	(
														collision::geometry_instance*		arg_0,
														ai::game_object&					arg_1,
														u32									arg_2,
														void*								arg_3
													) :
		m_geometry_instance( *arg_0 ),
		m_ai_object( arg_1 ) { /* no source */ }

	// PDB spells arg_0 vostok::render::base_scene_ptr - the same resource_ptr
	// type our render tree typedefs as scene_ptr
	virtual	void									render				( render::scene_ptr const& arg_0, render::debug::renderer& arg_1 ) const override { /* no source */ }

	virtual	bool									aabb_query			( math::aabb const& arg_0, vectora< collision::triangle_result >& arg_1 ) const override { /* no source */ return false; }
	virtual	bool									cuboid_query		( math::cuboid const& arg_0, vectora< collision::triangle_result >& arg_1 ) const override { /* no source */ return false; }
	virtual	bool									ray_query			(
														float3 const&		arg_0,
														float3 const&		arg_1,
														float				arg_2,
														float&				arg_3,
														vectora< collision::ray_triangle_result >&	arg_4,
														fastdelegate::FastDelegate< bool( collision::ray_triangle_result const& ) > const&	arg_5
													) const override { /* no source */ return false; }

	virtual	bool									aabb_test			( math::aabb const& arg_0 ) const override { /* no source */ return false; }
	virtual	bool									cuboid_test			( math::cuboid const& arg_0 ) const override { /* no source */ return false; }
	virtual	bool									ray_test			(
														float3 const&		arg_0,
														float3 const&		arg_1,
														float				arg_2,
														float&				arg_3
													) const override { /* no source */ return false; }

	virtual	void									add_triangles		( vectora< collision::triangle_result >& arg_0 ) const override { /* no source */ }

	virtual	math::aabb								update_aabb			( float4x4 const& arg_0 ) override { /* no source */ return get_aabb( ); }

	virtual	ai::game_object&						get_game_object		( ) const override { /* no source */ return m_ai_object; }
	virtual	float3									get_origin			( ) const override { /* no source */ return float3( 0.f, 0.f, 0.f ); }
	static	inline	ai_collision_object*			new_ai_geometry_object(
														memory::base_allocator*		arg_0,
														ai::game_object*			arg_1,
														const u32					arg_2,
														float4x4 const&				arg_3,
														collision::geometry*		arg_4,
														void*						arg_5
													) { /* no source */ return NULL; }
	static	inline	ai_collision_object*			new_ai_geometry_object(
														memory::base_allocator*				arg_0,
														ai::game_object*					arg_1,
														const u32							arg_2,
														collision::geometry_instance*		arg_3,
														void*								arg_4
													) { /* no source */ return NULL; }
	static	inline	void							delete_ai_collision_object( memory::base_allocator* arg_0, ai_collision_object* arg_1 ) { /* no source */ }

	inline	collision::geometry_instance const&		get_geom_instance	( ) const { /* no source */ return m_geometry_instance; }

private:
	/* 0x0000 */	/* collision::object */
	/* 0x0030 */	/* ai::collision_object */
	/* 0x0034 */	/* boost::noncopyable */
	/* 0x0034 */	collision::geometry_instance&		m_geometry_instance;
	/* 0x0038 */	ai::game_object&					m_ai_object;
}; // class ai_collision_object

STATIC_SIZE_ASSERT(ai_collision_object, 0x3C);

} // namespace survarium

#endif // #ifndef AI_COLLISION_OBJECT_H_INCLUDED
