#include "pch.h"
#include "system_renderer.h"

namespace vostok {
namespace render {

// STATE[STUB]
untyped_buffer* system_renderer::create_quad_ib( )
{
	// LOCALS
	// u16[24576] 						indices
	// ******

	return NULL;

	// FUNCTION BODY[0x6449a0]: 28
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
	// <0x6449ad>|0x00d|+0x013:'81'
	// <0>
	// <1>
	// <2>
	// <0x6449c0>|0x020|+0x009:'85'
	// <0x6449c9>|0x029|+0x009:'86'
	// <0x6449d2>|0x032|+0x009:'87'
	// <0>
	// <0x6449db>|0x03b|+0x009:'89'
	// <0x6449e4>|0x044|+0x006:'90'
	// <0x6449ea>|0x04a|+0x006:'91'
	// <0>
	// <0x6449f0>|0x050|+0x008:'93'
	// <0>
	// <1>
	// <2>
	// <0x6449f8>|0x058|+0x01a:'97'
	// ******
}

// STATE[STUB]
bool system_renderer::is_effects_ready( ) const
{
	return false;

	// FUNCTION BODY[0x644700]: 17
	// <0x644700>|0x000|+0x012:'254'
	// <0x644712>|0x012|-0x00a:'254'
	// <0>
	// <0x644708>|0x008|+0x005:'256'
	// <0>
	// <1>
	// <0x64470d>|0x00d|+0x050:'259'
	// <0x64475d>|0x05d|-0x042:'260'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x64471b>|0x01b|+0x045:'270'
	// <0x644760>|0x060|-0x004:'270'
	// <0x64475c>|0x05c|+0x003:'271'
	// <0x64475f>|0x05f|+0x003:'271'
	// ******
}

// STATE[STUB]
system_renderer::~system_renderer( )
{
	// FUNCTION BODY[0x644e70]: 1
	// <0>
	// ******
}

// STATE[STUB]
void system_renderer::set_w( float4x4 const& m )
{
	// FUNCTION BODY[0x644990]: 1
	// <0x644990>|0x000|+0x009:'293'
	// ******
}

// STATE[STUB]
void system_renderer::draw_lines(
	vertex_colored const* const		vertices_begin,
	vertex_colored const* const		vertices_end,
	u16 const* const				indices_begin,
	u16 const* const				indices_end,
	bool							covering_effect
)
{
	// LOCALS
	// u32 								ioffset
	// u32 								voffset
	// ******

	// FUNCTION BODY[0x646460]: 28
	// <0x646467>|0x007|+0x016:'326'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x64647d>|0x01d|+0x004:'335'
	// <0x646481>|0x021|+0x008:'336'
	// <0x646489>|0x029|+0x01f:'337'
	// <0x6464a8>|0x048|+0x028:'338'
	// <0>
	// <1>
	// <2>
	// <0x6464d0>|0x070|+0x004:'342'
	// <0x6464d4>|0x074|+0x016:'343'
	// <0x6464ea>|0x08a|+0x010:'344'
	// <0x6464fa>|0x09a|+0x024:'345'
	// <0x64651e>|0x0be|+0x00b:'346'
	// <0>
	// <0x646529>|0x0c9|+0x007:'348'
	// <0x646530>|0x0d0|+0x024:'349'
	// <0x646554>|0x0f4|+0x002:'350'
	// <0x646556>|0x0f6|+0x02a:'351'
	// <0>
	// <0x646580>|0x120|+0x019:'353'
	// ******
}

// STATE[STUB]
float2 clip_2_screen(
	float3 const&		world_pixel,
	float4x4 const&		wvpMatrix,
	u32					screen_width,
	u32					screen_height
)
{
	// LOCALS
	// float4 							result
	// ******

	return vostok::math::float2(1., 1.);

	// FUNCTION BODY[0x6447d0]: 16
	// <0x6447d3>|0x003|+0x0b1:'358'
	// <0>
	// <0x644884>|0x0b4|+0x023:'360'
	// <0x6448a7>|0x0d7|+0x008:'361'
	// <0>
	// <0x6448af>|0x0df|+0x008:'363'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6448b7>|0x0e7|+0x0b2:'370'
	// <0x644969>|0x199|+0x01a:'371'
	// <0>
	// <0x644983>|0x1b3|+0x007:'373'
	// ******
}

// STATE[STUB]
float frac( float f )
{
	return 0.0f;

	// FUNCTION BODY[0x6446f0]: 1
	// <0x6446f0>|0x000|+0x00c:'380'
	// ******
}

// STATE[STUB]
u8 calc_pattern( float2 const& begin, float2 const& end )
{
	return 0;

	// FUNCTION BODY[0x644770]: 1
	// <0x644776>|0x006|+0x048:'385'
	// ******
}

// STATE[STUB]
void system_renderer::draw_screen_lines(
	float3 const*			points,
	u32						count,
	math::color const&		color,
	float					width,
	u32						pattern,
	bool					use_depth,
	bool					is_screen_space_coord
)
{
	// LOCALS
	// float 							shift
	// u32 								ioffset
	// u32 								screen_width
	// vertex_colored_sl* 				vbuffer
	// float4x4 						wvpMatrix
	// u32 								vertex_count
	// u32 								u32_color
	// u32 								voffset
	// u32 								screen_height
	// u32 								i
	// float2 							screen_end
	// float3 							current_point
	// float2 							screen_start
	// ******

	// FUNCTION BODY[0x646070]: 72
	// <0x64607d>|0x00d|+0x014:'390'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x646091>|0x021|+0x035:'398'
	// <0>
	// <0x6460c6>|0x056|+0x02c:'400'
	// <0x6460f2>|0x082|+0x01d:'401'
	// <0>
	// <1>
	// <0x64610f>|0x09f|+0x071:'404'
	// <0x646180>|0x110|-0x06c:'404'
	// <0x646114>|0x0a4|+0x007:'405'
	// <0>
	// <1>
	// <2>
	// <0x64611b>|0x0ab|+0x018:'409'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x646133>|0x0c3|+0x01c:'414'
	// <0>
	// <0x64614f>|0x0df|+0x003:'416'
	// <0>
	// <0x646152>|0x0e2|+0x1a9:'418'
	// <0x6462fb>|0x28b|-0x177:'418'
	// <0>
	// <1>
	// <0x646184>|0x114|+0x008:'421'
	// <0>
	// <0x64618c>|0x11c|+0x005:'423'
	// <0x646191>|0x121|+0x00f:'424'
	// <0>
	// <0x6461a0>|0x130|+0x012:'426'
	// <0x6461b2>|0x142|+0x007:'427'
	// <0x6461b9>|0x149|+0x004:'428'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6461bd>|0x14d|+0x038:'433'
	// <0x6461f5>|0x185|+0x01f:'434'
	// <0>
	// <0x646214>|0x1a4|+0x058:'436'
	// <0>
	// <0x64626c>|0x1fc|+0x01a:'438'
	// <0x646286>|0x216|+0x025:'439'
	// <0>
	// <0x6462ab>|0x23b|+0x00a:'441'
	// <0x6462b5>|0x245|+0x00b:'442'
	// <0>
	// <1>
	// <0x6462c0>|0x250|+0x020:'445'
	// <0x6462e0>|0x270|+0x020:'446'
	// <0>
	// <1>
	// <0x646300>|0x290|+0x02d:'449'
	// <0x64632d>|0x2bd|+0x02e:'450'
	// <0>
	// <0x64635b>|0x2eb|+0x008:'452'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x646363>|0x2f3|+0x021:'457'
	// <0>
	// <0x646384>|0x314|+0x059:'459'
	// <0>
	// <0x6463dd>|0x36d|+0x078:'461'
	// ******
}

// STATE[STUB]
void system_renderer::draw_3D_point(
	float3 const&			position,
	float					width,
	math::color const&		color,
	bool					use_depth
)
{
	// LOCALS
	// float4x4 						proj_matrix
	// float 							dist
	// fixed_vector< vertex_colored, 4 > vertices
	// u32 								screen_width
	// float4x4 						inv_view_matrix
	// float3[4] 						quad_position
	// fixed_vector< u16, 6 > 			indices
	// float3 							offset_by_x
	// float4x4 						view_matrix
	// float4x4 						inv_view_proj_matrix
	// ******

	// TYPEDEFS
	// typedef
	// 	fixed_vector< vertex_colored, 4 >
	// 	colored_vertices_type;

	// typedef
	// 	fixed_vector< u16, 6 >
	// 	colored_indices_type;

	// ******

	// FUNCTION BODY[0x645a60]: 50
	// <0x645a60>|0x000|+0x022:'466'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x645a82>|0x022|+0x020:'471'
	// <0>
	// <0x645aa2>|0x042|+0x019:'473'
	// <0x645abb>|0x05b|+0x014:'474'
	// <0>
	// <0x645acf>|0x06f|+0x018:'476'
	// <0>
	// <0x645ae7>|0x087|+0x00f:'478'
	// <0>
	// <1>
	// <0x645af6>|0x096|+0x01d:'481'
	// <0>
	// <1>
	// <0x645b13>|0x0b3|+0x05f:'484'
	// <0>
	// <1>
	// <0x645b72>|0x112|+0x176:'487'
	// <0x645ce8>|0x288|+0x0d2:'488'
	// <0>
	// <1>
	// <0x645dba>|0x35a|+0x0dc:'491'
	// <0>
	// <0x645e96>|0x436|+0x010:'493'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x645ea6>|0x446|+0x00e:'499'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x645eb4>|0x454|+0x105:'504'
	// <0>
	// <1>
	// <0x645fb9>|0x559|+0x019:'507'
	// <0x645fd2>|0x572|+0x06b:'508'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x64603d>|0x5dd|+0x026:'515'
	// ******
}

// STATE[STUB]
void system_renderer::fill_surface(
	render_target_ptr		surface0,
	render_target_ptr		surface1,
	render_target_ptr		surface2,
	render_target_ptr		surface3,
	render_target_ptr		depth_rt,
	bool					reset_depth_rt,
	D3D11_VIEWPORT*			user_view_port,
	float					pos_x,
	float					pos_y,
	float					size_x,
	float					size_y
)
{
	// LOCALS
	// D3D11_VIEWPORT 					prev_view_port
	// D3D11_VIEWPORT 					view_port
	// u32 								offset
	// float 							pos_x0
	// float 							pos_x1
	// float 							pos_y1
	// float 							pos_y0
	// ******

	// CALL SITE INFO
	// <0x644a52> -> void < unknown >( u32*, D3D11_VIEWPORT* )
	// ******

	// FUNCTION BODY[0x644a20]: 42
	// <0>
	// <1>
	// <0x644a29>|0x009|+0x02b:'532'
	// <0>
	// <0x644a54>|0x034|+0x017:'534'
	// <0x644a6b>|0x04b|+0x010:'535'
	// <0x644a7b>|0x05b|+0x007:'536'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x644a82>|0x062|+0x035:'541'
	// <0>
	// <0x644ab7>|0x097|+0x006:'543'
	// <0x644abd>|0x09d|+0x015:'544'
	// <0x644ad2>|0x0b2|+0x002:'545'
	// <0x644ad4>|0x0b4|+0x019:'546'
	// <0>
	// <0x644aed>|0x0cd|+0x00d:'548'
	// <0x644afa>|0x0da|+0x019:'549'
	// <0x644b13>|0x0f3|+0x002:'550'
	// <0x644b15>|0x0f5|+0x01e:'551'
	// <0>
	// <1>
	// <2>
	// <0x644b33>|0x113|+0x015:'555'
	// <0x644b48>|0x128|+0x008:'556'
	// <0>
	// <1>
	// <2>
	// <0x644b50>|0x130|+0x06d:'560'
	// <0x644bbd>|0x19d|+0x060:'561'
	// <0x644c1d>|0x1fd|+0x033:'562'
	// <0x644c50>|0x230|+0x06b:'563'
	// <0x644cbb>|0x29b|+0x036:'564'
	// <0x644cf1>|0x2d1|+0x037:'565'
	// <0>
	// <0x644d28>|0x308|+0x008:'567'
	// <0>
	// <0x644d30>|0x310|+0x076:'569'
	// <0>
	// <0x644da6>|0x386|+0x02a:'571'
	// ******
}

// STATE[STUB]
void system_renderer::draw_aabb( math::aabb const& aabb, math::color const& color )
{
	// LOCALS
	// vertex_colored[8] 				vertices
	// ******

	// FUNCTION BODY[0x6465a0]: 20
	// <0x6465a6>|0x006|+0x00d:'576'
	// <0>
	// <1>
	// <2>
	// <0x6465b3>|0x013|+0x004:'580'
	// <0x6465b7>|0x017|+0x008:'581'
	// <0x6465bf>|0x01f|+0x053:'582'
	// <0x646612>|0x072|+0x01d:'583'
	// <0x64662f>|0x08f|+0x031:'584'
	// <0x646660>|0x0c0|+0x038:'585'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x646698>|0x0f8|+0x0a8:'595'
	// ******
}

// STATE[STUB]
void system_renderer::draw_triangles(
	vertex_colored const* const		vertices_begin,
	vertex_colored const* const		vertices_end,
	u16 const* const				indices_begin,
	u16 const* const				indices_end,
	bool							covering_effect
)
{
	// LOCALS
	// u32 								ioffset
	// u32 								voffset
	// ******

	// FUNCTION BODY[0x6457c0]: 47
	// <0x6457cd>|0x00d|+0x00f:'630'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6457dc>|0x01c|+0x004:'638'
	// <0x6457e0>|0x020|+0x016:'639'
	// <0x6457f6>|0x036|+0x00f:'640'
	// <0x645805>|0x045|+0x028:'641'
	// <0>
	// <1>
	// <2>
	// <0x64582d>|0x06d|+0x008:'645'
	// <0x645835>|0x075|+0x012:'646'
	// <0x645847>|0x087|+0x010:'647'
	// <0x645857>|0x097|+0x024:'648'
	// <0>
	// <0x64587b>|0x0bb|+0x00b:'650'
	// <0>
	// <0x645886>|0x0c6|+0x007:'652'
	// <0>
	// <0x64588d>|0x0cd|+0x029:'654'
	// <0>
	// <0x6458b6>|0x0f6|+0x002:'656'
	// <0>
	// <0x6458b8>|0x0f8|+0x009:'658'
	// <0x6458c1>|0x101|+0x005:'659'
	// <0x6458c6>|0x106|+0x002:'660'
	// <0x6458c8>|0x108|+0x009:'661'
	// <0x6458d1>|0x111|+0x002:'662'
	// <0x6458d3>|0x113|+0x002:'663'
	// <0x6458d5>|0x115|+0x010:'664'
	// <0>
	// <1>
	// <0x6458e5>|0x125|+0x022:'667'
	// <0>
	// <0x645907>|0x147|+0x079:'669'
	// <0>
	// <1>
	// <2>
	// <0x645980>|0x1c0|+0x029:'673'
	// <0x6459a9>|0x1e9|+0x00b:'674'
	// <0x6459b4>|0x1f4|+0x022:'675'
	// <0x6459d6>|0x216|+0x07b:'676'
	// ******
}

// STATE[STUB]
void system_renderer::draw_ui_vertices(
	vertex_formats::TL const*		vertices,
	u32 const&						count,
	s32								prim_type,
	s32								point_type
)
{
	// LOCALS
	// u32 								v_offset
	// ******

	// FUNCTION BODY[0x645680]: 39
	// <0x64568c>|0x00c|+0x00f:'697'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x64569b>|0x01b|+0x016:'702'
	// <0x6456b1>|0x031|+0x01c:'703'
	// <0x6456cd>|0x04d|+0x029:'704'
	// <0>
	// <0x6456f6>|0x076|+0x00b:'706'
	// <0>
	// <0x645701>|0x081|+0x007:'708'
	// <0>
	// <0x645708>|0x088|+0x008:'710'
	// <0x645710>|0x090|+0x004:'711'
	// <0>
	// <1>
	// <0x645714>|0x094|+0x00a:'714'
	// <0>
	// <1>
	// <0x64571e>|0x09e|+0x010:'717'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x64572e>|0x0ae|+0x024:'722'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x645752>|0x0d2|+0x012:'727'
	// <0>
	// <0x645764>|0x0e4|-0x019:'729'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x64574b>|0x0cb|+0x065:'736'
	// ******
}

// STATE[STUB]
void system_renderer::draw_render_models_selection( vector< render_model_instance_impl_ptr >& render_models )
{
	// LOCALS
	// math::aabb 						draw_box
	// ******

	// CALL SITE INFO
	// <0x6469b2> -> math::aabb < unknown >()
	// ******

	// FUNCTION BODY[0x646910]: 44
	// <0x64691b>|0x00b|+0x011:'1003'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x64692c>|0x01c|+0x014:'1008'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x646940>|0x030|+0x05b:'1013'
	// <0>
	// <0x64699b>|0x08b|+0x005:'1015'
	// <0>
	// <1>
	// <0x6469a0>|0x090|+0x03b:'1018'
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
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <0x6469db>|0x0cb|+0x045:'1037'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x646a20>|0x110|+0x0c4:'1046'
	// ******
}

// STATE[STUB]
void system_renderer::draw_particle_system_instance_selections( vector< resources::unmanaged_resource_ptr > const& instances )
{
	// LOCALS
	// particle::world* 				part_world
	// resources::unmanaged_resource_ptr const* instance_it
	// vectora< particle::render_particle_emitter_instance* > emitters
	// particle::render_particle_emitter_instance* const* it
	// ******

	// STATICS
	// static < NoType > 				 = <0x645668>;
	// ******

	// CALL SITE INFO
	// <0x64532d> -> void < unknown >( resources::unmanaged_resource_ptr, vectora< particle::render_particle_emitter_instance* >& )
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x634385 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x634389 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x634390 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1") })
	// ******

	// FUNCTION BODY[0x6451f0]: 80
	// <0x6451f0>|0x000|+0x01a:'1070'
	// <0>
	// <1>
	// <0x64520a>|0x01a|+0x00f:'1073'
	// <0>
	// <1>
	// <0x645219>|0x029|+0x013:'1076'
	// <0>
	// <1>
	// <0x64522c>|0x03c|+0x00c:'1079'
	// <0>
	// <1>
	// <2>
	// <0x645238>|0x048|+0x05e:'1083'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x645296>|0x0a6|+0x05b:'1091'
	// <0>
	// <0x6452f1>|0x101|+0x00f:'1093'
	// <0>
	// <0x645300>|0x110|+0x005:'1095'
	// <0>
	// <0x645305>|0x115|+0x02a:'1097'
	// <0>
	// <0x64532f>|0x13f|+0x014:'1099'
	// <0>
	// <0x645343>|0x153|+0x002:'1101'
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
	// <13>
	// <0x645345>|0x155|+0x026:'1116'
	// <0>
	// <0x64536b>|0x17b|+0x008:'1118'
	// <0>
	// <1>
	// <0x645373>|0x183|+0x012:'1121'
	// <0>
	// <0x645385>|0x195|+0x002:'1123'
	// <0x645387>|0x197|+0x002:'1124'
	// <0>
	// <1>
	// <0x645389>|0x199|+0x005:'1127'
	// <0x64538e>|0x19e|+0x002:'1128'
	// <0>
	// <1>
	// <2>
	// <0x645390>|0x1a0|+0x010:'1132'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x6453a0>|0x1b0|+0x243:'1142'
	// <0x6455e3>|0x3f3|+0x014:'1143'
	// <0>
	// <0x6455f7>|0x407|+0x00f:'1145'
	// <0>
	// <0x645606>|0x416|+0x034:'1147'
	// <0>
	// <0x64563a>|0x44a|+0x024:'1149'
	// ******
}

// STATE[STUB]
void system_renderer::draw_speedtree_instance_selections( vector< speedtree_instance_ptr > const& instances )
{
	// LOCALS
	// math::aabb 						draw_box
	// ******

	// FUNCTION BODY[0x646750]: 77
	// <0x64675a>|0x00a|+0x00f:'1154'
	// <0>
	// <1>
	// <0x646769>|0x019|+0x011:'1157'
	// <0>
	// <1>
	// <2>
	// <0x64677a>|0x02a|+0x07e:'1161'
	// <0>
	// <0x6467f8>|0x0a8|+0x029:'1163'
	// <0>
	// <1>
	// <0x646821>|0x0d1|+0x030:'1166'
	// <0x646851>|0x101|-0x04d:'1167'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x646804>|0x0b4|+0x062:'1176'
	// <0x646866>|0x116|+0x094:'1176'
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
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <22>
	// <23>
	// <24>
	// <25>
	// <26>
	// <27>
	// <28>
	// <29>
	// <30>
	// <31>
	// <32>
	// <33>
	// <34>
	// <35>
	// <36>
	// <37>
	// <38>
	// <39>
	// <40>
	// <41>
	// <42>
	// <43>
	// <44>
	// <45>
	// <46>
	// <47>
	// <48>
	// <49>
	// <50>
	// <51>
	// <52>
	// <53>
	// ******
}

} // namespace render
} // namespace vostok
