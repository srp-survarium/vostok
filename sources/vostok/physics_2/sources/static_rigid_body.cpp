////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.08.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "static_rigid_body.h"

namespace vostok {
namespace physics {

// STATE[STUB]
// vostok::physics::bt_static_rigid_body::bt_static_rigid_body(vostok::resources::resource_ptr<vostok::physics::bt_collision_shape,vostok::resources::unmanaged_intrusive_base>, btRigidBody*)
bt_static_rigid_body::bt_static_rigid_body(
	resources::resource_ptr<bt_collision_shape,resources::unmanaged_intrusive_base> shape,
	btRigidBody*                       body)
{
	// FUNCTION BODY
	// <0x584294>|0x000|0x000:'41'
	// ******
}

// STATE[STUB]
// btRigidBody* vostok::physics::bt_static_rigid_body::get_rigid_body()
btRigidBody* bt_static_rigid_body::get_rigid_body( )
{
	return NULL;

	// FUNCTION BODY
	// <0x584000>|0x000|0x000:'51'
	// ******
}

// STATE[STUB]
// vostok::math::float4x4 vostok::physics::bt_static_rigid_body::get_transform() const
float4x4 bt_static_rigid_body::get_transform( )
{
	float4x4 result = float4x4();
	return result;

	// FUNCTION BODY




	// <0x584130>|0x000|0x000:'70'
	// ******
}

// STATE[STUB]
// void vostok::physics::bt_static_rigid_body::set_transform(vostok::math::float4x4 const&)
void bt_static_rigid_body::set_transform(
	float4x4 const&                    transform)
{
	// FUNCTION BODY




	// <0x58415b>|0x000|0x000:'79'
	// <0x584172>|0x017|0x017:'80'
	// ******
}

// STATE[STUB]
// void vostok::physics::bt_static_rigid_body::apply_impulse(vostok::math::float3 const&, vostok::math::float3 const&)
void bt_static_rigid_body::apply_impulse(
	float3 const&                      impulse,
	float3 const&                      point_in_world)
{
	// LOCALS
	// btVector3                       rel_pos
	// ******

	// FUNCTION BODY
	// <0x584079>|0x000|0x000:'85'
	// <0x584097>|0x01e|0x01e:'86'
	// <0x5840df>|0x066|0x048:'87'
	// ******
}

// STATE[STUB]
// unsigned short vostok::physics::bt_static_rigid_body::get_triangle_material(const int, const bool) const
u16 bt_static_rigid_body::get_triangle_material(
	s32                                triangle_id,
	bool                               is_shape_index)
{
	return 0;

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x57305f }, type_index: TypeIndex(0x7c27) })
	// ******

}

// STATE[STUB]
// unsigned short vostok::physics::bt_static_rigid_body::get_collision_group() const
u16 bt_static_rigid_body::get_collision_group( )
{
	return 0;

	// FUNCTION BODY
	// <0x584040>|0x000|0x000:'97'
	// ******
}

// STATE[STUB]
// btCollisionObject* vostok::physics::bt_static_rigid_body::get_bt_collision_obect()
btCollisionObject* bt_static_rigid_body::get_bt_collision_obect( )
{
	return NULL;

	// FUNCTION BODY
	// <0x583ff0>|0x000|0x000:'108'
	// ******
}

// STATE[STUB]
// void vostok::physics::destroy_static_rigid_body(vostok::physics::bt_static_rigid_body*)
void destroy_static_rigid_body(
	bt_static_rigid_body*              body)
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x573030 }, type_index: TypeIndex(0x94ed) })
	// ******

	// FUNCTION BODY
	// <0x584011>|0x000|0x000:'120'
	// ******
}

// STATE[STUB]
// vostok::physics::bt_static_rigid_body* vostok::physics::create_static_rigid_body(vostok::physics::bt_rigid_body_construction_info const&)
bt_static_rigid_body* create_static_rigid_body(
	bt_rigid_body_construction_info const& construction_info)
{
	return NULL;

	// LOCALS
	// resources::resource_ptr<bt_collision_shape,resources::unmanaged_intrusive_base> shape
	// btRigidBody::btRigidBodyConstructionInfo info
	// btVector3                       local_inertia
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x573361 }, type_index: TypeIndex(0x8238) })
	// ******

	// FUNCTION BODY

	// <0x5842dc>|0x000|0x000:'126'

	// <0x5842e4>|0x008|0x008:'128'

	// <0x58431b>|0x03f|0x037:'130'

	// <0x584342>|0x066|0x027:'132'
	// <0x584363>|0x087|0x021:'133'
	// <0x584365>|0x089|0x002:'134'






	// <0x58437d>|0x0a1|0x018:'141'



	// <0x58439f>|0x0c3|0x022:'145'














	// <0x5843a5>|0x0c9|0x006:'160'
	// <0x584405>|0x129|0x060:'161'
	// <0x584413>|0x137|0x00e:'162'



	// <0x58441f>|0x143|0x00c:'166'
	// <0x58448d>|0x1b1|0x06e:'167'
	// <0x5844d4>|0x1f8|0x047:'168'
	// ******
}

// STATE[STUB]
// void vostok::physics::bt_static_rigid_body::~bt_static_rigid_body()
void bt_static_rigid_body::~bt_static_rigid_body( )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5731c3 }, type_index: TypeIndex(0x16683) })
	// ******

	// FUNCTION BODY

	// <0x58419a>|0x000|0x000:'174'
	// ******
}

} // namespace physics
} // namespace vostok