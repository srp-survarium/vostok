////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "damage_zone_core.h"

namespace survarium {

// STATE[STUB]
// survarium::hit_receiver_info::hit_receiver_info(survarium::hit_receiver*, vostok::physics::base_physics_object*)
hit_receiver_info::hit_receiver_info( hit_receiver* receiver, vostok::physics::base_physics_object* rigid_body )
{
}

// STATE[STUB]
// bool survarium::hit_receiver_info::operator==(survarium::hit_receiver_info const&) const
bool hit_receiver_info::operator==( hit_receiver_info const& rhs ) const
{
	return false;
	// FUNCTION BODY
	// <0x597d87>|0x000|0x000:'28'
	// ******
}

// STATE[STUB]
// survarium::damage_zone_core::damage_zone_core()
damage_zone_core::damage_zone_core( )
{
}

// STATE[STUB]
// void survarium::damage_zone_core::~damage_zone_core()
void damage_zone_core::~damage_zone_core( )
{
}

// STATE[STUB]
// void survarium::damage_zone_core::load(vostok::configs::binary_config_value const&)
void damage_zone_core::load( vostok::configs::binary_config_value const& t )
{
	// LOCALS
	// vostok::configs::binary_config_value const* end
	// vostok::configs::binary_config_value bone_parts_filter
	// vostok::configs::binary_config_value const* it
	// ******

	// FUNCTION BODY
	// <0x5994e0>|0x000|0x000:'50'
	// 1
	// <0x5994ef>|0x00f|0x00f:'52'
	// <0x599531>|0x051|0x042:'53'
	// <0x599573>|0x093|0x042:'54'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// 10
	// 11
	// 12
	// 13
	// 14
	// 15
	// 16
	// 17
	// 18
	// 19
	// <0x5995b5>|0x0d5|0x042:'74'
	// <0x5995d6>|0x0f6|0x021:'75'
	// <0x5995f6>|0x116|0x020:'76'
	// <0x599616>|0x136|0x020:'77'
	// <0x599636>|0x156|0x020:'78'
	// <0x599656>|0x176|0x020:'79'
	// <0x599674>|0x194|0x01e:'80'
	// 1
	// 2
	// <0x599692>|0x1b2|0x01e:'83'
	// <0x5996c2>|0x1e2|0x030:'84'
	// <0x5996cd>|0x1ed|0x00b:'85'
	// <0x5996d8>|0x1f8|0x00b:'86'
	// <0x5996eb>|0x20b|0x013:'87'
	// ******
}

// STATE[STUB]
// bool survarium::compare_bone_data_predicate(stlp_std::pair<vostok::collision::bone_collision_data *,float> const&, stlp_std::pair<vostok::collision::bone_collision_data *,float> const&)
bool compare_bone_data_predicate( std::pair<vostok::collision::bone_collision_data *,float> const& lhs, std::pair<vostok::collision::bone_collision_data *,float> const& rhs )
{
	return false;
	// FUNCTION BODY
	// <0x597d63>|0x000|0x000:'118'
	// ******
}

// STATE[STUB]
// float survarium::distance_from_sphere_center_to_point_on_shape(float)
float distance_from_sphere_center_to_point_on_shape( float radius )
{
	return 0.0f;
	// FUNCTION BODY
	// <0x597d53>|0x000|0x000:'123'
	// ******
}

// STATE[STUB]
// float survarium::distance_from_box_center_to_point_on_shape(vostok::math::float4x4 const&, vostok::math::float3 const&, vostok::math::float3 const&)
float distance_from_box_center_to_point_on_shape( vostok::math::float4x4 const& transform, vostok::math::float3 const& dim, vostok::math::float3 const& source_position )
{
	// LOCALS
	// vostok::math::float3 		dir
	// vostok::math::float3 		result
	// vostok::math::float3 		half_sides
	// s32 							i<1>
	// float 						dist<2>
	// vostok::math::float3 		axis<2>
	// ******

	// SKIPPED BLOCKS
	// <0x598538><2>
	// ******

	return 0.0f;
	// FUNCTION BODY
	// <0x5984d6>|0x000|0x000:'128'
	// <0x5984f2>|0x01c|0x01c:'129'
	// <0x59850e>|0x038|0x01c:'130'
	// 1
	// <0x598522>|0x04c|0x014|[1]:'132'
	// 1
	// <0x59853e>|0x068|0x01c:'134'
	// <0x59855d>|0x087|0x01f:'135'
	// <0x598565>|0x08f|0x008:'136'
	// 1
	// <0x598575>|0x09f|0x010:'138'
	// <0x59858a>|0x0b4|0x015:'139'
	// <0x59859e>|0x0c8|0x014:'140'
	// <0x5985ba>|0x0e4|0x01c:'141'
	// 1
	// <0x5985d5>|0x0ff|0x01b:'143'
	// <0x5985f1>|0x11b|0x01c:'144'
	// 1
	// <0x5985f6>|0x120|0x005:'146'
	// ******
}

// STATE[STUB]
// float survarium::distance_from_capsule_center_to_point_on_shape(vostok::math::float4x4 const&, float, float, vostok::math::float3 const&)
float distance_from_capsule_center_to_point_on_shape(
	vostok::math::float4x4 const&		transform,
	float								half_length,
	float								radius,
	vostok::math::float3 const&			source_position)
{
	// LOCALS
	// float 						proj_to_y_axis
	// vostok::math::float3 		bottom_surface_center
	// vostok::math::float3 		y_axis
	// vostok::math::float3 		surface_center
	// vostok::math::float3 		top_surface_center
	// vostok::math::float3 		height_vector
	// vostok::math::float3 		center
	// vostok::math::float3 		dir<1>
	// vostok::math::float3 		height_vector_proj_point<1>
	// ******

	// SKIPPED BLOCKS
	// <0x59839a><1>
	// ******

	return 0.0f;
	// FUNCTION BODY
	// <0x5982a9>|0x000|0x000:'151'
	// <0x5982c5>|0x01c|0x01c:'152'
	// <0x5982e1>|0x038|0x01c:'153'
	// <0x598304>|0x05b|0x023:'154'
	// <0x598327>|0x07e|0x023:'155'
	// <0x598339>|0x090|0x012:'156'
	// 1
	// <0x59837c>|0x0d3|0x043:'158'
	// 1
	// <0x5983a0>|0x0f7|0x024:'160'
	// <0x5983c6>|0x11d|0x026:'161'
	// <0x5983d8>|0x12f|0x012:'162'
	// 1
	// 2
	// <0x598426>|0x17d|0x04e:'165'
	// <0x59846b>|0x1c2|0x045:'166'
	// ******
}

// STATE[STUB]
// float survarium::distance_from_cylinder_center_to_point_on_shape(vostok::math::float4x4 const&, float, float, vostok::math::float3 const&)
float distance_from_cylinder_center_to_point_on_shape(
	vostok::math::float4x4 const&		transform,
	float								radius,
	float								half_length,
	vostok::math::float3 const&			source_position)
{
	// LOCALS
	// float 						proj_to_y_axis
	// vostok::math::float3 		circle_point_dir
	// vostok::math::float3 		bottom_surface_center
	// vostok::math::float3 		circle_proj_vec
	// vostok::math::float3 		proj
	// vostok::math::float3 		y_axis
	// vostok::math::float3 		surface_center
	// vostok::math::float3 		top_surface_center
	// vostok::math::float3 		height_vector
	// vostok::math::float3 		center
	// vostok::math::float3 		dir<1>
	// vostok::math::float3 		height_vector_proj_point<1>
	// ******

	// SKIPPED BLOCKS
	// <0x598140><1>
	// ******

	return 0.0f;
	// FUNCTION BODY
	// <0x598049>|0x000|0x000:'171'
	// <0x598065>|0x01c|0x01c:'172'
	// <0x598081>|0x038|0x01c:'173'
	// <0x5980a7>|0x05e|0x026:'174'
	// <0x5980cd>|0x084|0x026:'175'
	// <0x5980df>|0x096|0x012:'176'
	// 1
	// <0x598122>|0x0d9|0x043:'178'
	// 1
	// <0x598146>|0x0fd|0x024:'180'
	// <0x59816c>|0x123|0x026:'181'
	// 1
	// <0x598181>|0x138|0x015:'183'
	// 1
	// 2
	// <0x5981d2>|0x189|0x051:'186'
	// <0x598217>|0x1ce|0x045:'187'
	// <0x598229>|0x1e0|0x012:'188'
	// <0x598251>|0x208|0x028:'189'
	// 1
	// <0x598263>|0x21a|0x012:'191'
	// ******
}

// STATE[STUB]
// float survarium::dz_bone_data_contact_test_predicate::add_single_result(void*, vostok::physics::primitive_type, vostok::math::float4x4 const&, vostok::math::float3 const&, vostok::physics::primitive_type, vostok::math::float4x4 const&, vostok::math::float3 const&)
float dz_bone_data_contact_test_predicate::add_single_result(
	void*								user_data,
	vostok::physics::primitive_type		first_shape_type,
	vostok::math::float4x4 const&		first_shape_transform,
	vostok::math::float3 const&			first_shape_dimension,
	vostok::physics::primitive_type		second_shape_type,
	vostok::math::float4x4 const&		second_shape_transform,
	vostok::math::float3 const&			second_shape_dimension)
{
	// LOCALS
	// std::pair<vostok::collision::bone_collision_data *,float> result
	// float 						d_1
	// float 						max_distance
	// ******

	// STATICS
	// static <NoType> 				 = <0xc7de0>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0xb6cbe }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN5") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0xb6cde }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0xb6d02 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0xb6d42 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// ******

	return 0.0f;
	// FUNCTION BODY
	// <0xc7bea>|0x000|0x000:'222'
	// <0xc7c21>|0x037|0x037:'223'
	// <0xc7c35>|0x04b|0x014:'224'
	// 1
	// <0xc7c3b>|0x051|0x006:'226'
	// 1
	// <0xc7c44>|0x05a|0x009:'228'
	// <0xc7c9a>|0x0b0|0x056:'229'
	// 1
	// 2
	// <0xc7ca1>|0x0b7|0x007:'232'
	// <0xc7cae>|0x0c4|0x00d:'233'
	// 1
	// 2
	// <0xc7cbe>|0x0d4|0x010:'236'
	// <0xc7cd9>|0x0ef|0x01b:'237'
	// 1
	// 2
	// <0xc7cde>|0x0f4|0x005:'240'
	// <0xc7cfd>|0x113|0x01f:'241'
	// 1
	// 2
	// <0xc7d02>|0x118|0x005:'244'
	// <0xc7d40>|0x156|0x03e:'245'
	// 1
	// 2
	// <0xc7d42>|0x158|0x002:'248'
	// <0xc7d80>|0x196|0x03e:'249'
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0xc7d82>|0x198|0x002:'255'
	// <0xc7db2>|0x1c8|0x030:'256'
	// <0xc7dc1>|0x1d7|0x00f:'257'
	// <0xc7dd6>|0x1ec|0x015:'258'
	// ******
}

// STATE[STUB]
// void survarium::damage_zone_core::on_inside(vostok::buffer_vector<vostok::physics::base_physics_object *> const&)
void damage_zone_core::on_inside( vostok::buffer_vector<vostok::physics::base_physics_object *> const& objects )
{
	// FUNCTION BODY
	// 1
	// ******
}

// STATE[STUB]
// void survarium::damage_zone_core::on_leave(vostok::buffer_vector<vostok::physics::base_physics_object *> const&)
void damage_zone_core::on_leave( vostok::buffer_vector<vostok::physics::base_physics_object *> const& objects )
{
	// LOCALS
	// vostok::physics::base_physics_object** end
	// vostok::physics::base_physics_object** it
	// hit_receiver* 				receiver<1>
	// hit_receiver_info 			info<1>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x586e81 }, type_index: TypeIndex(0x2dca) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x586ed1 }, type_index: TypeIndex(0x9146) })
	// ******

	// FUNCTION BODY
	// <0x597e39>|0x000|0x000:'272'
	// <0x597e41>|0x008|0x008:'273'
	// <0x597e4a>|0x011|0x009:'274'
	// 1
	// <0x597e61>|0x028|0x017|[1]:'276'
	// <0x597e6d>|0x034|0x00c:'277'
	// <0x597e86>|0x04d|0x019:'278'
	// <0x597e92>|0x059|0x00c:'279'
	// 1
	// <0x597e9e>|0x065|0x00c:'281'
	// 1
	// <0x597eaa>|0x071|0x00c:'283'
	// <0x597ed3>|0x09a|0x029:'284'
	// 1
	// 2
	// 3
	// <0x597ef5>|0x0bc|0x022:'288'
	// <0x597f03>|0x0ca|0x00e:'289'
	// <0x597f0f>|0x0d6|0x00c:'290'
	// <0x597f5c>|0x123|0x04d:'291'
	// ******
}

// STATE[STUB]
// void survarium::damage_zone_core::on_enter(vostok::buffer_vector<vostok::physics::base_physics_object *> const&)
void damage_zone_core::on_enter( vostok::buffer_vector<vostok::physics::base_physics_object *> const& objects )
{
	// LOCALS
	// vostok::physics::base_physics_object** end
	// vostok::physics::base_physics_object** it
	// hit_receiver* 				receiver<1>
	// hit_receiver_info 			info<1>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5877b1 }, type_index: TypeIndex(0x2dca) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x587801 }, type_index: TypeIndex(0x9146) })
	// ******

	// FUNCTION BODY
	// <0x598769>|0x000|0x000:'296'
	// <0x598771>|0x008|0x008:'297'
	// <0x59877a>|0x011|0x009:'298'
	// 1
	// <0x598791>|0x028|0x017|[1]:'300'
	// <0x59879d>|0x034|0x00c:'301'
	// <0x5987b6>|0x04d|0x019:'302'
	// <0x5987c2>|0x059|0x00c:'303'
	// 1
	// <0x5987ce>|0x065|0x00c:'305'
	// 1
	// <0x5987da>|0x071|0x00c:'307'
	// <0x598803>|0x09a|0x029:'308'
	// 1
	// 2
	// <0x598825>|0x0bc|0x022:'311'
	// <0x598837>|0x0ce|0x012:'312'
	// <0x598843>|0x0da|0x00c:'313'
	// <0x598855>|0x0ec|0x012:'314'
	// ******
}

// STATE[STUB]
// void survarium::damage_zone_core::tick(const unsigned int, const unsigned int)
void damage_zone_core::tick( u32 frame_delta, u32 current_time )
{
	// FUNCTION BODY
	// <0x599259>|0x000|0x000:'319'
	// 1
	// <0x599269>|0x010|0x010:'321'
	// 1
	// 2
	// <0x599287>|0x02e|0x01e:'324'
	// <0x599297>|0x03e|0x010:'325'
	// 1
	// <0x599299>|0x040|0x002:'327'
	// <0x5992a9>|0x050|0x010:'328'
	// 1
	// <0x5992ab>|0x052|0x002:'330'
	// 1
	// 2
	// 3
	// 4
	// ******
}

// STATE[STUB]
// bool survarium::remove_null_receivers_predicate(survarium::hit_receiver_info const&)
bool remove_null_receivers_predicate( hit_receiver_info const& info )
{
	return false;
}

// STATE[STUB]
// bool survarium::damage_zone_core::is_filter_passed(vostok::physics::base_physics_object*) const
bool damage_zone_core::is_filter_passed( vostok::physics::base_physics_object* object ) const
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x586d01 }, type_index: TypeIndex(0x94b4) })
	// ******

	return false;
	// FUNCTION BODY
	// <0x597cf7>|0x000|0x000:'391'
	// ******
}

// STATE[STUB]
// void survarium::damage_zone_core::hit_on_enter(const unsigned int, const unsigned int)
void damage_zone_core::hit_on_enter( u32 frame_delta, u32 current_time )
{
	// LOCALS
	// hit_receiver_info* 			end
	// hit_receiver_info* 			it
	// std::pair<vostok::collision::bone_collision_data *,float> const* ub_it<1>
	// vostok::vectora<std::pair<vostok::collision::bone_collision_data *,float> > unique_bones<1>
	// dz_bone_data_contact_test_predicate predicate<1>
	// std::insert_iterator<vostok::vectora<std::pair<vostok::collision::bone_collision_data *,float> > > insert_it<1>
	// std::pair<vostok::collision::bone_collision_data *,float> const* ub_end<1>
	// vostok::vectora<std::pair<vostok::collision::bone_collision_data *,float> > results<1>
	// ******

	// SKIPPED BLOCKS
	// <0x599017><1>
	// ******

	// TYPEDEFS
	// typedef
	// 	vostok::vectora<std::pair<vostok::collision::bone_collision_data *,float> >
	// 	bone_data_container;

	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5881d4 }, type_index: TypeIndex(0x913c) })
	// ******

	// FUNCTION BODY
	// <0x598f8f>|0x000|0x000:'396'
	// 1
	// <0x598fb2>|0x023|0x023:'398'
	// <0x598fd7>|0x048|0x025:'399'
	// 1
	// <0x598fdc>|0x04d|0x005:'401'
	// <0x598ff1>|0x062|0x015:'402'
	// <0x599006>|0x077|0x015:'403'
	// 1
	// <0x59901d>|0x08e|0x017:'405'
	// <0x599028>|0x099|0x00b:'406'
	// 1
	// 2
	// 3
	// 4
	// <0x59902a>|0x09b|0x002:'411'
	// <0x599055>|0x0c6|0x02b:'412'
	// <0x599077>|0x0e8|0x022:'413'
	// <0x599083>|0x0f4|0x00c:'414'
	// <0x599099>|0x10a|0x016:'415'
	// <0x5990c4>|0x135|0x02b:'416'
	// <0x5990dc>|0x14d|0x018:'417'
	// 1
	// <0x599115>|0x186|0x039:'419'
	// <0x599127>|0x198|0x012:'420'
	// 1
	// <0x599139>|0x1aa|0x012:'422'
	// 1
	// <0x599150>|0x1c1|0x017:'424'
	// 1
	// <0x599161>|0x1d2|0x011:'426'
	// <0x5991d6>|0x247|0x075:'427'
	// 1
	// <0x5991dd>|0x24e|0x007:'429'
	// 1
	// <0x5991e2>|0x253|0x005:'431'
	// 1
	// <0x5991ee>|0x25f|0x00c:'433'
	// <0x599208>|0x279|0x01a:'434'
	// <0x599232>|0x2a3|0x02a:'435'
	// ******
}

// STATE[STUB]
// void survarium::damage_zone_core::hit_on_inside(const unsigned int, const unsigned int)
void damage_zone_core::hit_on_inside( u32 frame_delta, u32 current_time )
{
	// LOCALS
	// hit_receiver_info* 			end<1>
	// hit_receiver_info* 			it<1>
	// std::pair<vostok::collision::bone_collision_data *,float> const* ub_it<2>
	// vostok::vectora<std::pair<vostok::collision::bone_collision_data *,float> > unique_bones<2>
	// dz_bone_data_contact_test_predicate predicate<2>
	// std::insert_iterator<vostok::vectora<std::pair<vostok::collision::bone_collision_data *,float> > > insert_it<2>
	// std::pair<vostok::collision::bone_collision_data *,float> const* ub_end<2>
	// vostok::vectora<std::pair<vostok::collision::bone_collision_data *,float> > results<2>
	// float 						hit_value<3>
	// float 						armor_piercing_value<3>
	// float 						hit_coeff<3>
	// ******

	// SKIPPED BLOCKS
	// <0x598c5c><1>
	// <0x598c9d><2>
	// <0x598dee><3>
	// ******

	// TYPEDEFS
	// typedef
	// 	vostok::vectora<std::pair<vostok::collision::bone_collision_data *,float> >
	// 	bone_data_container;

	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x587ef1 }, type_index: TypeIndex(0x913c) })
	// ******

	// FUNCTION BODY
	// 1
	// 2
	// <0x598bff>|0x000|0x000:'442'
	// 1
	// <0x598c1a>|0x01b|0x01b:'444'
	// <0x598c3f>|0x040|0x025:'445'
	// 1
	// <0x598c44>|0x045|0x005:'447'
	// 1
	// <0x598c62>|0x063|0x01e:'449'
	// <0x598c77>|0x078|0x015:'450'
	// <0x598c8c>|0x08d|0x015:'451'
	// 1
	// <0x598ca3>|0x0a4|0x017:'453'
	// 1
	// 2
	// 3
	// <0x598caa>|0x0ab|0x007:'457'
	// <0x598cd4>|0x0d5|0x02a:'458'
	// <0x598cf7>|0x0f8|0x023:'459'
	// <0x598d03>|0x104|0x00c:'460'
	// <0x598d19>|0x11a|0x016:'461'
	// <0x598d53>|0x154|0x03a:'462'
	// <0x598d6b>|0x16c|0x018:'463'
	// 1
	// <0x598da4>|0x1a5|0x039:'465'
	// <0x598db6>|0x1b7|0x012:'466'
	// <0x598dc8>|0x1c9|0x012:'467'
	// 1
	// <0x598ddf>|0x1e0|0x017:'469'
	// 1
	// <0x598df4>|0x1f5|0x015:'471'
	// <0x598e23>|0x224|0x02f:'472'
	// <0x598e39>|0x23a|0x016:'473'
	// <0x598e63>|0x264|0x02a:'474'
	// <0x598e8e>|0x28f|0x02b:'475'
	// <0x598ef3>|0x2f4|0x065:'476'
	// 1
	// <0x598efa>|0x2fb|0x007:'478'
	// 1
	// <0x598eff>|0x300|0x005:'480'
	// <0x598f0b>|0x30c|0x00c:'481'
	// <0x598f25>|0x326|0x01a:'482'
	// 1
	// <0x598f4f>|0x350|0x02a:'484'
	// <0x598f64>|0x365|0x015:'485'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::damage_zone_core::hit_on_motion_inside(const unsigned int, const unsigned int)
void damage_zone_core::hit_on_motion_inside( u32 frame_delta, u32 current_time )
{
	// LOCALS
	// hit_receiver_info* 			end
	// hit_receiver_info* 			it
	// std::pair<vostok::collision::bone_collision_data *,float> const* ub_it<1>
	// vostok::vectora<std::pair<vostok::collision::bone_collision_data *,float> > unique_bones<1>
	// dz_bone_data_contact_test_predicate predicate<1>
	// std::insert_iterator<vostok::vectora<std::pair<vostok::collision::bone_collision_data *,float> > > insert_it<1>
	// std::pair<vostok::collision::bone_collision_data *,float> const* ub_end<1>
	// vostok::vectora<std::pair<vostok::collision::bone_collision_data *,float> > results<1>
	// float 						on_bound_hit<2>
	// float 						hit_val<2>
	// float 						on_center_hit<2>
	// ******

	// SKIPPED BLOCKS
	// <0x5988f7><1>
	// <0x598a48><2>
	// ******

	// TYPEDEFS
	// typedef
	// 	vostok::vectora<std::pair<vostok::collision::bone_collision_data *,float> >
	// 	bone_data_container;

	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x587a71 }, type_index: TypeIndex(0x9141) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x587aab }, type_index: TypeIndex(0x9141) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x587b6a }, type_index: TypeIndex(0x913c) })
	// ******

	// FUNCTION BODY
	// <0x59886f>|0x000|0x000:'491'
	// 1
	// <0x598892>|0x023|0x023:'493'
	// <0x5988b7>|0x048|0x025:'494'
	// 1
	// <0x5988bc>|0x04d|0x005:'496'
	// <0x5988d1>|0x062|0x015:'497'
	// <0x5988e6>|0x077|0x015:'498'
	// 1
	// <0x5988fd>|0x08e|0x017:'500'
	// 1
	// 2
	// 3
	// <0x598904>|0x095|0x007:'504'
	// <0x59892e>|0x0bf|0x02a:'505'
	// <0x598951>|0x0e2|0x023:'506'
	// <0x59895d>|0x0ee|0x00c:'507'
	// <0x598973>|0x104|0x016:'508'
	// <0x5989ad>|0x13e|0x03a:'509'
	// <0x5989c5>|0x156|0x018:'510'
	// 1
	// <0x5989fe>|0x18f|0x039:'512'
	// <0x598a10>|0x1a1|0x012:'513'
	// <0x598a22>|0x1b3|0x012:'514'
	// 1
	// <0x598a39>|0x1ca|0x017:'516'
	// 1
	// <0x598a4e>|0x1df|0x015:'518'
	// <0x598a88>|0x219|0x03a:'519'
	// 1
	// <0x598ac2>|0x253|0x03a:'521'
	// <0x598afe>|0x28f|0x03c:'522'
	// <0x598b6c>|0x2fd|0x06e:'523'
	// 1
	// <0x598b73>|0x304|0x007:'525'
	// <0x598b78>|0x309|0x005:'526'
	// 1
	// <0x598b84>|0x315|0x00c:'528'
	// <0x598b9e>|0x32f|0x01a:'529'
	// <0x598bc8>|0x359|0x02a:'530'
	// ******
}

// STATE[STUB]
// void survarium::damage_zone_core::activate(survarium::zone_group*, vostok::physics::world*, survarium::scheduler&)
void damage_zone_core::activate( zone_group* owner, vostok::physics::world* p_world, scheduler& scheduler )
{
	// FUNCTION BODY
	// <0x598630>|0x000|0x000:'546'
	// <0x59863f>|0x00f|0x00f:'547'
	// <0x59864e>|0x01e|0x00f:'548'
	// <0x59865d>|0x02d|0x00f:'549'
	// <0x59866d>|0x03d|0x010:'550'
	// <0x59867c>|0x04c|0x00f:'551'
	// ******
}

// STATE[STUB]
// void survarium::damage_zone_core::deactivate()
void damage_zone_core::deactivate( )
{
	// LOCALS
	// hit_receiver_info* 			end<1>
	// hit_receiver_info* 			it<1>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x587013 }, type_index: TypeIndex(0x9146) })
	// ******

	// FUNCTION BODY
	// <0x597f79>|0x000|0x000:'556'
	// <0x597f81>|0x008|0x008:'557'
	// 1
	// <0x597f98>|0x01f|0x017:'559'
	// 1
	// <0x597fa5>|0x02c|0x00d:'561'
	// 1
	// <0x597fb1>|0x038|0x00c|[1]:'563'
	// <0x597fc3>|0x04a|0x012:'564'
	// <0x597fd5>|0x05c|0x012:'565'
	// <0x597fe8>|0x06f|0x013:'566'
	// 1
	// <0x598017>|0x09e|0x02f:'568'
	// <0x598025>|0x0ac|0x00e:'569'
	// ******
}

// STATE[STUB]
// void survarium::damage_zone_core::on_player_action(survarium::hit_receiver const*, survarium::player_actions_subscriber::action, float)
void damage_zone_core::on_player_action( hit_receiver const* receiver, player_actions_subscriber::action action, float param )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x586e1a }, type_index: TypeIndex(0x271f0) })
	// ******

	// FUNCTION BODY
	// <0x597de9>|0x000|0x000:'574'
	// ******
}

	/* TYPEDEFS

	typedef
		hit_receiver_info*
		iterator_type;

	typedef
		scheduler::record*
		iterator_type;

	typedef
		std::pair<vostok::collision::bone_collision_data *,float>*
		iterator_type;

	typedef
		void**
		iterator_type;

	typedef
		vostok::fixed_string<16>*
		iterator_type;

	typedef
		zone_group::zone_wrapper*
		iterator_type;

} // namespace survarium
