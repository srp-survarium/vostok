////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\portal_sector_system.h"

namespace vostok {
namespace render {
namespace culling {

// STATE[STUB]
void `dynamic initializer for 's_use_screeen_space_portals_intersection_cc''( )
{
	// FUNCTION BODY[0x7d7660]
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_portals_occlusion_culling_cc''( )
{
	// FUNCTION BODY[0x7d76b0]
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_draw_portals_cc''( )
{
	// FUNCTION BODY[0x7d7700]
	// ******
}

// STATE[STUB]
void `dynamic atexit destructor for 's_use_screeen_space_portals_intersection_cc''( )
{
	// FUNCTION BODY[0x7ef710]
	// ******
}

// STATE[STUB]
explicit portal_sector_system::portal_sector_system( portal_sector_structure_ptr structure )
{
	// FUNCTION BODY[0x5fc180]: 2
	// <0x5fc281>|0x101|+0x005:'91'
	// <0x5fc286>|0x106|+0x03f:'92'
	// ******
}

// STATE[STUB]
 portal_sector_system::~portal_sector_system( )
{
	// FUNCTION BODY[0x5fc080]: 5
	// <0x5fc08b>|0x00b|+0x00c:'97'
	// <0x5fc097>|0x017|+0x025:'98'
	// <0x5fc0bc>|0x03c|+0x00c:'99'
	// <0x5fc0c8>|0x048|+0x027:'100'
	// <0x5fc0ef>|0x06f|+0x028:'101'
	// ******
}

// STATE[STUB]
math::frustum create_frustum_from_four_points( float3 const& view_pos, const float3[4]& points, math::plane const& far_plane )
{
	// LOCALS
	// math::frustum 					result
	// math::plane[6] 					frustrum_planes
	// math::frustum 					result
	// math::plane[6] 					frustrum_planes
	// ******

	return vostok::math::frustum();

	// FUNCTION BODY[0x5fb460]: 41
	// <0x5fb478>|0x018|+0x011:'145'
	// <0x5fb489>|0x029|+0x024:'146'
	// <0x5fb4ad>|0x04d|+0x011:'147'
	// <0>
	// <1>
	// <2>
	// <0x5fb4be>|0x05e|+0x016:'151'
	// <0>
	// <0x5fb4d4>|0x074|+0x00f:'153'
	// <0>
	// <0x5fb4e3>|0x083|+0x00f:'155'
	// <0>
	// <0x5fb4f2>|0x092|+0x00e:'157'
	// <0>
	// <0x5fb500>|0x0a0|+0x00b:'159'
	// <0>
	// <1>
	// <0x5fb50b>|0x0ab|+0x05f:'162'
	// <0x5fb56a>|0x10a|+0x01b:'163'
	// <0>
	// <0x5fb585>|0x125|+0x002:'165'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5fb587>|0x127|+0x00a:'171'
	// <0>
	// <0x5fb591>|0x131|+0x013:'173'
	// <0>
	// <0x5fb5a4>|0x144|+0x00f:'175'
	// <0>
	// <0x5fb5b3>|0x153|+0x00f:'177'
	// <0x5fb5c2>|0x162|+0x01c:'178'
	// <0>
	// <1>
	// <0x5fb5de>|0x17e|+0x015:'181'
	// <0x5fb5f3>|0x193|+0x01a:'182'
	// <0>
	// <0x5fb60d>|0x1ad|+0x01b:'184'
	// <0>
	// ******
}

// STATE[STUB]
bool cull_points_by_frustum( math::frustum const& f, float3[4]& io_points )
{
	// LOCALS
	// buffer_vector< float3 > 			pos
	// u32 								longest_edge_id
	// Wm4::Box2< float > 				min_box
	// float4x4 						local_to_world
	// buffer_vector< float3 > 			temp
	// float4x4 						world_to_local
	// u32 								plane_id
	// const u32 						temp_count
	// u32 								i
	// u32 								i
	// ******

	// TYPEDEFS
	// typedef
	// 	buffer_vector< Wm4::Vector2< float > >
	// 	wm_vertices_2d_buffer_type;

	// typedef
	// 	buffer_vector< float3 >
	// 	vertices_buffer_type;

	// typedef
	// 	Wm4::Vector2< float >
	// 	wm_vertex_2d;

	// ******

	return false;

	// FUNCTION BODY[0x5fa5d0]: 76
	// <0x5fa5dd>|0x00d|+0x0dc:'190'
	// <0>
	// <1>
	// <0x5fa6b9>|0x0e9|+0x01c:'193'
	// <0x5fa6d5>|0x105|+0x00a:'194'
	// <0x5fa6df>|0x10f|+0x019:'195'
	// <0>
	// <1>
	// <0x5fa6f8>|0x128|+0x03a:'198'
	// <0>
	// <0x5fa732>|0x162|-0x030:'200'
	// <0>
	// <0x5fa702>|0x132|+0x023:'202'
	// <0x5fa725>|0x155|+0x275:'203'
	// <0x5fa99a>|0x3ca|-0x25a:'203'
	// <0>
	// <0x5fa740>|0x170|+0x02a:'205'
	// <0x5fa76a>|0x19a|+0x05d:'206'
	// <0x5fa7c7>|0x1f7|+0x01e:'207'
	// <0x5fa7e5>|0x215|+0x00c:'208'
	// <0x5fa7f1>|0x221|+0x17b:'209'
	// <0x5fa96c>|0x39c|+0x031:'210'
	// <0>
	// <0x5fa99d>|0x3cd|+0x977:'212'
	// <0x5fb314>|0xd44|-0x95a:'213'
	// <0x5fa9ba>|0x3ea|+0x003:'214'
	// <0x5fa9bd>|0x3ed|+0x026:'215'
	// <0>
	// <1>
	// <0x5fa9e3>|0x413|+0x01a:'218'
	// <0x5fa9fd>|0x42d|+0x006:'219'
	// <0>
	// <1>
	// <2>
	// <0x5faa03>|0x433|+0x02a:'223'
	// <0x5faa2d>|0x45d|+0x1b4:'224'
	// <0x5fabe1>|0x611|-0x180:'224'
	// <0>
	// <0x5faa61>|0x491|+0x18f:'226'
	// <0x5fabf0>|0x620|-0x139:'226'
	// <0x5faab7>|0x4e7|+0x17e:'227'
	// <0x5fac35>|0x665|-0x179:'227'
	// <0>
	// <1>
	// <0x5faabc>|0x4ec|+0x17e:'230'
	// <0x5fac3a>|0x66a|+0x01e:'230'
	// <0>
	// <1>
	// <0x5fac58>|0x688|+0x01d:'233'
	// <0x5fac75>|0x6a5|+0x006:'234'
	// <0x5fac7b>|0x6ab|+0x06b:'235'
	// <0x5face6>|0x716|+0x0d4:'236'
	// <0>
	// <0x5fadba>|0x7ea|+0x008:'238'
	// <0x5fadc2>|0x7f2|+0x005:'239'
	// <0x5fadc7>|0x7f7|+0x006:'240'
	// <0x5fadcd>|0x7fd|+0x01d:'241'
	// <0x5fadea>|0x81a|+0x003:'242'
	// <0>
	// <0x5faded>|0x81d|+0x023:'244'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5fae10>|0x840|+0x010:'249'
	// <0>
	// <0x5fae20>|0x850|+0x015:'251'
	// <0>
	// <0x5fae35>|0x865|+0x059:'253'
	// <0x5fae8e>|0x8be|+0x01a:'254'
	// <0>
	// <0x5faea8>|0x8d8|+0x00f:'256'
	// <0>
	// <0x5faeb7>|0x8e7|+0x09d:'258'
	// <0x5faf54>|0x984|+0x0c3:'259'
	// <0>
	// <0x5fb017>|0xa47|+0x0ee:'261'
	// <0x5fb105>|0xb35|+0x069:'262'
	// <0x5fb16e>|0xb9e|+0x089:'263'
	// <0x5fb1f7>|0xc27|+0x08e:'264'
	// <0x5fb285>|0xcb5|+0x0a1:'265'
	// <0x5fb326>|0xd56|-0x010:'265'
	// <0x5fb316>|0xd46|+0x017:'266'
	// ******
}

// STATE[STUB]
void portal_sector_system::render( system_renderer& r, float3 const& view_pos, float4x4 const& __formal )
{
	// FUNCTION BODY[0x5fc550]: 7
	// <0x5fc551>|0x001|+0x021:'270'
	// <0x5fc572>|0x022|+0x004:'271'
	// <0x5fc576>|0x026|+0x025:'272'
	// <0x5fc59b>|0x04b|+0x009:'273'
	// <0x5fc5a4>|0x054|+0x00c:'274'
	// <0x5fc5b0>|0x060|+0x009:'275'
	// <0x5fc5b9>|0x069|+0x009:'276'
	// ******
}

// STATE[STUB]
void portal_sector_system::test_action( )
{
	// FUNCTION BODY[0x5f9000]: 1
	// <0x5f9000>|0x000|+0x007:'293'
	// ******
}

// STATE[STUB]
void portal_sector_system::draw_quads( system_renderer& r )
{
	// LOCALS
	// const u16[8] 					quad_indices
	// const vertex_colored[4] 			vertices
	// ******

	// FUNCTION BODY[0x5fc300]: 18
	// <0>
	// <0x5fc303>|0x003|+0x00d:'299'
	// <0x5fc310>|0x010|+0x008:'300'
	// <0>
	// <1>
	// <2>
	// <0x5fc318>|0x018|+0x01e:'304'
	// <0x5fc336>|0x036|+0x02e:'305'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5fc364>|0x064|+0x006:'310'
	// <0x5fc36a>|0x06a|+0x007:'311'
	// <0>
	// <1>
	// <0x5fc371>|0x071|+0x072:'314'
	// <0>
	// ******
}

// STATE[STUB]
void portal_sector_system::draw_portals( system_renderer& r, const u32 active_sector_id )
{
	// LOCALS
	// const u16[12] 					frustum_indices
	// const vertex_colored[4] 			vertices
	// ******

	// FUNCTION BODY[0x5fc3f0]: 30
	// <0>
	// <1>
	// <2>
	// <0x5fc3f3>|0x003|+0x010:'323'
	// <0x5fc403>|0x013|+0x016:'324'
	// <0x5fc419>|0x029|+0x025:'325'
	// <0x5fc43e>|0x04e|+0x016:'326'
	// <0>
	// <1>
	// <0x5fc454>|0x064|+0x01a:'329'
	// <0x5fc46e>|0x07e|+0x00b:'330'
	// <0x5fc479>|0x089|+0x017:'331'
	// <0>
	// <0x5fc490>|0x0a0|+0x011:'333'
	// <0x5fc4a1>|0x0b1|+0x006:'334'
	// <0>
	// <1>
	// <2>
	// <0x5fc4a7>|0x0b7|+0x016:'338'
	// <0x5fc4bd>|0x0cd|+0x016:'339'
	// <0x5fc4d3>|0x0e3|+0x012:'340'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5fc4e5>|0x0f5|+0x048:'347'
	// <0>
	// <0x5fc52d>|0x13d|+0x00c:'349'
	// ******
}

// STATE[STUB]
void portal_sector_system::select_models(
	collision::space_partitioning_tree*		tree,
	float3 const&							view_pos,
	float4x4 const&							mat_vp,
	vector< render_surface_instance* >&		selection
)
{
	// LOCALS
	// math::frustum 					f
	// float4x4 						inverted_vp
	// buffer_vector< aab_rect > 		portal_rects
	// const u32 						active_sector_id
	// ******

	// FUNCTION BODY[0x5fbd80]: 43
	// <0x5fbd8d>|0x00d|+0x00e:'358'
	// <0x5fbd9b>|0x01b|+0x02e:'359'
	// <0x5fbdc9>|0x049|+0x029:'360'
	// <0>
	// <1>
	// <0x5fbdf2>|0x072|+0x010:'363'
	// <0x5fbe02>|0x082|+0x015:'364'
	// <0x5fbe17>|0x097|+0x03f:'365'
	// <0x5fbe56>|0x0d6|+0x019:'366'
	// <0x5fbe6f>|0x0ef|+0x009:'367'
	// <0>
	// <1>
	// <0x5fbe78>|0x0f8|+0x011:'370'
	// <0>
	// <0x5fbe89>|0x109|+0x005:'372'
	// <0>
	// <1>
	// <0x5fbe8e>|0x10e|+0x00d:'375'
	// <0>
	// <0x5fbe9b>|0x11b|+0x008:'377'
	// <0>
	// <1>
	// <0x5fbea3>|0x123|+0x030:'380'
	// <0x5fbed3>|0x153|+0x063:'381'
	// <0x5fbf36>|0x1b6|+0x027:'382'
	// <0x5fbf5d>|0x1dd|+0x008:'383'
	// <0x5fbf65>|0x1e5|+0x048:'384'
	// <0x5fbfad>|0x22d|+0x022:'385'
	// <0>
	// <0x5fbfcf>|0x24f|+0x002:'387'
	// <0>
	// <0x5fbfd1>|0x251|+0x001:'389'
	// <0>
	// <0x5fbfd2>|0x252|+0x018:'391'
	// <0>
	// <1>
	// <0x5fbfea>|0x26a|+0x020:'394'
	// <0x5fc00a>|0x28a|+0x009:'395'
	// <0>
	// <1>
	// <0x5fc013>|0x293|+0x03f:'398'
	// <0>
	// <0x5fc052>|0x2d2|+0x017:'400'
	// ******
}

// STATE[STUB]
void portal_sector_system::process_sector(
	u32						sector_id,
	u32						input_portal_id,
	float3 const&			view_pos,
	math::frustum const&	f
)
{
	// FUNCTION BODY[0x5f9110]: 8
	// <0x5f9110>|0x000|+0x018:'453'
	// <0x5f9128>|0x018|+0x009:'454'
	// <0x5f9131>|0x021|+0x004:'455'
	// <0>
	// <0x5f9135>|0x025|+0x020:'457'
	// <0>
	// <0x5f9155>|0x045|+0x01d:'459'
	// <0>
	// ******
}

// STATE[STUB]
void portal_sector_system::process_portal_by_frustum_intersection(
	u32						portal_id,
	math::frustum const&	f,
	u32						sector_id,
	float3 const&			view_pos
)
{
	// LOCALS
	// float3[4] 						points
	// ******

	// FUNCTION BODY[0x5fb8b0]: 20
	// <0>
	// <0x5fb8b0>|0x000|+0x096:'466'
	// <0>
	// <1>
	// <0x5fb946>|0x096|+0x01b:'469'
	// <0x5fb961>|0x0b1|+0x01d:'470'
	// <0>
	// <0x5fb97e>|0x0ce|+0x06e:'472'
	// <0x5fb9ec>|0x13c|+0x07c:'473'
	// <0>
	// <0x5fba68>|0x1b8|+0x00e:'475'
	// <0x5fba76>|0x1c6|+0x016:'476'
	// <0>
	// <1>
	// <2>
	// <0x5fba8c>|0x1dc|+0x018:'480'
	// <0x5fbaa4>|0x1f4|+0x015:'481'
	// <0x5fbab9>|0x209|+0x018:'482'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void portal_sector_system::process_sector(
	u32									sector_id,
	u32									input_portal_id,
	buffer_vector< aab_rect > const&	portals_rects,
	float3 const&						view_pos,
	math::plane const&					far_plane,
	float4x4 const&						mat_vp,
	float4x4 const&						inv_mat_vp,
	aab_rect const&						rect
)
{
	// FUNCTION BODY[0x5f9090]: 8
	// <0x5f9090>|0x000|+0x017:'491'
	// <0x5f90a7>|0x017|+0x00a:'492'
	// <0x5f90b1>|0x021|+0x006:'493'
	// <0>
	// <0x5f90b7>|0x027|+0x020:'495'
	// <0>
	// <0x5f90d7>|0x047|+0x031:'497'
	// <0>
	// ******
}

// STATE[STUB]
bool portal_screen_rect_to_four_points(
	aab_rect const&			portal_rect,
	math::plane const&		portal_plane,
	float4x4 const&			inv_mat_vp,
	aab_rect const&			limiting_rect,
	float3[4]&				io_points,
	aab_rect&				limited_rect
)
{
	// LOCALS
	// float3[4] 						ws_near_rect
	// float3[4] 						ws_far_rect
	// ******

	return false;

	// FUNCTION BODY[0x5f9bd0]: 29
	// <0x5f9bd0>|0x000|+0x04a:'592'
	// <0x5f9c1a>|0x04a|+0x00a:'593'
	// <0x5f9c24>|0x054|+0x01c:'594'
	// <0x5f9c40>|0x070|+0x02b:'595'
	// <0x5f9c6b>|0x09b|+0x030:'596'
	// <0x5f9c9b>|0x0cb|+0x006:'597'
	// <0>
	// <1>
	// <0x5f9ca1>|0x0d1|+0x13e:'600'
	// <0x5f9ddf>|0x20f|+0x115:'601'
	// <0x5f9ef4>|0x324|+0x133:'602'
	// <0>
	// <0x5fa027>|0x457|+0x0d7:'604'
	// <0>
	// <1>
	// <0x5fa0fe>|0x52e|+0x127:'607'
	// <0x5fa225>|0x655|+0x16b:'608'
	// <0x5fa390>|0x7c0|+0x0fb:'609'
	// <0>
	// <0x5fa48b>|0x8bb|+0x0a0:'611'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5fa52b>|0x95b|+0x080:'616'
	// <0>
	// <0x5fa5ab>|0x9db|+0x012:'618'
	// <0>
	// <0x5fa5bd>|0x9ed|-0x9a0:'620'
	// <0x5f9c1d>|0x04d|+0x9a3:'621'
	// ******
}

// STATE[STUB]
void portal_sector_system::process_portal_in_screen_space(
	u32									portal_id,
	u32									sector_id,
	buffer_vector< aab_rect > const&	portals_rects,
	float3 const&						view_pos,
	math::plane const&					far_plane,
	float4x4 const&						mat_vp,
	float4x4 const&						inv_mat_vp,
	aab_rect const&						limiting_rect
)
{
	// LOCALS
	// float3[4] 						points
	// aab_rect 						limited_portal_rect
	// math::frustum 					portal_frustum
	// ******

	// FUNCTION BODY[0x5fb630]: 29
	// <0x5fb638>|0x008|+0x01c:'627'
	// <0>
	// <1>
	// <0x5fb654>|0x024|+0x068:'630'
	// <0>
	// <1>
	// <2>
	// <0x5fb6bc>|0x08c|+0x01b:'634'
	// <0x5fb6d7>|0x0a7|+0x009:'635'
	// <0>
	// <0x5fb6e0>|0x0b0|+0x076:'637'
	// <0>
	// <0x5fb756>|0x126|+0x013:'639'
	// <0x5fb769>|0x139|+0x01a:'640'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5fb783>|0x153|+0x080:'646'
	// <0>
	// <0x5fb803>|0x1d3|+0x00f:'648'
	// <0x5fb812>|0x1e2|+0x016:'649'
	// <0x5fb828>|0x1f8|+0x01b:'650'
	// <0>
	// <0x5fb843>|0x213|+0x018:'652'
	// <0x5fb85b>|0x22b|+0x012:'653'
	// <0x5fb86d>|0x23d|+0x033:'654'
	// <0>
	// ******
}

// STATE[STUB]
void portal_sector_system::perform_frustum_culling_and_sectors_test(
	collision::space_partitioning_tree*		tree,
	u32										active_sector_id,
	math::frustum const&					f,
	vector< render_surface_instance* >&		visible_surfaces,
	float3 const&							view_pos,
	float4x4 const&							mat_vp
)
{
	// LOCALS
	// collision::object const* const* 	query_result_end
	// vector< render_surface_instance* > surfaces
	// vectora< collision::object const* > query_result
	// collision::object const** 		it
	// render_surface_instance* const* const surfaces_end
	// render_surface_instance* const* 	it
	// render_collision_object< render_model_instance_impl > const* const object
	// ******

	// CALL SITE INFO
	// <0x5fbb0f> -> bool < unknown >( u32, math::cuboid const&, vectora< collision::object const* >& )
	// <0x5fbbe0> -> void < unknown >( float4x4 const*, float3 const*, vector< render_surface_instance* >&, bool, u8, u32 )
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5ead72 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN413") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5ead72 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN414") })
	// ******

	// FUNCTION BODY[0x5fbae0]: 26
	// <0x5fbae3>|0x003|+0x005:'727'
	// <0x5fbae8>|0x008|+0x029:'728'
	// <0>
	// <1>
	// <0x5fbb11>|0x031|+0x004:'731'
	// <0x5fbb15>|0x035|+0x022:'732'
	// <0>
	// <0x5fbb37>|0x057|+0x006:'734'
	// <0>
	// <0x5fbb3d>|0x05d|+0x00d:'736'
	// <0>
	// <1>
	// <2>
	// <0x5fbb4a>|0x06a|+0x04c:'740'
	// <0>
	// <0x5fbb96>|0x0b6|+0x02a:'742'
	// <0>
	// <0x5fbbc0>|0x0e0|+0x022:'744'
	// <0>
	// <0x5fbbe2>|0x102|+0x01e:'746'
	// <0>
	// <0x5fbc00>|0x120|+0x168:'748'
	// <0x5fbd68>|0x288|-0x037:'748'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******
}

// STATE[STUB]
void portal_sector_system::get_portals_occlusion_bounds( float4* bounds )
{
	// FUNCTION BODY[0x5f9010]: 1
	// <0x5f9010>|0x000|+0x01f:'773'
	// ******
}

// STATE[STUB]
void portal_sector_system::initialize_portals_occlusion_bounds_and_results( )
{
	// LOCALS
	// portal const* const 				portals_end
	// portal const* 					it
	// const float 						sqr_radius
	// ******

	// FUNCTION BODY[0x5f9940]: 11
	// <0x5f9949>|0x009|+0x00c:'778'
	// <0x5f9955>|0x015|+0x018:'779'
	// <0>
	// <0x5f996d>|0x02d|+0x056:'781'
	// <0>
	// <1>
	// <2>
	// <0x5f99c3>|0x083|+0x115:'785'
	// <0x5f9ad8>|0x198|+0x064:'786'
	// <0>
	// <0x5f9b3c>|0x1fc|+0x01d:'788'
	// ******
}

// STATE[STUB]
void portal_sector_system::update_portals_occlusion_culling( pcbyte occlusion_results )
{
	// FUNCTION BODY[0x5f9040]: 8
	// <0x5f9040>|0x000|+0x009:'793'
	// <0>
	// <0x5f9049>|0x009|+0x020:'795'
	// <0>
	// <1>
	// <2>
	// <0x5f9069>|0x029|+0x006:'799'
	// <0x5f906f>|0x02f|-0x003:'799'
	// <0>
	// <0x5f906c>|0x02c|+0x020:'801'
	// ******
}

// STATE[STUB]
void portal_sector_system::calculate_portal_rects_in_screen_space(
	float4x4 const&					mat_vp,
	float							min_z,
	buffer_vector< aab_rect >&		rects,
	buffer_vector< float >&			distances
)
{
	// LOCALS
	// portal const* const 				portals_end
	// portal const* 					it
	// const float3[4] 					hs_f3
	// const float4[4] 					cs_f4
	// aab_rect 						portal_rect
	// ******

	// FUNCTION BODY[0x5f9180]: 50
	// <0x5f91cc>|0x04c|-0x036:'805'
	// <0x5f9196>|0x016|+0x006:'806'
	// <0x5f919c>|0x01c|+0x002:'807'
	// <0x5f919e>|0x01e|+0x010:'808'
	// <0x5f91ae>|0x02e|+0x021:'809'
	// <0>
	// <0x5f91cf>|0x04f|+0x006:'811'
	// <0>
	// <0x5f91d5>|0x055|+0x023:'813'
	// <0x5f91f8>|0x078|+0x713:'814'
	// <0x5f990b>|0x78b|-0x6fc:'814'
	// <0x5f920f>|0x08f|+0x005:'815'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5f9214>|0x094|+0x121:'820'
	// <0x5f9335>|0x1b5|+0x152:'821'
	// <0x5f9487>|0x307|+0x13d:'822'
	// <0>
	// <0x5f95c4>|0x444|+0x0d3:'824'
	// <0>
	// <1>
	// <2>
	// <0x5f9697>|0x517|+0x01f:'828'
	// <0x5f96b6>|0x536|+0x029:'829'
	// <0x5f96df>|0x55f|+0x02a:'830'
	// <0>
	// <0x5f9709>|0x589|+0x038:'832'
	// <0>
	// <0x5f9741>|0x5c1|+0x039:'834'
	// <0>
	// <0x5f977a>|0x5fa|+0x008:'836'
	// <0x5f9782>|0x602|+0x023:'837'
	// <0x5f97a5>|0x625|+0x013:'838'
	// <0x5f97b8>|0x638|+0x005:'839'
	// <0>
	// <1>
	// <2>
	// <0x5f97bd>|0x63d|+0x040:'843'
	// <0x5f97fd>|0x67d|+0x02f:'844'
	// <0x5f982c>|0x6ac|+0x029:'845'
	// <0x5f9855>|0x6d5|+0x01a:'846'
	// <0x5f986f>|0x6ef|+0x030:'847'
	// <0x5f989f>|0x71f|+0x01a:'848'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5f98b9>|0x739|+0x055:'853'
	// <0x5f990e>|0x78e|+0x017:'853'
	// <0>
	// ******
}

// STATE[STUB]
void portal_sector_system::sort_portals_and_calculate_rects_in_screen_space( float4x4 const& mat_vp, float min_z, buffer_vector< aab_rect >& rects )
{
	// LOCALS
	// buffer_vector< float > 			distances
	// ******

	// FUNCTION BODY[0x5f9b60]: 3
	// <0x5f9b66>|0x006|+0x031:'859'
	// <0x5f9b97>|0x037|+0x020:'860'
	// <0x5f9bb7>|0x057|+0x00f:'861'
	// ******
}

// STATE[STUB]
u32 get_aabb_furthest_vertex_id(
	const float3	arg_0 /* float3 dir */
)
{
	return 0;

	// FUNCTION BODY[0x5f8f90]: 22
	// <0x5f8f90>|0x000|+0x00c:'866'
	// <0>
	// <0x5f8f9c>|0x00c|+0x00b:'868'
	// <0>
	// <0x5f8fa7>|0x017|+0x018:'870'
	// <0>
	// <1>
	// <2>
	// <0x5f8fbf>|0x02f|+0x010:'874'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5f8fcf>|0x03f|+0x003:'879'
	// <0>
	// <0x5f8fd2>|0x042|+0x018:'881'
	// <0>
	// <1>
	// <2>
	// <0x5f8fea>|0x05a|+0x00b:'885'
	// <0x5f8ff5>|0x065|-0x037:'885'
	// <0>
	// <1>
	// <0x5f8fbe>|0x02e|+0x010:'888'
	// <0x5f8fce>|0x03e|+0x01b:'888'
	// <0x5f8fe9>|0x059|+0x00b:'888'
	// <0x5f8ff4>|0x064|+0x003:'888'
	// ******
}

// STATE[STUB]
void portal_sector_system::make_frustum_images( float3 const& view_dir )
{
	// LOCALS
	// const u32 						furthest_vertex_id
	// ******

	// FUNCTION BODY[0x5fb340]: 8
	// <0x5fb346>|0x006|+0x090:'892'
	// <0x5fb3d6>|0x096|+0x024:'893'
	// <0x5fb3fa>|0x0ba|+0x006:'894'
	// <0x5fb400>|0x0c0|+0x03a:'895'
	// <0x5fb43a>|0x0fa|-0x02a:'895'
	// <0>
	// <0x5fb410>|0x0d0|+0x02d:'897'
	// <0>
	// <0x5fb43d>|0x0fd|+0x010:'899'
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
	// 	vostok::render::culling::portal_sector_system::quad*
	// 	iterator_type;

	// typedef
	// 	vostok::render::geometry_batch*
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

} // namespace culling
} // namespace render
} // namespace vostok
