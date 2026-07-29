////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\sector_double_query_preventer.h"

namespace vostok {
namespace render {
namespace culling {

// STATE[STUB]
 sector_double_query_preventer::sector_double_query_preventer( spatial_sector const* sectors, u32 sectors_count )
{
	// CONSTANTS
	// const sector_double_query_preventer::{ctor}::__l2::< unnamed-tag > reserve_koef = 2;
	// ******

	// FUNCTION BODY[0x5e8c30]: 8
	// <0>
	// <0x5e8ca9>|0x079|+0x00a:'28'
	// <0x5e8cb3>|0x083|+0x00a:'29'
	// <0x5e8cbd>|0x08d|+0x013:'30'
	// <0>
	// <0x5e8cd0>|0x0a0|+0x011:'32'
	// <0x5e8ce1>|0x0b1|+0x01e:'33'
	// <0>
	// ******
}

// STATE[STUB]
 sector_double_query_preventer::~sector_double_query_preventer( )
{
	// FUNCTION BODY[0x5e8b60]: 5
	// <0x5e8b63>|0x003|+0x006:'39'
	// <0x5e8b69>|0x009|+0x03e:'40'
	// <0x5e8ba7>|0x047|+0x019:'41'
	// <0x5e8bc0>|0x060|+0x036:'42'
	// <0x5e8bf6>|0x096|+0x01b:'43'
	// ******
}

// STATE[STUB]
void sector_double_query_preventer::clear( )
{
	// FUNCTION BODY[0x5e84c0]: 10
	// <0x5e84c6>|0x006|+0x007:'48'
	// <0x5e84cd>|0x00d|+0x026:'49'
	// <0x5e84f3>|0x033|-0x01f:'49'
	// <0>
	// <0x5e84d4>|0x014|+0x026:'51'
	// <0>
	// <0x5e84fa>|0x03a|+0x006:'53'
	// <0x5e8500>|0x040|+0x025:'54'
	// <0x5e8525>|0x065|-0x01f:'54'
	// <0>
	// <0x5e8506>|0x046|+0x02a:'56'
	// <0>
	// ******
}

// STATE[STUB]
bool sector_double_query_preventer::is_possible_points_for_frustum( const float3[4]& vertices, u32 sector_id ) const
{
	return false;

	// FUNCTION BODY[0x5e86a0]: 17
	// <0x5e86a0>|0x000|+0x009:'62'
	// <0>
	// <0x5e86a9>|0x009|+0x03c:'64'
	// <0x5e86e5>|0x045|-0x025:'64'
	// <0>
	// <1>
	// <0x5e86c0>|0x020|+0x006:'67'
	// <0>
	// <0x5e86c6>|0x026|+0x019:'69'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5e86df>|0x03f|+0x010:'76'
	// <0>
	// <0x5e86ef>|0x04f|-0x00d:'78'
	// <0x5e86e2>|0x042|+0x010:'79'
	// ******
}

// STATE[STUB]
void sector_double_query_preventer::add_ss_aab_rect( aab_rect const& rect, u32 sector_id )
{
	// FUNCTION BODY[0x5e8b10]: 3
	// <0x5e8b10>|0x000|+0x010:'83'
	// <0>
	// <0x5e8b20>|0x010|+0x02a:'85'
	// <0x5e8b4a>|0x03a|-0x003:'85'
	// <0x5e8b47>|0x037|+0x00a:'86'
	// ******
}

// STATE[STUB]
bool sector_double_query_preventer::is_possible_ss_aab_rect( aab_rect const& rect, u32 sector_id ) const
{
	return false;

	// FUNCTION BODY[0x5e8540]: 8
	// <0x5e8540>|0x000|+0x00f:'90'
	// <0x5e854f>|0x00f|+0x003:'91'
	// <0x5e8552>|0x012|+0x01e:'92'
	// <0>
	// <0x5e8570>|0x030|+0x0ca:'94'
	// <0x5e863a>|0x0fa|-0x009:'95'
	// <0>
	// <0x5e8631>|0x0f1|+0x003:'97'
	// <0x5e8634>|0x0f4|+0x009:'98'
	// ******
}

// STATE[STUB]
void sector_double_query_preventer::add_frustum( math::frustum const& f, u32 sector_id )
{
	// FUNCTION BODY[0x5e8ad0]: 3
	// <0x5e8ad0>|0x000|+0x00b:'102'
	// <0>
	// <0x5e8adb>|0x00b|+0x024:'104'
	// <0x5e8aff>|0x02f|-0x003:'104'
	// <0x5e8afc>|0x02c|+0x00b:'105'
	// ******
}

// STATE[STUB]
bool sector_double_query_preventer::is_aabb_in_sector( math::aabb const& bbox, u32 sector_id ) const
{
	return false;

	// FUNCTION BODY[0x5e8650]: 8
	// <0x5e8650>|0x000|+0x00e:'109'
	// <0>
	// <0x5e865e>|0x00e|+0x012:'111'
	// <0>
	// <0x5e8670>|0x020|+0x01e:'113'
	// <0x5e868e>|0x03e|-0x008:'114'
	// <0>
	// <0x5e8686>|0x036|+0x003:'116'
	// <0x5e8689>|0x039|+0x008:'117'
	// ******
}

// STATE[STUB]
bool sector_double_query_preventer::is_visible_aabb( math::aabb const& bbox, u16 const* sectors_begin, u16 const* sectors_end ) const
{
	return false;

	// FUNCTION BODY[0x5e8700]: 6
	// <0x5e8708>|0x008|+0x040:'121'
	// <0x5e8748>|0x048|-0x035:'121'
	// <0>
	// <0x5e8713>|0x013|+0x04a:'123'
	// <0x5e875d>|0x05d|-0x009:'124'
	// <0>
	// <0x5e8754>|0x054|+0x003:'126'
	// <0x5e8757>|0x057|+0x009:'127'
	// ******
}

// STATE[STUB]
void sector_double_query_preventer::render( system_renderer& r )
{
	// LOCALS
	// const u16[24] 					frustrum_edges_indices
	// vertex_colored[8] 				vertices
	// ******

	// FUNCTION BODY[0x5e8d10]: 24
	// <0>
	// <1>
	// <0x5e8d20>|0x010|+0x041:'133'
	// <0x5e8d61>|0x051|-0x04b:'134'
	// <0x5e8d16>|0x006|+0x003:'135'
	// <0x5e8d19>|0x009|+0x085:'135'
	// <0x5e8d9e>|0x08e|-0x086:'135'
	// <0>
	// <1>
	// <0x5e8d18>|0x008|+0x0a6:'138'
	// <0x5e8dbe>|0x0ae|+0x042:'138'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5e8e00>|0x0f0|+0x01c:'143'
	// <0x5e8e1c>|0x10c|+0x007:'144'
	// <0x5e8e23>|0x113|+0x01d:'145'
	// <0x5e8e40>|0x130|+0x00a:'146'
	// <0x5e8e4a>|0x13a|+0x00a:'147'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5e8e54>|0x144|+0x0ae:'153'
	// <0>
	// ******
}

// STATE[STUB]
void sector_double_query_preventer::make_frustum_images( float3 const* furthest_vertices )
{
	// LOCALS
	// math::random32 					color_randomizer
	// vector< math::frustum > const* const sectors_max_frustums_end
	// vector< math::frustum > const* 	it
	// math::frustum const* const 		frutums_end
	// math::frustum const* 			frustum_it
	// const math::plane[6] 			planes
	// math::frustum 					f
	// ******

	// FUNCTION BODY[0x5e8770]: 37
	// <0>
	// <0x5e8777>|0x007|+0x069:'160'
	// <0x5e87e0>|0x070|-0x033:'161'
	// <0x5e87ad>|0x03d|+0x006:'162'
	// <0x5e87b3>|0x043|+0x2fa:'163'
	// <0x5e8aad>|0x33d|-0x2c6:'163'
	// <0>
	// <1>
	// <0x5e87e7>|0x077|+0x012:'166'
	// <0>
	// <0x5e87f9>|0x089|+0x06d:'168'
	// <0x5e8866>|0x0f6|-0x00e:'168'
	// <0x5e8858>|0x0e8|+0x01c:'169'
	// <0>
	// <0x5e8874>|0x104|+0x04f:'171'
	// <0>
	// <1>
	// <0x5e88c3>|0x153|+0x004:'174'
	// <0>
	// <1>
	// <2>
	// <0x5e88c7>|0x157|+0x0ac:'178'
	// <0>
	// <0x5e8973>|0x203|+0x02c:'180'
	// <0x5e899f>|0x22f|+0x03b:'181'
	// <0x5e89da>|0x26a|+0x01a:'182'
	// <0x5e89f4>|0x284|+0x021:'183'
	// <0>
	// <1>
	// <0x5e8a15>|0x2a5|+0x009:'186'
	// <0>
	// <0x5e8a1e>|0x2ae|+0x00a:'188'
	// <0>
	// <0x5e8a28>|0x2b8|+0x04f:'190'
	// <0x5e8a77>|0x307|+0x015:'191'
	// <0x5e8a8c>|0x31c|+0x037:'192'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
u32 sector_double_query_preventer::frustums_count( ) const
{
	return 0;

	// FUNCTION BODY[0x5e8480]: 7
	// <0>
	// <0x5e8480>|0x000|+0x005:'201'
	// <0x5e8485>|0x005|+0x00c:'202'
	// <0>
	// <0x5e8491>|0x011|+0x023:'204'
	// <0>
	// <0x5e84b4>|0x034|+0x003:'206'
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
	// 	vostok::math::frustum*
	// 	iterator_type;

	// typedef
	// 	vostok::render::culling::aab_rect*
	// 	iterator_type;

	// typedef
	// 	vostok::render::culling::sector_double_query_preventer::frustum_image*
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
