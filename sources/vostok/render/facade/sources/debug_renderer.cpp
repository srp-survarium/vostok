#include "pch.h"
#include <vostok/render/facade/sources/debug_renderer.h>

namespace vostok {
namespace render {
namespace debug {

// STATE[STUB]
renderer::renderer(
	one_way_render_channel&		channel,
	memory::base_allocator&		allocator,
	engine::world&				engine_world
)
	: m_render_engine_world( engine_world ),
	  m_channel( channel ),
	  m_allocator( allocator )
{
	// FUNCTION BODY[0x771310]: 0
	// ******
}

// STATE[STUB]
void renderer::draw_line(
	base_scene_ptr const&		scene,
	float3 const&				start_point,
	float3 const&				end_point,
	math::color const&			color,
	const bool					use_depth
)
{
	// LOCALS
	// const vertex_colored[2] 			vertices
	// const u16[2] 					indices
	// ******

	// FUNCTION BODY[0x771cc0]: 11
	// <0x771cd1>|0x011|+0x0b2:'36'
	// <0x771d83>|0x0c3|+0x02f:'37'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x771db2>|0x0f2|+0x01b:'44'
	// <0x771dcd>|0x10d|+0x016:'45'
	// <0x771de3>|0x123|+0x078:'46'
	// ******
}

// STATE[STUB]
void renderer::draw_origin(
	base_scene_ptr const&		scene,
	float4x4 const&				matrix,
	const float					half_size,
	const bool					use_depth
)
{
	// LOCALS
	// const vertex_colored[6] 			vertices
	// const u16[6] 					indices
	// ******

	// FUNCTION BODY[0x771800]: 17
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
	// <0x771806>|0x006|+0x1a3:'62'
	// <0x7719a9>|0x1a9|+0x122:'63'
	// <0x771acb>|0x2cb|+0x103:'64'
	// <0>
	// <0x771bce>|0x3ce|+0x03c:'66'
	// <0x771c0a>|0x40a|+0x0a2:'67'
	// ******
}

// STATE[STUB]
void renderer::draw_cross(
	base_scene_ptr const&		scene,
	float3 const&				p,
	const float					half_size,
	math::color const&			color,
	const bool					use_depth
)
{
	// LOCALS
	// const vertex_colored[6] 			vertices
	// const u16[6] 					indices
	// ******

	// FUNCTION BODY[0x771600]: 12
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x771609>|0x009|+0x059:'78'
	// <0x771662>|0x062|+0x06b:'79'
	// <0x7716cd>|0x0cd|-0x0ca:'80'
	// <0>
	// <0x771603>|0x003|+0x12b:'82'
	// <0x77172e>|0x12e|+0x03b:'82'
	// <0x771769>|0x169|+0x089:'83'
	// ******
}

// STATE[STUB]
void renderer::draw_triangle(
	base_scene_ptr const&		scene,
	vertex_colored const (&vertices)[3],
	const bool					use_depth
)
{
	// LOCALS
	// const u16[3] 					indices
	// ******

	// FUNCTION BODY[0x771560]: 2
	// <0x771567>|0x007|+0x00c:'115'
	// <0x771573>|0x013|+0x078:'116'
	// ******
}

// STATE[STUB]
void renderer::draw_cube(
	base_scene_ptr const&		scene,
	float4x4 const&				matrix,
	float3 const&				size,
	math::color const&			color,
	const bool					use_depth
)
{
	// LOCALS
	// math::aabb 						bb
	// ******

	// FUNCTION BODY[0x773390]: 15
	// <0x773399>|0x009|+0x082:'121'
	// <0x77341b>|0x08b|+0x017:'122'
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
	// <0x773432>|0x0a2|+0x01f:'135'
	// ******
}

// STATE[STUB]
void renderer::draw_frustum(
	base_scene_ptr const&		scene,
	const float					fov_in_radians,
	const float					near_plane_distance,
	const float					far_plane_distance,
	const float					aspect_ratio,
	float3 const&				position,
	float3 const&				direction,
	float3						up_vector,
	math::color const&			color,
	bool						use_depth
)
{
	// LOCALS
	// float 							window_right_coord
	// float 							window_left_coord
	// float 							window_top_coord
	// float 							window_bottom_coord
	// const float3[4] 					corner_vertices
	// const float3[4] 					projectors
	// const float3[4] 					far_plane_points
	// bool 							do_debug_break
	// ******

	// STATICS
	// static bool 						debug_macro_helper_ignore_always = <0x4c25af6>;
	// static u32 						occurances_left = <0xa6bd8c>;
	// ******

	// FUNCTION BODY[0x772920]: 71
	// <0>
	// <0x77292c>|0x00c|+0x00a:'152'
	// <0>
	// <0x772936>|0x016|+0x01b:'154'
	// <0>
	// <0x772951>|0x031|+0x024:'156'
	// <0x772975>|0x055|+0x002:'157'
	// <0>
	// <0x772977>|0x057|+0x0e6:'159'
	// <0>
	// <0x772a5d>|0x13d|+0x0b1:'161'
	// <0x772b0e>|0x1ee|+0x0c3:'162'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x772bd1>|0x2b1|+0x152:'167'
	// <0x772d23>|0x403|+0x068:'168'
	// <0x772d8b>|0x46b|+0x028:'169'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x772db3>|0x493|+0x135:'175'
	// <0x772ee8>|0x5c8|+0x0a3:'176'
	// <0x772f8b>|0x66b|+0x0a3:'177'
	// <0>
	// <0x77302e>|0x70e|+0x0d1:'179'
	// <0>
	// <0x7730ff>|0x7df|+0x0b5:'181'
	// <0>
	// <1>
	// <0x7731b4>|0x894|+0x045:'184'
	// <0x7731f9>|0x8d9|+0x04f:'185'
	// <0x773248>|0x928|+0x049:'186'
	// <0>
	// <0x773291>|0x971|+0x025:'188'
	// <0>
	// <1>
	// <2>
	// <0x7732b6>|0x996|+0x038:'192'
	// <0x7732ee>|0x9ce|+0x011:'193'
	// <0x7732ff>|0x9df|+0x014:'194'
	// <0x773313>|0x9f3|+0x014:'195'
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
	// <0x773327>|0xa07|+0x012:'218'
	// <0x773339>|0xa19|+0x015:'219'
	// <0x77334e>|0xa2e|+0x018:'220'
	// <0x773366>|0xa46|-0x910:'221'
	// <0x772a56>|0x136|+0x925:'222'
	// ******
}

// STATE[STUB]
void renderer::draw_aabb(
	base_scene_ptr const&		scene,
	float3 const&				center,
	float3 const&				size,
	math::color const&			color,
	const bool					use_depth
)
{
	// FUNCTION BODY[0x773a70]: 9
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x773a7a>|0x00a|+0x021:'361'
	// ******
}

// STATE[STUB]
void renderer::draw_ellipsoid(
	base_scene_ptr const&		scene,
	float4x4 const&				matrix,
	float3 const&				size,
	math::color const&			color,
	const bool					use_depth
)
{
	// FUNCTION BODY[0x7728f0]: 11
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
	// <0x7728f0>|0x000|+0x024:'404'
	// ******
}

// STATE[STUB]
void renderer::draw_cylinder(
	base_scene_ptr const&		scene,
	float4x4 const&				matrix,
	float3 const&				size,
	math::color const&			color,
	const bool					use_depth
)
{
	// FUNCTION BODY[0x7728c0]: 15
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
	// <0x7728c0>|0x000|+0x024:'438'
	// ******
}

// STATE[STUB]
void renderer::draw_sphere(
	base_scene_ptr const&		scene,
	float4x4 const&				m,
	float const&				radius,
	math::color const&			color,
	const bool					use_depth
)
{
	// LOCALS
	// math::sphere 					sp
	// ******

	// FUNCTION BODY[0x7739f0]: 11
	// <0x7739f3>|0x003|+0x013:'443'
	// <0x773a06>|0x016|+0x01c:'444'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x773a22>|0x032|+0x03a:'453'
	// ******
}

// STATE[STUB]
void renderer::draw_cone(
	base_scene_ptr const&		scene,
	float4x4 const&				matrix,
	float3 const&				size,
	math::color const&			color,
	const bool					use_depth
)
{
	// LOCALS
	// math::sphere 					sp
	// ******

	// FUNCTION BODY[0x772850]: 15
	// <0x77285b>|0x00b|+0x01d:'458'
	// <0x772878>|0x028|+0x01c:'459'
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
	// <0x772894>|0x044|+0x021:'472'
	// ******
}

// STATE[STUB]
void renderer::draw_cube_solid(
	base_scene_ptr const&		scene,
	float4x4 const&				matrix,
	float3 const&				size,
	math::color const&			color,
	const bool					use_depth
)
{
	// FUNCTION BODY[0x772820]: 15
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
	// <0x772820>|0x000|+0x028:'525'
	// ******
}

// STATE[STUB]
void renderer::draw_cylinder_solid(
	base_scene_ptr const&		scene,
	float4x4 const&				matrix,
	float3 const&				size,
	math::color const&			color,
	const bool					use_depth
)
{
	// FUNCTION BODY[0x772790]: 13
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
	// <0x77279c>|0x00c|+0x07b:'542'
	// ******
}

// STATE[STUB]
void renderer::draw_sphere_solid(
	base_scene_ptr const&		scene,
	float3 const&				center,
	float const&				radius,
	math::color const&			color,
	const bool					use_depth
)
{
	// LOCALS
	// math::sphere 					sp
	// ******

	// FUNCTION BODY[0x772710]: 11
	// <0x772713>|0x003|+0x00f:'547'
	// <0x772722>|0x012|+0x01a:'548'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x77273c>|0x02c|+0x04e:'557'
	// ******
}

// STATE[STUB]
void renderer::draw_ellipsoid_solid(
	base_scene_ptr const&		scene,
	float4x4 const&				matrix,
	float3 const&				size,
	math::color const&			color,
	const bool					use_depth
)
{
	// FUNCTION BODY[0x7726d0]: 11
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
	// <0x7726d0>|0x000|+0x02e:'572'
	// ******
}

// STATE[STUB]
void renderer::draw_primitive_solid(
	base_scene_ptr const&		scene,
	float4x4 const&				matrix,
	float3 const&				size,
	float const* const			vertices,
	u32							vertex_count,
	u16 const* const			faces,
	const u32					index_count,
	math::color const&			color,
	const bool					use_depth
)
{
	// LOCALS
	// vectora< vertex_colored > 		temp_vertices
	// float4x4 						trnsform
	// vertex_colored 					temp_vertex
	// vectora< u16 > 					temp_indices
	// u32 								i
	// ******

	// FUNCTION BODY[0x772090]: 25
	// <0x77209f>|0x00f|+0x069:'577'
	// <0>
	// <0x772108>|0x078|+0x00c:'579'
	// <0x772114>|0x084|+0x028:'580'
	// <0>
	// <1>
	// <0x77213c>|0x0ac|+0x005:'583'
	// <0>
	// <0x772141>|0x0b1|+0x3d6:'585'
	// <0x772517>|0x487|-0x388:'585'
	// <0>
	// <1>
	// <2>
	// <0x77218f>|0x0ff|+0x39e:'589'
	// <0x77252d>|0x49d|-0x395:'589'
	// <0>
	// <0x772198>|0x108|+0x39e:'591'
	// <0x772536>|0x4a6|-0x2df:'591'
	// <0x772257>|0x1c7|+0x39e:'592'
	// <0x7725f5>|0x565|+0x01e:'592'
	// <0>
	// <1>
	// <0x772613>|0x583|+0x006:'595'
	// <0x772619>|0x589|+0x038:'596'
	// <0>
	// <0x772651>|0x5c1|+0x00f:'598'
	// <0x772660>|0x5d0|+0x017:'599'
	// <0>
	// <0x772677>|0x5e7|+0x01c:'601'
	// ******
}

// STATE[STUB]
void renderer::draw_arrow(
	base_scene_ptr const&		scene,
	float3 const&				start_point,
	float3 const&				end_point,
	math::color const&			line_color,
	math::color const&			cone_color,
	const bool					use_depth
)
{
	// LOCALS
	// float3 							up_vector
	// float4x4 						matrix
	// float3 							direction
	// float 							length
	// float3 							sizes
	// ******

	// FUNCTION BODY[0x7736f0]: 31
	// <0x773700>|0x010|+0x01f:'606'
	// <0x77371f>|0x02f|+0x056:'607'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x773775>|0x085|+0x014:'615'
	// <0>
	// <0x773789>|0x099|+0x048:'617'
	// <0x7737d1>|0x0e1|+0x006:'618'
	// <0>
	// <1>
	// <0x7737d7>|0x0e7|+0x008:'621'
	// <0>
	// <0x7737df>|0x0ef|+0x055:'623'
	// <0>
	// <0x773834>|0x144|+0x093:'625'
	// <0x7738c7>|0x1d7|+0x036:'626'
	// <0x7738fd>|0x20d|+0x002:'627'
	// <0>
	// <0x7738ff>|0x20f|+0x019:'629'
	// <0x773918>|0x228|+0x028:'630'
	// <0x773940>|0x250|+0x002:'631'
	// <0x773942>|0x252|+0x073:'632'
	// <0>
	// <1>
	// <0x7739b5>|0x2c5|+0x003:'635'
	// <0x7739b8>|0x2c8|+0x027:'636'
	// ******
}

// STATE[STUB]
void renderer::draw_arrow(
	base_scene_ptr const&		scene,
	float3 const&				start_point,
	float3 const&				end_point,
	math::color const&			color,
	const bool					use_depth
)
{
	// FUNCTION BODY[0x773ab0]: 8
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x773ab0>|0x000|+0x015:'648'
	// ******
}

// STATE[STUB]
void renderer::draw_line_ellipsoid(
	base_scene_ptr const&		scene,
	float4x4 const&				matrix,
	math::color const&			color,
	const bool					use_depth
)
{
	// FUNCTION BODY[0x7714f0]: 32
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x7714fb>|0x00b|+0x01c:'662'
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
	// <0x771517>|0x027|+0x01c:'673'
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
	// <0x771533>|0x043|+0x01c:'684'
	// ******
}

// STATE[STUB]
void renderer::draw_line_hemisphere(
	base_scene_ptr const&		scene,
	float4x4 const&				matrix,
	float3 const&				size,
	bool						bdraw_base,
	math::color const&			color,
	const bool					use_depth
)
{
	// FUNCTION BODY[0x771fe0]: 39
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
	// <0x771fe6>|0x006|+0x048:'700'
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
	// <0x77202e>|0x04e|+0x055:'712'
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
	// ******
}

// STATE[STUB]
void renderer::draw_line_capsule(
	base_scene_ptr const&		scene,
	float4x4 const&				matrix,
	float3 const&				size,
	math::color const&			color,
	const bool					use_depth
)
{
	// FUNCTION BODY[0x7735c0]: 14
	// <0x7735cc>|0x00c|+0x02c:'732'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7735f8>|0x038|+0x06f:'738'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x773667>|0x0a7|+0x07d:'744'
	// <0>
	// ******
}

// STATE[STUB]
void renderer::draw_solid_capsule(
	base_scene_ptr const&		scene,
	float4x4 const&				matrix,
	float3 const&				size,
	math::color const&			color,
	const bool					use_depth
)
{
	// FUNCTION BODY[0x773460]: 27
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
	// <0x773472>|0x012|+0x07b:'764'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7734ed>|0x08d|+0x063:'770'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x773550>|0x0f0|+0x064:'775'
	// <0>
	// ******
}

// STATE[STUB]
void renderer::draw_lines(
	base_scene_ptr const&				scene,
	vectora< vertex_colored > const&	vertices,
	vectora< u16 > const&				indices,
	const bool							use_depth
)
{
	// FUNCTION BODY[0x771f70]: 2
	// <0>
	// <0x771f74>|0x004|+0x067:'782'
	// ******
}

// STATE[STUB]
void renderer::draw_lines(
	base_scene_ptr const&		scene,
	float4x4 const&				matrix,
	float const* const			vertices,
	const u32					vertex_count,
	u16 const* const			pairs,
	const u32					pair_count,
	math::color const&			color,
	bool						use_depth
)
{
	// LOCALS
	// buffer_vector< vertex_colored > 	temp_vertices
	// buffer_vector< u16 > 			temp_indices
	// ******

	// FUNCTION BODY[0x771330]: 24
	// <0>
	// <1>
	// <2>
	// <0x77133b>|0x00b|+0x03c:'811'
	// <0>
	// <1>
	// <0x771377>|0x047|+0x015:'814'
	// <0x77138c>|0x05c|+0x00b:'815'
	// <0>
	// <0x771397>|0x067|+0x003:'817'
	// <0>
	// <0x77139a>|0x06a|+0x016:'819'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x7713b0>|0x080|+0x0bc:'828'
	// <0>
	// <1>
	// <0x77146c>|0x13c|+0x06a:'831'
	// ******
}

// STATE[STUB]
void renderer::draw_triangles(
	base_scene_ptr const&				scene,
	vectora< vertex_colored > const&	vertices,
	vectora< u16 > const&				indices,
	const bool							use_depth
)
{
	// FUNCTION BODY[0x771f00]: 2
	// <0>
	// <0x771f04>|0x004|+0x066:'856'
	// ******
}

// STATE[STUB]
void renderer::draw_lines(
	base_scene_ptr const&		scene,
	float4x4 const&				matrix,
	float3 const&				size,
	float const* const			vertices,
	const u32					vertex_count,
	u16 const* const			pairs,
	const u32					pair_count,
	math::color const&			color,
	bool						use_depth
)
{
	// FUNCTION BODY[0x771e70]: 11
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
	// <0x771e7c>|0x00c|+0x07c:'881'
	// ******
}

} // namespace debug
} // namespace render
} // namespace vostok
