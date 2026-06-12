////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef AI_COLLISION_OBJECT_H_INCLUDED
#define AI_COLLISION_OBJECT_H_INCLUDED

/* INCLUDES */
class vostok::collision::geometry_instance;
class vostok::collision::object;
struct vostok::ai::collision_object;
struct vostok::ai::game_object;
class vostok::render::base_scene;

/* FORWARD REFS */
class fastdelegate::FastDelegate<bool __cdecl(vostok::collision::ray_triangle_result const &)>;
class vostok::collision::geometry;
class vostok::math::aabb;
class vostok::math::cuboid;
class vostok::memory::base_allocator;
class vostok::render::debug::renderer;
class vostok::vectora<vostok::collision::ray_triangle_result>;
class vostok::vectora<vostok::collision::triangle_result>;

namespace survarium {

class ai_collision_object : public collision::object , public ai::collision_object , public boost::noncopyable {
public:
	inline											ai_collision_object	(
														collision::geometry*	arg_0,
														float4x4 const&			arg_1,
														ai::game_object&		arg_2,
														u32						arg_3,
														void*					arg_4
													) { /* no source */ }
	inline											ai_collision_object	(
														collision::geometry_instance*		arg_0,
														ai::game_object&					arg_1,
														u32									arg_2,
														void*								arg_3
													) { /* no source */ }

	virtual	void									render				( render::base_scene_ptr const& arg_0, render::debug::renderer& arg_1 ) const override { /* no source */ }

	virtual	bool									aabb_query			( math::aabb const& arg_0, vectora< collision::triangle_result >& arg_1 ) const override { /* no source */ }
	virtual	bool									cuboid_query		( math::cuboid const& arg_0, vectora< collision::triangle_result >& arg_1 ) const override { /* no source */ }
	virtual	bool									ray_query			(
														float3 const&		arg_0,
														float3 const&		arg_1,
														float				arg_2,
														float&				arg_3,
														vectora< collision::ray_triangle_result >&	arg_4,
														fastdelegate::FastDelegate< bool( collision::ray_triangle_result const& ) > const&	arg_5
													) const override { /* no source */ }

	virtual	bool									aabb_test			( math::aabb const& arg_0 ) const override { /* no source */ }
	virtual	bool									cuboid_test			( math::cuboid const& arg_0 ) const override { /* no source */ }
	virtual	bool									ray_test			(
														float3 const&		arg_0,
														float3 const&		arg_1,
														float				arg_2,
														float&				arg_3
													) const override { /* no source */ }

	virtual	void									add_triangles		( vectora< collision::triangle_result >& arg_0 ) const override { /* no source */ }

	virtual	math::aabb								update_aabb			( float4x4 const& arg_0 ) override { /* no source */ }

	virtual	ai::game_object&						get_game_object		( ) const override { /* no source */ }
	virtual	float3									get_origin			( ) const override { /* no source */ }
	inline	collision::geometry_instance const&		get_geom_instance	( ) const { /* no source */ }

	virtual											~ai_collision_object( ) { /* no source */ }

	static	inline	ai_collision_object*					new_ai_geometry_object(
														memory::base_allocator*		arg_0,
														ai::game_object*			arg_1,
														const u32					arg_2,
														float4x4 const&				arg_3,
														collision::geometry*		arg_4,
														void*						arg_5
													) { /* no source */ }
	static	inline	ai_collision_object*					new_ai_geometry_object(
														memory::base_allocator*				arg_0,
														ai::game_object*					arg_1,
														const u32							arg_2,
														collision::geometry_instance*		arg_3,
														void*								arg_4
													) { /* no source */ }
	static	inline	void									delete_ai_collision_object( memory::base_allocator* arg_0, ai_collision_object* arg_1 ) { /* no source */ }

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
