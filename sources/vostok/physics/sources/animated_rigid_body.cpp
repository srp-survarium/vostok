////////////////////////////////////////////////////////////////////////////
//	Created 	: 21.09.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/physics/animated_rigid_body.h>

#include "bullet_include.h"
#include <vostok/collision/primitives.h> // sushi@TODO: Should be imported in header only
#include <vostok/collision/bone_collision_data.h>
#include <vostok/collision/animated_object.h>

namespace vostok {
namespace physics {

// STATE[STUB]
bt_animated_rigid_body::bt_animated_rigid_body(
	btCompoundShape*                   shape,
	btRigidBody*                       body,
	u16								   game_material_id) :
	m_bt_body							(body),
	m_shape								(shape),
	m_game_material_id					(10) /* sushi@TODO: Either linker removed it, since used only with 10, or `game_material_id` argument is unused, which would be strange */
{
	body->setUserPointer(this); // <0x6bfe5a>|0x000|0x000:'25'
}
// STATE[STUB]
btRigidBody* bt_animated_rigid_body::get_rigid_body( )
{
	return m_bt_body;											// <0x6bf410>|0x000|0x000:'30'
}
// STATE[STUB]
u16 bt_animated_rigid_body::get_triangle_material( s32 triangle_id, bool is_shape_index ) const
{
	VOSTOK_UNREFERENCED_PARAMETERS ( triangle_id, is_shape_index );
	return m_game_material_id;									// <0x6bf4b0>|0x000|0x000:'36'
}
// STATE[STUB]
void bt_animated_rigid_body::apply_impulse( float3 const& impulse, float3 const& point_in_world)
{
	VOSTOK_UNREFERENCED_PARAMETERS ( impulse, point_in_world ); // sushi@NOTE: This function is empty
}
// STATE[STUB]
void bt_animated_rigid_body::set_transform( float4x4 const& transform )
{
	m_bt_body->setWorldTransform( from_vostok( transform ) );	// <0x6bf95b>|0x000|0x000:'46'
}
// STATE[STUB]
float4x4 bt_animated_rigid_body::get_transform( ) const
{
	return from_bullet( m_bt_body->getWorldTransform( ) );		// <0x6bf750>|0x000|0x000:'51'
}
// STATE[STUB]
void bt_animated_rigid_body::update_bone_matrix( u32 index, float4x4 const& new_transform, bool recalculate_aabb )
{
	btTransform new_child_transform = from_vostok( new_transform );					// <0x6bf92a>|0x000|0x000:'56'
	m_shape->updateChildTransform( index, new_child_transform, recalculate_aabb );	// <0x6bf933>|0x009|0x009:'57'
}
// STATE[STUB]
math::aabb bt_animated_rigid_body::get_aabb( ) const
{
	btVector3 aabbMin, aabbMax;
	m_bt_body->getAabb( aabbMin, aabbMax ); 												// <0x6bf4c9>|0x000|0x000:'63'
	return math::create_aabb_min_max ( from_bullet( aabbMin ), from_bullet( aabbMax ) );	// <0x6bf4e8>|0x01f|0x01f:'64'
}
// STATE[STUB]
float4x4 bt_animated_rigid_body::get_bone_transform( u32 index ) const
{	
	btTransform transform = m_shape->getChildTransform( index );	// <0x6bf730>|0x000|0x000:'69'
	return from_bullet ( transform );								// <0x6bf741>|0x011|0x011:'70'
}
// STATE[STUB]: sushi@TODO: Make it conform
btCollisionShape* new_bt_primitive( collision::primitive_type type, float3 const& dimension, memory::base_allocator* allocator )
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

// STATE[STUB]
// btCompoundShape* vostok::physics::new_bt_element_joint(vostok::configs::binary_config_value const&, vostok::memory::base_allocator*, vostok::collision::bone_collision_data*)
btCompoundShape* new_bt_element_joint(
	configs::binary_config_value const& target,
	memory::base_allocator*             allocator,
	collision::bone_collision_data*     data)
{
	return NULL;
	
	// LOCALS
	// float4x4                        child_transform
	// btTransform                     child_local_transform
	// float3                          rotation
	// float3                          scale
	// float3                          position
	// btCompoundShape*                bt_shape
	// ******

	// FUNCTION BODY
	// <0x6bf824>|0x000|0x000:'123'

	// <0x6bf848>|0x024|0x024:'125'
	// <0x6bf856>|0x032|0x00e:'126'
	// <0x6bf86b>|0x047|0x015:'127'
	// <0x6bf88e>|0x06a|0x023:'128'

	// <0x6bf8ad>|0x089|0x01f:'130'

	// <0x6bf8be>|0x09a|0x011:'132'

	// <0x6bf8fb>|0x0d7|0x03d:'134'
	// <0x6bf90b>|0x0e7|0x010:'135'


	// ******
}

// STATE[STUB]
// btCompoundShape* vostok::physics::new_compound_shape_from_hit_targets_config(vostok::configs::binary_config_value const&, vostok::buffer_vector<vostok::collision::bone_collision_data>&, vostok::memory::base_allocator*)
btCompoundShape* new_compound_shape_from_hit_targets_config(
	configs::binary_config_value const&            config,
	buffer_vector<collision::bone_collision_data>& geometries_data,
	memory::base_allocator*                        allocator)
{
	return NULL;

	// LOCALS
	// configs::binary_config_value const& targets_table
	// btCompoundShape*                bt_shape
	// bone_collision_data             data
	// btCompoundShape*                element_joint
	// btTransform                     joint_transform
	// ******

	// FUNCTION BODY
	// <0x6bf98f>|0x000|0x000:'146'
	// <0x6bf99b>|0x00c|0x00c:'147'




	// <0x6bf9af>|0x020|0x014:'152'

	// <0x6bf9e2>|0x053|0x033:'154'

	// <0x6bfa0b>|0x07c|0x029:'156'
	// <0x6bfae1>|0x152|0x0d6:'157'

	// <0x6bfb17>|0x188|0x036:'159'
	// <0x6bf9ec>|0x05d|-0x12b:'160'
	// <0x6bfc8c>|0x2fd|0x2a0:'161'



	// <0x6bfdfa>|0x46b|0x16e:'165'
	// ******
}

// STATE[STUB]
// unsigned int vostok::physics::calculate_bt_hit_target_size(vostok::configs::binary_config_value const&)
u32 calculate_bt_hit_target_size(
	configs::binary_config_value const& config)
{
	return 0;

	// STATICS
	// static <NoType>                  = <0x6bf6d0>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ae6be }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN5") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ae6c7 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// ******

	// FUNCTION BODY
	// <0x6bf6a3>|0x000|0x000:'170'

	// <0x6bf6b7>|0x014|0x014:'172'


	// <0x6bf6be>|0x01b|0x007:'175'








	// <0x6bf6c7>|0x024|0x009:'184'




	// ******
}

// STATE[STUB]
// unsigned int vostok::physics::calculate_bt_joint_size(vostok::configs::binary_config_value const&)
u32 calculate_bt_joint_size(
	configs::binary_config_value const& config)
{
	return 0;

	// STATICS
	// static <NoType>                  = <0x6bf718>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ae70a }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ae6fe }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN7") })
	// ******

	// FUNCTION BODY

	// <0x6bf6e3>|0x000|0x000:'194'


	// ******
}

// STATE[STUB]
// unsigned int vostok::physics::calculate_bt_animated_body_size_from_hit_targets_config(vostok::configs::binary_config_value const&)
u32 calculate_bt_animated_body_size_from_hit_targets_config(
	configs::binary_config_value const& config)
{
	return 0;

	// LOCALS
	// u32                             i
	// ******

	// STATICS
	// static <NoType>                  = <0x6bf800>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ae7da }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN61") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ae7d3 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN64") })
	// ******

	// FUNCTION BODY
	// <0x6bf777>|0x000|0x000:'201'
	// <0x6bf783>|0x00c|0x00c:'202'

	// <0x6bf7a1>|0x02a|0x01e:'204'

	// <0x6bf7a6>|0x02f|0x005:'206'
	// <0x6bf7b3>|0x03c|0x00d:'207'

	// <0x6bf7f4>|0x07d|0x041:'209'
	// ******
}

// STATE[STUB]
// vostok::physics::bt_animated_rigid_body* vostok::physics::new_animated_rigid_body(btCompoundShape*, unsigned short, vostok::memory::base_allocator*)
bt_animated_rigid_body* new_animated_rigid_body(
	btCompoundShape*                   shape,
	u16                                game_material_id,
	memory::base_allocator*            allocator)
{
	return NULL;

	// LOCALS
	// btRigidBody::btRigidBodyConstructionInfo info
	// btVector3                       local_inertia
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6aeeb2 }, type_index: TypeIndex(0x21095) })
	// ******

	// FUNCTION BODY
	// <0x6bfe7c>|0x000|0x000:'214'
	// <0x6bfe7f>|0x003|0x003:'215'

	// <0x6bfeb4>|0x038|0x035:'217'

	// <0x6bfec6>|0x04a|0x012:'219'
	// <0x6bff10>|0x094|0x04a:'220'
	// <0x6bff1a>|0x09e|0x00a:'221'

	// ******
}

// STATE[STUB]: sushi@TODO: Where is the header? Where is this guy used? no XREFs to this code
void destroy_animated_rigid_body( bt_animated_rigid_body* body, memory::base_allocator* allocator )
{
	VOSTOK_FREE_IMPL( allocator, body ); // <0x6bf475>|0x000|0x000:'227'
}

// STATE[STUB]
// vostok::collision::animated_object* vostok::physics::new_animated_bt_hit_model(vostok::configs::binary_config_value const&, vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, vostok::memory::base_allocator*)
collision::animated_object* new_animated_bt_hit_model(
	configs::binary_config_value const& config,
	resources::resource_ptr<animation::skeleton,resources::unmanaged_intrusive_base> const& model_skeleton,
	memory::base_allocator*            allocator)
{
	return NULL;

	// LOCALS
	// memory::stack_allocator         stack_allocator
	// ******

	// FUNCTION BODY

	// <0x6bff5e>|0x000|0x000:'237'

	// <0x6bff67>|0x009|0x009:'239'


	// <0x6bff77>|0x019|0x010:'242'

	// <0x6bffae>|0x050|0x037:'244'
	// <0x6bffb5>|0x057|0x007:'245'


	// ******
}

// STATE[STUB]
// unsigned short vostok::physics::bt_animated_rigid_body::get_collision_group() const
u16 bt_animated_rigid_body::get_collision_group( ) const
{
	return m_bt_body->getBroadphaseHandle()->m_collisionFilterGroup; // <0x6bf460>|0x000|0x000:'252'
}

// STATE[STUB]
float3 const& bt_animated_rigid_body::center_of_mass_offset( ) const
{
	static float3 offset( 0.0f, 0.0f, 0.0f );	// <0x6bf420>|0x000|0x000:'257'
	return offset;								// <0x6bf44e>|0x02e|0x02e:'258'
}

// STATE[STUB]
btCollisionObject* bt_animated_rigid_body::get_bt_collision_obect( )
{
	return m_bt_body;
}

} // namespace physics
} // namespace vostok