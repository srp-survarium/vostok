////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/bullet.h>

namespace survarium {

/*
// STATE[STUB]
// void survarium::`dynamic atexit destructor for 'bullet_tracer_exposition''()
void `dynamic atexit destructor for 'bullet_tracer_exposition''( )
{
	// FUNCTION BODY
	// <0x7db380>|0x000|      :'30'	{
	// ******
}
*/

// STATE[STUB]
// survarium::bullet::bullet(survarium::bullet_manager&, vostok::math::float3 const&, vostok::math::float3 const&, const unsigned int, float, vostok::resources::resource_ptr<survarium::weapon_ammunition,vostok::resources::unmanaged_intrusive_base> const&, survarium::weapon_core const&, survarium::hit_initiator const* const, survarium::hit_receiver const* const)
bullet::bullet(
	bullet_manager&						bullet_manager,
	float3 const&						position,
	float3 const&						velocity,
	u32									born_time_in_ms,
	float								air_resistance,
	resources::resource_ptr<weapon_ammunition,resources::unmanaged_intrusive_base> const&	wa,
	weapon_core const&					wc,
	hit_initiator const*				initiator,
	hit_receiver const*					ignorable_object
)
{
	// FUNCTION BODY
	// <0x5916fa>|0x24a|+0x00c:'85'
	// <0x591706>|0x256|+0x00c:'86'
	// <0x591712>|0x262|+0x041:'87'
	// <0x591753>|0x2a3|+0x00c:'88'
	// ******
}

// STATE[STUB]
// survarium::bullet::bullet(survarium::bullet const&)
bullet::bullet( bullet const& other )
{
	// FUNCTION BODY
	// <0x590e3a>|0x03a|+0x016:'96'
	// <0x590e50>|0x050|+0x01c:'97'
	// <0x590e6c>|0x06c|+0x019:'98'
	// <0x590e85>|0x085|+0x01c:'99'
	// <0x590ea1>|0x0a1|+0x00c:'100'
	// <0>
	// <0x590ead>|0x0ad|+0x00c:'102'
	// <0x590eb9>|0x0b9|+0x00c:'103'
	// <0x590ec5>|0x0c5|+0x00c:'104'
	// <0x590ed1>|0x0d1|+0x00c:'105'
	// <0x590edd>|0x0dd|+0x00c:'106'
	// <0x590ee9>|0x0e9|+0x00c:'107'
	// <0>
	// <0x590ef5>|0x0f5|+0x00c:'109'
	// <0x590f01>|0x101|+0x00c:'110'
	// <0x590f0d>|0x10d|+0x00c:'111'
	// <0x590f19>|0x119|+0x00c:'112'
	// <0x590f25>|0x125|+0x00c:'113'
	// <0x590f31>|0x131|+0x00c:'114'
	// <0x590f3d>|0x13d|+0x00c:'115'
	// <0x590f49>|0x149|+0x00e:'116'
	// <0>
	// <0x590f57>|0x157|+0x00c:'118'
	// ******
}

// STATE[STUB]
// void survarium::bullet::~bullet()
bullet::~bullet( )
{
	// FUNCTION BODY
	// <0x590de0>|0x000|+0x007:'122'	{
	// <0x590de7>|0x007|      :'123'	}
	// ******
}

// STATE[STUB]
// void survarium::bullet::tick(unsigned int)
void bullet::tick( u32 current_time_in_ms )
{
	// LOCALS
	// u16 							invalid_tracer_idx
	// float 						high_time
	// float 						low_time
	// float3 const& 				gravity
	// float3 						zero_velocity
	// collision_result 			result<1>
	// float 						time<1>
	// float3 						d<2>
	// float 						d_len<2>
	// float 						speed<2>
	// float 						length<2>
	// ******

	// SKIPPED BLOCKS
	// <0x592c94><2>
	// ******

	// FUNCTION BODY
	// <0x592a2f>|0x00f|+0x00c:'127'
	// <0x592a3b>|0x01b|+0x01a:'128'
	// <0x592a55>|0x035|+0x009:'129'
	// <0x592a5e>|0x03e|+0x010:'130'
	// <0x592a6e>|0x04e|+0x031:'131'
	// <0x592a9f>|0x07f|+0x00c:'132'
	// <0>
	// <0x592aab>|0x08b|+0x00f:'134'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x592aba>|0x09a|+0x016|[1]:'143'
	// <0>
	// <0x592ad0>|0x0b0|+0x01a:'145'
	// <0x592aea>|0x0ca|+0x005:'146'
	// <0>
	// <1>
	// <0x592aef>|0x0cf|+0x00c:'149'
	// <0>
	// <0x592afb>|0x0db|+0x01a:'151'
	// <0x592b15>|0x0f5|+0x005:'152'
	// <0>
	// <1>
	// <0x592b1a>|0x0fa|+0x00f:'155'
	// <0x592b29>|0x109|+0x005:'156'
	// <0>
	// <0x592b2e>|0x10e|+0x020:'158'
	// <0x592b4e>|0x12e|+0x00f:'159'
	// <0>
	// <0x592b5d>|0x13d|+0x01a:'161'
	// <0x592b77>|0x157|+0x005:'162'
	// <0>
	// <1>
	// <0x592b7c>|0x15c|+0x037:'165'
	// <0x592bb3>|0x193|+0x006:'166'
	// <0>
	// <0x592bb9>|0x199|+0x01a:'168'
	// <0x592bd3>|0x1b3|+0x005:'169'
	// <0>
	// <1>
	// <0x592bd8>|0x1b8|+0x00c:'172'
	// <0>
	// <0x592be4>|0x1c4|+0x010:'174'
	// <0x592bf4>|0x1d4|+0x00f:'175'
	// <0x592c03>|0x1e3|+0x010:'176'
	// <0>
	// <1>
	// <0x592c13>|0x1f3|+0x012:'179'
	// <0>
	// <0x592c25>|0x205|+0x01d:'181'
	// <0>
	// <0x592c42>|0x222|+0x01a:'183'
	// <0x592c5c>|0x23c|+0x005:'184'
	// <0>
	// <1>
	// <2>
	// <0x592c61>|0x241|+0x023:'188'
	// <0>
	// <0x592c84>|0x264|+0x016:'190'
	// <0>
	// <0x592c9a>|0x27a|+0x01b:'192'
	// <0x592cb5>|0x295|+0x00b:'193'
	// <0x592cc0>|0x2a0|+0x023:'194'
	// <0x592ce3>|0x2c3|+0x011:'195'
	// <0>
	// <0x592cf4>|0x2d4|+0x022:'197'
	// <0>
	// <1>
	// <0x592d16>|0x2f6|+0x031:'200'
	// <0x592d47>|0x327|+0x012:'201'
	// <0>
	// <0x592d59>|0x339|+0x017:'203'
	// <0x592d70>|0x350|+0x00a:'204'
	// <0>
	// <0x592d7a>|0x35a|+0x00e:'206'
	// <0x592d88>|0x368|+0x027:'207'
	// <0>
	// <0x592daf>|0x38f|+0x002:'209'
	// <0>
	// <1>
	// <0x592db1>|0x391|+0x00c:'212'
	// <0x592dbd>|0x39d|+0x00a:'213'
	// <0x592dc7>|0x3a7|+0x011:'214'
	// <0>
	// ******
}

// STATE[STUB]
// vostok::math::float3 survarium::bullet::compute_parabolic_velocity(float, vostok::math::float3 const&)
float3 bullet::compute_parabolic_velocity( float time, float3 const& gravity )
{
	// LOCALS
	// float3 						xz_velocity
	// ******

	return vostok::math::float3(1., 1., 1.);

	// FUNCTION BODY
	// <0x5913ca>|0x00a|+0x022:'240'
	// <0>
	// <1>
	// <0x5913ec>|0x02c|+0x01f:'243'
	// <0x59140b>|0x04b|+0x02b:'244'
	// <0>
	// <0x591436>|0x076|+0x067:'246'
	// ******
}

// STATE[STUB]
// vostok::math::float3 survarium::bullet::compute_trajectory_velocity(float, vostok::math::float3 const&)
float3 bullet::compute_trajectory_velocity( float time, float3 const& gravity )
{
	// LOCALS
	// float3 const& 				parabolic_vel
	// float 						fall_down_time
	// float 						parabolic_time
	// ******

	return vostok::math::float3(1., 1., 1.);

	// FUNCTION BODY
	// <0x591879>|0x009|+0x00b:'251'
	// <0x591884>|0x014|+0x00f:'252'
	// <0>
	// <0x591893>|0x023|+0x00e:'254'
	// <0x5918a1>|0x031|+0x01c:'255'
	// <0>
	// <0x5918bd>|0x04d|+0x01d:'257'
	// <0x5918da>|0x06a|+0x026:'258'
	// ******
}

// STATE[STUB]
// vostok::math::float3 survarium::bullet::compute_parabolic_position(float, vostok::math::float3 const&)
float3 bullet::compute_parabolic_position( float time, float3 const& gravity )
{
	// LOCALS
	// float3 						xz_velocity
	// float 						sqr_t_div_2
	// ******

	return vostok::math::float3(1., 1., 1.);

	// FUNCTION BODY
	// <0x591241>|0x011|+0x028:'263'
	// <0>
	// <0x591269>|0x039|+0x01f:'265'
	// <0x591288>|0x058|+0x070:'266'
	// <0>
	// <0x5912f8>|0x0c8|+0x015:'268'
	// <0x59130d>|0x0dd|+0x0ab:'269'
	// ******
}

// STATE[STUB]
// vostok::math::float3 survarium::bullet::compute_trajectory_position(float, vostok::math::float3 const&)
float3 bullet::compute_trajectory_position( float time, float3 const& gravity )
{
	// LOCALS
	// float3 const& 				parabolic_pos
	// float3 const& 				parabolic_vel
	// float 						fall_down_time
	// float 						parabolic_time
	// ******

	return vostok::math::float3(1., 1., 1.);

	// FUNCTION BODY
	// <0x59177a>|0x00a|+0x00b:'274'
	// <0x591785>|0x015|+0x00f:'275'
	// <0>
	// <0x591794>|0x024|+0x00e:'277'
	// <0x5917a2>|0x032|+0x01f:'278'
	// <0>
	// <0x5917c1>|0x051|+0x01d:'280'
	// <0x5917de>|0x06e|+0x01d:'281'
	// <0>
	// <0x5917fb>|0x08b|+0x05f:'283'
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
	// <0>
	// <0x5911f9>|0x009|+0x031:'289'
	// ******
}

// STATE[STUB]
// float survarium::bullet::pick_next_permissible_time(float, float, vostok::math::float3 const&)
float bullet::pick_next_permissible_time( float low_time, float high_time, float3 const& gravity )
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
	// <0x592039>|0x009|+0x00c:'294'
	// <0x592045>|0x015|+0x00a:'295'
	// <0>
	// <0x59204f>|0x01f|+0x025:'297'
	// <0>
	// <0x592074>|0x044|+0x00b:'299'
	// <0x59207f>|0x04f|+0x008:'300'
	// <0>
	// <0x592087>|0x057|+0x008:'302'
	// <0>
	// <1>
	// <0x59208f>|0x05f|+0x00a:'305'
	// <0x592099>|0x069|+0x00a:'306'
	// <0x5920a3>|0x073|+0x00d:'307'
	// <0x5920b0>|0x080|+0x01f:'308'
	// <0>
	// <0x5920cf>|0x09f|+0x01d:'310'
	// <0>
	// <0x5920ec>|0x0bc|+0x00b:'312'
	// <0x5920f7>|0x0c7|+0x00a:'313'
	// <0x592101>|0x0d1|+0x002:'314'
	// <0x592103>|0x0d3|+0x00a:'315'
	// <0>
	// <0x59210d>|0x0dd|+0x017:'317'
	// <0x592124>|0x0f4|+0x002:'318'
	// <0>
	// <0x592126>|0x0f6|+0x00c:'320'
	// <0x592132>|0x102|+0x003:'321'
	// ******
}

// STATE[STUB]
// bool survarium::bullet::pick_permissible_range(float&, float, float, vostok::math::float3 const&)
bool bullet::pick_permissible_range(
	float&				result,
	float				low_time,
	float				high_time,
	float3 const&		gravity
)
{
	// LOCALS
	// float 						parabolic_time
	// ******

	return false;

	// FUNCTION BODY
	// <0>
	// <1>
	// <0x591f59>|0x009|+0x00b:'328'
	// <0>
	// <0x591f64>|0x014|+0x00b:'330'
	// <0>
	// <0x591f6f>|0x01f|+0x01f:'332'
	// <0x591f8e>|0x03e|+0x007:'333'
	// <0>
	// <1>
	// <0x591f95>|0x045|+0x00b:'336'
	// <0>
	// <0x591fa0>|0x050|+0x01f:'338'
	// <0x591fbf>|0x06f|+0x004:'339'
	// <0>
	// <1>
	// <0x591fc3>|0x073|+0x01f:'342'
	// <0>
	// <0x591fe2>|0x092|+0x01f:'344'
	// <0x592001>|0x0b1|+0x004:'345'
	// <0>
	// <1>
	// <0x592005>|0x0b5|+0x01f:'348'
	// <0x592024>|0x0d4|+0x002:'349'
	// ******
}

// STATE[STUB]
// float survarium::bullet::get_check_time_in_vacuum(float, float, vostok::math::float3 const&)
float bullet::get_check_time_in_vacuum( float start_low, float high, float3 const& gravity )
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
	// <0x591dfa>|0x00a|+0x015:'354'
	// <0x591e0f>|0x01f|+0x00f:'355'
	// <0x591e1e>|0x02e|+0x056:'356'
	// <0>
	// <0x591e74>|0x084|+0x00b:'358'
	// <0x591e7f>|0x08f|+0x008:'359'
	// <0>
	// <0x591e87>|0x097|+0x01f:'361'
	// <0x591ea6>|0x0b6|+0x014:'362'
	// <0x591eba>|0x0ca|+0x035:'363'
	// <0x591eef>|0x0ff|+0x00c:'364'
	// <0>
	// <0x591efb>|0x10b|+0x00c:'366'
	// <0x591f07>|0x117|+0x00f:'367'
	// <0x591f16>|0x126|+0x017:'368'
	// <0x591f2d>|0x13d|+0x00c:'369'
	// <0>
	// <0x591f39>|0x149|+0x003:'371'
	// ******
}

// STATE[STUB]
// float survarium::get_max_error_time(const float, const float)
float get_max_error_time( float t0, float t1 )
{
	return 0.0f;

	// FUNCTION BODY
	// <0x590dc3>|0x003|+0x00c:'376'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
// float survarium::bullet::get_check_time(const float, float, vostok::math::float3 const&)
float bullet::get_check_time( float start_low, float high, float3 const& gravity )
{
	// LOCALS
	// float 						max_test_distance
	// float 						low
	// float3 						start
	// float 						check_time
	// float3 						intermediate<1>
	// float3 						target<1>
	// float 						distance<1>
	// ******

	// SKIPPED BLOCKS
	// <0x591d16><1>
	// ******

	return 0.0f;

	// FUNCTION BODY
	// <0x591cb9>|0x009|+0x015:'384'
	// <0x591cce>|0x01e|+0x017:'385'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x591ce5>|0x035|+0x00a:'391'
	// <0x591cef>|0x03f|+0x00a:'392'
	// <0>
	// <0x591cf9>|0x049|+0x023:'394'
	// <0>
	// <0x591d1c>|0x06c|+0x028:'396'
	// <0x591d44>|0x094|+0x017:'397'
	// <0x591d5b>|0x0ab|+0x037:'398'
	// <0x591d92>|0x0e2|+0x00b:'399'
	// <0x591d9d>|0x0ed|+0x00a:'400'
	// <0x591da7>|0x0f7|+0x002:'401'
	// <0x591da9>|0x0f9|+0x00a:'402'
	// <0>
	// <0x591db3>|0x103|+0x017:'404'
	// <0x591dca>|0x11a|+0x005:'405'
	// <0>
	// <0x591dcf>|0x11f|+0x00c:'407'
	// <0x591ddb>|0x12b|+0x003:'408'
	// ******
}

// STATE[STUB]
// float survarium::bullet::compute_max_error(float, float, vostok::math::float3 const&)
float bullet::compute_max_error( float low, float high, float3 const& gravity )
{
	// LOCALS
	// float3 						start_to_target
	// float3 						target
	// float3 						start
	// float3 						start_to_max_error
	// float 						max_error_time
	// float 						magnitude
	// float 						cosine_alpha
	// float3 						max_error
	// float 						sine_alpha
	// ******

	return 0.0f;

	// FUNCTION BODY
	// <0x591b89>|0x009|+0x017:'413'
	// <0>
	// <0x591ba0>|0x020|+0x017:'415'
	// <0x591bb7>|0x037|+0x017:'416'
	// <0x591bce>|0x04e|+0x017:'417'
	// <0>
	// <0x591be5>|0x065|+0x012:'419'
	// <0x591bf7>|0x077|+0x00b:'420'
	// <0x591c02>|0x082|+0x015:'421'
	// <0x591c17>|0x097|+0x02a:'422'
	// <0x591c41>|0x0c1|+0x032:'423'
	// <0x591c73>|0x0f3|+0x025:'424'
	// <0>
	// <0x591c98>|0x118|+0x006:'426'
	// ******
}

// STATE[STUB]
// survarium::collision_result survarium::bullet::check_collision(vostok::math::float3, float, float)
collision_result bullet::check_collision( float3 start_position, float start_time, float current_time )
{
	// LOCALS
	// float3 const& 				target_position
	// collision_result 			result
	// physics::world& 				p_world
	// float 						distance
	// float3 						new_start_position
	// float3 						direction
	// bool 						ignorable_object_was_hit<1>
	// float3 						triangle_normal<1>
	// physics::closest_ray_result 	ray_result<1>
	// float 						cos_alpha<1>
	// triangle_orientation 		orientation<1>
	// ******

	// CALL SITE INFO
	// <0x5928ac> -> physics::closest_ray_result <unknown>(float3 const&, float3 const&, const float, u16, u16)
	// <0x592930> -> hit_receiver* <unknown>()
	// ******

	// FUNCTION BODY
	// <0x5927df>|0x00f|+0x007:'431'
	// <0>
	// <0x5927e6>|0x016|+0x029:'433'
	// <0x59280f>|0x03f|+0x00c:'434'
	// <0>
	// <0x59281b>|0x04b|+0x012:'436'
	// <0x59282d>|0x05d|+0x00b:'437'
	// <0x592838>|0x068|+0x014:'438'
	// <0x59284c>|0x07c|+0x008:'439'
	// <0>
	// <0x592854>|0x084|+0x015:'441'
	// <0>
	// <0x592869>|0x099|+0x00f:'443'
	// <0>
	// <0x592878>|0x0a8|+0x012:'445'
	// <0>
	// <1>
	// <0x59288a>|0x0ba|+0x024|[1]:'448'
	// <0x5928ae>|0x0de|+0x006:'449'
	// <0x5928b4>|0x0e4|+0x008:'450'
	// <0>
	// <0x5928bc>|0x0ec|+0x012:'452'
	// <0x5928ce>|0x0fe|+0x010:'453'
	// <0x5928de>|0x10e|+0x02d:'454'
	// <0>
	// <1>
	// <2>
	// <0x59290b>|0x13b|+0x04b:'458'
	// <0>
	// <0x592956>|0x186|+0x012:'460'
	// <0>
	// <0x592968>|0x198|+0x01d:'462'
	// <0x592985>|0x1b5|+0x060:'463'
	// <0x5929e5>|0x215|+0x005:'464'
	// <0>
	// <1>
	// <0x5929ea>|0x21a|+0x028:'467'
	// <0x592a12>|0x242|+0x005:'468'
	// ******
}

// STATE[STUB]
// survarium::collision_result survarium::bullet::process_ray_query(vostok::physics::closest_ray_result const&, float, vostok::math::float3&, vostok::math::float3&, float&, float&)
collision_result bullet::process_ray_query(
	physics::closest_ray_result const&	ray_result,
	float								distance,
	float3&								start_position,
	float3&								fly_direction,
	float&								start_time,
	float&								current_time
)
{
	// LOCALS
	// float3 						collide_point
	// float3 						triangle_normal
	// physics::bt_rigid_body_base* body
	// float 						cos_alpha
	// float 						speed
	// float 						distance_to_hit_point
	// float 						angle_alpha
	// float 						collision_time
	// triangle_orientation 		orientation
	// u16 							game_material_id
	// ******

	// CALL SITE INFO
	// <0x592615> -> u16 <unknown>(const int, const bool) const
	// ******

	// FUNCTION BODY
	// <0x59250f>|0x00f|+0x017:'480'
	// <0x592526>|0x026|+0x01d:'481'
	// <0x592543>|0x043|+0x023:'482'
	// <0x592566>|0x066|+0x028:'483'
	// <0>
	// <0x59258e>|0x08e|+0x010:'485'
	// <0>
	// <1>
	// <0x59259e>|0x09e|+0x02d:'488'
	// <0>
	// <0x5925cb>|0x0cb|+0x006:'490'
	// <0x5925d1>|0x0d1|+0x007:'491'
	// <0>
	// <0x5925d8>|0x0d8|+0x014:'493'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5925ec>|0x0ec|+0x007:'498'
	// <0>
	// <1>
	// <0x5925f3>|0x0f3|+0x008:'501'
	// <0>
	// <0x5925fb>|0x0fb|+0x020:'503'
	// <0>
	// <1>
	// <0x59261b>|0x11b|+0x025:'506'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x592640>|0x140|+0x00c:'513'
	// <0>
	// <1>
	// <0x59264c>|0x14c|+0x03a:'516'
	// <0>
	// <1>
	// <0x592686>|0x186|+0x03c:'519'
	// <0x5926c2>|0x1c2|+0x00b:'520'
	// <0x5926cd>|0x1cd|+0x00f:'521'
	// <0x5926dc>|0x1dc|+0x012:'522'
	// <0>
	// <1>
	// <0x5926ee>|0x1ee|+0x04e:'525'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <0x59273c>|0x23c|+0x04e:'536'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <0x59278a>|0x28a|+0x035:'548'
	// ******
}

// STATE[STUB]
// void survarium::bullet::fix_collision_point_and_time(vostok::math::float3&, float&, float, float, const survarium::triangle_orientation, vostok::math::float3 const&, vostok::math::float3 const&)
void bullet::fix_collision_point_and_time(
	float3&					collide_point,
	float&					collision_time,
	float					start_time,
	float					current_time,
	triangle_orientation	orientation,
	float3 const&			triangle_normal,
	float3 const&			gravity
)
{
	// LOCALS
	// float 						delta
	// float3 						new_collide_point
	// float 						high_time<1>
	// float 						low_time<1>
	// ******

	// SKIPPED BLOCKS
	// <0x591a85><1>
	// ******

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x591a39>|0x009|+0x019:'569'
	// <0x591a52>|0x022|+0x021:'570'
	// <0>
	// <0x591a73>|0x043|+0x018:'572'
	// <0>
	// <0x591a8b>|0x05b|+0x00a:'574'
	// <0x591a95>|0x065|+0x00a:'575'
	// <0>
	// <0x591a9f>|0x06f|+0x018:'577'
	// <0>
	// <1>
	// <0x591ab7>|0x087|+0x028:'580'
	// <0x591adf>|0x0af|+0x00c:'581'
	// <0x591aeb>|0x0bb|+0x002:'582'
	// <0x591aed>|0x0bd|+0x00c:'583'
	// <0>
	// <0x591af9>|0x0c9|+0x019:'585'
	// <0>
	// <0x591b12>|0x0e2|+0x02a:'587'
	// <0x591b3c>|0x10c|+0x023:'588'
	// <0x591b5f>|0x12f|+0x005:'589'
	// <0>
	// <0x591b64>|0x134|+0x014:'591'
	// <0>
	// ******
}

// STATE[STUB]
// survarium::collision_result survarium::bullet::collide_front_face(vostok::math::float3 const&, vostok::math::float3 const&, vostok::math::float3 const&, float, float, vostok::math::float3&, float&, float&, vostok::physics::closest_ray_result const&)
collision_result bullet::collide_front_face(
	float3 const&						collide_point,
	float3 const&						bullet_direction,
	float3 const&						triangle_normal,
	float								speed,
	float								collision_time,
	float3&								start_position,
	float&								start_time,
	float&								current_time,
	physics::closest_ray_result const&	ray_result
)
{
	// LOCALS
	// physics::bt_rigid_body_base* target
	// float 						new_speed
	// material_pair const* 		mtl_pair<1>
	// hit_receiver* 				hit_target<1>
	// ******

	// SKIPPED BLOCKS
	// <0x592186><1>
	// <0x592313><1>
	// ******

	// CALL SITE INFO
	// <0x592329> -> hit_receiver* <unknown>()
	// <0x5923a2> -> void <unknown>(hit_initiator const* const, const u32, pcstr, const float, const float, bullet* const)
	// ******

	// FUNCTION BODY
	// <0x592157>|0x017|+0x00c:'607'
	// <0x592163>|0x023|+0x00c:'608'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x59216f>|0x02f|+0x01d:'616'
	// <0>
	// <0x59218c>|0x04c|+0x046:'618'
	// <0x5921d2>|0x092|+0x006:'619'
	// <0>
	// <0x5921d8>|0x098|+0x011:'621'
	// <0x5921e9>|0x0a9|+0x033:'622'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x59221c>|0x0dc|+0x00f:'627'
	// <0x59222b>|0x0eb|+0x023:'628'
	// <0x59224e>|0x10e|+0x005:'629'
	// <0x592253>|0x113|+0x0b1:'630'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x592304>|0x1c4|+0x008:'638'
	// <0x59230c>|0x1cc|+0x00d:'639'
	// <0>
	// <0x592319>|0x1d9|+0x015:'641'
	// <0x59232e>|0x1ee|+0x014:'642'
	// <0x592342>|0x202|+0x062:'643'
	// <0>
	// <1>
	// <0x5923a4>|0x264|+0x033:'646'
	// <0x5923d7>|0x297|+0x00a:'647'
	// <0>
	// <0x5923e1>|0x2a1|+0x05a:'649'
	// <0>
	// <0x59243b>|0x2fb|+0x029:'651'
	// <0>
	// <0x592464>|0x324|+0x04a:'653'
	// <0>
	// <0x5924ae>|0x36e|+0x01c:'655'
	// <0x5924ca>|0x38a|+0x00e:'656'
	// <0x5924d8>|0x398|+0x013:'657'
	// <0>
	// <0x5924eb>|0x3ab|+0x005:'659'
	// ******
}

// STATE[STUB]
// survarium::collision_result survarium::bullet::try_reflect(vostok::math::float3 const&, vostok::math::float3, vostok::math::float3 const&, float, float, vostok::math::float3&, float&, float&, float)
collision_result bullet::try_reflect(
	float3 const&		collide_point,
	float3				direction,
	float3 const&		triangle_normal,
	float				speed,
	float				collision_time,
	float3&				start_position,
	float&				start_time,
	float&				current_time,
	float				cos_alpha
)
{
	// LOCALS
	// float 						fin_ricochet_angle
	// float 						angle_alpha
	// float 						calculated_koeff
	// ******

	// FUNCTION BODY
	// <0x591059>|0x009|+0x022:'674'
	// <0x59107b>|0x02b|+0x018:'675'
	// <0>
	// <1>
	// <0x591093>|0x043|+0x01b:'678'
	// <0>
	// <0x5910ae>|0x05e|+0x041:'680'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5910ef>|0x09f|+0x00e:'689'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5910fd>|0x0ad|+0x00a:'695'
	// <0>
	// <1>
	// <0x591107>|0x0b7|+0x063:'698'
	// <0>
	// <0x59116a>|0x11a|+0x026:'700'
	// <0>
	// <0x591190>|0x140|+0x019:'702'
	// <0x5911a9>|0x159|+0x00b:'703'
	// <0x5911b4>|0x164|+0x013:'704'
	// <0>
	// <0x5911c7>|0x177|+0x00f:'706'
	// <0>
	// <0x5911d6>|0x186|+0x005:'708'
	// ******
}

// STATE[STUB]
// void survarium::bullet::change_trajectory(vostok::math::float3 const&, vostok::math::float3 const&, float)
void bullet::change_trajectory( float3 const& new_position, float3 const& new_velocity, float collision_time )
{
	// FUNCTION BODY
	// <0x590f79>|0x009|+0x00f:'714'
	// <0>
	// <0x590f88>|0x018|+0x019:'716'
	// <0x590fa1>|0x031|+0x019:'717'
	// <0x590fba>|0x04a|+0x019:'718'
	// <0x590fd3>|0x063|+0x01c:'719'
	// <0x590fef>|0x07f|+0x00c:'720'
	// <0x590ffb>|0x08b|+0x039:'721'
	// <0x591034>|0x0c4|+0x010:'722'
	// ******
}

// STATE[STUB]
// bool survarium::bullet::update_bullet_position(float, vostok::math::float3 const&)
bool bullet::update_bullet_position( float time, float3 const& gravity )
{
	// LOCALS
	// float3 const& 				new_position
	// ******

	return false;

	// FUNCTION BODY
	// <0x591919>|0x009|+0x01d:'727'
	// <0x591936>|0x026|+0x016:'728'
	// <0x59194c>|0x03c|+0x007:'729'
	// <0>
	// <0x591953>|0x043|+0x023:'731'
	// <0>
	// <0x591976>|0x066|+0x011:'733'
	// <0>
	// <0x591987>|0x077|+0x00c:'735'
	// <0x591993>|0x083|+0x007:'736'
	// <0>
	// <1>
	// <0x59199a>|0x08a|+0x030:'739'
	// <0x5919ca>|0x0ba|+0x022:'740'
	// <0x5919ec>|0x0dc|+0x004:'741'
	// <0>
	// <0x5919f0>|0x0e0|+0x016:'743'
	// <0x591a06>|0x0f6|+0x00c:'744'
	// <0>
	// <0x591a12>|0x102|+0x00d:'746'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x591a1f>|0x10f|+0x002:'752'
	// ******
}

} // namespace survarium
