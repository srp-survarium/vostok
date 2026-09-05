// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef AI_COLLISION_OBJECT_H_INCLUDED
#define AI_COLLISION_OBJECT_H_INCLUDED

#include <vostok/collision/object.h>
#include <vostok/collision/geometry_instance.h>
#include <vostok/ai/collision_object.h>
#include "game_memory.h"

namespace survarium {

// sushi@TODO: Legacy geometry-wrapper models; recover retail construction and query consumers.
class ai_collision_object : public collision::object , public ai::collision_object , private boost::noncopyable {
public:
	// sushi@TODO: Verify the global inner-geometry allocator versus the factory's object allocator.
	inline											ai_collision_object	(
														collision::geometry*	geometry,
														float4x4 const&			transform,
														ai::game_object&		ai_object,
														u32						object_type,
														void*					user_data
													) :
		m_geometry_instance( *collision::new_composite_geometry_instance( g_allocator, transform, geometry ) ),
		m_ai_object( ai_object )
	{
		m_user_data = user_data;
		set_type( object_type );
	}
	// sushi@TODO: Verify whether the supplied instance transfers deletion ownership to this wrapper.
	inline											ai_collision_object	(
														collision::geometry_instance*		instance,
														ai::game_object&					ai_object,
														u32									object_type,
														void*								user_data
													) :
		m_geometry_instance( *instance ),
		m_ai_object( ai_object )
	{
		m_user_data = user_data;
		set_type( object_type );
	}

	// PDB spells arg_0 vostok::render::base_scene_ptr - the same resource_ptr
	// type our render tree typedefs as scene_ptr
	virtual	void									render				( render::scene_ptr const& scene, render::debug::renderer& renderer ) const override { m_geometry_instance.render( scene, renderer ); }

	virtual	bool									aabb_query			( math::aabb const& aabb, vectora< collision::triangle_result >& triangles ) const override { return m_geometry_instance.aabb_query( this, aabb, triangles ); }
	virtual	bool									cuboid_query		( math::cuboid const& cuboid, vectora< collision::triangle_result >& triangles ) const override { return m_geometry_instance.cuboid_query( this, cuboid, triangles ); }
	virtual	bool									ray_query			(
														float3 const&		origin,
														float3 const&		direction,
														float				max_distance,
														float&				distance,
														vectora< collision::ray_triangle_result >&	triangles,
														fastdelegate::FastDelegate< bool( collision::ray_triangle_result const& ) > const&	predicate
													) const override { return m_geometry_instance.ray_query( this, origin, direction, max_distance, distance, triangles, predicate ); }

	virtual	bool									aabb_test			( math::aabb const& aabb ) const override { return m_geometry_instance.aabb_test( aabb ); }
	virtual	bool									cuboid_test			( math::cuboid const& cuboid ) const override { return m_geometry_instance.cuboid_test( cuboid ); }
	virtual	bool									ray_test			(
														float3 const&		origin,
														float3 const&		direction,
														float				max_distance,
														float&				distance
													) const override { return m_geometry_instance.ray_test( origin, direction, max_distance, distance ); }

	virtual	void									add_triangles		( vectora< collision::triangle_result >& triangles ) const override { m_geometry_instance.add_triangles( triangles ); }

	virtual	math::aabb								update_aabb			( float4x4 const& local_to_world ) override
	{
		m_geometry_instance.set_matrix( local_to_world );
		return m_geometry_instance.get_aabb( );
	}

	virtual	ai::game_object&						get_game_object		( ) const override { return m_ai_object; }
	virtual	float3									get_origin			( ) const override { return get_aabb( ).center( ); }
	static	inline	ai_collision_object*			new_ai_geometry_object(
														memory::base_allocator*		allocator,
														ai::game_object*			ai_game_object,
														const u32					object_type,
														float4x4 const&				matrix,
														collision::geometry*		geometry,
														void*						user_data
													) { return VOSTOK_NEW_IMPL( allocator, ai_collision_object )( geometry, matrix, *ai_game_object, object_type, user_data ); }
	static	inline	ai_collision_object*			new_ai_geometry_object(
														memory::base_allocator*				allocator,
														ai::game_object*					ai_game_object,
														const u32							object_type,
														collision::geometry_instance*		instance,
														void*								user_data
													) { return VOSTOK_NEW_IMPL( allocator, ai_collision_object )( instance, *ai_game_object, object_type, user_data ); }
	// sushi@TODO: Legacy explicit geometry cleanup; verify retail allocator and instance-ownership policy.
	static	inline	void							delete_ai_collision_object( memory::base_allocator* allocator, ai_collision_object* object )
	{
		if ( !allocator || !object )
			return;
		collision::delete_geometry_instance( allocator, &object->m_geometry_instance );
		VOSTOK_DELETE_IMPL( allocator, object );
	}

	inline	collision::geometry_instance const&		get_geom_instance	( ) const { return m_geometry_instance; }

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
