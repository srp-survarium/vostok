////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "collision_geometry.h"

namespace survarium {

// STATE[STUB]
// survarium::collision_geometry::collision_geometry()
collision_geometry::collision_geometry( )
{
}

// STATE[STUB]
// void survarium::collision_geometry::~collision_geometry()
void collision_geometry::~collision_geometry( )
{
	// FUNCTION BODY
	// <0x6ff192>|0x000|0x000:'25'
	// <0x6ff19e>|0x00c|0x00c:'26'
	// <0x6ff1aa>|0x018|0x00c:'27'
	// ******
}

// STATE[STUB]
// void survarium::collision_geometry::destroy_ghost_object()
void collision_geometry::destroy_ghost_object( )
{
	// FUNCTION BODY
	// <0x6ff167>|0x000|0x000:'32'
	// ******
}

// STATE[STUB]
// void survarium::collision_geometry::load(vostok::configs::binary_config_value const&)
void collision_geometry::load( vostok::configs::binary_config_value const& cfg_val )
{
	// LOCALS
	// vostok::math::float4x4 		transform
	// vostok::resources::resource_ptr<vostok::physics::bt_collision_shape,vostok::resources::unmanaged_intrusive_base> shape
	// vostok::configs::binary_config_value meshes
	// vostok::math::float3 const& 	rotation
	// vostok::math::float3 const& 	scale
	// vostok::math::float3 const& 	position
	// ******

	// FUNCTION BODY
	// <0x6feefb>|0x000|0x000:'37'
	// <0x6fef16>|0x01b|0x01b:'38'
	// <0x6fef2b>|0x030|0x015:'39'
	// <0x6fef40>|0x045|0x015:'40'
	// <0x6fef55>|0x05a|0x015:'41'
	// 1
	// <0x6fefa8>|0x0ad|0x053:'43'
	// 1
	// <0x6fefc0>|0x0c5|0x018:'45'
	// <0x6ff049>|0x14e|0x089:'46'
	// 1
	// 2
	// <0x6ff04e>|0x153|0x005:'49'
	// <0x6ff06c>|0x171|0x01e:'50'
	// <0x6ff09c>|0x1a1|0x030:'51'
	// <0x6ff0dc>|0x1e1|0x040:'52'
	// <0x6ff100>|0x205|0x024:'53'
	// 1
	// <0x6ff10f>|0x214|0x00f:'55'
	// <0x6ff12e>|0x233|0x01f:'56'
	// ******
}

// STATE[STUB]
// unsigned int survarium::collision_geometry::get_overlapping_objects_count() const
u32 collision_geometry::get_overlapping_objects_count( ) const
{
	return 0;
	// FUNCTION BODY
	// <0x6fece9>|0x000|0x000:'61'
	// <0x6fecf5>|0x00c|0x00c:'62'
	// ******
}

// STATE[STUB]
// void survarium::collision_geometry::get_overlapping_objects(vostok::buffer_vector<vostok::physics::base_physics_object *>&) const
void collision_geometry::get_overlapping_objects( vostok::buffer_vector<vostok::physics::base_physics_object *>& result ) const
{
	// FUNCTION BODY
	// <0x6fecb9>|0x000|0x000:'67'
	// <0x6fecc5>|0x00c|0x00c:'68'
	// ******
}

// STATE[STUB]
// void survarium::collision_geometry::contact_test(vostok::physics::base_physics_object*, vostok::physics::contact_test_predicate&)
void collision_geometry::contact_test( vostok::physics::base_physics_object* object, vostok::physics::contact_test_predicate& predicate )
{
	// FUNCTION BODY
	// <0x6fee19>|0x000|0x000:'73'
	// <0x6fee25>|0x00c|0x00c:'74'
	// ******
}

// STATE[STUB]
// bool survarium::collision_geometry::contact_test()
bool collision_geometry::contact_test( )
{
	return false;
	// FUNCTION BODY
	// <0x6fec89>|0x000|0x000:'79'
	// <0x6fec95>|0x00c|0x00c:'80'
	// ******
}

// STATE[STUB]
// void survarium::collision_geometry::get_shapes_centers(vostok::vectora<vostok::math::float3>&) const
void collision_geometry::get_shapes_centers( vostok::vectora<vostok::math::float3>& centers_results ) const
{
	// FUNCTION BODY
	// <0x6fee89>|0x000|0x000:'91'
	// <0x6fee95>|0x00c|0x00c:'92'
	// ******
}

// STATE[STUB]
// void survarium::collision_geometry::insert(vostok::physics::world*)
void collision_geometry::insert( vostok::physics::world* world )
{
	// FUNCTION BODY
	// <0x6feb89>|0x000|0x000:'97'
	// <0x6feb92>|0x009|0x009:'98'
	// <0x6feb9e>|0x015|0x00c:'99'
	// <0x6febad>|0x024|0x00f:'100'
	// ******
}

// STATE[STUB]
// void survarium::collision_geometry::remove()
void collision_geometry::remove( )
{
	// FUNCTION BODY
	// <0x6feb49>|0x000|0x000:'105'
	// <0x6feb55>|0x00c|0x00c:'106'
	// <0x6feb6a>|0x021|0x015:'107'
	// ******
}

// STATE[STUB]
// void survarium::collision_geometry::subscribe(vostok::physics::world*, survarium::collision_geometry_subscriber*)
void collision_geometry::subscribe( vostok::physics::world* world, collision_geometry_subscriber* subscriber )
{
	// FUNCTION BODY
	// <0x6fedb9>|0x000|0x000:'112'
	// 1
	// <0x6fedc5>|0x00c|0x00c:'114'
	// <0x6fede5>|0x02c|0x020:'115'
	// 1
	// <0x6fedf1>|0x038|0x00c:'117'
	// ******
}

// STATE[STUB]
// void survarium::collision_geometry::unsubscribe(survarium::collision_geometry_subscriber*)
void collision_geometry::unsubscribe( collision_geometry_subscriber* subscriber )
{
	// FUNCTION BODY
	// <0x6febe9>|0x000|0x000:'122'
	// <0x6febf5>|0x00c|0x00c:'123'
	// 1
	// <0x6fec4a>|0x061|0x055:'125'
	// <0x6fec6a>|0x081|0x020:'126'
	// ******
}

// STATE[STUB]
// void survarium::collision_geometry::set_transform(vostok::math::float4x4 const&)
void collision_geometry::set_transform( vostok::math::float4x4 const& transform )
{
	// FUNCTION BODY
	// <0x6feeb8>|0x000|0x000:'132'
	// ******
}

// STATE[STUB]
// vostok::math::float4x4 survarium::collision_geometry::get_transform()
vostok::math::float4x4 collision_geometry::get_transform( )
{
	return vostok::math::float4x4();
	// FUNCTION BODY
	// <0x6fee57>|0x000|0x000:'136'
	// ******
}

	/* TYPEDEFS

	typedef
		void**
		iterator_type;

} // namespace survarium
