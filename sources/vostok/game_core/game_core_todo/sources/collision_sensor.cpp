////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "collision_sensor.h"

namespace survarium {

// STATE[STUB]
// survarium::collision_sensor::collision_sensor()
collision_sensor::collision_sensor( )
{
}

// STATE[STUB]
// void survarium::collision_sensor::~collision_sensor()
void collision_sensor::~collision_sensor( )
{
	// FUNCTION BODY
	// <0x59af2c>|0x000|0x000:'24'
	// ******
}

// STATE[STUB]
// void survarium::collision_sensor::load(vostok::configs::binary_config_value const&)
void collision_sensor::load( vostok::configs::binary_config_value const& cfg )
{
	// LOCALS
	// vostok::configs::binary_config_value collision_table
	// ******

	// FUNCTION BODY
	// <0x59b01a>|0x000|0x000:'29'
	// <0x59b04a>|0x030|0x030:'30'
	// <0x59b058>|0x03e|0x00e:'31'
	// 1
	// <0x59b064>|0x04a|0x00c:'33'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// ******
}

// STATE[STUB]
// void survarium::collision_sensor::resolve_links(survarium::base_project*, vostok::configs::binary_config_value)
void collision_sensor::resolve_links( base_project* p, vostok::configs::binary_config_value cfg )
{
	// LOCALS
	// vostok::configs::binary_config_value collision_table
	// u32 							i<1>
	// pcstr 						geom_name<2>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x589924 }, type_index: TypeIndex(0x94fb) })
	// ******

	// FUNCTION BODY
	// <0x59a8a9>|0x000|0x000:'47'
	// <0x59a8d9>|0x030|0x030|[1]:'48'
	// 1
	// <0x59a8f6>|0x04d|0x01d|[2]:'50'
	// <0x59a915>|0x06c|0x01f:'51'
	// 1
	// ******
}

// STATE[STUB]
// bool survarium::remove_loosed_ptrs_predicate(vostok::physics::base_physics_object*)
bool remove_loosed_ptrs_predicate( vostok::physics::base_physics_object* object )
{
	return false;
	// FUNCTION BODY
	// <0x59a203>|0x000|0x000:'90'
	// ******
}

// STATE[STUB]
// void survarium::collision_sensor::tick(const unsigned int, const unsigned int)
void collision_sensor::tick( u32 time_delta_ms, u32 current_time_ms )
{
	// LOCALS
	// u32 							old_objects_count
	// u32 							objects_count
	// u32 							all_sensed_objects_count
	// vostok::buffer_vector<vostok::physics::base_physics_object *> sensed_objects
	// vostok::buffer_vector<vostok::physics::base_physics_object *> all_sensed_objects
	// u32 							i<1>
	// vostok::buffer_vector<vostok::physics::base_physics_object *> sensed_objects<2>
	// u32 							i<2>
	// u32 							i<3>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x589cac }, type_index: TypeIndex(0xada4) })
	// ******

	// FUNCTION BODY
	// <0x59ab90>|0x000|0x000:'95'
	// 1
	// <0x59abb3>|0x023|0x023:'97'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// <0x59abc4>|0x034|0x011:'106'
	// <0x59ac7e>|0x0ee|0x0ba:'107'
	// <0x59ac91>|0x101|0x013:'108'
	// 1
	// <0x59acae>|0x11e|0x01d:'110'
	// <0x59acb5>|0x125|0x007|[1]:'111'
	// <0x59acd5>|0x145|0x020:'112'
	// 1
	// <0x59acf1>|0x161|0x01c:'114'
	// 1
	// <0x59acf7>|0x167|0x006|[2]:'116'
	// <0x59ad12>|0x182|0x01b:'117'
	// <0x59ad21>|0x191|0x00f:'118'
	// 1
	// 2
	// <0x59ad47>|0x1b7|0x026:'121'
	// <0x59ad72>|0x1e2|0x02b|[2]:'122'
	// <0x59ad92>|0x202|0x020:'123'
	// 1
	// <0x59adac>|0x21c|0x01a:'125'
	// 1
	// <0x59adbb>|0x22b|0x00f:'127'
	// <0x59adc7>|0x237|0x00c:'128'
	// 1
	// <0x59adfb>|0x26b|0x034|[3]:'130'
	// 1
	// <0x59ae15>|0x285|0x01a:'132'
	// <0x59ae51>|0x2c1|0x03c:'133'
	// <0x59ae75>|0x2e5|0x024:'134'
	// 1
	// <0x59ae77>|0x2e7|0x002:'136'
	// <0x59ae86>|0x2f6|0x00f:'137'
	// <0x59ae91>|0x301|0x00b:'138'
	// ******
}

// STATE[STUB]
// bool survarium::collision_sensor::is_filter_passed(vostok::physics::base_physics_object*) const
bool collision_sensor::is_filter_passed( vostok::physics::base_physics_object* object ) const
{
	return false;
	// FUNCTION BODY
	// <0x59a1e7>|0x000|0x000:'143'
	// ******
}

// STATE[STUB]
// bool survarium::left_objects_predicate::operator()(vostok::physics::base_physics_object*) const
bool left_objects_predicate::operator()( vostok::physics::base_physics_object* obj ) const
{
	// LOCALS
	// bool 						result
	// ******

	return false;
	// FUNCTION BODY
	// <0xc6619>|0x000|0x000:'156'
	// <0xc665c>|0x043|0x043:'157'
	// <0xc6664>|0x04b|0x008:'158'
	// 1
	// <0xc6668>|0x04f|0x004:'160'
	// <0xc6677>|0x05e|0x00f:'161'
	// ******
}

// STATE[STUB]
// void survarium::collision_sensor::notify_and_erase_left_objects(vostok::buffer_vector<vostok::physics::base_physics_object *>&)
void collision_sensor::notify_and_erase_left_objects( vostok::buffer_vector<vostok::physics::base_physics_object *>& sensed_objects )
{
	// LOCALS
	// vostok::buffer_vector<vostok::physics::base_physics_object *> objects_to_delete
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5897ff }, type_index: TypeIndex(0xada0) })
	// ******

	// FUNCTION BODY
	// <0x59a6a0>|0x000|0x000:'171'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// <0x59a709>|0x069|0x069:'179'
	// 1
	// <0x59a7d8>|0x138|0x0cf:'181'
	// <0x59a7ea>|0x14a|0x012:'182'
	// ******
}

// STATE[STUB]
// void survarium::collision_sensor::notify_and_add_incoming_objects(vostok::buffer_vector<vostok::physics::base_physics_object *>&)
void collision_sensor::notify_and_add_incoming_objects( vostok::buffer_vector<vostok::physics::base_physics_object *>& sensed_objects )
{
	// LOCALS
	// vostok::physics::base_physics_object** inc_end
	// vostok::buffer_vector<vostok::physics::base_physics_object *> incoming_objects
	// vostok::physics::base_physics_object** end
	// vostok::physics::base_physics_object** inc_it
	// vostok::physics::base_physics_object** it
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x589b15 }, type_index: TypeIndex(0xada0) })
	// ******

	// FUNCTION BODY
	// <0x59aa09>|0x000|0x000:'187'
	// <0x59aa50>|0x047|0x047:'188'
	// <0x59aa58>|0x04f|0x008:'189'
	// <0x59aa61>|0x058|0x009:'190'
	// <0x59aa78>|0x06f|0x017:'191'
	// <0x59aae8>|0x0df|0x070:'192'
	// 1
	// <0x59aaf4>|0x0eb|0x00c:'194'
	// <0x59ab06>|0x0fd|0x012:'195'
	// 1
	// <0x59ab17>|0x10e|0x011:'197'
	// <0x59ab1d>|0x114|0x006:'198'
	// 1
	// <0x59ab23>|0x11a|0x006:'200'
	// <0x59ab36>|0x12d|0x013:'201'
	// ******
}

// STATE[STUB]
// void survarium::collision_sensor::notify_objects_inside()
void collision_sensor::notify_objects_inside( )
{
	// LOCALS
	// vostok::buffer_vector<vostok::physics::base_physics_object *> objects_inside
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5892dd }, type_index: TypeIndex(0xada0) })
	// ******

	// FUNCTION BODY
	// <0x59a249>|0x000|0x000:'206'
	// <0x59a28a>|0x041|0x041:'207'
	// <0x59a2ce>|0x085|0x044:'208'
	// ******
}

// STATE[STUB]
// void survarium::collision_sensor::filter_sensed_objects(vostok::buffer_vector<vostok::physics::base_physics_object *>&)
void collision_sensor::filter_sensed_objects( vostok::buffer_vector<vostok::physics::base_physics_object *>& sensed_objects )
{
	// FUNCTION BODY
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// <0x59a31f>|0x000|0x000:'220'
	// 1
	// <0x59a36f>|0x050|0x050:'222'
	// 1
	// 2
	// 3
	// 4
	// <0x59a390>|0x071|0x021:'227'
	// ******
}

// STATE[STUB]
// void survarium::collision_sensor::contact_test(vostok::physics::base_physics_object*, vostok::physics::contact_test_predicate&)
void collision_sensor::contact_test( vostok::physics::base_physics_object* object, vostok::physics::contact_test_predicate& predicate )
{
	// LOCALS
	// u32 							i<1>
	// ******

	// FUNCTION BODY
	// <0x59a969>|0x000|0x000|[1]:'232'
	// <0x59a986>|0x01d|0x01d:'233'
	// ******
}

// STATE[STUB]
// bool survarium::collision_sensor::contact_test(vostok::physics::base_physics_object*)
bool collision_sensor::contact_test( vostok::physics::base_physics_object* __formal )
{
	// LOCALS
	// u32 							i<1>
	// ******

	return false;
	// FUNCTION BODY
	// <0x59a649>|0x000|0x000|[1]:'238'
	// 1
	// <0x59a666>|0x01d|0x01d:'240'
	// <0x59a67e>|0x035|0x018:'241'
	// <0x59a682>|0x039|0x004:'242'
	// <0x59a684>|0x03b|0x002:'243'
	// ******
}

// STATE[STUB]
// void survarium::collision_sensor::get_shapes_centers(vostok::vectora<vostok::math::float3>&) const
void collision_sensor::get_shapes_centers( vostok::vectora<vostok::math::float3>& centers ) const
{
	// LOCALS
	// u32 							i<1>
	// ******

	// FUNCTION BODY
	// <0x59a9b9>|0x000|0x000|[1]:'254'
	// <0x59a9d6>|0x01d|0x01d:'255'
	// ******
}

// STATE[STUB]
// void survarium::collision_sensor::insert(vostok::physics::world*)
void collision_sensor::insert( vostok::physics::world* world )
{
	// LOCALS
	// u32 							i<1>
	// ******

	// FUNCTION BODY
	// <0x59a849>|0x000|0x000:'260'
	// <0x59a855>|0x00c|0x00c:'261'
	// 1
	// <0x59a85c>|0x013|0x007|[1]:'263'
	// <0x59a879>|0x030|0x01d:'264'
	// ******
}

// STATE[STUB]
// void survarium::collision_sensor::remove()
void collision_sensor::remove( )
{
	// LOCALS
	// vostok::buffer_vector<vostok::physics::base_physics_object *> leaved
	// u32 							i<1>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5895ee }, type_index: TypeIndex(0xada0) })
	// ******

	// FUNCTION BODY
	// <0x59a3f1>|0x000|0x000:'269'
	// <0x59a3fd>|0x00c|0x00c:'270'
	// 1
	// <0x59a407>|0x016|0x00a|[1]:'272'
	// <0x59a427>|0x036|0x020:'273'
	// 1
	// <0x59a444>|0x053|0x01d:'275'
	// <0x59a467>|0x076|0x023:'276'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// <0x59a46c>|0x07b|0x005:'286'
	// 1
	// <0x59a526>|0x135|0x0ba:'288'
	// <0x59a58f>|0x19e|0x069:'289'
	// <0x59a5d9>|0x1e8|0x04a:'290'
	// <0x59a5f0>|0x1ff|0x017:'291'
	// ******
}

// STATE[STUB]
// survarium::collision_geometry* survarium::collision_sensor::get_collision_geometry(unsigned int)
collision_geometry* collision_sensor::get_collision_geometry( u32 index )
{
	return NULL;
	// FUNCTION BODY
	// <0x59a219>|0x000|0x000:'296'
	// <0x59a225>|0x00c|0x00c:'297'
	// ******
}

	/* TYPEDEFS

	typedef
		base_project::resolve_link_object*
		iterator_type;

	typedef
		void**
		iterator_type;

} // namespace survarium
