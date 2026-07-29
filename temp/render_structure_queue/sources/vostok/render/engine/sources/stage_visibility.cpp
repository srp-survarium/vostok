////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\stage_visibility.h"

namespace vostok {
namespace render {

// STATE[STUB]
void `dynamic initializer for 's_no_trees''( )
{
	// FUNCTION BODY[0x7d7370]
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_no_bushes''( )
{
	// FUNCTION BODY[0x7d73c0]
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_no_terrain''( )
{
	// FUNCTION BODY[0x7d7410]
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_no_house''( )
{
	// FUNCTION BODY[0x7d7460]
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_no_background''( )
{
	// FUNCTION BODY[0x7d74b0]
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_no_flora''( )
{
	// FUNCTION BODY[0x7d7500]
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_no_other''( )
{
	// FUNCTION BODY[0x7d7550]
	// ******
}

// STATE[STUB]
void `dynamic atexit destructor for 's_no_trees''( )
{
	// FUNCTION BODY[0x7ef410]
	// ******
}

// STATE[STUB]
float4 aabb_to_occlusion_bound( math::aabb const& in_aabb, float4x4 const& in_instance_transform )
{
	// LOCALS
	// math::aabb 						bound_box
	// ******

	return vostok::math::float4(1., 1., 1., 1.);

	// FUNCTION BODY[0x604bd0]: 7
	// <0x604bd3>|0x003|+0x019:'58'
	// <0x604bec>|0x01c|+0x00e:'59'
	// <0x604bfa>|0x02a|+0x02b:'60'
	// <0>
	// <0x604c25>|0x055|+0x04f:'62'
	// <0>
	// <0x604c74>|0x0a4|+0x021:'64'
	// ******
}

// STATE[STUB]
 stage_visibility::stage_visibility( renderer* in_renderer, renderer_context* context )
{
	// FUNCTION BODY[0x606490]: 10
	// <0x6064ac>|0x01c|+0x004:'83'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6064b0>|0x020|+0x036:'88'
	// <0>
	// <0x6064e6>|0x056|+0x025:'90'
	// <0x60650b>|0x07b|+0x044:'91'
	// <0x60654f>|0x0bf|+0x007:'92'
	// ******
}

// STATE[STUB]
 stage_visibility::~stage_visibility( )
{
	// FUNCTION BODY[0x605f20]: 3
	// <0x605f27>|0x007|+0x01b:'97'
	// <0x605f42>|0x022|+0x017:'98'
	// <0x605f59>|0x039|+0x029:'99'
	// ******
}

// STATE[STUB]
void stage_visibility::debug_render( )
{
	// STATICS
	// static bool 						s_draw = <0x4c265b6>;
	// ******

	// FUNCTION BODY[0x605f90]: 4
	// <0>
	// <1>
	// <0x605f90>|0x000|+0x009:'106'
	// <0x605f99>|0x009|+0x013:'107'
	// ******
}

// STATE[STUB]
void stage_visibility::execute( )
{
	// LOCALS
	// vector< render_model_instance_impl* > models
	// fixed_string< 128 > 				model_name
	// ******

	// TYPEDEFS
	// typedef
	// 	vector< render_model_instance_impl* >
	// 	models_array_type;

	// ******

	// CALL SITE INFO
	// <0x6063b6> -> u32 < unknown >() const
	// <0x6063d0> -> u32 < unknown >() const
	// ******

	// FUNCTION BODY[0x606050]: 91
	// <0>
	// <1>
	// <0x60605c>|0x00c|+0x019:'114'
	// <0>
	// <0x606075>|0x025|+0x006:'116'
	// <0>
	// <0x60607b>|0x02b|+0x002:'118'
	// <0>
	// <1>
	// <2>
	// <0x60607d>|0x02d|+0x010:'122'
	// <0>
	// <0x60608d>|0x03d|+0x009:'124'
	// <0>
	// <0x606096>|0x046|+0x005:'126'
	// <0>
	// <0x60609b>|0x04b|+0x005:'128'
	// <0x6060a0>|0x050|+0x003:'129'
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
	// <0x6060a3>|0x053|+0x03c:'140'
	// <0>
	// <1>
	// <2>
	// <0x6060df>|0x08f|+0x009:'144'
	// <0>
	// <0x6060e8>|0x098|+0x012:'146'
	// <0x6060fa>|0x0aa|+0x011:'147'
	// <0>
	// <1>
	// <0x60610b>|0x0bb|+0x01c:'150'
	// <0>
	// <0x606127>|0x0d7|+0x009:'152'
	// <0>
	// <0x606130>|0x0e0|+0x006:'154'
	// <0>
	// <0x606136>|0x0e6|+0x03c:'156'
	// <0>
	// <0x606172>|0x122|+0x00c:'158'
	// <0>
	// <0x60617e>|0x12e|+0x01f:'160'
	// <0x60619d>|0x14d|+0x027:'161'
	// <0x6061c4>|0x174|+0x021:'162'
	// <0x6061e5>|0x195|+0x021:'163'
	// <0x606206>|0x1b6|+0x021:'164'
	// <0x606227>|0x1d7|+0x01d:'165'
	// <0x606244>|0x1f4|+0x01d:'166'
	// <0x606261>|0x211|+0x01d:'167'
	// <0x60627e>|0x22e|+0x023:'168'
	// <0>
	// <0x6062a1>|0x251|+0x00c:'170'
	// <0>
	// <0x6062ad>|0x25d|+0x007:'172'
	// <0x6062b4>|0x264|+0x009:'173'
	// <0>
	// <1>
	// <0x6062bd>|0x26d|+0x025:'176'
	// <0x6062e2>|0x292|+0x00b:'177'
	// <0>
	// <0x6062ed>|0x29d|+0x025:'179'
	// <0x606312>|0x2c2|+0x00b:'180'
	// <0>
	// <0x60631d>|0x2cd|+0x025:'182'
	// <0x606342>|0x2f2|+0x00b:'183'
	// <0>
	// <0x60634d>|0x2fd|+0x025:'185'
	// <0x606372>|0x322|+0x00b:'186'
	// <0>
	// <0x60637d>|0x32d|+0x023:'188'
	// <0x6063a0>|0x350|+0x007:'189'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6063a7>|0x357|+0x0aa:'195'
	// <0>
	// <0x606451>|0x401|+0x012:'197'
	// <0>
	// <1>
	// <0x606463>|0x413|+0x01d:'200'
	// <0>
	// <0x606480>|0x430|+0x007:'202'
	// ******
}

// STATE[STUB]
bool stage_visibility::query_data( )
{
	return false;

	// FUNCTION BODY[0x604680]: 4
	// <0>
	// <1>
	// <2>
	// <0x604680>|0x000|+0x012:'210'
	// ******
}

// STATE[STUB]
void stage_visibility::frustum_culling( )
{
	// LOCALS
	// math::frustum 					view_frustum
	// scene* 							render_scene
	// math::random32 					random
	// vectora< collision::object const* > light_objects
	// collision::object const* const* 	e
	// collision::object const* const* 	i
	// vectora< collision::object const* > decals_objects
	// collision::object const* const* 	e
	// collision::object const* const* 	i
	// vectora< collision::object const* > probe_objects
	// collision::object const* const* 	e
	// collision::object const* const* 	i
	// ambient_volume** 				end
	// ******

	// CALL SITE INFO
	// <0x6059b1> -> void < unknown >( float4x4 const&, vectora< particle::render_particle_emitter_instance* >& )
	// <0x6059f5> -> bool < unknown >( u32, math::cuboid const&, vectora< collision::object const* >& )
	// <0x605af0> -> bool < unknown >( u32, math::cuboid const&, vectora< collision::object const* >& )
	// <0x605c95> -> bool < unknown >( u32, math::cuboid const&, vectora< collision::object const* >& )
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5f4f1a }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1132") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5f4cdc }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1133") })
	// ******

	// FUNCTION BODY[0x605760]: 88
	// <0x605771>|0x011|+0x011:'215'
	// <0>
	// <0x605782>|0x022|+0x009:'217'
	// <0x60578b>|0x02b|+0x006:'218'
	// <0>
	// <0x605791>|0x031|+0x03a:'220'
	// <0x6057cb>|0x06b|+0x03a:'221'
	// <0x605805>|0x0a5|+0x03a:'222'
	// <0x60583f>|0x0df|+0x03a:'223'
	// <0x605879>|0x119|+0x02f:'224'
	// <0x6058a8>|0x148|+0x03e:'225'
	// <0x6058e6>|0x186|+0x03a:'226'
	// <0x605920>|0x1c0|+0x03a:'227'
	// <0>
	// <1>
	// <0x60595a>|0x1fa|+0x00e:'230'
	// <0>
	// <0x605968>|0x208|+0x010:'232'
	// <0x605978>|0x218|+0x013:'233'
	// <0>
	// <1>
	// <2>
	// <0x60598b>|0x22b|+0x00a:'237'
	// <0x605995>|0x235|+0x01e:'238'
	// <0>
	// <1>
	// <2>
	// <0x6059b3>|0x253|+0x008:'242'
	// <0x6059bb>|0x25b|+0x01e:'243'
	// <0>
	// <0x6059d9>|0x279|+0x01e:'245'
	// <0>
	// <0x6059f7>|0x297|+0x0a4:'247'
	// <0x605a9b>|0x33b|-0x08a:'247'
	// <0x605a11>|0x2b1|+0x08e:'248'
	// <0x605a9f>|0x33f|+0x010:'249'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x605aaf>|0x34f|+0x02c:'255'
	// <0x605adb>|0x37b|+0x017:'256'
	// <0>
	// <0x605af2>|0x392|+0x01e:'258'
	// <0x605b10>|0x3b0|+0x1c2:'259'
	// <0x605cd2>|0x572|-0x091:'259'
	// <0x605c41>|0x4e1|+0x010:'260'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x605c51>|0x4f1|+0x02f:'267'
	// <0x605c80>|0x520|+0x017:'268'
	// <0>
	// <0x605c97>|0x537|-0x05a:'270'
	// <0x605c3d>|0x4dd|+0x074:'271'
	// <0x605cb1>|0x551|+0x02b:'271'
	// <0x605cdc>|0x57c|+0x109:'271'
	// <0x605de5>|0x685|+0x010:'272'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x605df5>|0x695|+0x004:'277'
	// <0x605df9>|0x699|+0x010:'278'
	// <0>
	// <0x605e09>|0x6a9|+0x007:'280'
	// <0>
	// <0x605e10>|0x6b0|+0x036:'282'
	// <0>
	// <1>
	// <0x605e46>|0x6e6|+0x037:'285'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x605e7d>|0x71d|+0x025:'291'
	// <0x605ea2>|0x742|+0x020:'292'
	// <0>
	// <1>
	// <0x605ec2>|0x762|+0x009:'295'
	// <0>
	// <1>
	// <2>
	// <0x605ecb>|0x76b|+0x01d:'299'
	// <0>
	// <0x605ee8>|0x788|+0x015:'301'
	// <0x605efd>|0x79d|+0x010:'302'
	// ******
}

// STATE[STUB]
void stage_visibility::occlusion_culling( )
{
	// LOCALS
	// u32 								occlusion_info_index_iterator
	// float4* 							bounds_it
	// ******

	// FUNCTION BODY[0x605fb0]: 15
	// <0x605fb3>|0x003|+0x004:'307'
	// <0>
	// <1>
	// <2>
	// <0x605fb7>|0x007|+0x01b:'311'
	// <0x605fd2>|0x022|+0x00e:'312'
	// <0x605fe0>|0x030|+0x00e:'313'
	// <0x605fee>|0x03e|+0x00b:'314'
	// <0x605ff9>|0x049|+0x00b:'315'
	// <0x606004>|0x054|+0x00b:'316'
	// <0x60600f>|0x05f|+0x00e:'317'
	// <0x60601d>|0x06d|+0x00f:'318'
	// <0>
	// <1>
	// <0x60602c>|0x07c|+0x017:'321'
	// ******
}

// STATE[STUB]
bool stage_visibility::occluded( const u32 index ) const
{
	return false;

	// FUNCTION BODY[0x6044a0]: 4
	// <0x6044a0>|0x000|+0x005:'326'
	// <0x6044a5>|0x005|+0x003:'327'
	// <0>
	// <0x6044a8>|0x008|-0x001:'329'
	// <0x6044a7>|0x007|+0x00e:'330'
	// ******
}

// STATE[STUB]
void stage_visibility::get_results_and_prepare_bounds_models( float4*& out_bounds, u32& out_counter )
{
	// FUNCTION BODY[0x605600]: 21
	// <0x605600>|0x000|+0x016:'334'
	// <0>
	// <1>
	// <0x605616>|0x016|+0x00d:'337'
	// <0>
	// <0x605623>|0x023|+0x008:'339'
	// <0>
	// <0x60562b>|0x02b|+0x002:'341'
	// <0>
	// <0x60562d>|0x02d|+0x01d:'343'
	// <0x60564a>|0x04a|+0x008:'344'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x605652>|0x052|+0x0fb:'353'
	// <0>
	// ******
}

// STATE[STUB]
void stage_visibility::get_results_and_prepare_bounds_lights( float4*& out_bounds, u32& out_counter )
{
	// LOCALS
	// light_ptr* 						end
	// ******

	// FUNCTION BODY[0x605470]: 9
	// <0x605473>|0x003|+0x01e:'359'
	// <0x605491>|0x021|+0x00e:'360'
	// <0>
	// <0x60549f>|0x02f|+0x008:'362'
	// <0>
	// <0x6054a7>|0x037|+0x025:'364'
	// <0x6054cc>|0x05c|+0x00a:'365'
	// <0x6054d6>|0x066|+0x113:'366'
	// <0>
	// ******
}

// STATE[STUB]
void stage_visibility::get_results_and_prepare_bounds_grass( float4*& out_bounds, u32& out_counter )
{
	// LOCALS
	// grass_patch** 					end
	// ******

	// FUNCTION BODY[0x6052f0]: 9
	// <0x6052f3>|0x003|+0x01f:'372'
	// <0x605312>|0x022|+0x00d:'373'
	// <0>
	// <0x60531f>|0x02f|+0x008:'375'
	// <0>
	// <0x605327>|0x037|+0x025:'377'
	// <0x60534c>|0x05c|+0x00a:'378'
	// <0x605356>|0x066|+0x10f:'379'
	// <0>
	// ******
}

// STATE[STUB]
void stage_visibility::get_results_and_prepare_bounds_decals( float4*& out_bounds, u32& out_counter )
{
	// FUNCTION BODY[0x605150]: 10
	// <0x605150>|0x000|+0x01c:'385'
	// <0x60516c>|0x01c|+0x00d:'386'
	// <0>
	// <0x605179>|0x029|+0x008:'388'
	// <0>
	// <1>
	// <0x605181>|0x031|+0x028:'391'
	// <0x6051a9>|0x059|+0x00d:'392'
	// <0x6051b6>|0x066|+0x124:'393'
	// <0>
	// ******
}

// STATE[STUB]
void stage_visibility::get_results_and_prepare_bounds_env_probes( float4*& out_bounds, u32& out_counter )
{
	// FUNCTION BODY[0x604fb0]: 10
	// <0x604fb0>|0x000|+0x01c:'399'
	// <0x604fcc>|0x01c|+0x00d:'400'
	// <0>
	// <0x604fd9>|0x029|+0x008:'402'
	// <0>
	// <1>
	// <0x604fe1>|0x031|+0x028:'405'
	// <0x605009>|0x059|+0x00d:'406'
	// <0x605016>|0x066|+0x12d:'407'
	// <0>
	// ******
}

// STATE[STUB]
void stage_visibility::get_results_and_prepare_bounds_ambient_volumes( float4*& out_bounds, u32& out_counter )
{
	// FUNCTION BODY[0x604e20]: 10
	// <0x604e20>|0x000|+0x01c:'413'
	// <0x604e3c>|0x01c|+0x00d:'414'
	// <0>
	// <0x604e49>|0x029|+0x008:'416'
	// <0>
	// <1>
	// <0x604e51>|0x031|+0x022:'419'
	// <0x604e73>|0x053|+0x00a:'420'
	// <0x604e7d>|0x05d|+0x124:'421'
	// <0>
	// ******
}

// STATE[STUB]
void stage_visibility::get_results_and_prepare_bounds_particles( float4*& out_bounds, u32& out_counter )
{
	// LOCALS
	// particle::render_particle_emitter_instance** end
	// ******

	// FUNCTION BODY[0x604ca0]: 11
	// <0x604ca3>|0x003|+0x01f:'427'
	// <0x604cc2>|0x022|+0x00d:'428'
	// <0>
	// <0x604ccf>|0x02f|+0x008:'430'
	// <0>
	// <0x604cd7>|0x037|+0x002:'432'
	// <0>
	// <0x604cd9>|0x039|+0x023:'434'
	// <0x604cfc>|0x05c|+0x009:'435'
	// <0x604d05>|0x065|+0x110:'436'
	// <0>
	// ******
}

// STATE[STUB]
void stage_visibility::get_results_and_prepare_bounds_portals( float4*& out_bounds, u32& out_counter )
{
	// FUNCTION BODY[0x604ae0]: 24
	// <0x604ae5>|0x005|+0x009:'442'
	// <0>
	// <0x604aee>|0x00e|+0x037:'444'
	// <0>
	// <0x604b25>|0x045|+0x006:'446'
	// <0>
	// <1>
	// <0x604b2b>|0x04b|+0x007:'449'
	// <0>
	// <0x604b32>|0x052|+0x00d:'451'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x604b3f>|0x05f|+0x02f:'457'
	// <0>
	// <1>
	// <0x604b6e>|0x08e|+0x009:'460'
	// <0>
	// <0x604b77>|0x097|+0x03b:'462'
	// <0x604bb2>|0x0d2|-0x013:'462'
	// <0>
	// <0x604b9f>|0x0bf|+0x017:'464'
	// <0x604bb6>|0x0d6|-0x00d:'464'
	// <0x604ba9>|0x0c9|+0x014:'465'
	// <0x604bbd>|0x0dd|-0x00e:'465'
	// <0x604baf>|0x0cf|+0x014:'466'
	// ******
}

// STATE[STUB]
bool is_not_occluded_predicate_light( light_ptr const& obj )
{
	return false;

	// FUNCTION BODY[0x604640]: 1
	// <0x604640>|0x000|+0x02c:'469'
	// <0x60466c>|0x02c|-0x001:'469'
	// <0x60466b>|0x02b|+0x00c:'470'
	// ******
}

// STATE[STUB]
bool is_occluded_predicate_light( light_ptr const& obj )
{
	return false;

	// FUNCTION BODY[0x604610]: 1
	// <0x604610>|0x000|+0x023:'472'
	// <0x604633>|0x023|-0x001:'472'
	// <0x604632>|0x022|+0x003:'473'
	// ******
}

// STATE[STUB]
bool is_not_occluded_predicate<vostok::render::render_surface_instance>( render_surface_instance const* const obj )
{
	return false;

	// FUNCTION BODY[0x604740]: 1
	// <0x604740>|0x000|+0x027:'476'
	// <0x604767>|0x027|-0x001:'476'
	// <0x604766>|0x026|+0x00c:'477'
	// ******
}

// STATE[STUB]
bool is_occluded_predicate<vostok::render::render_surface_instance>( render_surface_instance const* const obj )
{
	return false;

	// FUNCTION BODY[0x6046d0]: 1
	// <0x6046d0>|0x000|+0x01e:'480'
	// <0x6046ee>|0x01e|-0x001:'480'
	// <0x6046ed>|0x01d|+0x003:'481'
	// ******
}

// STATE[STUB]
void stage_visibility::gather_statistics( ) const
{
	// CALL SITE INFO
	// <0x604827> -> bool < unknown >() const
	// <0x6049c7> -> bool < unknown >() const
	// ******

	// FUNCTION BODY[0x604780]: 55
	// <0x604780>|0x000|+0x00b:'485'
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
	// <11>
	// <12>
	// <0x60478b>|0x00b|+0x025:'499'
	// <0x6047b0>|0x030|+0x020:'499'
	// <0>
	// <1>
	// <0x6047d0>|0x050|+0x03d:'502'
	// <0>
	// <1>
	// <0x60480d>|0x08d|+0x034:'505'
	// <0>
	// <1>
	// <0x604841>|0x0c1|+0x01f:'508'
	// <0x604860>|0x0e0|+0x01d:'508'
	// <0>
	// <1>
	// <0x60487d>|0x0fd|+0x040:'511'
	// <0>
	// <1>
	// <0x6048bd>|0x13d|+0x040:'514'
	// <0>
	// <1>
	// <0x6048fd>|0x17d|+0x03d:'517'
	// <0>
	// <1>
	// <2>
	// <0x60493a>|0x1ba|+0x038:'521'
	// <0>
	// <1>
	// <0x604972>|0x1f2|+0x03b:'524'
	// <0>
	// <1>
	// <0x6049ad>|0x22d|+0x034:'527'
	// <0>
	// <1>
	// <0x6049e1>|0x261|+0x01f:'530'
	// <0x604a00>|0x280|+0x01d:'530'
	// <0>
	// <1>
	// <0x604a1d>|0x29d|+0x040:'533'
	// <0>
	// <1>
	// <0x604a5d>|0x2dd|-0x2bf:'536'
	// <0>
	// <1>
	// <0x60479e>|0x01e|+0x0b5:'539'
	// <0x604853>|0x0d3|+0x1a0:'539'
	// <0x6049f3>|0x273|+0x0aa:'539'
	// <0x604a9d>|0x31d|+0x040:'539'
	// ******
}

	// TYPEDEFS
	// typedef
	// 	pbyte
	// 	iterator_type;

	// typedef
	// 	pcvoid*
	// 	iterator_type;

	// typedef
	// 	u16*
	// 	iterator_type;

	// typedef
	// 	void**
	// 	iterator_type;

	// typedef
	// 	vostok::fixed_string< 32 >*
	// 	iterator_type;

	// typedef
	// 	vostok::render::geometry_batch*
	// 	iterator_type;

	// typedef
	// 	vostok::render::light_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::res_pass_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::res_shader_technique_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_binding*
	// 	iterator_type;

	// typedef
	// 	vostok::render::signature_layout_pair*
	// 	iterator_type;

	// typedef
	// 	vostok::render::texture_named_instance*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok
