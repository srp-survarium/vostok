////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.08.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/physics/ghost_object.h>

#include "bullet_include.h"

namespace vostok {
namespace physics {

// survarium::collision_geometry::load
// vostok::physics::create_ghost_object
// STATE[UNVERIFIED]
bt_ghost_object::bt_ghost_object( bt_collision_shape_ptr shape, btPairCachingGhostObject* obj ):
	m_shape			( shape ),
	m_bt_object		( obj )
{
	obj->setUserPointer( this ); // <0x583e86>|0x000|0x000:'23'
}

// STATE[UNVERIFIED]
bt_ghost_object::~bt_ghost_object( )
{
	m_bt_object->~btPairCachingGhostObject(); // <0x583d97>|0x000|0x000:'28'
}

// STATE[STUB]
// void vostok::physics::bt_ghost_object::get_overlapping_objects(vostok::buffer_vector<vostok::physics::base_physics_object *>&) const
void bt_ghost_object::get_overlapping_objects(
	buffer_vector<base_physics_object *>& result) const
{
	// LOCALS
	// base_physics_object*            user_ptr
	// ******

	// FUNCTION BODY
	// <0x583875>|0x000|0x000:'33'

	// <0x583885>|0x010|0x010:'35'

	// <0x583890>|0x01b|0x00b:'37'
	// <0x5838a2>|0x02d|0x012:'38'

	// ******
}

// STATE[STUB]
u32 bt_ghost_object::get_overlapping_objects_count( ) const
{
	return m_bt_object->getNumOverlappingObjects(); // <0x583860>|0x000|0x000:'44'
}

// STATE[STUB]
// unsigned short vostok::physics::bt_ghost_object::get_collision_group() const
u16 bt_ghost_object::get_collision_group( ) const
{
	// ASSERT?
	return m_bt_object->getBroadphaseHandle()->m_collisionFilterGroup; 	// <0x583740>|0x000|0x000:'50'
}

// STATE[STUB]
// vostok::physics::bt_ghost_object* vostok::physics::create_ghost_object(vostok::resources::resource_ptr<vostok::physics::bt_collision_shape,vostok::resources::unmanaged_intrusive_base>, vostok::math::float4x4 const&)
bt_ghost_object* create_ghost_object(
	resources::resource_ptr<bt_collision_shape,resources::unmanaged_intrusive_base> shape,
	float4x4 const&                    transform)
{
	return NULL;

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x572f78 }, type_index: TypeIndex(0x1397d) })
	// ******

	// FUNCTION BODY
	// <0x583f26>|0x000|0x000:'55'

	// <0x583f4f>|0x029|0x029:'57'
	// <0x583f62>|0x03c|0x013:'58'

	// <0x583f7a>|0x054|0x018:'60'
	// ******
}

// STATE[STUB]
// void vostok::physics::destroy_ghost_object(vostok::physics::bt_ghost_object*)
void destroy_ghost_object(
	bt_ghost_object*                   obj)
{
	// FUNCTION BODY

	// <0x583ec1>|0x000|0x000:'66'
	// <0x583ef3>|0x032|0x032:'67'
	// ******
}

// STATE[STUB]
// void vostok::physics::bt_ghost_object::contact_test(vostok::physics::world*, vostok::physics::base_physics_object*, vostok::physics::contact_test_predicate&)
void bt_ghost_object::contact_test(
	world*                             world,
	base_physics_object*               object,
	contact_test_predicate&            predicate)
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x572cc9 }, type_index: TypeIndex(0x94b7) })
	// ******

	// FUNCTION BODY

	// <0x583cc3>|0x000|0x000:'73'
	// ******
}

// STATE[STUB]
// bool vostok::physics::bt_ghost_object::contact_test(vostok::physics::world*)
bool bt_ghost_object::contact_test(
	world*                             world)
{
	return true;

	// LOCALS
	// s32                             pairs_count
	// s32                             i
	// btAlignedObjectArray<btPersistentManifold *> manifold_results
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x572765 }, type_index: TypeIndex(0x15030) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5727a4 }, type_index: TypeIndex(0x15012) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5727cf }, type_index: TypeIndex(0x1385b) })
	// ******

	// FUNCTION BODY

	// <0x583753>|0x000|0x000:'79'
	// <0x58375c>|0x009|0x009:'80'

	// <0x583769>|0x016|0x00d:'82'
	// <0x58376c>|0x019|0x003:'83'


	// <0x583780>|0x02d|0x014:'86'
	// <0x5837a6>|0x053|0x026:'87'


	// <0x5837aa>|0x057|0x004:'90'


	// <0x5837af>|0x05c|0x005:'93'
	// <0x5837d1>|0x07e|0x022:'94'

	// <0x5837e0>|0x08d|0x00f:'96'
	// <0x58382c>|0x0d9|0x04c:'97'

	// <0x5837f0>|0x09d|-0x03c:'99'

	// <0x583823>|0x0d0|0x033:'101'
	// ******
}

// STATE[STUB]
// void vostok::physics::get_non_compound_shapes_centers(btCollisionShape*, btTransform const&, vostok::vectora<vostok::math::float3>&)
void get_non_compound_shapes_centers(
	btCollisionShape*                  shape,
	btTransform const&                 transform,
	vectora<float3>&                   centres_results)
{
	// LOCALS
	// float3                          center
	// btTransform                     shape_transform
	// ******

	// FUNCTION BODY
	// <0x5838c6>|0x000|0x000:'134'

	// <0x5838d8>|0x012|0x012:'136'
	// <0x5838e0>|0x01a|0x008:'137'




	// <0x583940>|0x07a|0x060:'142'
	// <0x583943>|0x07d|0x003:'143'

	// <0x58394b>|0x085|0x008:'145'
	// <0x583c68>|0x3a2|0x31d:'146'

	// ******
}

// STATE[STUB]
// void vostok::physics::bt_ghost_object::non_compound_shapes_centers(vostok::vectora<vostok::math::float3>&) const
void bt_ghost_object::non_compound_shapes_centers(
	vectora<float3>&                   centres_results)
{
	// FUNCTION BODY
	// <0x583d31>|0x000|0x000:'152'
	// <0x583d34>|0x003|0x003:'153'
	// <0x583d3f>|0x00e|0x00b:'154'

	// ******
}

// STATE[STUB]
// void vostok::physics::bt_ghost_object::insert(vostok::physics::world*, unsigned short, unsigned short)
void bt_ghost_object::insert(
	world*                             w,
	u16                                group,
	u16                                mask)
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x572733 }, type_index: TypeIndex(0x15eda) })
	// ******

	// FUNCTION BODY



	// ******
}

// STATE[STUB]
void bt_ghost_object::remove( world* w )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x572713 }, type_index: TypeIndex(0x21009) })
	// ******

	// FUNCTION BODY

	// ******
}

// STATE[STUB]
// btCollisionObject* vostok::physics::bt_ghost_object::get_bt_collision_obect()
btCollisionObject* bt_ghost_object::get_bt_collision_obect( )
{
	return NULL;

	// FUNCTION BODY
	// <0x5836f0>|0x000|0x000:'174'
	// ******
}

// STATE[STUB]
void bt_ghost_object::set_transform( float4x4 const& transform )
{
	m_bt_object->setWorldTransform( from_vostok( transform ) ); // <0x583d6a>|0x000|0x000:'179'
}

// STATE[STUB]
float4x4 bt_ghost_object::get_transform( ) const
{
	return from_bullet( m_bt_object->getWorldTransform() ); // <0x583d10>|0x000|0x000:'184'
}

} // namespace physics
} // namespace vostok