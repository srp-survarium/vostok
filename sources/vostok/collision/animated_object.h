////////////////////////////////////////////////////////////////////////////
//	Created		: 14.04.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_COLLISION_ANIMATED_OBJECT_H_INCLUDED
#define VOSTOK_COLLISION_ANIMATED_OBJECT_H_INCLUDED

#include <vostok/collision/api.h>
#include <vostok/collision/bone_collision_data.h>
#include <vostok/physics/animated_rigid_body.h>

namespace vostok {

namespace collision {

class geometry;
struct game_object;

#pragma warning( push )
#pragma warning( disable : 4231 )
template class VOSTOK_CORE_API buffer_vector< bone_collision_data >;
#pragma warning( pop )

class VOSTOK_COLLISION_API animated_object
{
public:
			animated_object			(
				configs::binary_config_value const& config,
				animation::skeleton_ptr const& model_skeleton,
				u32 const bones_count,
				memory::stack_allocator& allocator
			);
			// sushi@NOTE: It seems like they added another inline constructor with `bool` at the end.
			// animated_object(configs::binary_config_value const&, animation::skeleton_ptr const&, u32, memory::stack_allocator&, bool)

			~animated_object		( );

			void	update			( float4x4 const* const bones_matrices_begin, float4x4 const* const bones_matrices_end );
			void	destroy			( memory::base_allocator* allocator );

			math::aabb	get_aabb	( ) const;

	inline	u32									get_bones_count	( ) const { return m_geometries_data.size(); }
	inline	physics::bt_animated_rigid_body*	get_rigid_body	( ) const { return m_body; }
	inline	void								set_owner		( game_object* owner ) { m_body->user_data = owner; } // sushi@TODO: What is set exactly?
	inline	geometry*							get_geometry	( ) const { return m_geometry; }

			pcstr	body_part_name			( u32 const bone_index ) const;

			float3	get_random_surface_point( u32 const current_time ) const;
			float3	get_head_bone_center	( ) const;
			float3	get_eyes_direction		( ) const;

private:
	typedef buffer_vector< bone_collision_data > bone_collisions_type;

private:
	bone_collisions_type				m_geometries_data;
	memory::stack_allocator				m_allocator;
	geometry*							m_geometry; // sushi@TODO: I need to figure out who can set `m_geometry` and when? It seems like in the target it is always `NULL` (though the optimizer didn't understand that).
	physics::bt_animated_rigid_body*	m_body;		// sushi@NOTE: We are owning this ptr
	u32									m_head_bone_index;
}; // class animated_object

STATIC_SIZE_ASSERT(animated_object, 0x2C);

typedef	resources::resource_ptr < animated_object, resources::unmanaged_intrusive_base > animated_object_ptr;

} // namespace collision
} // namespace vostok

#endif // #ifndef VOSTOK_COLLISION_ANIMATED_OBJECT_H_INCLUDED
