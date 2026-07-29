////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\statistics.h"

namespace vostok {
namespace render {

// STATE[STUB]
void `dynamic atexit destructor for 's_short_statistics''( )
{
	// FUNCTION BODY[0x7ee350]
	// ******
}

// STATE[STUB]
explicit statistics_group::statistics_group( pcstr group_name )
{
	// FUNCTION BODY[0x638c60]: 2
	// <0x638c87>|0x027|+0x00d:'36'
	// <0x638c94>|0x034|+0x002:'37'
	// ******
}

// STATE[STUB]
 statistics_group::~statistics_group( )
{
	// FUNCTION BODY[0x6387d0]: 1
	// <0>
	// ******
}

// STATE[STUB]
void statistics_group::start( )
{
	// CALL SITE INFO
	// <0x6387ae> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x6387a0]: 4
	// <0x6387a1>|0x001|+0x006:'48'
	// <0>
	// <0x6387a7>|0x007|+0x014:'50'
	// <0>
	// ******
}

// STATE[STUB]
void push_point(
	vectora< ui::vertex >&		out_vertices,
	u32							index,
	float						x,
	float						y,
	float						z,
	u32							c,
	float						u,
	float						v
)
{
	// FUNCTION BODY[0x638870]: 4
	// <0x638870>|0x000|+0x00e:'90'
	// <0>
	// <1>
	// <0x63887e>|0x00e|+0x041:'93'
	// ******
}

// STATE[STUB]
void make_ui_vertices(
	vectora< ui::vertex >&		out_vertices,
	pcstr						in_text,
	ui::font const&				in_font,
	float2 const&				in_position,
	math::color const&			in_color,
	math::color const&			in_selection_color,
	u32							max_line_width,
	bool						is_multiline,
	u32							start_selection_index,
	u32							end_selection_index
)
{
	// LOCALS
	// float2 							pos
	// pcstr 							ch
	// float 							curr_word_len
	// const float 						height
	// pcstr 							next_word
	// float2 							pos_rt
	// u32 								symb_count
	// float 							height_ts
	// u32 								i
	// float3 							uv
	// ******

	// CALL SITE INFO
	// <0x638962> -> float const& < unknown >() const
	// <0x638975> -> float < unknown >() const
	// <0x6389a5> -> void < unknown >( pcstr, float&, pcstr& ) const
	// <0x638a0b> -> float3 < unknown >( u8 const& ) const
	// <0x638a20> -> float3 const& < unknown >( u8 const& ) const
	// <0x638a54> -> void < unknown >( pcstr, float&, pcstr& ) const
	// <0x638aa2> -> void < unknown >( pcstr, float&, pcstr& ) const
	// ******

	// FUNCTION BODY[0x638910]: 50
	// <0>
	// <1>
	// <0x638913>|0x003|+0x009:'110'
	// <0>
	// <0x63891c>|0x00c|+0x037:'112'
	// <0>
	// <1>
	// <2>
	// <0x638953>|0x043|+0x015:'116'
	// <0x638968>|0x058|+0x013:'117'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x63897b>|0x06b|+0x018:'122'
	// <0x638993>|0x083|+0x03d:'123'
	// <0>
	// <0x6389d0>|0x0c0|-0x029:'125'
	// <0>
	// <0x6389a7>|0x097|+0x031:'127'
	// <0>
	// <0x6389d8>|0x0c8|+0x01a:'129'
	// <0>
	// <0x6389f2>|0x0e2|+0x01b:'131'
	// <0x638a0d>|0x0fd|+0x015:'132'
	// <0>
	// <0x638a22>|0x112|+0x01c:'134'
	// <0>
	// <0x638a3e>|0x12e|+0x018:'136'
	// <0>
	// <0x638a56>|0x146|+0x029:'138'
	// <0>
	// <1>
	// <0x638a7f>|0x16f|+0x011:'141'
	// <0>
	// <0x638a90>|0x180|+0x014:'143'
	// <0x638aa4>|0x194|+0x01a:'144'
	// <0>
	// <0x638abe>|0x1ae|+0x00b:'146'
	// <0x638ac9>|0x1b9|+0x01d:'147'
	// <0>
	// <1>
	// <2>
	// <0x638ae6>|0x1d6|+0x05e:'151'
	// <0x638b44>|0x234|+0x044:'152'
	// <0x638b88>|0x278|+0x05a:'153'
	// <0x638be2>|0x2d2|+0x04a:'154'
	// <0>
	// <0x638c2c>|0x31c|+0x021:'156'
	// <0>
	// ******
}

// STATE[STUB]
 statistics_base::statistics_base( statistics_group* group, pcstr name )
{
	// FUNCTION BODY[0x6388c0]: 7
	// <0>
	// <0x6388e8>|0x028|+0x004:'265'
	// <0>
	// <0x6388ec>|0x02c|+0x012:'267'
	// <0x6388fe>|0x03e|+0x008:'268'
	// <0x638906>|0x046|+0x002:'269'
	// <0>
	// ******
}

// STATE[STUB]
 statistics_base::~statistics_base( )
{
	// FUNCTION BODY[0x6387c0]: 1
	// <0>
	// ******
}

// STATE[STUB]
void statistics_float::print( fs_new::virtual_path_string& out_result )
{
	// FUNCTION BODY[0x638d20]: 1
	// <0x638d26>|0x006|+0x035:'297'
	// ******
}

// STATE[STUB]
u32 get_num_digits( u32 v )
{
	return 0;

	// FUNCTION BODY[0x638830]: 7
	// <0x638834>|0x004|+0x015:'303'
	// <0>
	// <0x638849>|0x019|+0x002:'305'
	// <0>
	// <0x63884b>|0x01b|+0x01f:'307'
	// <0>
	// <0x63886a>|0x03a|+0x004:'309'
	// ******
}

// STATE[STUB]
fixed_string< 260 > u32_to_string( u32 v )
{
	// FUNCTION BODY[0x638cf0]: 3
	// <0x638cf0>|0x000|+0x003:'314'
	// <0x638cf3>|0x003|+0x023:'315'
	// <0x638d16>|0x026|+0x002:'316'
	// ******
}

// STATE[STUB]
void statistics_int::print( fs_new::virtual_path_string& out_result )
{
	// LOCALS
	// fixed_string< 260 > 				rule
	// ******

	// FUNCTION BODY[0x639520]: 16
	// <0x639530>|0x010|+0x02e:'321'
	// <0x63955e>|0x03e|+0x02d:'322'
	// <0x63958b>|0x06b|+0x024:'323'
	// <0>
	// <0x6395af>|0x08f|+0x045:'325'
	// <0x6395f4>|0x0d4|+0x026:'326'
	// <0x63961a>|0x0fa|+0x071:'327'
	// <0x63968b>|0x16b|+0x023:'328'
	// <0x6396ae>|0x18e|+0x023:'329'
	// <0x6396d1>|0x1b1|+0x071:'330'
	// <0x639742>|0x222|+0x028:'331'
	// <0x63976a>|0x24a|+0x023:'332'
	// <0x63978d>|0x26d|+0x071:'333'
	// <0x6397fe>|0x2de|+0x023:'334'
	// <0>
	// <0x639821>|0x301|+0x02a:'336'
	// ******
}

// STATE[STUB]
void statistics_cpu_gpu::print( fs_new::virtual_path_string& out_result )
{
	// FUNCTION BODY[0x638ca0]: 1
	// <0x638ca6>|0x006|+0x03f:'342'
	// ******
}

// STATE[STUB]
void statistics_cpu_gpu::start( )
{
	// CALL SITE INFO
	// <0x638782> -> void < unknown >()
	// <0x638794> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x638770]: 1
	// <0x638773>|0x003|+0x011:'347'
	// ******
}

// STATE[STUB]
explicit gbuffer_statistics_group::gbuffer_statistics_group( pcstr group_name )
{
	// FUNCTION BODY[0x63a4b0]
	// ******
}

// STATE[STUB]
explicit visibility_statistics_group::visibility_statistics_group( pcstr group_name )
{
	// FUNCTION BODY[0x6392d0]
	// ******
}

// STATE[STUB]
explicit lights_statistics_group::lights_statistics_group( pcstr group_name )
{
	// FUNCTION BODY[0x63a390]
	// ******
}

// STATE[STUB]
explicit particles_statistics_group::particles_statistics_group( pcstr group_name )
{
	// FUNCTION BODY[0x63a210]
	// ******
}

// STATE[STUB]
explicit cascaded_sun_shadow_statistics_group::cascaded_sun_shadow_statistics_group( pcstr group_name )
{
	// FUNCTION BODY[0x63a000]
	// ******
}

// STATE[STUB]
explicit postprocess_statistics_group::postprocess_statistics_group( pcstr group_name )
{
	// FUNCTION BODY[0x639f70]
	// ******
}

// STATE[STUB]
explicit forward_stage_statistics_group::forward_stage_statistics_group( pcstr group_name )
{
	// FUNCTION BODY[0x639ee0]
	// ******
}

// STATE[STUB]
explicit general_statistics_group::general_statistics_group( pcstr group_name )
{
	// FUNCTION BODY[0x639dd0]
	// ******
}

// STATE[STUB]
explicit speedtree_statistics_group::speedtree_statistics_group( pcstr group_name )
{
	// FUNCTION BODY[0x639d10]
	// ******
}

// STATE[STUB]
explicit forward_decals_statistics_group::forward_decals_statistics_group( pcstr group_name )
{
	// FUNCTION BODY[0x639240]
	// ******
}

// STATE[STUB]
explicit deferred_decals_statistics_group::deferred_decals_statistics_group( pcstr group_name )
{
	// FUNCTION BODY[0x6391b0]
	// ******
}

// STATE[STUB]
explicit distortion_pass_statistics_group::distortion_pass_statistics_group( pcstr group_name )
{
	// FUNCTION BODY[0x639c30]
	// ******
}

// STATE[STUB]
explicit ssao_statistics_group::ssao_statistics_group( pcstr group_name )
{
	// FUNCTION BODY[0x639b50]
	// ******
}

// STATE[STUB]
explicit sky_statistics_group::sky_statistics_group( pcstr group_name )
{
	// FUNCTION BODY[0x639ac0]
	// ******
}

// STATE[STUB]
explicit lpv_statistics_group::lpv_statistics_group( pcstr group_name )
{
	// FUNCTION BODY[0x639860]
	// ******
}

// STATE[STUB]
explicit grass_statistics_group::grass_statistics_group( pcstr group_name )
{
	// FUNCTION BODY[0x639120]
	// ******
}

// STATE[STUB]
explicit debug_statistics_group::debug_statistics_group( pcstr group_name )
{
	// FUNCTION BODY[0x638f10]
	// ******
}

// STATE[STUB]
 statistics::statistics( )
{
	// FUNCTION BODY[0x63a590]
	// ******
}

// STATE[STUB]
void statistics::start( )
{
	// CALL SITE INFO
	// <0x6387fd> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x6387e0]: 9
	// <0x6387e2>|0x002|+0x027:'623'
	// <0x638809>|0x029|-0x019:'623'
	// <0>
	// <0x6387f0>|0x010|+0x024:'625'
	// <0>
	// <1>
	// <0x638814>|0x034|+0x009:'628'
	// <0x63881d>|0x03d|+0x003:'629'
	// <0x638820>|0x040|+0x003:'630'
	// <0x638823>|0x043|+0x004:'631'
	// ******
}

// STATE[STUB]
 statistics::~statistics( )
{
	// FUNCTION BODY[0x638d70]: 1
	// <0>
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

	// typedef
	// 	vostok::render::ui::vertex*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok
