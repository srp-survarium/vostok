////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\stage_particles.h"

namespace vostok {
namespace render {

// STATE[STUB]
void effect_resolve_particles::compile( effect_compiler& compiler, custom_config_value const& custom_config )
{
	// FUNCTION BODY[0x68950]: 13
	// <0>
	// <1>
	// <0x6895b>|0x00b|+0x009:'52'
	// <0x68964>|0x014|+0x02d:'53'
	// <0x68991>|0x041|+0x048:'54'
	// <0x689d9>|0x089|+0x00a:'55'
	// <0x689e3>|0x093|+0x00a:'56'
	// <0x689ed>|0x09d|+0x014:'57'
	// <0x68a01>|0x0b1|+0x011:'58'
	// <0x68a12>|0x0c2|+0x014:'59'
	// <0>
	// <0x68a26>|0x0d6|+0x006:'61'
	// <0x68a2c>|0x0dc|+0x005:'62'
	// ******
}

// STATE[STUB]
void `dynamic atexit destructor for 's_particle_render_mode''( )
{
	// FUNCTION BODY[0x7ee1e0]
	// ******
}

// STATE[STUB]
 particle_shader_constants::particle_shader_constants( )
{
	// FUNCTION BODY[0x641d80]: 11
	// <0x641d83>|0x003|+0x05f:'78'
	// <0x641de2>|0x062|+0x059:'79'
	// <0x641e3b>|0x0bb|+0x059:'80'
	// <0x641e94>|0x114|+0x059:'81'
	// <0>
	// <0x641eed>|0x16d|+0x059:'83'
	// <0>
	// <0x641f46>|0x1c6|+0x059:'85'
	// <0x641f9f>|0x21f|+0x059:'86'
	// <0x641ff8>|0x278|+0x059:'87'
	// <0>
	// ******
}

// STATE[STUB]
void particle_shader_constants::set_time( float time )
{
	// FUNCTION BODY[0x641d30]: 1
	// <0x641d30>|0x000|+0x04c:'93'
	// ******
}

// STATE[STUB]
void particle_shader_constants::set(
	const float3							arg_0 /* float3 up_vector */,
	const float3							arg_1 /* float3 right_vector */,
	const float3							arg_2 /* float3 view_location */,
	particle::enum_particle_locked_axis		locked_axis,
	particle::enum_particle_screen_alignment	screen_alignment
)
{
	// LOCALS
	// float3 							rotation_fixed_axis
	// float 							f_use_fixed_axis
	// float 							f_locked_no_ratate_axis_index
	// float 							f_use_align_by_dir
	// ******

	// FUNCTION BODY[0x641a70]: 40
	// <0x641a78>|0x008|+0x04e:'102'
	// <0x641ac6>|0x056|+0x042:'103'
	// <0x641b08>|0x098|+0x03f:'104'
	// <0>
	// <0x641b47>|0x0d7|+0x003:'106'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x641b4a>|0x0da|+0x038:'112'
	// <0>
	// <0x641b82>|0x112|+0x006:'114'
	// <0x641b88>|0x118|+0x003:'115'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x641b8b>|0x11b|+0x044:'122'
	// <0>
	// <0x641bcf>|0x15f|+0x018:'124'
	// <0>
	// <0x641be7>|0x177|-0x00c:'126'
	// <0>
	// <1>
	// <2>
	// <0x641bdb>|0x16b|+0x01a:'130'
	// <0>
	// <1>
	// <0x641bf5>|0x185|+0x002:'133'
	// <0>
	// <0x641bf7>|0x187|+0x013:'135'
	// <0>
	// <1>
	// <0x641c0a>|0x19a|+0x042:'138'
	// <0x641c4c>|0x1dc|+0x042:'139'
	// <0x641c8e>|0x21e|+0x047:'140'
	// <0x641cd5>|0x265|+0x04c:'141'
	// <0x641d21>|0x2b1|-0x006:'141'
	// <0x641d1b>|0x2ab|+0x00d:'142'
	// ******
}

// STATE[STUB]
 stage_particles::stage_particles( renderer* in_renderer, renderer_context* context )
{
	// FUNCTION BODY[0x642600]: 3
	// <0x642601>|0x001|+0x03a:'146'
	// <0>
	// <0x64263b>|0x03b|+0x00b:'148'
	// ******
}

// STATE[STUB]
 stage_particles::~stage_particles( )
{
	// FUNCTION BODY[0x642060]: 0
	// ******
}

// STATE[STUB]
bool stage_particles::is_effects_ready( ) const
{
	return false;

	// FUNCTION BODY[0x641a60]: 1
	// <0x641a60>|0x000|+0x008:'157'
	// ******
}

// STATE[STUB]
void stage_particles::execute( )
{
	// LOCALS
	// D3D11_VIEWPORT 					tmp_viewport
	// bool 							has_particles
	// vectora< particle::render_particle_emitter_instance* >& emitters
	// D3D11_VIEWPORT 					orig_viewport
	// particle::render_particle_emitter_instance* const* it
	// ******

	// CALL SITE INFO
	// <0x642148> -> bool < unknown >() const
	// <0x64232a> -> bool < unknown >() const
	// <0x6425f2> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x642130]: 98
	// <0x642142>|0x012|+0x4a8:'162'
	// <0>
	// <0x6425ea>|0x4ba|-0x48d:'164'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x64215d>|0x02d|+0x003:'172'
	// <0>
	// <0x642160>|0x030|+0x00e:'174'
	// <0>
	// <0x64216e>|0x03e|+0x019:'176'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x642187>|0x057|+0x009:'181'
	// <0>
	// <1>
	// <0x642190>|0x060|+0x003:'184'
	// <0>
	// <0x642193>|0x063|+0x047:'186'
	// <0x6421da>|0x0aa|+0x03b:'187'
	// <0x642215>|0x0e5|+0x003:'188'
	// <0>
	// <0x642218>|0x0e8|+0x032:'190'
	// <0>
	// <0x64224a>|0x11a|+0x314:'192'
	// <0x64255e>|0x42e|-0x283:'192'
	// <0>
	// <1>
	// <0x6422db>|0x1ab|+0x01b:'195'
	// <0>
	// <1>
	// <0x6422f6>|0x1c6|+0x009:'198'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6422ff>|0x1cf|+0x022:'204'
	// <0>
	// <0x642321>|0x1f1|+0x002:'206'
	// <0>
	// <0x642323>|0x1f3|+0x011:'208'
	// <0>
	// <1>
	// <0x642334>|0x204|+0x022:'211'
	// <0>
	// <0x642356>|0x226|+0x008:'213'
	// <0>
	// <1>
	// <0x64235e>|0x22e|+0x00f:'216'
	// <0>
	// <0x64236d>|0x23d|+0x01c:'218'
	// <0>
	// <0x642389>|0x259|+0x012:'220'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x64239b>|0x26b|+0x17b:'228'
	// <0x642516>|0x3e6|+0x014:'229'
	// <0>
	// <0x64252a>|0x3fa|+0x00f:'231'
	// <0>
	// <0x642539>|0x409|+0x013:'233'
	// <0>
	// <0x64254c>|0x41c|+0x00b:'235'
	// <0x642557>|0x427|-0x006:'236'
	// <0>
	// <1>
	// <2>
	// <0x642551>|0x421|+0x00b:'240'
	// <0x64255c>|0x42c|+0x009:'240'
	// <0>
	// <0x642565>|0x435|+0x023:'242'
	// <0>
	// <1>
	// <2>
	// <0x642588>|0x458|+0x01e:'246'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x6425a6>|0x476|+0x014:'256'
	// <0>
	// <0x6425ba>|0x48a|+0x00d:'258'
	// <0x6425c7>|0x497|-0x448:'259'
	// <0x64217f>|0x04f|+0x463:'260'
	// <0x6425e2>|0x4b2|+0x012:'260'
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
