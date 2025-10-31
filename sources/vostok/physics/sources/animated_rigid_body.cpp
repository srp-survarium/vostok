////////////////////////////////////////////////////////////////////////////
//	Created 	: 21.09.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/physics/animated_rigid_body.h>

#include "bullet_include.h"
#include <vostok/collision/primitives.h> // sushi@TODO: Should be imported in header only
#include <vostok/collision/bone_collision_data.h>
#include <vostok/collision/animated_object.h>
#include <vostok/collision/shell_creation_utils.h>
#include <vostok/physics/bullet_utils.h>

namespace vostok {
namespace physics {

// STATE[PARTIAL:98%]: Linker removed `game_material_id`.
bt_animated_rigid_body::bt_animated_rigid_body(
	btCompoundShape*                   shape,
	btRigidBody*                       body,
	u16								   game_material_id) :
	m_bt_body							(body),
	m_shape								(shape),
	m_game_material_id					(game_material_id)
{
	body->setUserPointer(this); // <0x6bfe5a>|0x000|0x000:'25'
}
// STATE[DONE]
btRigidBody* bt_animated_rigid_body::get_rigid_body( )
{
	return m_bt_body;											// <0x6bf410>|0x000|0x000:'30'
}
// STATE[DONE]
u16 bt_animated_rigid_body::get_triangle_material( s32 triangle_id, bool is_shape_index ) const
{
	VOSTOK_UNREFERENCED_PARAMETERS ( triangle_id, is_shape_index );
	return m_game_material_id;									// <0x6bf4b0>|0x000|0x000:'36'
}
// STATE[DONE]
void bt_animated_rigid_body::apply_impulse( float3 const& impulse, float3 const& point_in_world)
{
	VOSTOK_UNREFERENCED_PARAMETERS ( impulse, point_in_world ); // sushi@NOTE: This function is empty
}
// STATE[PARTIAL: 66%]: Looks like linker optimizations
void bt_animated_rigid_body::set_transform( float4x4 const& transform )
{
	m_bt_body->setWorldTransform( from_vostok( transform ) );	// <0x6bf95b>|0x000|0x000:'46'
}
// STATE[DONE]
float4x4 bt_animated_rigid_body::get_transform( ) const
{
	return from_bullet( m_bt_body->getWorldTransform( ) );		// <0x6bf750>|0x000|0x000:'51'
}
// STATE[PARTIAL: 69%]: Looks like linker optimizations
void bt_animated_rigid_body::update_bone_matrix( u32 index, float4x4 const& new_transform, bool recalculate_aabb )
{
	btTransform new_child_transform = from_vostok( new_transform );					// <0x6bf92a>|0x000|0x000:'56'
	m_shape->updateChildTransform( index, new_child_transform, recalculate_aabb );	// <0x6bf933>|0x009|0x009:'57'
}
// STATE[DONE]
math::aabb bt_animated_rigid_body::get_aabb( ) const
{
	btVector3 aabbMin, aabbMax;
	m_bt_body->getAabb( aabbMin, aabbMax ); 												// <0x6bf4c9>|0x000|0x000:'63'
	return math::create_aabb_min_max ( from_bullet( aabbMin ), from_bullet( aabbMax ) );	// <0x6bf4e8>|0x01f|0x01f:'64'
}
// STATE[DONE]
float4x4 bt_animated_rigid_body::get_bone_transform( u32 index ) const
{
	btTransform& transform = m_shape->getChildTransform( index );	// <0x6bf730>|0x000|0x000:'69'
	return from_bullet ( transform );								// <0x6bf741>|0x011|0x011:'70'
}
// STATE[UNVERIFIED]: sushi@TODO: Make it conform to structure
static btCollisionShape* new_bt_primitive( collision::primitive_type type, float3 const& dimension, memory::base_allocator* allocator )
{
	switch ( type ) // <0x6bf579>|0x000|0x000:'86'
	{
		case collision::primitive_sphere:
		{
			float radius = dimension.x;
			return VOSTOK_NEW_IMPL( allocator, btSphereShape )( radius );
		}
		case collision::primitive_box:
		{
			btVector3 half_extents = from_vostok( dimension );
			return VOSTOK_NEW_IMPL( allocator, btBoxShape )( half_extents );
		}
		case collision::primitive_cylinder:
		{
			btVector3 half_extents = from_vostok( dimension );
			return VOSTOK_NEW_IMPL( allocator, btCylinderShape )( half_extents );
		}
		case collision::primitive_capsule:
		{
			float radius = dimension.x;
			float height = dimension.y;
			return VOSTOK_NEW_IMPL( allocator, btCapsuleShape )( radius, height );
		}
		default:
			NODEFAULT( );
	}
}

// STATE[UNVERIFIED]
static btCompoundShape* new_bt_element_joint( configs::binary_config_value const& target, memory::base_allocator* allocator, collision::bone_collision_data* data )
{
	btCompoundShape* bt_shape = VOSTOK_NEW_IMPL( allocator, btCompoundShape );

	collision::primitive_type type = (collision::primitive_type)(u32)target["type"];
	float3 position = (float3)target["position"];
	float3 rotation = (float3)target["rotation"];
	float3 scale    = (float3)target["scale"];

	btCollisionShape* shape = new_bt_primitive( type, scale, allocator );
	shape->setUserPointer( data );

	float4x4	child_transform			= create_rotation( rotation ) * create_translation( position );
	btTransform child_local_transform	= from_vostok( child_transform );

	bt_shape->addChildShape( child_local_transform, shape );

	return bt_shape;
}

// STATE[UNVERIFIED]: sushi@NOTE: Verify. Fills in `geometries_data`?
btCompoundShape* new_compound_shape_from_hit_targets_config( configs::binary_config_value const& config, geometries_type& geometries_data, memory::base_allocator* allocator )
{
	configs::binary_config_value const& targets_table = config["hit_targets"];
	u32 hit_targets_count = 24 * targets_table.count / 24;

	btCompoundShape* bt_shape = VOSTOK_NEW_IMPL( allocator, btCompoundShape );

	for ( u32 i = 0 ; i < hit_targets_count ; ++i )
	{
		pcstr hit_param		= (pcstr)targets_table[i]["hit_param"];
		pcstr animation_bone = (pcstr)targets_table[i]["animation_bone"];

		collision::bone_collision_data data( animation_bone, NULL, hit_param );
		geometries_data[i] = data;
		btCompoundShape* element_joint = new_bt_element_joint( targets_table[i], allocator, &data ); // something got assigned to (3) zero + mov of hit_targets_count
		bt_shape->addChildShape( from_vostok( float4x4().identity() ), element_joint );
	}
	return bt_shape;
}

// STATE[UNVERIFIED]
static u32 calculate_bt_hit_target_size( configs::binary_config_value const& config )
{
	collision::primitive_type type = (collision::primitive_type)(u32)config["type"];

	switch ( type )
	{
		case collision::primitive_sphere:
			return sizeof(btSphereShape);
		case collision::primitive_box:
			return sizeof(btBoxShape);
		case collision::primitive_cylinder:
			return sizeof(btCylinderShape);
		case collision::primitive_capsule:
			return sizeof(btCapsuleShape);
		default:
			NODEFAULT( );
	}
}

// STATE[UNVERIFIED]: sushi@TODO: I don't know what those sizes are
static u32 calculate_bt_joint_size( configs::binary_config_value const& config )
{
	collision::primitive_type type = (collision::primitive_type)(u32)config["type"];

	switch ( type )
	{
		case collision::primitive_sphere:
			return 0xA0;
		case collision::primitive_box:
			return 0xB0;
		case collision::primitive_cylinder:
			return 0xB0;
		case collision::primitive_capsule:
			return 0xB0;
		default:
			NODEFAULT( );
	}
}

// STATE[UNVERIFIED]: sushi@NOTE: What are label symbols, figure out.
u32 calculate_bt_animated_body_size_from_hit_targets_config( configs::binary_config_value const& config )
{
	configs::binary_config_value const& targets_table = config["hit_targets"];	// <0x6bf777>|0x000|0x000:'201'
	u32 hit_targets_count = 24 * targets_table.count / 24;						// <0x6bf783>|0x00c|0x00c:'202'

	u32 result = 112 * hit_targets_count + 96;									// <0x6bf7a1>|0x02a|0x01e:'204'

	for ( u32 i = 0 ; i < hit_targets_count ; ++i )								// <0x6bf7a6>|0x02f|0x005:'206'
		result += calculate_bt_hit_target_size(targets_table[i]) + 96;			// <0x6bf7b3>|0x03c|0x00d:'207'

	return result;																// <0x6bf7f4>|0x07d|0x041:'209'
}

// STATE[UNVERIFIED]: sushi@NOTE: Does it take ownership of shape?
bt_animated_rigid_body* new_animated_rigid_body(
	btCompoundShape*                   shape,
	u16                                game_material_id,
	memory::base_allocator*            allocator)
{
	btVector3	local_inertia(0.f, 0.f, 0.f); // xorps   xmm0, xmm0 ; right?
	shape->calculateLocalInertia( 0.f, local_inertia );

	btRigidBody::btRigidBodyConstructionInfo info( 0.f, NULL, shape, local_inertia ); // sushi@NOTE: In target linker remove btMotionState* argument and sets NULL in the function instead

	btRigidBody*			body		= VOSTOK_NEW_IMPL( allocator, btRigidBody )( info );
	body->setCollisionFlags(game_material_id);
	bt_animated_rigid_body* rigid_body	= VOSTOK_NEW_IMPL( allocator, bt_animated_rigid_body )( shape, body, 10 );
	return rigid_body;
}

// STATE[UNVERIFIED]: sushi@NOTE: It is used, but is being inlined in `animated_object` destructor.
// sushi@TODO: What is concerning, however, is that there are two NEW calls in `new`, but only one DELETE here.
// The target structure shows that there is only a single statement, however.
void destroy_animated_rigid_body( bt_animated_rigid_body* body, memory::base_allocator* allocator )
{
	VOSTOK_DELETE_IMPL( allocator, body ); // <0x6bf475>|0x000|0x000:'227'
}

// STATE[UNVERIFIED]
collision::animated_object* new_animated_bt_hit_model(
	configs::binary_config_value const& config,
	animation::skeleton_ptr const&		model_skeleton,
	memory::base_allocator*				allocator)
{
	memory::stack_allocator				stack_allocator; // sushi@NOTE: Why is this stack allocator? Aren't it in the heap
	u32 const arena_size			=	calculate_bt_animated_body_size_from_hit_targets_config( config ) + 0x304;

	pvoid const arena				=	VOSTOK_MALLOC_IMPL( allocator, arena_size, "collision::animated_object memory" );


	stack_allocator.initialize			( arena, arena_size, "collision::animated_object memory" );
	// sushi@TODO: Incorrect method: get_bones_count_from_hit_targets_config
	u32 bones_count = collision::get_bones_count_from_physics_shell_config<vostok::configs::binary_config_value>(config);
	collision::animated_object* object = VOSTOK_NEW_IMPL( stack_allocator, collision::animated_object )( config, model_skeleton, bones_count, stack_allocator );

	return object;
}

// STATE[DONE]
u16 bt_animated_rigid_body::get_collision_group( ) const
{
	return m_bt_body->getBroadphaseHandle()->m_collisionFilterGroup; // <0x6bf460>|0x000|0x000:'252'
}

// STATE[DONE]
float3 const& bt_animated_rigid_body::center_of_mass_offset( ) const
{
	static float3 offset( 0.0f, 0.0f, 0.0f );	// <0x6bf420>|0x000|0x000:'257'
	return offset;								// <0x6bf44e>|0x02e|0x02e:'258'
}

// STATE[DONE]
btCollisionObject* bt_animated_rigid_body::get_bt_collision_obect( )
{
	return m_bt_body;
}

} // namespace physics
} // namespace vostok