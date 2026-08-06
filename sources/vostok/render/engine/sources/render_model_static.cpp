#include "pch.h"
// claude@NOTE: legacy-harvest disposition: the remaining template helpers and LPV/shadow-pass/LOD/batched-vertex/sectors machinery below have no legacy ancestor (new-in-target; legacy twin keeps only load/append_surface/get_surfaces) - matcher-phase work.
#include "help_math.h"
#include "batched_vertex_source.h"
#include <vostok/render/culling/possible_sectors_holder.h>
#include "render_model_static.h"

namespace vostok {
namespace render {

static_render_surface::static_render_surface( bool colored )
{
	// claude@NOTE: legacy ctor had no parameter and always set static_mesh_vertex_input_type;
	// colored selector inferred from the canonical bool param + vertex_input_type enum.
	m_vertex_input_type	= colored ? static_mesh_vertex_colored_input_type : static_mesh_vertex_input_type;

	// FUNCTION BODY[0x631d50]: 1
	// <0x631d57>|0x007|+0x013:'47'
	// ******
}

// STATE[STUB]
template < u32 Size >
bool read_diffuse_colors(
	material_effects_instance_ptr		m_materail_effects_instance,
	math::color (&results)[Size][Size]
)
{
	// LOCALS
	// res_texture_ptr 					tex
	// render_target_ptr 				rt
	// u32 								row_pitch
	// ******

	// CALL SITE INFO
	// <0x631e8f> -> void < unknown >()
	// <0x631ee9> -> void < unknown >()
	// ******

	return false;

	// FUNCTION BODY[0x631d70]: 67
	// <0x631d70>|0x000|+0x00d:'53'
	// <0x631d7d>|0x00d|+0x006:'54'
	// <0>
	// <1>
	// <2>
	// <0x631d83>|0x013|+0x00d:'58'
	// <0x631d90>|0x020|+0x006:'59'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x631d96>|0x026|+0x036:'68'
	// <0x631dcc>|0x05c|+0x02f:'69'
	// <0>
	// <0x631dfb>|0x08b|+0x025:'71'
	// <0x631e20>|0x0b0|+0x05b:'72'
	// <0>
	// <0x631e7b>|0x10b|+0x016:'74'
	// <0x631e91>|0x121|+0x00b:'75'
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
	// <0x631e9c>|0x12c|+0x02a:'87'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x631ec6>|0x156|+0x00e:'94'
	// <0>
	// <0x631ed4>|0x164|+0x017:'96'
	// <0x631eeb>|0x17b|+0x00b:'97'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x631ef6>|0x186|+0x00c:'102'
	// <0>
	// <0x631f02>|0x192|+0x004:'104'
	// <0x631f06>|0x196|+0x066:'105'
	// <0>
	// <0x631f6c>|0x1fc|+0x009:'107'
	// <0>
	// <1>
	// <2>
	// <0x631f75>|0x205|+0x00b:'111'
	// <0x631f80>|0x210|+0x011:'112'
	// <0>
	// <0x631f91>|0x221|+0x00c:'114'
	// <0>
	// <1>
	// <0x631f9d>|0x22d|+0x01c:'117'
	// <0>
	// <0x631fb9>|0x249|-0x055:'119'
	// <0x631f64>|0x1f4|+0x0b0:'120'
	// ******
}

template bool read_diffuse_colors< 64 >(
	material_effects_instance_ptr,
	math::color (&)[64][64]
);

// claude@NOTE: the target defines its own file-local `static float frac( float f )` here
// (lines 123-125, 0x630150) - NOT a COMDAT copy of help_math.h's: this one is
// `math::abs( f ) - math::abs( math::floor( f ) )`, i.e. it calls the int-returning
// math::floor. Restore it with interpolated_color<64>, its only caller.

// STATE[STUB]
template < u32 Size >
math::color interpolated_color(
	math::color (&color_grid)[Size][Size],
	const float2			arg_1 /* float2 uv */
)
{
	// LOCALS
	// const float 						y
	// ******

	return vostok::math::color();

	// FUNCTION BODY[0x6301a0]: 27
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
	// <0x6301a1>|0x001|+0x033:'145'
	// <0x6301d4>|0x034|+0x043:'146'
	// <0>
	// <1>
	// <0x630217>|0x077|+0x01f:'149'
	// <0>
	// <0x630236>|0x096|+0x02d:'151'
	// <0x630263>|0x0c3|+0x02b:'152'
	// <0x63028e>|0x0ee|-0x017:'152'
	// <0>
	// <0x630277>|0x0d7|+0x021:'154'
	// <0x630298>|0x0f8|-0x00f:'154'
	// <0>
	// <0x630289>|0x0e9|+0x01f:'156'
	// <0x6302a8>|0x108|-0x01c:'156'
	// <0x63028c>|0x0ec|+0x01f:'157'
	// ******
}

template math::color interpolated_color< 64 >(
	math::color (&)[64][64],
	const float2
);

// STATE[STUB]
template < typename StaticVertex, typename OptimizedVertex >
void create_shadow_pass_geometry_type(
	render_geometry&					in_render_geometry,
	pcbyte								data,
	const u32							num_vertices,
	const u32							stride,
	D3D11_INPUT_ELEMENT_DESC const*		layout,
	const u32							num_decl_elements
)
{
	// LOCALS
	// static_render_surface::create_shadow_pass_geometry::__l2::opt_static_vertex* temp_data
	// ******

	// FUNCTION BODY[0x631630]: 84
	// <0>
	// <1>
	// <2>
	// <0x631639>|0x009|+0x009:'251'
	// <0x631642>|0x012|+0x014:'252'
	// <0>
	// <1>
	// <0x631656>|0x026|+0x00a:'255'
	// <0>
	// <1>
	// <0x631660>|0x030|+0x01f:'258'
	// <0x63167f>|0x04f|+0x00e:'259'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x63168d>|0x05d|+0x01e:'267'
	// <0>
	// <0x6316ab>|0x07b|+0x015:'269'
	// <0x6316c0>|0x090|+0x01f:'270'
	// <0>
	// <0x6316df>|0x0af|+0x036:'272'
	// <0>
	// <0x631715>|0x0e5|+0x01a:'274'
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
	// <56>
	// ******
}

// STATE[STUB]
template < typename StaticVertex >
void fill_static_lpv_vertex_color(
	batched_geometry_interface*			in_out_lpv_geometry,
	render_geometry&					in_render_geometry,
	material_effects_instance_ptr&		in_materail_effects_instance,
	float4x4 const&						in_transform
)
{
	// LOCALS
	// const u32 						num_vertices
	// u16* 							indices_temp_data
	// untyped_buffer_ptr 				vb
	// lpv_vertex* 						lpv_temp_data
	// untyped_buffer_ptr 				lpv_vb
	// batched_vertex_source* 			static_temp_data
	// const D3D11_INPUT_ELEMENT_DESC[3] lpv_layout
	// static_render_surface::fill_lpv_vertex_color::__l2::static_vertex0* temp_data
	// math::color[64][64] 				color_grid
	// untyped_buffer_ptr 				temp_vb
	// untyped_buffer_ptr 				temp_ib
	// math::color 						value
	// ******

	// CALL SITE INFO
	// <0x632728> -> void < unknown >()
	// <0x6328ef> -> void < unknown >()
	// <0x632976> -> void < unknown >( batched_vertex_source const*, const u32, u16 const*, const u32, float4x4 const&, material_effects_instance_ptr const& )
	// ******

	// FUNCTION BODY[0x632600]: 109
	// <0x63260e>|0x00e|+0x011:'340'
	// <0>
	// <0x63261f>|0x01f|+0x02f:'342'
	// <0>
	// <0x63264e>|0x04e|+0x008:'344'
	// <0>
	// <1>
	// <0x632656>|0x056|+0x020:'347'
	// <0>
	// <0x632676>|0x076|+0x00c:'349'
	// <0>
	// <0x632682>|0x082|+0x012:'351'
	// <0>
	// <1>
	// <0x632694>|0x094|+0x01d:'354'
	// <0>
	// <1>
	// <0x6326b1>|0x0b1|+0x017:'357'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6326c8>|0x0c8|+0x02c:'366'
	// <0>
	// <0x6326f4>|0x0f4|+0x01f:'368'
	// <0x632713>|0x113|+0x017:'369'
	// <0>
	// <0x63272a>|0x12a|+0x023:'371'
	// <0>
	// <1>
	// <0x63274d>|0x14d|+0x014:'374'
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
	// <0x632761>|0x161|+0x0b5:'389'
	// <0x632816>|0x216|+0x003:'390'
	// <0x632819>|0x219|+0x02f:'391'
	// <0>
	// <1>
	// <0x632848>|0x248|+0x019:'394'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x632861>|0x261|+0x00f:'400'
	// <0>
	// <0x632870>|0x270|+0x010:'402'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x632880>|0x280|+0x02b:'411'
	// <0x6328ab>|0x2ab|+0x02f:'412'
	// <0x6328da>|0x2da|+0x017:'413'
	// <0>
	// <0x6328f1>|0x2f1|+0x027:'415'
	// <0x632918>|0x318|+0x010:'416'
	// <0x632928>|0x328|+0x022:'417'
	// <0>
	// <1>
	// <2>
	// <0x63294a>|0x34a|+0x00b:'421'
	// <0x632955>|0x355|+0x049:'422'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x63299e>|0x39e|+0x023:'431'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6329c1>|0x3c1|+0x098:'440'
	// <0x632a59>|0x459|+0x020:'441'
	// <0>
	// <0x632a79>|0x479|+0x038:'443'
	// <0>
	// <0x632ab1>|0x4b1|+0x021:'445'
	// <0x632ad2>|0x4d2|+0x01c:'446'
	// <0x632aee>|0x4ee|+0x01c:'447'
	// <0x632b0a>|0x50a|+0x017:'448'
	// ******
}

// STATE[STUB]
void static_render_surface::fill_lpv_vertex_color( batched_geometry_interface* in_out_geometry, float4x4 const& transform )
{
	// FUNCTION BODY[0x632bb0]: 16
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
	// <0x632bb6>|0x006|+0x008:'465'
	// <0x632bbe>|0x00e|+0x020:'466'
	// <0x632bde>|0x02e|-0x009:'467'
	// <0x632bd5>|0x025|+0x00e:'468'
	// <0x632be3>|0x033|-0x00b:'468'
	// <0x632bd8>|0x028|+0x025:'469'
	// ******
}

// STATE[STUB]
template < typename StaticVertex >
void fill_source_vertices_impl(
	render_geometry&					in_render_geometry,
	vector< batched_vertex_source >&	out_vertices,
	vector< u16 >&						out_indices
)
{
	// LOCALS
	// untyped_buffer_ptr 				vb
	// untyped_buffer_ptr 				temp_vb
	// u16* 							indices_temp_data
	// ******

	// CALL SITE INFO
	// <0x630dc7> -> void < unknown >()
	// <0x630f1c> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x630ce0]: 63
	// <0x630ce3>|0x003|+0x01b:'552'
	// <0>
	// <0x630cfe>|0x01e|+0x013:'554'
	// <0>
	// <0x630d11>|0x031|+0x00e:'556'
	// <0>
	// <1>
	// <0x630d1f>|0x03f|+0x00e:'559'
	// <0x630d2d>|0x04d|+0x036:'560'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x630d63>|0x083|+0x02c:'568'
	// <0>
	// <0x630d8f>|0x0af|+0x023:'570'
	// <0x630db2>|0x0d2|+0x017:'571'
	// <0>
	// <0x630dc9>|0x0e9|+0x023:'573'
	// <0>
	// <1>
	// <0x630dec>|0x10c|+0x006:'576'
	// <0>
	// <0x630df2>|0x112|+0x004:'578'
	// <0>
	// <1>
	// <0x630df6>|0x116|+0x03b:'581'
	// <0x630e31>|0x151|+0x00e:'582'
	// <0x630e3f>|0x15f|+0x006:'583'
	// <0x630e45>|0x165|+0x005:'584'
	// <0x630e4a>|0x16a|+0x003:'585'
	// <0x630e4d>|0x16d|+0x017:'586'
	// <0>
	// <1>
	// <0x630e64>|0x184|+0x019:'589'
	// <0>
	// <0x630e7d>|0x19d|+0x00c:'591'
	// <0>
	// <0x630e89>|0x1a9|+0x010:'593'
	// <0>
	// <0x630e99>|0x1b9|+0x00f:'595'
	// <0x630ea8>|0x1c8|+0x016:'596'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x630ebe>|0x1de|+0x022:'604'
	// <0>
	// <0x630ee0>|0x200|+0x028:'606'
	// <0x630f08>|0x228|+0x016:'607'
	// <0>
	// <0x630f1e>|0x23e|+0x022:'609'
	// <0x630f40>|0x260|+0x00f:'610'
	// <0x630f4f>|0x26f|+0x01e:'611'
	// <0>
	// <0x630f6d>|0x28d|+0x01e:'613'
	// <0x630f8b>|0x2ab|+0x014:'614'
	// ******
}

// STATE[STUB]
void fill_source_vertices(
	const enum_vertex_input_type		arg_0 /* enum_vertex_input_type in_vertex_input_type */,
	render_geometry&					in_render_geometry,
	vector< batched_vertex_source >&	out_vertices,
	vector< u16 >&						out_indices
)
{
	// FUNCTION BODY[0x631090]: 16
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
	// <0x631094>|0x004|+0x005:'634'
	// <0x631099>|0x009|+0x00c:'635'
	// <0x6310a5>|0x015|-0x004:'636'
	// <0x6310a1>|0x011|+0x009:'637'
	// <0x6310aa>|0x01a|-0x006:'637'
	// <0x6310a4>|0x014|+0x011:'638'
	// ******
}

// STATE[STUB]
void static_render_surface::add_shadow_vertices( batched_geometry_interface* in_out_geometry, float4x4 const& transform )
{
	// LOCALS
	// vector< u16 > 					out_indices
	// vector< batched_vertex_source > 	out_vertices
	// ******

	// CALL SITE INFO
	// <0x6313cd> -> void < unknown >( batched_vertex_source const*, const u32, u16 const*, const u32, float4x4 const&, material_effects_instance_ptr const& )
	// ******

	// FUNCTION BODY[0x6312f0]: 21
	// <0>
	// <1>
	// <2>
	// <0x6312fe>|0x00e|+0x064:'646'
	// <0>
	// <0x631362>|0x072|+0x0bc:'648'
	// <0x63141e>|0x12e|-0x093:'649'
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
	// <0x63138b>|0x09b|+0x06f:'663'
	// <0x6313fa>|0x10a|+0x059:'664'
	// ******
}

// STATE[STUB]
void static_render_surface::create_shadow_pass_geometry( pcbyte data, const u32 num_vertices, const u32 stride )
{
	// LOCALS
	// const D3D11_INPUT_ELEMENT_DESC[4] colored_layout
	// const D3D11_INPUT_ELEMENT_DESC[3] layout
	// ******

	// FUNCTION BODY[0x6317f0]: 55
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
	// <0x6317f6>|0x006|+0x01e:'708'
	// <0x631814>|0x024|+0x010:'709'
	// <0x631824>|0x034|+0x010:'710'
	// <0>
	// <1>
	// <2>
	// <0x631834>|0x044|+0x014:'714'
	// <0x631848>|0x058|+0x019:'715'
	// <0x631861>|0x071|+0x01c:'716'
	// <0x63187d>|0x08d|+0x01c:'717'
	// <0>
	// <0x631899>|0x0a9|+0x0a8:'719'
	// <0x631941>|0x151|+0x02c:'720'
	// <0x63196d>|0x17d|+0x005:'721'
	// <0x631972>|0x182|-0x010:'722'
	// <0x631962>|0x172|+0x032:'723'
	// ******
}

// STATE[STUB]
// claude@NOTE: legacy body needs D3DXGetDeclLength/D3DXGetDeclVertexSize (d3dx9mesh) which are
// not available in this tree - skipped; legacy seed kept in temp/render_legacy.
void static_render_surface::load( configs::binary_config_value const& properties, memory::chunk_reader& chunk )
{
	// LOCALS
	// memory::reader 					reader
	// untyped_buffer_ptr 				vb
	// static_render_surface::load::__l5::static_vertex* vertices
	// res_declaration_ptr 				decl
	// u32 								vStride
	// _D3DVERTEXELEMENT9 const* 		vFormat
	// vector< D3D11_INPUT_ELEMENT_DESC > decl_code
	// untyped_buffer_ptr 				ib
	// ******

	// TYPEDEFS
	// typedef
	// 	static_render_surface::load::__l5::static_vertex
	// 	static_render_surface::load::__l5::static_vertex;

	// ******

	// FUNCTION BODY[0x6319a0]: 68
	// <0x6319a9>|0x009|+0x01a:'769'
	// <0>
	// <1>
	// <2>
	// <0x6319c3>|0x023|+0x014:'773'
	// <0>
	// <0x6319d7>|0x037|+0x00b:'775'
	// <0>
	// <1>
	// <0x6319e2>|0x042|+0x088:'778'
	// <0>
	// <0x631a6a>|0x0ca|+0x03c:'780'
	// <0x631aa6>|0x106|+0x004:'781'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x631aaa>|0x10a|+0x00c:'786'
	// <0>
	// <0x631ab6>|0x116|+0x00b:'788'
	// <0>
	// <0x631ac1>|0x121|+0x0d0:'790'
	// <0>
	// <1>
	// <0x631b91>|0x1f1|+0x00e:'793'
	// <0x631b9f>|0x1ff|+0x02f:'794'
	// <0>
	// <1>
	// <2>
	// <0x631bce>|0x22e|+0x014:'798'
	// <0x631be2>|0x242|+0x002:'799'
	// <0x631be4>|0x244|+0x007:'800'
	// <0>
	// <0x631beb>|0x24b|+0x033:'802'
	// <0>
	// <0x631c1e>|0x27e|+0x042:'804'
	// <0>
	// <1>
	// <0x631c60>|0x2c0|+0x014:'807'
	// <0>
	// <1>
	// <0x631c74>|0x2d4|+0x013:'810'
	// <0>
	// <0x631c87>|0x2e7|+0x012:'812'
	// <0>
	// <0x631c99>|0x2f9|+0x004:'814'
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
	// <0x631c9d>|0x2fd|+0x01a:'826'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x631cb7>|0x317|+0x01b:'836'
	// ******
}

static_render_model_instance::static_render_model_instance( )
:m_surface_instances( NULL ),
m_instances_count	( 0 )
{
	// FUNCTION BODY[0x6311a0]
	// ******
}

static_render_model_instance::~static_render_model_instance( )
{
	DELETE_ARRAY		( m_surface_instances );

	// FUNCTION BODY[0x631120]: 2
	// <0x631124>|0x004|+0x019:'846'
	// <0x63113d>|0x01d|+0x01e:'847'
	// ******
}

void static_render_model_instance::assign_original( static_render_model_ptr v )
{
	m_original			= v;
	m_instances_count	= m_original->m_childs_count;
	m_surface_instances = NEW_ARRAY( render_surface_instance, m_instances_count );

	for( u8 i = 0; i < m_instances_count; ++i )
	{
		render_surface_instance& info = m_surface_instances[i];
		info.m_parent				= this;
		info.m_render_surface		= m_original->m_childs[i];
		info.m_transform			= &m_transform;
		// claude@NOTE: legacy set info.m_visible = true; canonical replaced the bool with m_flags.
		info.m_flags				= visible_flag;
	}

	// FUNCTION BODY[0x631200]: 12
	// <0x631200>|0x000|+0x046:'852'
	// <0x631246>|0x046|+0x012:'853'
	// <0x631258>|0x058|+0x00e:'854'
	// <0>
	// <0x631266>|0x066|+0x01a:'856'
	// <0>
	// <0x631280>|0x080|+0x016:'858'
	// <0x631296>|0x096|+0x003:'859'
	// <0x631299>|0x099|+0x012:'860'
	// <0x6312ab>|0x0ab|+0x003:'861'
	// <0x6312ae>|0x0ae|+0x015:'862'
	// <0>
	// ******
}

// STATE[STUB]
void static_render_model_instance::set_lod_params(
	u8			type,
	bool		use_default,
	float		p0,
	float		p1,
	float		p2
)
{
	// FUNCTION BODY[0x630110]: 6
	// <0x630110>|0x000|+0x00c:'873'
	// <0>
	// <1>
	// <0x63011c>|0x00c|+0x013:'876'
	// <0x63012f>|0x01f|+0x00b:'877'
	// <0x63013a>|0x02a|+0x011:'878'
	// ******
}

// STATE[STUB]
u8 static_render_model_instance::select_lod( float4x4 const& mat_vp, float3 const& view_pos )
{
	// LOCALS
	// float 							params_mult
	// math::aabb 						owner_aabb
	// float 							distance
	// float3 							pt_max
	// float3[8] 						vertices
	// float3 							pt_min
	// ******

	return 0;

	// FUNCTION BODY[0x630320]: 69
	// <0x630326>|0x006|+0x014:'883'
	// <0>
	// <0x63033a>|0x01a|+0x005:'885'
	// <0x63033f>|0x01f|+0x00a:'886'
	// <0>
	// <0x630349>|0x029|+0x006:'888'
	// <0x63034f>|0x02f|+0x00a:'889'
	// <0>
	// <0x630359>|0x039|+0x019:'891'
	// <0>
	// <1>
	// <2>
	// <0x630372>|0x052|+0x01d:'895'
	// <0>
	// <0x63038f>|0x06f|+0x00e:'897'
	// <0>
	// <1>
	// <0x63039d>|0x07d|+0x024:'900'
	// <0x6303c1>|0x0a1|+0x015:'901'
	// <0>
	// <0x6303d6>|0x0b6|+0x2b9:'903'
	// <0>
	// <1>
	// <2>
	// <0x63068f>|0x36f|+0x081:'907'
	// <0x630710>|0x3f0|+0x00d:'908'
	// <0x63071d>|0x3fd|+0x006:'909'
	// <0>
	// <0x630723>|0x403|+0x018:'911'
	// <0x63073b>|0x41b|+0x006:'912'
	// <0>
	// <0x630741>|0x421|+0x00c:'914'
	// <0x63074d>|0x42d|+0x004:'915'
	// <0>
	// <0x630751>|0x431|-0x194:'917'
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
	// <0x6305bd>|0x29d|+0x020:'928'
	// <0x6305dd>|0x2bd|+0x03f:'929'
	// <0>
	// <1>
	// <0x63061c>|0x2fc|+0x020:'932'
	// <0>
	// <0x63063c>|0x31c|+0x01a:'934'
	// <0x630656>|0x336|+0x006:'935'
	// <0>
	// <0x63065c>|0x33c|+0x00e:'937'
	// <0x63066a>|0x34a|+0x00a:'938'
	// <0>
	// <0x630674>|0x354|+0x011:'940'
	// <0x630685>|0x365|-0x29f:'941'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6303e6>|0x0c6|-0x0a5:'947'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x630341>|0x021|+0x010:'952'
	// <0x630351>|0x031|+0x31b:'952'
	// <0x63066c>|0x34c|+0x01b:'952'
	// <0x630687>|0x367|+0x0cc:'952'
	// ******
}

// claude@NOTE: legacy body diverged - legacy has only the no-arg get_surfaces_count overload; the lod_id overload is new-in-target; matcher-phase work.
// STATE[STUB]
u32 static_render_model_instance::get_surfaces_count( u32 lod_id ) const
{
	return 0;

	// FUNCTION BODY[0x6300f0]: 5
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6300f0>|0x000|+0x014:'960'
	// ******
}

// claude@NOTE: legacy body diverged - legacy get_surfaces(render_surface_instances&,bool) is a plain visible-only loop; lod/occlusion rewrite is new-in-target; matcher-phase work.
// STATE[STUB]
void static_render_model_instance::get_surfaces(
	float4x4 const*							mat_vp,
	float3 const*							view_pos,
	vector< render_surface_instance* >&		list,
	bool									visible_only,
	u8										lod_id,
	u32										surface_flags
)
{
	// LOCALS
	// bool 							need_reset_occlusion
	// u8 								lod_surfaces_count
	// render_surface_instance* 		inst
	// render_surface_instance* 		inst
	// ******

	// FUNCTION BODY[0x630760]: 71
	// <0x630761>|0x001|+0x00e:'970'
	// <0>
	// <1>
	// <0x63076f>|0x00f|+0x012:'973'
	// <0>
	// <1>
	// <2>
	// <0x630781>|0x021|+0x005:'977'
	// <0>
	// <0x630786>|0x026|+0x00c:'979'
	// <0>
	// <0x630792>|0x032|+0x012:'981'
	// <0x6307a4>|0x044|+0x00b:'982'
	// <0>
	// <1>
	// <0x6307af>|0x04f|+0x009:'985'
	// <0>
	// <1>
	// <2>
	// <0x6307b8>|0x058|+0x022:'989'
	// <0x6307da>|0x07a|+0x016:'990'
	// <0>
	// <0x6307f0>|0x090|+0x012:'992'
	// <0x630802>|0x0a2|+0x013:'993'
	// <0x630815>|0x0b5|+0x030:'994'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x630845>|0x0e5|+0x009:'999'
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
	// <0x63084e>|0x0ee|+0x016:'1011'
	// <0x630864>|0x104|+0x008:'1012'
	// <0>
	// <1>
	// <0x63086c>|0x10c|+0x017:'1015'
	// <0>
	// <0x630883>|0x123|+0x013:'1017'
	// <0>
	// <0x630896>|0x136|+0x005:'1019'
	// <0>
	// <0x63089b>|0x13b|+0x008:'1021'
	// <0>
	// <0x6308a3>|0x143|+0x005:'1023'
	// <0x6308a8>|0x148|+0x006:'1024'
	// <0>
	// <1>
	// <0x6308ae>|0x14e|+0x012:'1027'
	// <0>
	// <0x6308c0>|0x160|+0x00c:'1029'
	// <0>
	// <0x6308cc>|0x16c|+0x01a:'1031'
	// <0>
	// <0x6308e6>|0x186|+0x007:'1033'
	// <0>
	// <0x6308ed>|0x18d|+0x007:'1035'
	// <0x6308f4>|0x194|+0x004:'1036'
	// <0>
	// <0x6308f8>|0x198|-0x0b6:'1038'
	// <0>
	// <1>
	// <0x630842>|0x0e2|+0x0df:'1041'
	// ******
}

bool static_render_model_instance::get_locator( pcstr locator_name, model_locator_item& result ) const
{
	// CALL SITE INFO
	// <0x6300eb> -> bool < unknown >( pcstr, model_locator_item& ) const
	// ******

	return m_original->get_locator( locator_name, result );

	// FUNCTION BODY[0x6300e0]: 0
	// ******
}

void static_render_model_instance::get_surface_stats( u32 surface_id, surface_stats& stats ) const
{
	R_ASSERT			(surface_id<m_instances_count);
	render_surface_instance* inst = m_surface_instances+surface_id;
	stats.vcount		= inst->m_render_surface->m_render_geometry.vertex_count;
	stats.tricount		= inst->m_render_surface->m_render_geometry.primitive_count;
	if(inst->m_render_surface->m_materail_effects_instance.c_ptr())
		stats.material		= inst->m_render_surface->m_materail_effects_instance->get_material_name().c_str();
	else
		stats.material		= "_not_assigned";

	// FUNCTION BODY[0x6302b0]: 9
	// <0>
	// <0x6302b0>|0x000|+0x013:'1051'
	// <0x6302c3>|0x013|+0x00f:'1052'
	// <0x6302d2>|0x022|+0x008:'1053'
	// <0x6302da>|0x02a|+0x008:'1054'
	// <0x6302e2>|0x032|+0x013:'1055'
	// <0>
	// <0x6302f5>|0x045|+0x00f:'1057'
	// <0x630304>|0x054|-0x003:'1057'
	// <0>
	// <0x630301>|0x051|+0x01c:'1059'
	// ******
}

void static_render_model_instance::add_sectors_holder( configs::binary_config_value sectotrs_cfg )
{
	m_sectors_holder = NEW( culling::possible_sectors_holder )( sectotrs_cfg );
}

} // namespace render
} // namespace vostok
