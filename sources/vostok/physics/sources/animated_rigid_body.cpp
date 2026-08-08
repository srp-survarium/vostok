////////////////////////////////////////////////////////////////////////////
//	Created 	: 21.09.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/physics/animated_rigid_body.h>

#include "bullet_include.h"
#include <vostok/collision/primitives.h>
#include <vostok/collision/bone_collision_data.h>
#include <vostok/collision/animated_object.h>
#include <vostok/physics/bullet_utils.h>
#include <vostok/collision/hit_targets_creation_utils.h>

namespace vostok {
namespace physics {

bt_animated_rigid_body::bt_animated_rigid_body( btCompoundShape* shape, btRigidBody* body, u16 game_material_id ) :
	m_bt_body			( body ),
	m_shape				( shape ),
	m_game_material_id	( game_material_id )
{
	body->setUserPointer(this);
}

btRigidBody* bt_animated_rigid_body::get_rigid_body( )
{
	return m_bt_body;
}

u16 bt_animated_rigid_body::get_triangle_material( const s32 triangle_id, const bool is_shape_index ) const
{
	VOSTOK_UNREFERENCED_PARAMETERS ( triangle_id, is_shape_index );
	return m_game_material_id;
}

void bt_animated_rigid_body::apply_impulse( float3 const& impulse, float3 const& point_in_world)
{
	VOSTOK_UNREFERENCED_PARAMETERS ( impulse, point_in_world ); // sushi@NOTE: This function is empty
}

void bt_animated_rigid_body::set_transform( float4x4 const& transform )
{
	m_bt_body->setWorldTransform( from_vostok( transform ) );
}

float4x4 bt_animated_rigid_body::get_transform( ) const
{
	return from_bullet( m_bt_body->getWorldTransform( ) );
}

void bt_animated_rigid_body::update_bone_matrix( const u32 index, float4x4 const& new_transform, bool recalculate_aabb )
{
	btTransform new_child_transform = from_vostok( new_transform );
	m_shape->updateChildTransform( index, new_child_transform, recalculate_aabb );
}

math::aabb bt_animated_rigid_body::get_aabb( ) const
{
	btVector3 aabbMin, aabbMax;
	m_bt_body->getAabb( aabbMin, aabbMax );
	return math::create_aabb_min_max ( from_bullet( aabbMin ), from_bullet( aabbMax ) );
}

float4x4 bt_animated_rigid_body::get_bone_transform( const u32 index ) const
{
	btTransform& transform = m_shape->getChildTransform( index );
	return from_bullet ( transform );
}

static btCollisionShape* new_bt_primitive( const collision::primitive_type type, float3 const& dimension, memory::base_allocator* allocator )
{
	switch ( type )
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

btCompoundShape* new_compound_shape_from_hit_targets_config( configs::binary_config_value const& config, geometries_type& geometries_data, memory::base_allocator* allocator )
{
	configs::binary_config_value const& targets_table = config["hit_targets"];
	u32 hit_targets_count = targets_table.size( );
																													// <1>..<4>
	btCompoundShape* bt_shape = VOSTOK_NEW_IMPL( allocator, btCompoundShape );

	for ( u32 i = 0 ; i < hit_targets_count ; ++i )
	{
		pcstr hit_param		= (pcstr)targets_table[i]["hit_param"];
		pcstr animation_bone = (pcstr)targets_table[i]["animation_bone"];

		collision::bone_collision_data data( animation_bone, NULL, hit_param );
		geometries_data.push_back(data);
		btCompoundShape* element_joint = new_bt_element_joint( targets_table[i], allocator, &geometries_data[i] );
		btTransform joint_transform( from_vostok( float4x4().identity() ) );
		bt_shape->addChildShape( joint_transform, element_joint );
	}
	return bt_shape;
}

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

// The target retains this helper even though no shipped call site references it.
static u32 calculate_bt_joint_size( configs::binary_config_value const& config )
{
	collision::primitive_type type = (collision::primitive_type)(u32)config["type"];

	switch ( type )
	{
		case collision::primitive_sphere:
			return sizeof( btSphereShape ) + sizeof( btCompoundShape );
		case collision::primitive_box:
			return sizeof( btBoxShape ) + sizeof( btCompoundShape );
		case collision::primitive_cylinder:
			return sizeof( btCylinderShape ) + sizeof( btCompoundShape );
		case collision::primitive_capsule:
			return sizeof( btCapsuleShape ) + sizeof( btCompoundShape );
		default:
			NODEFAULT( );
	}
}

u32 calculate_bt_animated_body_size_from_hit_targets_config( configs::binary_config_value const& config )
{
	configs::binary_config_value const& targets_table = config["hit_targets"];
	u32 hit_targets_count = targets_table.size( );

	u32 result = sizeof( collision::bone_collision_data ) * hit_targets_count + sizeof( btCompoundShape );

	for ( u32 i = 0 ; i < hit_targets_count ; ++i )
		result += calculate_bt_hit_target_size(targets_table[i]) + sizeof( btCompoundShape );

	return result;
}

bt_animated_rigid_body* new_animated_rigid_body( btCompoundShape* shape, u16 game_material_id, memory::base_allocator* allocator )
{
	btVector3	local_inertia( 0.f, 0.f, 0.f );
	shape->calculateLocalInertia( 0.f, local_inertia );

	btRigidBody::btRigidBodyConstructionInfo info( 0.f, NULL, shape, local_inertia );

	btRigidBody*			body		= VOSTOK_NEW_IMPL( allocator, btRigidBody )( info );
	body->setCollisionFlags( btCollisionObject::CF_CHARACTER_OBJECT );
	bt_animated_rigid_body* rigid_body	= VOSTOK_NEW_IMPL( allocator, bt_animated_rigid_body )( shape, body, game_material_id );
	return rigid_body;
}

void destroy_animated_rigid_body( bt_animated_rigid_body* body, memory::base_allocator* allocator )
{
	VOSTOK_DELETE_IMPL( allocator, body );
}

collision::animated_object* new_animated_bt_hit_model(
	configs::binary_config_value const& config,
	animation::skeleton_ptr const&		model_skeleton,
	memory::base_allocator*				allocator)
{
	u32 const arena_size			=	calculate_bt_animated_body_size_from_hit_targets_config( config ) + 0x304; // sushi@TODO: Figure out where this constant is coming from
	pvoid const arena_ptr			=	VOSTOK_MALLOC_IMPL( allocator, arena_size, "collision::animated_object memory" );

	memory::stack_allocator				stack_allocator; // sushi@NOTE: Misnomer for bump allocator or arena?
	stack_allocator.initialize			( arena_ptr, arena_size, "collision::animated_object memory" );

	u32 bones_count = collision::get_bones_count_from_hit_targets_config<vostok::configs::binary_config_value>( config );
	collision::animated_object* object = VOSTOK_NEW_IMPL( stack_allocator, collision::animated_object )( config, model_skeleton, bones_count, stack_allocator );

	return object;
}

u16 bt_animated_rigid_body::get_collision_group( ) const
{
	return m_bt_body->getBroadphaseHandle()->m_collisionFilterGroup;
}

float3 const& bt_animated_rigid_body::center_of_mass_offset( ) const
{
	static float3 offset( 0.0f, 0.0f, 0.0f );
	return offset;
}

btCollisionObject* bt_animated_rigid_body::get_bt_collision_obect( )
{
	return m_bt_body;
}

} // namespace physics
} // namespace vostok
