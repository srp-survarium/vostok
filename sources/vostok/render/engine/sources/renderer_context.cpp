#include "pch.h"
#include "renderer_context.h"

#include <vostok/console_command.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/res_texture.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/shader_constant_binding.h>

#include "renderer_register_effects.h"
#include "scene_view.h"

static u32 s_lighting_material_strategy = 0;
static vostok::console_commands::cc_u32 s_cc_lighting_material_strategy(
	"lighting_material_strategy",
	s_lighting_material_strategy,
	0,
	4,
	true,
	vostok::console_commands::command_type_engine_internal
);

static float s_shadow_map_z_bias = 0.0001f;
static vostok::console_commands::cc_float s_cc_shadow_map_z_bias(
	"shadow_map_z_bias",
	s_shadow_map_z_bias,
	0,
	4,
	true,
	vostok::console_commands::command_type_engine_internal
);

namespace vostok {
namespace render {

renderer_context::renderer_context( ) :
	m_targets				( NULL ),
	m_t_null				( NULL ),
	m_g_quad_uv				( NULL ),
	m_g_quad_2uv			( NULL ),
	m_g_quad_eye_ray		( NULL ),
	m_quad_ib				( NULL ),
	m_t_shadow_cascade		( NULL ),
	m_time_delta			( 0.f ),
	m_scene					( NULL ),
	m_scene_view			( NULL ),
	m_view_pos				( 0.f, 0.f, 0.f, 1.f ),
	m_eye_pos_view_space	( 0.f, 0.f, 0.f, 1.f ),
	m_view_dir				( 0.f, 0.f, 1.f, 0.1f ),
	m_w						( float4x4( ).identity( ) ),
	m_w_transposed			( float4x4( ).identity( ) ),
	m_v						( float4x4( ).identity( ) ),
	m_v_transposed			( float4x4( ).identity( ) ),
	m_v_inverted			( float4x4( ).identity( ) ),
	m_v_inverted_transposed	( float4x4( ).identity( ) ),
	m_p						( float4x4( ).identity( ) ),
	m_p_transposed			( float4x4( ).identity( ) ),
	m_p_inverted			( float4x4( ).identity( ) ),
	m_wv					( float4x4( ).identity( ) ),
	m_wv_transposed			( float4x4( ).identity( ) ),
	m_vp					( float4x4( ).identity( ) ),
	m_vp_transposed			( float4x4( ).identity( ) ),
	m_wvp					( float4x4( ).identity( ) ),
	m_wvp_transposed		( float4x4( ).identity( ) ),
	m_v2shadow0				( float4x4( ).identity( ) ),
	m_v2shadow1				( float4x4( ).identity( ) ),
	m_v2shadow2				( float4x4( ).identity( ) ),
	m_v2shadow3				( float4x4( ).identity( ) ),
	m_c_w					( NULL ),
	m_c_w_inv				( NULL ),
	m_c_v					( NULL ),
	m_c_p					( NULL ),
	m_c_wv					( NULL ),
	m_c_vp					( NULL ),
	m_c_wvp					( NULL ),
	m_c_v2w					( NULL ),
	m_solid_color_specular		( 0.f, 0.f, 0.f, 0.f ),
	m_solid_material_parameters	( 10.f, 1.f, 0.f, 0.f )
{
	// LOCALS
	// sun_cascade[4] 					cascades
	// u32 								i
	// ******

	// FUNCTION BODY[0x6433c0]: 75
	// <0>
	// <1>
	// <0x643743>|0x383|+0x01c:'75'
	// <0x64375f>|0x39f|+0x05f:'76'
	// <0>
	// <1>
	// <0x6437be>|0x3fe|+0x00e:'79'
	// <0>
	// <0x6437cc>|0x40c|+0x054:'81'
	// <0>
	// <0x643820>|0x460|+0x012:'83'
	// <0x643832>|0x472|+0x005:'84'
	// <0>
	// <1>
	// <2>
	// <0x643837>|0x477|+0x011:'88'
	// <0x643848>|0x488|+0x011:'89'
	// <0>
	// <0x643859>|0x499|+0x011:'91'
	// <0x64386a>|0x4aa|+0x021:'92'
	// <0>
	// <0x64388b>|0x4cb|+0x01a:'94'
	// <0x6438a5>|0x4e5|+0x02d:'95'
	// <0>
	// <0x6438d2>|0x512|+0x023:'97'
	// <0x6438f5>|0x535|+0x04b:'98'
	// <0>
	// <1>
	// <0x643940>|0x580|+0x026:'101'
	// <0>
	// <0x643966>|0x5a6|+0x00f:'103'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x643975>|0x5b5|+0x030:'110'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6439a5>|0x5e5|+0x037:'115'
	// <0>
	// <0x6439dc>|0x61c|+0x00f:'117'
	// <0>
	// <0x6439eb>|0x62b|+0x13c:'119'
	// <0x643b27>|0x767|+0x13c:'120'
	// <0x643c63>|0x8a3|+0x0a8:'121'
	// <0x643d0b>|0x94b|+0x0a3:'122'
	// <0x643dae>|0x9ee|+0x0a2:'123'
	// <0x643e50>|0xa90|+0x0a3:'124'
	// <0x643ef3>|0xb33|+0x0a3:'125'
	// <0x643f96>|0xbd6|+0x0a3:'126'
	// <0x644039>|0xc79|+0x0a3:'127'
	// <0>
	// <0x6440dc>|0xd1c|+0x09f:'129'
	// <0x64417b>|0xdbb|+0x09d:'130'
	// <0x644218>|0xe58|+0x09d:'131'
	// <0x6442b5>|0xef5|+0x09d:'132'
	// <0x644352>|0xf92|+0x09d:'133'
	// <0>
	// <0x6443ef>|0x102f|+0x09d:'135'
	// <0x64448c>|0x10cc|+0x09d:'136'
	// <0x644529>|0x1169|+0x082:'137'
	// <0>
	// <0x6445ab>|0x11eb|+0x082:'139'
	// <0>
	// <0x64462d>|0x126d|+0x048:'141'
	// <0x644675>|0x12b5|-0x03c:'141'
	// <0>
	// <0x644639>|0x1279|+0x009:'143'
	// <0x644642>|0x1282|+0x047:'144'
	// <0>
	// <1>
	// <0x644689>|0x12c9|+0x03f:'147'
	// ******

	for ( u32 i = 0; i < rt_num_render_targets; ++i )
	{
		m_family[i].target	= NULL;
		m_family[i].texture	= NULL;
	}

	m_current_size			= math::uint2( 0, 0 );
	m_t_shadow_cascade		= NULL;

	memset					( &m_eye_rays, 0, sizeof( m_eye_rays ) );
	register_effect_descriptors( );

	sun_cascade cascades[sun_cascade::num_max_sun_shadow_cascades];

	cascades[0].size		= 10.0f;
	cascades[0].bias		= 0.0001f;

	cascades[1].size		= 30.0f;
	cascades[1].bias		= 0.00015f;

	cascades[2].size		= 90.0f;
	cascades[2].bias		= 0.0002f;

	cascades[3].size		= 270.0f;
	cascades[3].bias		= 0.00035f;

	for ( u32 cascade_index = 0; cascade_index < sun_cascade::num_max_sun_shadow_cascades; cascade_index++ )
		m_sun_cascades.push_back( cascades[cascade_index] );

	memset					( &m_eye_rays, 0, sizeof( m_eye_rays ) );
	float n					= 6000.f;
	float f					= 10000.f;
	float r					= 1.f / ( f - n );
	m_fog_params.set		( -n * r, r, r, r );
	m_screen_resolution		= float4( 1.f, 1.f, 1.f, 1.f );

	reset_matrices			( );
	m_c_w					= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "m_W", &m_w_transposed ) );
	m_c_v					= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "m_V", &m_v_transposed ) );
	m_c_p					= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "m_P", &m_p_transposed ) );
	m_c_v2w					= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "m_V2W", &m_v_inverted_transposed ) );
	m_c_wv_inv				= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "m_WV_inverted", &m_wv_inverted_transposed ) );
	m_c_wv					= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "m_WV", &m_wv_transposed ) );
	m_c_vp					= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "m_VP", &m_vp_transposed ) );
	m_c_wvp					= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "m_WVP", &m_wvp_transposed ) );
	m_c_near_far			= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "near_far_invn_invf", &m_near_far_invn_invf ) );

	m_c_fog_params			= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "fog_params", &m_fog_params ) );
	m_c_screen_resolution	= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "screen_res", &m_screen_resolution ) );
	m_c_view_pos			= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "eye_position", &m_view_pos ) );
	m_c_eye_pos_view_space	= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "eye_position_view_space", &m_eye_pos_view_space ) );
	m_c_view_dir			= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "eye_direction", &m_view_dir ) );

	m_c_solid_color_specular		= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "solid_color_specular", &m_solid_color_specular ) );
	m_c_solid_material_parameters	= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "solid_material_params", &m_solid_material_parameters ) );
	m_c_solid_emission_color		= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "solid_emission_color", &m_solid_emission_color ) );

	m_c_scene_time			= resource_manager::ref( ).register_constant_binding( shader_constant_binding( "scene_time", &m_current_time ) );

	for ( u32 i = 0; i < rt_num_render_targets; ++i )
	{
		pcstr const name	= rt_index_to_name( enum_render_target_index( i ) );
		if ( name )
			m_family[i].texture	= resource_manager::ref( ).create_texture( name, 0, 0, false, true, true, 0 );
	}

	m_t_null				= resource_manager::ref( ).create_texture( "$user$null", 0, 0, false, true, true, 0 );
}

void renderer_context::set_scene( render::scene* in_scene )
{
	// FUNCTION BODY[0x642720]: 2
	// <0x642720>|0x000|+0x006:'169'
	// <0>
	// ******

	m_scene					= in_scene;
}

scene_view const* renderer_context::scene_view( ) const
{
	// FUNCTION BODY[0x642740]: 1
	// <0x642740>|0x000|+0x006:'175'
	// ******

	return static_cast_checked< vostok::render::scene_view const* >( m_scene_view.c_ptr( ) );
}

scene_view* renderer_context::get_scene_view( )
{
	// FUNCTION BODY[0x642730]: 1
	// <0x642730>|0x000|+0x006:'180'
	// ******

	return static_cast_checked< vostok::render::scene_view* >( m_scene_view.c_ptr( ) );
}

void renderer_context::set_scene_view( base_scene_view_ptr view_ptr )
{
	// FUNCTION BODY[0x643360]: 9
	// <0x643362>|0x002|+0x010:'185'
	// <0>
	// <0x643372>|0x012|+0x006:'187'
	// <0>
	// <0x643378>|0x018|+0x00c:'189'
	// <0>
	// <1>
	// <2>
	// <0x643384>|0x024|+0x00d:'193'
	// ******

	m_scene_view			= view_ptr;

	vostok::render::scene_view* view = static_cast_checked< vostok::render::scene_view* >( view_ptr.c_ptr( ) );

	set_v					( view->camera( ).get_view_transform( ) );
	set_p					( view->camera( ).get_projection_transform( ) );
}

void renderer_context::set_view2shadow( float4x4 const& view2shadow, u32 index )
{
	// STATICS
	// static < NoType > 				 = <0x64270c>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6316c2 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6316d4 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6316e6 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6316f8 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1") })
	// ******

	// FUNCTION BODY[0x6426b0]: 15
	// <0x6426b5>|0x005|+0x00d:'198'
	// <0>
	// <1>
	// <0x6426c2>|0x012|+0x012:'201'
	// <0>
	// <1>
	// <0x6426d4>|0x024|+0x012:'204'
	// <0>
	// <1>
	// <0x6426e6>|0x036|-0x01e:'207'
	// <0>
	// <1>
	// <0x6426c8>|0x018|+0x012:'210'
	// <0x6426da>|0x02a|+0x012:'210'
	// <0x6426ec>|0x03c|+0x00c:'210'
	// <0x6426f8>|0x048|-0x027:'210'
	// <0>
	// <1>
	// <0x6426d1>|0x021|+0x012:'213'
	// <0x6426e3>|0x033|+0x012:'213'
	// <0x6426f5>|0x045|+0x012:'213'
	// ******

	switch ( index )
	{
		case 0:
			m_v2shadow0		= view2shadow;
			break;
		case 1:
			m_v2shadow1		= view2shadow;
			break;
		case 2:
			m_v2shadow2		= view2shadow;
			break;
		case 3:
			m_v2shadow3		= view2shadow;
			break;
	}
}

float4x4 const& renderer_context::get_view2shadow( u32 index ) const
{
	// STATICS
	// static < NoType > 				 = <0x642694>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63168e }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN5") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x63167c }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x631682 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x631688 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1") })
	// ******

	// FUNCTION BODY[0x642670]: 12
	// <0x642670>|0x000|+0x00c:'217'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x64267c>|0x00c|+0x006:'222'
	// <0>
	// <0x642682>|0x012|+0x006:'224'
	// <0>
	// <0x642688>|0x018|+0x006:'226'
	// <0>
	// <0x64268e>|0x01e|-0x00d:'228'
	// <0x642681>|0x011|+0x006:'229'
	// <0x642687>|0x017|+0x006:'229'
	// <0x64268d>|0x01d|+0x006:'229'
	// ******

	switch ( index )
	{
		case 0:
			return m_v2shadow0;
		case 1:
			return m_v2shadow1;
		case 2:
			return m_v2shadow2;
		case 3:
			return m_v2shadow3;
	}
	return m_v2shadow0;
}

void renderer_context::set_target_context( renderer_context_targets const* targets_context, bool force_set )
{
	// FUNCTION BODY[0x642ac0]: 18
	// <0x642ac3>|0x003|+0x02e:'233'
	// <0>
	// <1>
	// <0x642af1>|0x031|+0x002:'236'
	// <0x642af3>|0x033|+0x018:'237'
	// <0>
	// <0x642b0b>|0x04b|+0x004:'239'
	// <0>
	// <0x642b0f>|0x04f|+0x011:'241'
	// <0x642b20>|0x060|+0x021:'242'
	// <0x642b41>|0x081|+0x016:'243'
	// <0>
	// <1>
	// <0x642b57>|0x097|+0x054:'246'
	// <0>
	// <1>
	// <0x642bab>|0x0eb|+0x01c:'249'
	// <0x642bc7>|0x107|-0x08f:'250'
	// <0x642b38>|0x078|+0x0ad:'251'
	// ******

	if (
		!force_set &&
		m_targets == targets_context &&
		m_current_size == targets_context->size( )
	)
		return;

	m_targets				= targets_context;
	m_current_size			= targets_context->size( );

	if ( !targets_context )
	{
		for ( u32 i = 0; i < rt_num_render_targets; ++i )
			m_family[i].texture->clone( &*m_t_null );
		return;
	}

	m_screen_resolution		= float4(
		float( m_current_size.x ),
		float( m_current_size.y ),
		1.f / float( m_current_size.x ),
		1.f / float( m_current_size.y )
	);

	for ( u32 i = 0; i < rt_num_render_targets; ++i )
	{
		res_texture_ptr const& source = targets_context->m_family[i].texture;
		if ( source )
			m_family[i].texture->clone( &*source );
	}
}

void renderer_context::set_time_delta( const float time_delta )
{
	// FUNCTION BODY[0x642660]: 1
	// <0x642660>|0x000|+0x008:'255'
	// ******

	m_time_delta			= time_delta;
}

void renderer_context::set_current_time( const float current_time )
{
	// FUNCTION BODY[0x642650]: 1
	// <0x642650>|0x000|+0x008:'260'
	// ******

	m_current_time			= current_time;
}

void renderer_context::set_w( float4x4 const& m )
{
	// FUNCTION BODY[0x643190]: 26
	// <0x6431a2>|0x012|+0x011:'265'
	// <0x6431b3>|0x023|+0x023:'266'
	// <0x6431d6>|0x046|+0x014:'267'
	// <0>
	// <1>
	// <0x6431ea>|0x05a|+0x021:'270'
	// <0x64320b>|0x07b|+0x008:'271'
	// <0x643213>|0x083|+0x013:'272'
	// <0>
	// <0x643226>|0x096|+0x01a:'274'
	// <0x643240>|0x0b0|+0x02b:'275'
	// <0>
	// <0x64326b>|0x0db|+0x022:'277'
	// <0>
	// <0x64328d>|0x0fd|+0x025:'279'
	// <0x6432b2>|0x122|+0x016:'280'
	// <0x6432c8>|0x138|+0x016:'281'
	// <0x6432de>|0x14e|+0x016:'282'
	// <0>
	// <0x6432f4>|0x164|+0x01c:'284'
	// <0x643310>|0x180|+0x016:'285'
	// <0x643326>|0x196|+0x016:'286'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******

	m_w						= m;
	m_wv					= m_w * m_v;
	m_wvp					= mul4x4( m_wv, m_p );

	m_wv_inverted_transposed = m_wv;
	m_wv_inverted_transposed.try_invert( m_wv_inverted_transposed );
	m_wv_inverted_transposed = transpose( m_wv_inverted_transposed );

	m_w_transposed			= transpose( m_w );
	m_wv_transposed			= transpose( m_wv );
	m_wvp_transposed		= transpose( m_wvp );

	backend::ref( ).set_vs_constant( m_c_w, m_w_transposed );
	backend::ref( ).set_vs_constant( m_c_wv_inv, m_wv_inverted_transposed );
	backend::ref( ).set_vs_constant( m_c_wv, m_wv_transposed );
	backend::ref( ).set_vs_constant( m_c_wvp, m_wvp_transposed );

	backend::ref( ).set_ps_constant( m_c_w, m_w_transposed );
	backend::ref( ).set_ps_constant( m_c_wv, m_wv_transposed );
	backend::ref( ).set_ps_constant( m_c_wvp, m_wvp_transposed );
}

void renderer_context::set_v( float4x4 const& m )
{
	// FUNCTION BODY[0x642da0]: 35
	// <0x642db0>|0x010|+0x024:'295'
	// <0>
	// <0x642dd4>|0x034|+0x023:'297'
	// <0x642df7>|0x057|+0x00d:'298'
	// <0>
	// <0x642e04>|0x064|+0x03f:'300'
	// <0x642e43>|0x0a3|+0x04d:'301'
	// <0x642e90>|0x0f0|+0x038:'302'
	// <0>
	// <0x642ec8>|0x128|+0x066:'304'
	// <0x642f2e>|0x18e|+0x04f:'305'
	// <0>
	// <0x642f7d>|0x1dd|+0x01d:'307'
	// <0>
	// <1>
	// <0x642f9a>|0x1fa|+0x0bc:'310'
	// <0x643056>|0x2b6|+0x016:'311'
	// <0>
	// <0x64306c>|0x2cc|+0x016:'313'
	// <0x643082>|0x2e2|+0x016:'314'
	// <0>
	// <0x643098>|0x2f8|+0x016:'316'
	// <0x6430ae>|0x30e|+0x016:'317'
	// <0>
	// <0x6430c4>|0x324|+0x016:'319'
	// <0x6430da>|0x33a|+0x016:'320'
	// <0>
	// <0x6430f0>|0x350|+0x016:'322'
	// <0x643106>|0x366|+0x013:'323'
	// <0>
	// <0x643119>|0x379|+0x01e:'325'
	// <0x643137>|0x397|+0x016:'326'
	// <0>
	// <0x64314d>|0x3ad|+0x016:'328'
	// <0x643163>|0x3c3|+0x016:'329'
	// ******

	m_v_transposed			= transpose( m_v = m );

	m_v_inverted.try_invert	( m );
	m_v_inverted_transposed	= transpose( m_v_inverted );

	m_wv_transposed			= transpose( m_wv = m_w * m_v );
	m_vp_transposed			= transpose( m_vp = mul4x4( m_v, m_p ) );
	m_wvp_transposed		= transpose( m_wvp = mul4x4( m_wv, m_p ) );

	m_view_pos				= float4( m_v_inverted.c.xyz( ), 1 );
	m_view_dir				= float4( m_v_inverted.k.xyz( ), 0 );
	m_eye_pos_view_space	= float4( m_v.transform_position( m_view_pos.xyz( ) ), 1.f );

	backend::ref( ).set_ps_constant( m_c_view_pos, m_view_pos );
	backend::ref( ).set_vs_constant( m_c_view_pos, m_view_pos );
	backend::ref( ).set_ps_constant( m_c_eye_pos_view_space, m_eye_pos_view_space );
	backend::ref( ).set_vs_constant( m_c_eye_pos_view_space, m_eye_pos_view_space );
	backend::ref( ).set_ps_constant( m_c_view_dir, m_view_dir );
	backend::ref( ).set_vs_constant( m_c_view_dir, m_view_dir );

	backend::ref( ).set_vs_constant( m_c_v, m_v_transposed );
	backend::ref( ).set_vs_constant( m_c_vp, m_vp_transposed );
	backend::ref( ).set_vs_constant( m_c_wv, m_wv_transposed );
	backend::ref( ).set_vs_constant( m_c_wvp, m_wvp_transposed );

	backend::ref( ).set_ps_constant( m_c_v, m_v_transposed );
	backend::ref( ).set_ps_constant( m_c_vp, m_vp_transposed );
	backend::ref( ).set_ps_constant( m_c_wv, m_wv_transposed );
	backend::ref( ).set_ps_constant( m_c_wvp, m_wvp_transposed );
}

void renderer_context::set_p( float4x4 const& m )
{
	// FUNCTION BODY[0x642bf0]: 31
	// <0x642bf0>|0x000|+0x026:'345'
	// <0x642c16>|0x026|+0x018:'346'
	// <0x642c2e>|0x03e|+0x01f:'347'
	// <0>
	// <1>
	// <2>
	// <0x642c4d>|0x05d|+0x01a:'351'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x642c67>|0x077|+0x03e:'356'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x642ca5>|0x0b5|+0x048:'362'
	// <0>
	// <0x642ced>|0x0fd|+0x009:'364'
	// <0x642cf6>|0x106|+0x005:'365'
	// <0>
	// <0x642cfb>|0x10b|+0x022:'367'
	// <0x642d1d>|0x12d|+0x016:'368'
	// <0>
	// <0x642d33>|0x143|+0x010:'370'
	// <0>
	// <0x642d43>|0x153|+0x01c:'372'
	// <0x642d5f>|0x16f|+0x016:'373'
	// <0>
	// <0x642d75>|0x185|+0x010:'375'
	// ******

	m_p						= m;
	m_p_transposed			= transpose( m_p = m );
	m_p						= m;

	m_p_inverted			= math::invert4x4( m );

	m_vp_transposed			= transpose( m_vp = mul4x4( m_v, m_p ) );

	m_wvp_transposed		= transpose( m_wvp = mul4x4( m_wv, m_p ) );

	update_near_far			( );
	update_eye_rays			( );

	backend::ref( ).set_vs_constant( m_c_p, m_p_transposed );
	backend::ref( ).set_vs_constant( m_c_vp, m_vp_transposed );
	backend::ref( ).set_vs_constant( m_c_wvp, m_wvp_transposed );

	backend::ref( ).set_ps_constant( m_c_p, m_p_transposed );
	backend::ref( ).set_ps_constant( m_c_vp, m_vp_transposed );
	backend::ref( ).set_ps_constant( m_c_wvp, m_wvp_transposed );
}

void renderer_context::update_near_far( )
{
	// FUNCTION BODY[0x6429f0]: 11
	// <0>
	// <0x6429f0>|0x000|+0x04b:'381'
	// <0x642a3b>|0x04b|+0x00c:'382'
	// <0>
	// <1>
	// <0x642a47>|0x057|+0x048:'385'
	// <0x642a8f>|0x09f|+0x004:'386'
	// <0>
	// <1>
	// <0x642a93>|0x0a3|+0x00f:'389'
	// <0x642aa2>|0x0b2|+0x01c:'390'
	// ******

	float4 tmp_transformed	= m_p_inverted.transform( float4( 0.f, 0.f, 0.f, 1.f ) );
	m_near_far_invn_invf.x	= tmp_transformed.z / tmp_transformed.w;

	tmp_transformed			= m_p_inverted.transform( float4( 0.f, 0.f, 1.f, 1.f ) );
	m_near_far_invn_invf.y	= tmp_transformed.z / tmp_transformed.w;

	m_near_far_invn_invf.z	= 1.f / m_near_far_invn_invf.x;
	m_near_far_invn_invf.w	= 1.f / m_near_far_invn_invf.y;
}

void renderer_context::reset_matrices( )
{
	// FUNCTION BODY[0x642900]: 17
	// <0x642903>|0x003|+0x00b:'395'
	// <0x64290e>|0x00e|+0x00b:'396'
	// <0x642919>|0x019|+0x00b:'397'
	// <0x642924>|0x024|+0x00b:'398'
	// <0x64292f>|0x02f|+0x00b:'399'
	// <0x64293a>|0x03a|+0x00b:'400'
	// <0x642945>|0x045|+0x00b:'401'
	// <0x642950>|0x050|+0x00b:'402'
	// <0x64295b>|0x05b|+0x00b:'403'
	// <0x642966>|0x066|+0x00b:'404'
	// <0x642971>|0x071|+0x00b:'405'
	// <0x64297c>|0x07c|+0x00b:'406'
	// <0x642987>|0x087|+0x00b:'407'
	// <0x642992>|0x092|+0x00b:'408'
	// <0x64299d>|0x09d|+0x00b:'409'
	// <0>
	// <0x6429a8>|0x0a8|+0x035:'411'
	// ******

	m_w.identity			( );
	m_w_transposed.identity	( );
	m_v.identity			( );
	m_v_transposed.identity	( );
	m_v_inverted.identity	( );
	m_v_inverted_transposed.identity( );
	m_p.identity			( );
	m_p_transposed.identity	( );
	m_p_inverted.identity	( );
	m_wv.identity			( );
	m_wv_transposed.identity( );
	m_vp.identity			( );
	m_vp_transposed.identity( );
	m_wvp.identity			( );
	m_wvp_transposed.identity( );

	m_near_far_invn_invf	= float4( 0.f, 0.f, 0.f, 0.f );
}

u32 renderer_context::get_lighting_material_strtegy( )
{
	return s_lighting_material_strategy;
}

float renderer_context::get_shadow_map_z_bias( )
{
	return s_shadow_map_z_bias;
}

void renderer_context::update_eye_rays( )
{
	// LOCALS
	// float4 							eye_00
	// ******

	// FUNCTION BODY[0x642750]: 27
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x642753>|0x003|+0x0be:'436'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x642811>|0x0c1|+0x016:'441'
	// <0x642827>|0x0d7|+0x00a:'442'
	// <0x642831>|0x0e1|+0x006:'443'
	// <0x642837>|0x0e7|+0x00e:'444'
	// <0x642845>|0x0f5|+0x028:'445'
	// <0x64286d>|0x11d|+0x028:'446'
	// <0x642895>|0x145|+0x02b:'447'
	// <0x6428c0>|0x170|+0x036:'448'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// ******

	float4x4 const& inv_proj = m_p_inverted;

	float4 eye_00			( -1.f, 1.f, 1.f, 1.f );

	eye_00					= inv_proj.transform( eye_00 );

	float inv_w				= 1.f / eye_00.w;
	eye_00.x				*= inv_w;
	eye_00.y				*= inv_w;
	eye_00.z				*= inv_w;
	m_eye_rays[0]			= float3( eye_00.x, eye_00.y, eye_00.z );
	m_eye_rays[1]			= float3( eye_00.x, -eye_00.y, eye_00.z );
	m_eye_rays[2]			= float3( -eye_00.x, eye_00.y, eye_00.z );
	m_eye_rays[3]			= float3( -eye_00.x, -eye_00.y, eye_00.z );
}

void renderer_context::clear_resources( )
{
	// FUNCTION BODY[0x643350]: 1
	// <0x643351>|0x001|+0x00e:'481'
	// ******

	m_scene_view			= NULL;
}

	// TYPEDEFS
	// typedef
	// 	pbyte
	// 	iterator_type;

	// typedef
	// 	pcvoid*
	// 	iterator_type;

	// typedef
	// 	void**
	// 	iterator_type;

	// typedef
	// 	vostok::fixed_string< 32 >*
	// 	iterator_type;

	// typedef
	// 	vostok::render::res_pass_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::res_shader_technique_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_binding*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_buffer_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::signature_layout_pair*
	// 	iterator_type;

	// typedef
	// 	vostok::render::speedtree_forest::tree_render_info*
	// 	iterator_type;

	// typedef
	// 	vostok::render::texture_named_instance*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok
