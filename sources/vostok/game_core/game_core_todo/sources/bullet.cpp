////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "bullet.h"

namespace survarium {

// STATE[STUB]
// void survarium::`dynamic atexit destructor for 'bullet_tracer_exposition''()
void `dynamic atexit destructor for 'bullet_tracer_exposition''( )
{
}

// STATE[STUB]
// survarium::bullet::bullet(survarium::bullet_manager&, vostok::math::float3 const&, vostok::math::float3 const&, const unsigned int, float, vostok::resources::resource_ptr<survarium::weapon_ammunition,vostok::resources::unmanaged_intrusive_base> const&, survarium::weapon_core const&, survarium::hit_initiator const* const, survarium::hit_receiver const* const)
bullet::bullet(
	bullet_manager&						bullet_manager,
	vostok::math::float3 const&			position,
	vostok::math::float3 const&			velocity,
	u32									born_time_in_ms,
	float								air_resistance,
	vostok::resources::resource_ptr<weapon_ammunition,vostok::resources::unmanaged_intrusive_base> const&	wa,
	weapon_core const&					wc,
	hit_initiator const*				initiator,
	hit_receiver const*					ignorable_object)
{
	// FUNCTION BODY
	// <0x5916fa>|0x000|0x000:'85'
	// <0x591706>|0x00c|0x00c:'86'
	// <0x591712>|0x018|0x00c:'87'
	// <0x591753>|0x059|0x041:'88'
	// ******
}

// STATE[STUB]
// survarium::bullet::bullet(survarium::bullet const&)
bullet::bullet( bullet const& other )
{
	// FUNCTION BODY
	// <0x590e3a>|0x000|0x000:'96'
	// <0x590e50>|0x016|0x016:'97'
	// <0x590e6c>|0x032|0x01c:'98'
	// <0x590e85>|0x04b|0x019:'99'
	// <0x590ea1>|0x067|0x01c:'100'
	// 1
	// <0x590ead>|0x073|0x00c:'102'
	// <0x590eb9>|0x07f|0x00c:'103'
	// <0x590ec5>|0x08b|0x00c:'104'
	// <0x590ed1>|0x097|0x00c:'105'
	// <0x590edd>|0x0a3|0x00c:'106'
	// <0x590ee9>|0x0af|0x00c:'107'
	// 1
	// <0x590ef5>|0x0bb|0x00c:'109'
	// <0x590f01>|0x0c7|0x00c:'110'
	// <0x590f0d>|0x0d3|0x00c:'111'
	// <0x590f19>|0x0df|0x00c:'112'
	// <0x590f25>|0x0eb|0x00c:'113'
	// <0x590f31>|0x0f7|0x00c:'114'
	// <0x590f3d>|0x103|0x00c:'115'
	// <0x590f49>|0x10f|0x00c:'116'
	// 1
	// <0x590f57>|0x11d|0x00e:'118'
	// ******
}

// STATE[STUB]
// void survarium::bullet::~bullet()
void bullet::~bullet( )
{
}

// STATE[STUB]
// void survarium::bullet::tick(unsigned int)
void bullet::tick( u32 current_time_in_ms )
{
	// LOCALS
	// u16 							invalid_tracer_idx
	// float 						high_time
	// float 						low_time
	// vostok::math::float3 const& 	gravity
	// vostok::math::float3 		zero_velocity
	// collision_result 			result<1>
	// float 						time<1>
	// vostok::math::float3 		d<2>
	// float 						d_len<2>
	// float 						speed<2>
	// float 						length<2>
	// ******

	// SKIPPED BLOCKS
	// <0x592c94><2>
	// ******

	// FUNCTION BODY
	// <0x592a2f>|0x000|0x000:'127'
	// <0x592a3b>|0x00c|0x00c:'128'
	// <0x592a55>|0x026|0x01a:'129'
	// <0x592a5e>|0x02f|0x009:'130'
	// <0x592a6e>|0x03f|0x010:'131'
	// <0x592a9f>|0x070|0x031:'132'
	// 1
	// <0x592aab>|0x07c|0x00c:'134'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// <0x592aba>|0x08b|0x00f|[1]:'143'
	// 1
	// <0x592ad0>|0x0a1|0x016:'145'
	// <0x592aea>|0x0bb|0x01a:'146'
	// 1
	// 2
	// <0x592aef>|0x0c0|0x005:'149'
	// 1
	// <0x592afb>|0x0cc|0x00c:'151'
	// <0x592b15>|0x0e6|0x01a:'152'
	// 1
	// 2
	// <0x592b1a>|0x0eb|0x005:'155'
	// <0x592b29>|0x0fa|0x00f:'156'
	// 1
	// <0x592b2e>|0x0ff|0x005:'158'
	// <0x592b4e>|0x11f|0x020:'159'
	// 1
	// <0x592b5d>|0x12e|0x00f:'161'
	// <0x592b77>|0x148|0x01a:'162'
	// 1
	// 2
	// <0x592b7c>|0x14d|0x005:'165'
	// <0x592bb3>|0x184|0x037:'166'
	// 1
	// <0x592bb9>|0x18a|0x006:'168'
	// <0x592bd3>|0x1a4|0x01a:'169'
	// 1
	// 2
	// <0x592bd8>|0x1a9|0x005:'172'
	// 1
	// <0x592be4>|0x1b5|0x00c:'174'
	// <0x592bf4>|0x1c5|0x010:'175'
	// <0x592c03>|0x1d4|0x00f:'176'
	// 1
	// 2
	// <0x592c13>|0x1e4|0x010:'179'
	// 1
	// <0x592c25>|0x1f6|0x012:'181'
	// 1
	// <0x592c42>|0x213|0x01d:'183'
	// <0x592c5c>|0x22d|0x01a:'184'
	// 1
	// 2
	// 3
	// <0x592c61>|0x232|0x005:'188'
	// 1
	// <0x592c84>|0x255|0x023:'190'
	// 1
	// <0x592c9a>|0x26b|0x016:'192'
	// <0x592cb5>|0x286|0x01b:'193'
	// <0x592cc0>|0x291|0x00b:'194'
	// <0x592ce3>|0x2b4|0x023:'195'
	// 1
	// <0x592cf4>|0x2c5|0x011:'197'
	// 1
	// 2
	// <0x592d16>|0x2e7|0x022:'200'
	// <0x592d47>|0x318|0x031:'201'
	// 1
	// <0x592d59>|0x32a|0x012:'203'
	// <0x592d70>|0x341|0x017:'204'
	// 1
	// <0x592d7a>|0x34b|0x00a:'206'
	// <0x592d88>|0x359|0x00e:'207'
	// 1
	// <0x592daf>|0x380|0x027:'209'
	// 1
	// 2
	// <0x592db1>|0x382|0x002:'212'
	// <0x592dbd>|0x38e|0x00c:'213'
	// <0x592dc7>|0x398|0x00a:'214'
	// 1
	// ******
}

// STATE[STUB]
// vostok::math::float3 survarium::bullet::compute_parabolic_velocity(float, vostok::math::float3 const&)
vostok::math::float3 bullet::compute_parabolic_velocity( float time, vostok::math::float3 const& gravity )
{
	// LOCALS
	// vostok::math::float3 		xz_velocity
	// ******

	return vostok::math::float3(1., 1., 1.);
	// FUNCTION BODY
	// <0x5913ca>|0x000|0x000:'240'
	// 1
	// 2
	// <0x5913ec>|0x022|0x022:'243'
	// <0x59140b>|0x041|0x01f:'244'
	// 1
	// <0x591436>|0x06c|0x02b:'246'
	// ******
}

// STATE[STUB]
// vostok::math::float3 survarium::bullet::compute_trajectory_velocity(float, vostok::math::float3 const&)
vostok::math::float3 bullet::compute_trajectory_velocity( float time, vostok::math::float3 const& gravity )
{
	// LOCALS
	// vostok::math::float3 const& 	parabolic_vel
	// float 						fall_down_time
	// float 						parabolic_time
	// ******

	return vostok::math::float3(1., 1., 1.);
	// FUNCTION BODY
	// <0x591879>|0x000|0x000:'251'
	// <0x591884>|0x00b|0x00b:'252'
	// 1
	// <0x591893>|0x01a|0x00f:'254'
	// <0x5918a1>|0x028|0x00e:'255'
	// 1
	// <0x5918bd>|0x044|0x01c:'257'
	// <0x5918da>|0x061|0x01d:'258'
	// ******
}

// STATE[STUB]
// vostok::math::float3 survarium::bullet::compute_parabolic_position(float, vostok::math::float3 const&)
vostok::math::float3 bullet::compute_parabolic_position( float time, vostok::math::float3 const& gravity )
{
	// LOCALS
	// vostok::math::float3 		xz_velocity
	// float 						sqr_t_div_2
	// ******

	return vostok::math::float3(1., 1., 1.);
	// FUNCTION BODY
	// <0x591241>|0x000|0x000:'263'
	// 1
	// <0x591269>|0x028|0x028:'265'
	// <0x591288>|0x047|0x01f:'266'
	// 1
	// <0x5912f8>|0x0b7|0x070:'268'
	// <0x59130d>|0x0cc|0x015:'269'
	// ******
}

// STATE[STUB]
// vostok::math::float3 survarium::bullet::compute_trajectory_position(float, vostok::math::float3 const&)
vostok::math::float3 bullet::compute_trajectory_position( float time, vostok::math::float3 const& gravity )
{
	// LOCALS
	// vostok::math::float3 const& 	parabolic_pos
	// vostok::math::float3 const& 	parabolic_vel
	// float 						fall_down_time
	// float 						parabolic_time
	// ******

	return vostok::math::float3(1., 1., 1.);
	// FUNCTION BODY
	// <0x59177a>|0x000|0x000:'274'
	// <0x591785>|0x00b|0x00b:'275'
	// 1
	// <0x591794>|0x01a|0x00f:'277'
	// <0x5917a2>|0x028|0x00e:'278'
	// 1
	// <0x5917c1>|0x047|0x01f:'280'
	// <0x5917de>|0x064|0x01d:'281'
	// 1
	// <0x5917fb>|0x081|0x01d:'283'
	// ******
}

// STATE[STUB]
// float survarium::bullet::get_parabolic_time()
float bullet::get_parabolic_time( )
{
	// STATICS
	// static float 				air_resistance_epsilon = <0x9863b8>;
	// ******

	return 0.0f;
	// FUNCTION BODY
	// 1
	// <0x5911f9>|0x000|0x000:'289'
	// ******
}

// STATE[STUB]
// float survarium::bullet::pick_next_permissible_time(float, float, vostok::math::float3 const&)
float bullet::pick_next_permissible_time( float low_time, float high_time, vostok::math::float3 const& gravity )
{
	// LOCALS
	// float 						low
	// float 						start_high_time
	// float 						epsilon
	// float 						check_time
	// float 						distance<1>
	// ******

	// SKIPPED BLOCKS
	// <0x5920cd><1>
	// ******

	return 0.0f;
	// FUNCTION BODY
	// <0x592039>|0x000|0x000:'294'
	// <0x592045>|0x00c|0x00c:'295'
	// 1
	// <0x59204f>|0x016|0x00a:'297'
	// 1
	// <0x592074>|0x03b|0x025:'299'
	// <0x59207f>|0x046|0x00b:'300'
	// 1
	// <0x592087>|0x04e|0x008:'302'
	// 1
	// 2
	// <0x59208f>|0x056|0x008:'305'
	// <0x592099>|0x060|0x00a:'306'
	// <0x5920a3>|0x06a|0x00a:'307'
	// <0x5920b0>|0x077|0x00d:'308'
	// 1
	// <0x5920cf>|0x096|0x01f:'310'
	// 1
	// <0x5920ec>|0x0b3|0x01d:'312'
	// <0x5920f7>|0x0be|0x00b:'313'
	// <0x592101>|0x0c8|0x00a:'314'
	// <0x592103>|0x0ca|0x002:'315'
	// 1
	// <0x59210d>|0x0d4|0x00a:'317'
	// <0x592124>|0x0eb|0x017:'318'
	// 1
	// <0x592126>|0x0ed|0x002:'320'
	// <0x592132>|0x0f9|0x00c:'321'
	// ******
}

// STATE[STUB]
// bool survarium::bullet::pick_permissible_range(float&, float, float, vostok::math::float3 const&)
bool bullet::pick_permissible_range(
	float&							result,
	float							low_time,
	float							high_time,
	vostok::math::float3 const&		gravity)
{
	// LOCALS
	// float 						parabolic_time
	// ******

	return false;
	// FUNCTION BODY
	// 1
	// 2
	// <0x591f59>|0x000|0x000:'328'
	// 1
	// <0x591f64>|0x00b|0x00b:'330'
	// 1
	// <0x591f6f>|0x016|0x00b:'332'
	// <0x591f8e>|0x035|0x01f:'333'
	// 1
	// 2
	// <0x591f95>|0x03c|0x007:'336'
	// 1
	// <0x591fa0>|0x047|0x00b:'338'
	// <0x591fbf>|0x066|0x01f:'339'
	// 1
	// 2
	// <0x591fc3>|0x06a|0x004:'342'
	// 1
	// <0x591fe2>|0x089|0x01f:'344'
	// <0x592001>|0x0a8|0x01f:'345'
	// 1
	// 2
	// <0x592005>|0x0ac|0x004:'348'
	// <0x592024>|0x0cb|0x01f:'349'
	// ******
}

// STATE[STUB]
// float survarium::bullet::get_check_time_in_vacuum(float, float, vostok::math::float3 const&)
float bullet::get_check_time_in_vacuum( float start_low, float high, vostok::math::float3 const& gravity )
{
	// LOCALS
	// float 						max_test_distance
	// float 						result
	// float 						fall_down_velocity_magnitude
	// float 						time
	// float 						time_delta
	// float 						positive_gravity
	// float 						time_to_fly
	// ******

	return 0.0f;
	// FUNCTION BODY
	// <0x591dfa>|0x000|0x000:'354'
	// <0x591e0f>|0x015|0x015:'355'
	// <0x591e1e>|0x024|0x00f:'356'
	// 1
	// <0x591e74>|0x07a|0x056:'358'
	// <0x591e7f>|0x085|0x00b:'359'
	// 1
	// <0x591e87>|0x08d|0x008:'361'
	// <0x591ea6>|0x0ac|0x01f:'362'
	// <0x591eba>|0x0c0|0x014:'363'
	// <0x591eef>|0x0f5|0x035:'364'
	// 1
	// <0x591efb>|0x101|0x00c:'366'
	// <0x591f07>|0x10d|0x00c:'367'
	// <0x591f16>|0x11c|0x00f:'368'
	// <0x591f2d>|0x133|0x017:'369'
	// 1
	// <0x591f39>|0x13f|0x00c:'371'
	// ******
}

// STATE[STUB]
// float survarium::get_max_error_time(const float, const float)
float get_max_error_time( float t0, float t1 )
{
	return 0.0f;
	// FUNCTION BODY
	// <0x590dc3>|0x000|0x000:'376'
	// 1
	// 2
	// 3
	// ******
}

// STATE[STUB]
// float survarium::bullet::get_check_time(const float, float, vostok::math::float3 const&)
float bullet::get_check_time( float start_low, float high, vostok::math::float3 const& gravity )
{
	// LOCALS
	// float 						max_test_distance
	// float 						low
	// vostok::math::float3 		start
	// float 						check_time
	// vostok::math::float3 		intermediate<1>
	// vostok::math::float3 		target<1>
	// float 						distance<1>
	// ******

	// SKIPPED BLOCKS
	// <0x591d16><1>
	// ******

	return 0.0f;
	// FUNCTION BODY
	// <0x591cb9>|0x000|0x000:'384'
	// <0x591cce>|0x015|0x015:'385'
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x591ce5>|0x02c|0x017:'391'
	// <0x591cef>|0x036|0x00a:'392'
	// 1
	// <0x591cf9>|0x040|0x00a:'394'
	// 1
	// <0x591d1c>|0x063|0x023:'396'
	// <0x591d44>|0x08b|0x028:'397'
	// <0x591d5b>|0x0a2|0x017:'398'
	// <0x591d92>|0x0d9|0x037:'399'
	// <0x591d9d>|0x0e4|0x00b:'400'
	// <0x591da7>|0x0ee|0x00a:'401'
	// <0x591da9>|0x0f0|0x002:'402'
	// 1
	// <0x591db3>|0x0fa|0x00a:'404'
	// <0x591dca>|0x111|0x017:'405'
	// 1
	// <0x591dcf>|0x116|0x005:'407'
	// <0x591ddb>|0x122|0x00c:'408'
	// ******
}

// STATE[STUB]
// float survarium::bullet::compute_max_error(float, float, vostok::math::float3 const&)
float bullet::compute_max_error( float low, float high, vostok::math::float3 const& gravity )
{
	// LOCALS
	// vostok::math::float3 		start_to_target
	// vostok::math::float3 		target
	// vostok::math::float3 		start
	// vostok::math::float3 		start_to_max_error
	// float 						max_error_time
	// float 						magnitude
	// float 						cosine_alpha
	// vostok::math::float3 		max_error
	// float 						sine_alpha
	// ******

	return 0.0f;
	// FUNCTION BODY
	// <0x591b89>|0x000|0x000:'413'
	// 1
	// <0x591ba0>|0x017|0x017:'415'
	// <0x591bb7>|0x02e|0x017:'416'
	// <0x591bce>|0x045|0x017:'417'
	// 1
	// <0x591be5>|0x05c|0x017:'419'
	// <0x591bf7>|0x06e|0x012:'420'
	// <0x591c02>|0x079|0x00b:'421'
	// <0x591c17>|0x08e|0x015:'422'
	// <0x591c41>|0x0b8|0x02a:'423'
	// <0x591c73>|0x0ea|0x032:'424'
	// 1
	// <0x591c98>|0x10f|0x025:'426'
	// ******
}

// STATE[STUB]
// survarium::collision_result survarium::bullet::check_collision(vostok::math::float3, float, float)
collision_result bullet::check_collision( vostok::math::float3 start_position, float start_time, float current_time )
{
	// LOCALS
	// vostok::math::float3 const& 	target_position
	// collision_result 			result
	// vostok::physics::world& 		p_world
	// float 						distance
	// vostok::math::float3 		new_start_position
	// vostok::math::float3 		direction
	// bool 						ignorable_object_was_hit<1>
	// vostok::math::float3 		triangle_normal<1>
	// vostok::physics::closest_ray_result ray_result<1>
	// float 						cos_alpha<1>
	// triangle_orientation 		orientation<1>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5818ac }, type_index: TypeIndex(0x946c) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x581930 }, type_index: TypeIndex(0x2dca) })
	// ******

	// FUNCTION BODY
	// <0x5927df>|0x000|0x000:'431'
	// 1
	// <0x5927e6>|0x007|0x007:'433'
	// <0x59280f>|0x030|0x029:'434'
	// 1
	// <0x59281b>|0x03c|0x00c:'436'
	// <0x59282d>|0x04e|0x012:'437'
	// <0x592838>|0x059|0x00b:'438'
	// <0x59284c>|0x06d|0x014:'439'
	// 1
	// <0x592854>|0x075|0x008:'441'
	// 1
	// <0x592869>|0x08a|0x015:'443'
	// 1
	// <0x592878>|0x099|0x00f:'445'
	// 1
	// 2
	// <0x59288a>|0x0ab|0x012|[1]:'448'
	// <0x5928ae>|0x0cf|0x024:'449'
	// <0x5928b4>|0x0d5|0x006:'450'
	// 1
	// <0x5928bc>|0x0dd|0x008:'452'
	// <0x5928ce>|0x0ef|0x012:'453'
	// <0x5928de>|0x0ff|0x010:'454'
	// 1
	// 2
	// 3
	// <0x59290b>|0x12c|0x02d:'458'
	// 1
	// <0x592956>|0x177|0x04b:'460'
	// 1
	// <0x592968>|0x189|0x012:'462'
	// <0x592985>|0x1a6|0x01d:'463'
	// <0x5929e5>|0x206|0x060:'464'
	// 1
	// 2
	// <0x5929ea>|0x20b|0x005:'467'
	// <0x592a12>|0x233|0x028:'468'
	// ******
}

// STATE[STUB]
// survarium::collision_result survarium::bullet::process_ray_query(vostok::physics::closest_ray_result const&, float, vostok::math::float3&, vostok::math::float3&, float&, float&)
collision_result bullet::process_ray_query(
	vostok::physics::closest_ray_result const&	ray_result,
	float								distance,
	vostok::math::float3&				start_position,
	vostok::math::float3&				fly_direction,
	float&								start_time,
	float&								current_time)
{
	// LOCALS
	// vostok::math::float3 		collide_point
	// vostok::math::float3 		triangle_normal
	// vostok::physics::bt_rigid_body_base* body
	// float 						cos_alpha
	// float 						speed
	// float 						distance_to_hit_point
	// float 						angle_alpha
	// float 						collision_time
	// triangle_orientation 		orientation
	// u16 							game_material_id
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x581615 }, type_index: TypeIndex(0x94c6) })
	// ******

	// FUNCTION BODY
	// <0x59250f>|0x000|0x000:'480'
	// <0x592526>|0x017|0x017:'481'
	// <0x592543>|0x034|0x01d:'482'
	// <0x592566>|0x057|0x023:'483'
	// 1
	// <0x59258e>|0x07f|0x028:'485'
	// 1
	// 2
	// <0x59259e>|0x08f|0x010:'488'
	// 1
	// <0x5925cb>|0x0bc|0x02d:'490'
	// <0x5925d1>|0x0c2|0x006:'491'
	// 1
	// <0x5925d8>|0x0c9|0x007:'493'
	// 1
	// 2
	// 3
	// 4
	// <0x5925ec>|0x0dd|0x014:'498'
	// 1
	// 2
	// <0x5925f3>|0x0e4|0x007:'501'
	// 1
	// <0x5925fb>|0x0ec|0x008:'503'
	// 1
	// 2
	// <0x59261b>|0x10c|0x020:'506'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// <0x592640>|0x131|0x025:'513'
	// 1
	// 2
	// <0x59264c>|0x13d|0x00c:'516'
	// 1
	// 2
	// <0x592686>|0x177|0x03a:'519'
	// <0x5926c2>|0x1b3|0x03c:'520'
	// <0x5926cd>|0x1be|0x00b:'521'
	// <0x5926dc>|0x1cd|0x00f:'522'
	// 1
	// 2
	// <0x5926ee>|0x1df|0x012:'525'
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
	// <0x59273c>|0x22d|0x04e:'536'
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
	// <0x59278a>|0x27b|0x04e:'548'
	// ******
}

// STATE[STUB]
// void survarium::bullet::fix_collision_point_and_time(vostok::math::float3&, float&, float, float, const survarium::triangle_orientation, vostok::math::float3 const&, vostok::math::float3 const&)
void bullet::fix_collision_point_and_time(
	vostok::math::float3&			collide_point,
	float&							collision_time,
	float							start_time,
	float							current_time,
	triangle_orientation			orientation,
	vostok::math::float3 const&		triangle_normal,
	vostok::math::float3 const&		gravity)
{
	// LOCALS
	// float 						delta
	// vostok::math::float3 		new_collide_point
	// float 						high_time<1>
	// float 						low_time<1>
	// ******

	// SKIPPED BLOCKS
	// <0x591a85><1>
	// ******

	// FUNCTION BODY
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// <0x591a39>|0x000|0x000:'569'
	// <0x591a52>|0x019|0x019:'570'
	// 1
	// <0x591a73>|0x03a|0x021:'572'
	// 1
	// <0x591a8b>|0x052|0x018:'574'
	// <0x591a95>|0x05c|0x00a:'575'
	// 1
	// <0x591a9f>|0x066|0x00a:'577'
	// 1
	// 2
	// <0x591ab7>|0x07e|0x018:'580'
	// <0x591adf>|0x0a6|0x028:'581'
	// <0x591aeb>|0x0b2|0x00c:'582'
	// <0x591aed>|0x0b4|0x002:'583'
	// 1
	// <0x591af9>|0x0c0|0x00c:'585'
	// 1
	// <0x591b12>|0x0d9|0x019:'587'
	// <0x591b3c>|0x103|0x02a:'588'
	// <0x591b5f>|0x126|0x023:'589'
	// 1
	// <0x591b64>|0x12b|0x005:'591'
	// 1
	// ******
}

// STATE[STUB]
// survarium::collision_result survarium::bullet::collide_front_face(vostok::math::float3 const&, vostok::math::float3 const&, vostok::math::float3 const&, float, float, vostok::math::float3&, float&, float&, vostok::physics::closest_ray_result const&)
collision_result bullet::collide_front_face(
	vostok::math::float3 const&			collide_point,
	vostok::math::float3 const&			bullet_direction,
	vostok::math::float3 const&			triangle_normal,
	float								speed,
	float								collision_time,
	vostok::math::float3&				start_position,
	float&								start_time,
	float&								current_time,
	vostok::physics::closest_ray_result const&	ray_result)
{
	// LOCALS
	// vostok::physics::bt_rigid_body_base* target
	// float 						new_speed
	// material_pair const* 		mtl_pair<1>
	// hit_receiver* 				hit_target<1>
	// ******

	// SKIPPED BLOCKS
	// <0x592186><1>
	// <0x592313><1>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x581329 }, type_index: TypeIndex(0x2dca) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5813a2 }, type_index: TypeIndex(0x913e) })
	// ******

	// FUNCTION BODY
	// <0x592157>|0x000|0x000:'607'
	// <0x592163>|0x00c|0x00c:'608'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// <0x59216f>|0x018|0x00c:'616'
	// 1
	// <0x59218c>|0x035|0x01d:'618'
	// <0x5921d2>|0x07b|0x046:'619'
	// 1
	// <0x5921d8>|0x081|0x006:'621'
	// <0x5921e9>|0x092|0x011:'622'
	// 1
	// 2
	// 3
	// 4
	// <0x59221c>|0x0c5|0x033:'627'
	// <0x59222b>|0x0d4|0x00f:'628'
	// <0x59224e>|0x0f7|0x023:'629'
	// <0x592253>|0x0fc|0x005:'630'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// <0x592304>|0x1ad|0x0b1:'638'
	// <0x59230c>|0x1b5|0x008:'639'
	// 1
	// <0x592319>|0x1c2|0x00d:'641'
	// <0x59232e>|0x1d7|0x015:'642'
	// <0x592342>|0x1eb|0x014:'643'
	// 1
	// 2
	// <0x5923a4>|0x24d|0x062:'646'
	// <0x5923d7>|0x280|0x033:'647'
	// 1
	// <0x5923e1>|0x28a|0x00a:'649'
	// 1
	// <0x59243b>|0x2e4|0x05a:'651'
	// 1
	// <0x592464>|0x30d|0x029:'653'
	// 1
	// <0x5924ae>|0x357|0x04a:'655'
	// <0x5924ca>|0x373|0x01c:'656'
	// <0x5924d8>|0x381|0x00e:'657'
	// 1
	// <0x5924eb>|0x394|0x013:'659'
	// ******
}

// STATE[STUB]
// survarium::collision_result survarium::bullet::try_reflect(vostok::math::float3 const&, vostok::math::float3, vostok::math::float3 const&, float, float, vostok::math::float3&, float&, float&, float)
collision_result bullet::try_reflect(
	vostok::math::float3 const&		collide_point,
	vostok::math::float3			direction,
	vostok::math::float3 const&		triangle_normal,
	float							speed,
	float							collision_time,
	vostok::math::float3&			start_position,
	float&							start_time,
	float&							current_time,
	float							cos_alpha)
{
	// LOCALS
	// float 						fin_ricochet_angle
	// float 						angle_alpha
	// float 						calculated_koeff
	// ******

	// FUNCTION BODY
	// <0x591059>|0x000|0x000:'674'
	// <0x59107b>|0x022|0x022:'675'
	// 1
	// 2
	// <0x591093>|0x03a|0x018:'678'
	// 1
	// <0x5910ae>|0x055|0x01b:'680'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// <0x5910ef>|0x096|0x041:'689'
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x5910fd>|0x0a4|0x00e:'695'
	// 1
	// 2
	// <0x591107>|0x0ae|0x00a:'698'
	// 1
	// <0x59116a>|0x111|0x063:'700'
	// 1
	// <0x591190>|0x137|0x026:'702'
	// <0x5911a9>|0x150|0x019:'703'
	// <0x5911b4>|0x15b|0x00b:'704'
	// 1
	// <0x5911c7>|0x16e|0x013:'706'
	// 1
	// <0x5911d6>|0x17d|0x00f:'708'
	// ******
}

// STATE[STUB]
// void survarium::bullet::change_trajectory(vostok::math::float3 const&, vostok::math::float3 const&, float)
void bullet::change_trajectory( vostok::math::float3 const& new_position, vostok::math::float3 const& new_velocity, float collision_time )
{
	// FUNCTION BODY
	// <0x590f79>|0x000|0x000:'714'
	// 1
	// <0x590f88>|0x00f|0x00f:'716'
	// <0x590fa1>|0x028|0x019:'717'
	// <0x590fba>|0x041|0x019:'718'
	// <0x590fd3>|0x05a|0x019:'719'
	// <0x590fef>|0x076|0x01c:'720'
	// <0x590ffb>|0x082|0x00c:'721'
	// <0x591034>|0x0bb|0x039:'722'
	// ******
}

// STATE[STUB]
// bool survarium::bullet::update_bullet_position(float, vostok::math::float3 const&)
bool bullet::update_bullet_position( float time, vostok::math::float3 const& gravity )
{
	// LOCALS
	// vostok::math::float3 const& 	new_position
	// ******

	return false;
	// FUNCTION BODY
	// <0x591919>|0x000|0x000:'727'
	// <0x591936>|0x01d|0x01d:'728'
	// <0x59194c>|0x033|0x016:'729'
	// 1
	// <0x591953>|0x03a|0x007:'731'
	// 1
	// <0x591976>|0x05d|0x023:'733'
	// 1
	// <0x591987>|0x06e|0x011:'735'
	// <0x591993>|0x07a|0x00c:'736'
	// 1
	// 2
	// <0x59199a>|0x081|0x007:'739'
	// <0x5919ca>|0x0b1|0x030:'740'
	// <0x5919ec>|0x0d3|0x022:'741'
	// 1
	// <0x5919f0>|0x0d7|0x004:'743'
	// <0x591a06>|0x0ed|0x016:'744'
	// 1
	// <0x591a12>|0x0f9|0x00c:'746'
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x591a1f>|0x106|0x00d:'752'
	// ******
}

	/* TYPEDEFS

	typedef
		long
		counter_type;

	typedef
		vostok::memory::multi_threading_single_size_allocator_policy<vostok::memory::single_size_buffer_allocator<128,vostok::threading::simple_lock>::node>::free_list_type
		free_list_type;

} // namespace survarium
