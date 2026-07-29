////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\stage_gbuffer.h"

namespace vostok {
namespace render {

// STATE[STUB]
void `dynamic initializer for 's_debug_profile_dip_cc''( )
{
	// FUNCTION BODY[0x7d65d0]
	// ******
}

// STATE[STUB]
void `dynamic atexit destructor for 's_debug_profile_dip_cc''( )
{
	// FUNCTION BODY[0x7ee450]
	// ******
}

// STATE[STUB]
void effect_copy_depth_rt::compile( effect_compiler& compiler, custom_config_value const& __formal )
{
	// FUNCTION BODY[0x72410]: 9
	// <0x7241b>|0x00b|+0x009:'77'
	// <0x72424>|0x014|+0x02d:'78'
	// <0x72451>|0x041|+0x048:'79'
	// <0x72499>|0x089|+0x00a:'80'
	// <0x724a3>|0x093|+0x00a:'81'
	// <0x724ad>|0x09d|+0x014:'82'
	// <0x724c1>|0x0b1|+0x010:'83'
	// <0x724d1>|0x0c1|+0x006:'84'
	// <0x724d7>|0x0c7|+0x005:'85'
	// ******
}

// STATE[STUB]
void fill_surface( render_target_ptr surf, renderer_context* context )
{
	// LOCALS
	// u32 								offset
	// float 							w
	// float 							h
	// ******

	// FUNCTION BODY[0x62cec0]: 26
	// <0x62cec0>|0x000|+0x01e:'108'
	// <0x62cede>|0x01e|+0x014:'109'
	// <0>
	// <1>
	// <0x62cef2>|0x032|+0x063:'112'
	// <0x62cf55>|0x095|+0x006:'113'
	// <0x62cf5b>|0x09b|+0x038:'114'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x62cf93>|0x0d3|+0x01a:'124'
	// <0x62cfad>|0x0ed|+0x055:'125'
	// <0x62d002>|0x142|+0x041:'126'
	// <0x62d043>|0x183|+0x061:'127'
	// <0x62d0a4>|0x1e4|+0x046:'128'
	// <0x62d0ea>|0x22a|+0x02c:'129'
	// <0>
	// <0x62d116>|0x256|+0x00b:'131'
	// <0>
	// <0x62d121>|0x261|+0x083:'133'
	// ******
}

// STATE[STUB]
 stage_gbuffer::stage_gbuffer( renderer* in_renderer, renderer_context* context )
{
	// LOCALS
	// u8[1024] 						data
	// effect_options_descriptor 		desc
	// ******

	// FUNCTION BODY[0x62d800]: 37
	// <0x62d82e>|0x02e|+0x059:'140'
	// <0x62d887>|0x087|+0x058:'141'
	// <0x62d8df>|0x0df|+0x058:'142'
	// <0x62d937>|0x137|+0x058:'143'
	// <0x62d98f>|0x18f|+0x058:'144'
	// <0x62d9e7>|0x1e7|+0x058:'145'
	// <0x62da3f>|0x23f|+0x058:'146'
	// <0x62da97>|0x297|+0x058:'147'
	// <0>
	// <0x62daef>|0x2ef|+0x058:'149'
	// <0x62db47>|0x347|+0x058:'150'
	// <0x62db9f>|0x39f|+0x058:'151'
	// <0x62dbf7>|0x3f7|+0x058:'152'
	// <0x62dc4f>|0x44f|+0x058:'153'
	// <0>
	// <0x62dca7>|0x4a7|+0x058:'155'
	// <0x62dcff>|0x4ff|+0x058:'156'
	// <0>
	// <0x62dd57>|0x557|+0x058:'158'
	// <0>
	// <0x62ddaf>|0x5af|+0x058:'160'
	// <0x62de07>|0x607|+0x04e:'161'
	// <0x62de55>|0x655|+0x04e:'162'
	// <0x62dea3>|0x6a3|+0x04e:'163'
	// <0>
	// <0x62def1>|0x6f1|+0x04e:'165'
	// <0x62df3f>|0x73f|+0x04e:'166'
	// <0>
	// <0x62df8d>|0x78d|+0x00d:'168'
	// <0>
	// <1>
	// <2>
	// <0x62df9a>|0x79a|+0x014:'172'
	// <0x62dfae>|0x7ae|+0x033:'173'
	// <0x62dfe1>|0x7e1|+0x011:'174'
	// <0>
	// <0x62dff2>|0x7f2|+0x00c:'176'
	// ******
}

// STATE[STUB]
 stage_gbuffer::~stage_gbuffer( )
{
	// FUNCTION BODY[0x62ce50]: 1
	// <0>
	// ******
}

// STATE[STUB]
bool stage_gbuffer::is_effects_ready( ) const
{
	return false;

	// FUNCTION BODY[0x62ce30]: 4
	// <0>
	// <0x62ce30>|0x000|+0x012:'187'
	// <0x62ce42>|0x012|-0x001:'187'
	// <0>
	// <1>
	// <0x62ce41>|0x011|+0x003:'190'
	// ******
}

// STATE[STUB]
void stage_gbuffer::render_models(
	vector< render_surface_instance* >&		models,
	u32										shader_lod_index,
	u32&									out_num_rendered,
	bool									z_only
)
{
	// LOCALS
	// render_surface_instance* const* 	end
	// float3 							wind_info_parameters
	// const float 						wet_intensity
	// ******

	// FUNCTION BODY[0x62d1d0]: 65
	// <0x62d1d4>|0x004|+0x002:'220'
	// <0x62d1d6>|0x006|+0x00a:'221'
	// <0>
	// <0x62d1e0>|0x010|+0x010:'223'
	// <0>
	// <0x62d1f0>|0x020|+0x002:'225'
	// <0>
	// <0x62d1f2>|0x022|+0x027:'227'
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
	// <0x62d219>|0x049|+0x010:'240'
	// <0>
	// <0x62d229>|0x059|+0x005:'242'
	// <0x62d22e>|0x05e|+0x026:'243'
	// <0x62d254>|0x084|+0x002:'244'
	// <0x62d256>|0x086|+0x022:'245'
	// <0>
	// <0x62d278>|0x0a8|+0x00a:'247'
	// <0x62d282>|0x0b2|+0x008:'248'
	// <0>
	// <0x62d28a>|0x0ba|+0x00d:'250'
	// <0>
	// <0x62d297>|0x0c7|+0x00f:'252'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x62d2a6>|0x0d6|+0x029:'258'
	// <0>
	// <0x62d2cf>|0x0ff|+0x04c:'260'
	// <0>
	// <1>
	// <0x62d31b>|0x14b|+0x00b:'263'
	// <0>
	// <0x62d326>|0x156|+0x00e:'265'
	// <0>
	// <0x62d334>|0x164|+0x011:'267'
	// <0x62d345>|0x175|+0x030:'268'
	// <0>
	// <0x62d375>|0x1a5|+0x002:'270'
	// <0>
	// <0x62d377>|0x1a7|+0x050:'272'
	// <0>
	// <1>
	// <2>
	// <0x62d3c7>|0x1f7|+0x074:'276'
	// <0>
	// <0x62d43b>|0x26b|+0x018:'278'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// ******
}

// STATE[STUB]
bool remove_model_if_not_lod_predicate::operator()( render_surface_instance* in_model )
{
	return false;

	// FUNCTION BODY[0x72510]: 14
	// <0x72510>|0x000|+0x013:'315'
	// <0x72523>|0x013|+0x002:'316'
	// <0>
	// <0x72525>|0x015|+0x00a:'318'
	// <0x7252f>|0x01f|+0x002:'319'
	// <0>
	// <0x72531>|0x021|+0x027:'321'
	// <0>
	// <1>
	// <2>
	// <0x72558>|0x048|+0x014:'325'
	// <0x7256c>|0x05c|-0x005:'326'
	// <0>
	// <0x72567>|0x057|+0x002:'328'
	// <0x72569>|0x059|+0x005:'329'
	// ******
}

// STATE[STUB]
bool remove_model_if_not_static_predicate::operator()( render_surface_instance* in_model )
{
	return false;

	// FUNCTION BODY[0x72580]: 7
	// <0>
	// <1>
	// <0x72580>|0x000|+0x01a:'364'
	// <0>
	// <0x7259a>|0x01a|+0x024:'366'
	// <0>
	// <0x725be>|0x03e|+0x010:'368'
	// <0x725ce>|0x04e|-0x001:'368'
	// <0x725cd>|0x04d|+0x006:'369'
	// ******
}

// STATE[STUB]
bool remove_model_if_not_skeletal_predicate::operator()( render_surface_instance* in_model )
{
	return false;

	// FUNCTION BODY[0x725e0]: 7
	// <0x725e0>|0x000|+0x027:'375'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x72607>|0x027|+0x023:'381'
	// <0x7262a>|0x04a|-0x001:'381'
	// <0x72629>|0x049|+0x006:'382'
	// ******
}

// STATE[STUB]
bool remove_model_if_not_translucency_predicate::operator()( render_surface_instance* in_model )
{
	return false;

	// FUNCTION BODY[0x72630]: 3
	// <0x72630>|0x000|+0x029:'389'
	// <0x72659>|0x029|-0x00e:'389'
	// <0>
	// <0x7264b>|0x01b|+0x018:'391'
	// <0x72663>|0x033|-0x00b:'391'
	// <0x72658>|0x028|+0x018:'392'
	// ******
}

// STATE[STUB]
bool sort_by_ps_predicate::operator()( render_surface_instance const* left, render_surface_instance const* right ) const
{
	return false;

	// FUNCTION BODY[0x64d60]: 7
	// <0x64d60>|0x000|+0x029:'404'
	// <0x64d89>|0x029|+0x028:'405'
	// <0>
	// <1>
	// <0x64db1>|0x051|+0x02c:'408'
	// <0>
	// <0x64ddd>|0x07d|+0x01b:'410'
	// ******
}

// STATE[STUB]
void stage_gbuffer::z_only_pass( )
{
	// LOCALS
	// vector< render_surface_instance* > m_visible_skeletal_models
	// vector< render_surface_instance* > m_visible_translucency_models
	// vector< render_surface_instance* > m_visible_models
	// vector< render_surface_instance* > m_visible_static_models
	// u32 								num_rendered
	// ******

	// FUNCTION BODY[0x62e060]: 49
	// <0x62e064>|0x004|+0x009:'418'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x62e06d>|0x00d|+0x03e:'424'
	// <0x62e0ab>|0x04b|+0x00e:'425'
	// <0x62e0b9>|0x059|+0x00e:'426'
	// <0x62e0c7>|0x067|+0x00e:'427'
	// <0>
	// <0x62e0d5>|0x075|+0x019:'429'
	// <0>
	// <0x62e0ee>|0x08e|+0x00c:'431'
	// <0x62e0fa>|0x09a|+0x00e:'432'
	// <0x62e108>|0x0a8|+0x00e:'433'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x62e116>|0x0b6|+0x054:'442'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x62e16a>|0x10a|+0x054:'450'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x62e1be>|0x15e|+0x05a:'458'
	// <0>
	// <1>
	// <2>
	// <0x62e218>|0x1b8|+0x016:'462'
	// <0x62e22e>|0x1ce|+0x012:'463'
	// <0x62e240>|0x1e0|+0x012:'464'
	// <0x62e252>|0x1f2|+0x02f:'465'
	// <0x62e281>|0x221|+0x008:'466'
	// ******
}

// STATE[STUB]
void stage_gbuffer::execute( )
{
	// LOCALS
	// vector< render_surface_instance* > m_visible_skeletal_models
	// u32 								num_rendered
	// vector< render_surface_instance* > m_visible_translucency_models
	// vector< render_surface_instance* > m_visible_models
	// vector< render_surface_instance* > m_visible_static_models
	// u32 								num_shader_lods
	// render_surface_instance* const* 	end_d
	// ******

	// CALL SITE INFO
	// <0x62e318> -> bool < unknown >() const
	// <0x62eb82> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x62e300]: 180
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x62e30e>|0x00e|+0x86d:'477'
	// <0>
	// <0x62eb7b>|0x87b|-0x843:'479'
	// <0>
	// <1>
	// <2>
	// <0x62e338>|0x038|+0x00b:'483'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x62e343>|0x043|+0x045:'493'
	// <0x62e388>|0x088|+0x00e:'494'
	// <0x62e396>|0x096|+0x00e:'495'
	// <0x62e3a4>|0x0a4|+0x00e:'496'
	// <0>
	// <1>
	// <0x62e3b2>|0x0b2|+0x0a5:'499'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x62e457>|0x157|+0x06b:'505'
	// <0>
	// <1>
	// <2>
	// <0x62e4c2>|0x1c2|+0x020:'509'
	// <0>
	// <0x62e4e2>|0x1e2|+0x00a:'511'
	// <0x62e4ec>|0x1ec|+0x00e:'512'
	// <0>
	// <1>
	// <0x62e4fa>|0x1fa|+0x01c:'515'
	// <0>
	// <0x62e516>|0x216|+0x282:'517'
	// <0x62e798>|0x498|-0x27a:'517'
	// <0>
	// <0x62e51e>|0x21e|+0x01c:'519'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x62e53a>|0x23a|+0x04c:'527'
	// <0>
	// <0x62e586>|0x286|+0x00e:'529'
	// <0x62e594>|0x294|+0x00e:'530'
	// <0x62e5a2>|0x2a2|+0x00e:'531'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x62e5b0>|0x2b0|+0x058:'540'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x62e608>|0x308|+0x058:'548'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x62e660>|0x360|+0x05c:'556'
	// <0>
	// <0x62e6bc>|0x3bc|+0x01f:'558'
	// <0x62e6db>|0x3db|+0x00f:'559'
	// <0x62e6ea>|0x3ea|+0x019:'560'
	// <0>
	// <0x62e703>|0x403|+0x016:'562'
	// <0x62e719>|0x419|+0x018:'563'
	// <0x62e731>|0x431|+0x019:'564'
	// <0>
	// <0x62e74a>|0x44a|+0x016:'566'
	// <0x62e760>|0x460|+0x01b:'567'
	// <0x62e77b>|0x47b|+0x021:'568'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x62e79c>|0x49c|+0x01f:'573'
	// <0>
	// <0x62e7bb>|0x4bb|+0x038:'575'
	// <0>
	// <0x62e7f3>|0x4f3|+0x008:'577'
	// <0>
	// <1>
	// <0x62e7fb>|0x4fb|+0x007:'580'
	// <0>
	// <0x62e802>|0x502|+0x022:'582'
	// <0x62e824>|0x524|+0x02a:'583'
	// <0x62e84e>|0x54e|+0x026:'584'
	// <0>
	// <1>
	// <0x62e874>|0x574|+0x074:'587'
	// <0>
	// <0x62e8e8>|0x5e8|+0x00a:'589'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x62e8f2>|0x5f2|+0x13b:'598'
	// <0x62ea2d>|0x72d|-0x0a9:'598'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x62e984>|0x684|+0x009:'604'
	// <0>
	// <0x62e98d>|0x68d|+0x021:'606'
	// <0x62e9ae>|0x6ae|+0x034:'607'
	// <0>
	// <0x62e9e2>|0x6e2|+0x009:'609'
	// <0>
	// <1>
	// <2>
	// <0x62e9eb>|0x6eb|+0x015:'613'
	// <0>
	// <0x62ea00>|0x700|+0x13a:'615'
	// <0x62eb3a>|0x83a|-0x132:'615'
	// <0>
	// <0x62ea08>|0x708|+0x002:'617'
	// <0>
	// <0x62ea0a>|0x70a|+0x006:'619'
	// <0>
	// <0x62ea10>|0x710|+0x024:'621'
	// <0x62ea34>|0x734|+0x00a:'621'
	// <0>
	// <1>
	// <0x62ea3e>|0x73e|+0x00d:'624'
	// <0>
	// <1>
	// <0x62ea4b>|0x74b|+0x008:'627'
	// <0>
	// <1>
	// <0x62ea53>|0x753|+0x00a:'630'
	// <0>
	// <1>
	// <0x62ea5d>|0x75d|+0x010:'633'
	// <0>
	// <0x62ea6d>|0x76d|+0x026:'635'
	// <0>
	// <0x62ea93>|0x793|+0x00a:'637'
	// <0>
	// <0x62ea9d>|0x79d|+0x008:'639'
	// <0>
	// <0x62eaa5>|0x7a5|+0x09d:'641'
	// <0>
	// <1>
	// <2>
	// <0x62eb42>|0x842|+0x007:'645'
	// <0x62eb49>|0x849|+0x01b:'646'
	// <0x62eb64>|0x864|+0x004:'647'
	// <0>
	// <0x62eb68>|0x868|+0x004:'649'
	// <0x62eb6c>|0x86c|+0x007:'650'
	// <0x62eb73>|0x873|+0x011:'651'
	// ******
}

// STATE[STUB]
void stage_gbuffer::render_grass( bool z_only )
{
	// FUNCTION BODY[0x62e010]: 2
	// <0x62e010>|0x000|+0x013:'655'
	// <0x62e023>|0x013|+0x02c:'656'
	// ******
}

// STATE[STUB]
void stage_gbuffer::render_particles( bool z_only )
{
	// LOCALS
	// vectora< particle::render_particle_emitter_instance* > emitters
	// particle::render_particle_emitter_instance* const* it
	// ******

	// CALL SITE INFO
	// <0x62d4aa> -> void < unknown >( float4x4 const&, vectora< particle::render_particle_emitter_instance* >& )
	// ******

	// FUNCTION BODY[0x62d460]: 50
	// <0>
	// <0x62d469>|0x009|+0x00f:'662'
	// <0>
	// <0x62d478>|0x018|+0x00c:'664'
	// <0>
	// <0x62d484>|0x024|+0x00a:'666'
	// <0>
	// <0x62d48e>|0x02e|+0x01e:'668'
	// <0>
	// <0x62d4ac>|0x04c|+0x327:'670'
	// <0x62d7d3>|0x373|-0x313:'670'
	// <0>
	// <0x62d4c0>|0x060|+0x002:'672'
	// <0x62d4c2>|0x062|+0x01e:'673'
	// <0>
	// <0x62d4e0>|0x080|+0x008:'675'
	// <0>
	// <1>
	// <0x62d4e8>|0x088|+0x00f:'678'
	// <0>
	// <0x62d4f7>|0x097|+0x01c:'680'
	// <0>
	// <0x62d513>|0x0b3|+0x035:'682'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x62d548>|0x0e8|+0x231:'690'
	// <0>
	// <0x62d779>|0x319|+0x014:'692'
	// <0>
	// <0x62d78d>|0x32d|+0x00f:'694'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x62d79c>|0x33c|+0x011:'702'
	// <0>
	// <0x62d7ad>|0x34d|+0x002:'704'
	// <0>
	// <0x62d7af>|0x34f|+0x028:'706'
	// <0>
	// <1>
	// <0x62d7d7>|0x377|+0x010:'709'
	// <0>
	// ******
}

// STATE[STUB]
void stage_gbuffer::render_speedtree( )
{
	// FUNCTION BODY[0x62ce20]: 56
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
	// <54>
	// <55>
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
	// 	vostok::render::effect_compiler::shader_cache_info*
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
	// 	vostok::render::texture_named_instance*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok
