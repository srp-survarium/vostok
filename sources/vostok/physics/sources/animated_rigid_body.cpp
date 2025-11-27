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

// STATE[98%|PARTIAL]: LTCG for `game_material_id`.
bt_animated_rigid_body::bt_animated_rigid_body(
	btCompoundShape*                   shape,
	btRigidBody*                       body,
	u16								   game_material_id) :
	m_bt_body							(body),
	m_shape								(shape),
	m_game_material_id					(game_material_id)
{
	body->setUserPointer(this);	// <0x6bfe5a>|0x000|0x000:'25'
}

// STATE[100%|DONE]
btRigidBody* bt_animated_rigid_body::get_rigid_body( )
{
	return m_bt_body;	// <0x6bf410>|0x000|0x000:'30'
}

// STATE[100%|DONE]
u16 bt_animated_rigid_body::get_triangle_material( s32 triangle_id, bool is_shape_index ) const
{
	VOSTOK_UNREFERENCED_PARAMETERS ( triangle_id, is_shape_index );
	return m_game_material_id;	// <0x6bf4b0>|0x000|0x000:'36'
}

// STATE[100%|DONE]
void bt_animated_rigid_body::apply_impulse( float3 const& impulse, float3 const& point_in_world)
{
	VOSTOK_UNREFERENCED_PARAMETERS ( impulse, point_in_world ); // sushi@NOTE: This function is empty
}

// STATE[100%|DONE]
void bt_animated_rigid_body::set_transform( float4x4 const& transform )
{
	m_bt_body->setWorldTransform( from_vostok( transform ) );	// <0x6bf95b>|0x000|0x000:'46'
}

// STATE[100%|DONE]
float4x4 bt_animated_rigid_body::get_transform( ) const
{
	return from_bullet( m_bt_body->getWorldTransform( ) );	// <0x6bf750>|0x000|0x000:'51'
}

// STATE[100%|DONE]
void bt_animated_rigid_body::update_bone_matrix( u32 index, float4x4 const& new_transform, bool recalculate_aabb )
{
	btTransform new_child_transform = from_vostok( new_transform );					// <0x6bf92a>|0x000|0x000:'56'
	m_shape->updateChildTransform( index, new_child_transform, recalculate_aabb );	// <0x6bf933>|0x009|0x009:'57'
}

// STATE[100%|DONE]
math::aabb bt_animated_rigid_body::get_aabb( ) const
{
	btVector3 aabbMin, aabbMax;
	m_bt_body->getAabb( aabbMin, aabbMax );													// <0x6bf4c9>|0x000|0x000:'63'
	return math::create_aabb_min_max ( from_bullet( aabbMin ), from_bullet( aabbMax ) );	// <0x6bf4e8>|0x01f|0x01f:'64'
}

// STATE[100%|DONE]
float4x4 bt_animated_rigid_body::get_bone_transform( u32 index ) const
{
	btTransform& transform = m_shape->getChildTransform( index );	// <0x6bf730>|0x000|0x000:'69'
	return from_bullet ( transform );								// <0x6bf741>|0x011|0x011:'70'
}

// STATE[STUB]: sushi@TODO: Make it conform to structure
static btCollisionShape* new_bt_primitive( collision::primitive_type type, float3 const& dimension, memory::base_allocator* allocator )
{
	switch ( type )	// <0x6bf579>|0x000|0x000:'86'
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

// STATE[90%|DONE]: LTCG is different for `binary_config_value::operator[]`, `new_bt_primitive`.
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

// STATE[99.54%]: sushi@TODO: Both reads and appends to `geometries_data`. Confusing. It just builds it, right?
btCompoundShape* new_compound_shape_from_hit_targets_config( configs::binary_config_value const& config, geometries_type& geometries_data, memory::base_allocator* allocator )
{
	configs::binary_config_value const& targets_table = config["hit_targets"];										// <0x6bf98f>|0x000|0x000:'146'
	u32 hit_targets_count = targets_table.size( );																	// <0x6bf99b>|0x00c|0x00c:'147'
																													// <1>..<4>
	btCompoundShape* bt_shape = VOSTOK_NEW_IMPL( allocator, btCompoundShape );										// <0x6bf9af>|0x020|0x014:'152'

	for ( u32 i = 0 ; i < hit_targets_count ; ++i )																	// <0x6bf9e2>|0x053|0x033:'154'
	{
		pcstr hit_param		= (pcstr)targets_table[i]["hit_param"];													// <0x6bfa0b>|0x07c|0x029:'156'
		pcstr animation_bone = (pcstr)targets_table[i]["animation_bone"];

		collision::bone_collision_data data( animation_bone, NULL, hit_param );
		geometries_data.push_back(data);
		btCompoundShape* element_joint = new_bt_element_joint( targets_table[i], allocator, &geometries_data[i] );	// <0x6bfb17>|0x188|0x036:'159'
		btTransform joint_transform( from_vostok( float4x4().identity() ) );										// <0x6bf9ec>|0x05d|-0x12b:'160'
		bt_shape->addChildShape( joint_transform, element_joint );													// <0x6bfc8c>|0x2fd|0x2a0:'161'
	}
	return bt_shape;																								// <0x6bfdfa>|0x46b|0x16e:'165'
}

// STATE[100%|DONE]
static u32 calculate_bt_hit_target_size( configs::binary_config_value const& config )
{
	collision::primitive_type type = (collision::primitive_type)(u32)config["type"];

	switch ( type )
	{
		case collision::primitive_sphere:
			return sizeof( btSphereShape );
		case collision::primitive_box:
			return sizeof( btBoxShape );
		case collision::primitive_cylinder:
			return sizeof( btCylinderShape );
		case collision::primitive_capsule:
			return sizeof( btCapsuleShape );
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

// STATE[98.38%|STUB]: sushi@NOTE: What are label symbols, figure out. TODO
u32 calculate_bt_animated_body_size_from_hit_targets_config( configs::binary_config_value const& config )
{
	configs::binary_config_value const& targets_table = config["hit_targets"];	// <0x6bf777>|0x000|0x000:'201'
	u32 hit_targets_count = targets_table.size( );								// <0x6bf783>|0x00c|0x00c:'202'

	u32 result = 0x70 * hit_targets_count + 0x60;								// <0x6bf7a1>|0x02a|0x01e:'204'

	for ( u32 i = 0 ; i < hit_targets_count ; ++i )								// <0x6bf7a6>|0x02f|0x005:'206'
		result += calculate_bt_hit_target_size(targets_table[i]) + 0x60;		// <0x6bf7b3>|0x03c|0x00d:'207'

	return result;																// <0x6bf7f4>|0x07d|0x041:'209'
}

// STATE[UNVERIFIED]: sushi@NOTE: Does it take ownership of shape?
bt_animated_rigid_body* new_animated_rigid_body(
	btCompoundShape*                   shape,
	u16                                game_material_id,
	memory::base_allocator*            allocator)
{
	btVector3	local_inertia( 0.f, 0.f, 0.f ); // xorps   xmm0, xmm0 ; right?
	shape->calculateLocalInertia( 0.f, local_inertia );

	btRigidBody::btRigidBodyConstructionInfo info( 0.f, NULL, shape, local_inertia ); // sushi@NOTE: In target linker remove btMotionState* argument and sets NULL in the function instead

	btRigidBody*			body		= VOSTOK_NEW_IMPL( allocator, btRigidBody )( info );
	body->setCollisionFlags(game_material_id);
	bt_animated_rigid_body* rigid_body	= VOSTOK_NEW_IMPL( allocator, bt_animated_rigid_body )( shape, body, 10 );
	return rigid_body;
}

// STATE[100%|DONE]: sushi@NOTE: There are two NEW calls in `new`, but only one DELETE here.
void destroy_animated_rigid_body( bt_animated_rigid_body* body, memory::base_allocator* allocator )
{
	VOSTOK_DELETE_IMPL( allocator, body );	// <0x6bf475>|0x000|0x000:'227'
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

// STATE[100%|DONE]
u16 bt_animated_rigid_body::get_collision_group( ) const
{
	return m_bt_body->getBroadphaseHandle()->m_collisionFilterGroup;	// <0x6bf460>|0x000|0x000:'252'
}

// STATE[100%|DONE]
float3 const& bt_animated_rigid_body::center_of_mass_offset( ) const
{
	static float3 offset( 0.0f, 0.0f, 0.0f );	// <0x6bf420>|0x000|0x000:'257'
	return offset;								// <0x6bf44e>|0x02e|0x02e:'258'
}

// STATE[100%|DONE]
btCollisionObject* bt_animated_rigid_body::get_bt_collision_obect( )
{
	return m_bt_body;
}

} // namespace physics
} // namespace vostok