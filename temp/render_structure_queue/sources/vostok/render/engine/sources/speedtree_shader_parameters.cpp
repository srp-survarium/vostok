////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\speedtree_shader_parameters.h"

namespace vostok {
namespace render {

// STATE[STUB]
 speedtree_wind_parameters::speedtree_wind_parameters( )
{
	// FUNCTION BODY[0x779210]: 11
	// <0x779213>|0x003|+0x059:'30'
	// <0x77926c>|0x05c|+0x059:'31'
	// <0x7792c5>|0x0b5|+0x059:'32'
	// <0x77931e>|0x10e|+0x059:'33'
	// <0x779377>|0x167|+0x059:'34'
	// <0x7793d0>|0x1c0|+0x059:'35'
	// <0x779429>|0x219|+0x059:'36'
	// <0x779482>|0x272|+0x059:'37'
	// <0x7794db>|0x2cb|+0x059:'38'
	// <0x779534>|0x324|+0x059:'39'
	// <0x77958d>|0x37d|+0x059:'40'
	// ******
}

// STATE[STUB]
void speedtree_wind_parameters::set( SpeedTree::CWind const& wind )
{
	// FUNCTION BODY[0x778320]: 13
	// <0>
	// <1>
	// <0x77832a>|0x00a|+0x07a:'47'
	// <0x7783a4>|0x084|+0x07f:'48'
	// <0x778423>|0x103|+0x07f:'49'
	// <0x7784a2>|0x182|+0x071:'50'
	// <0x778513>|0x1f3|+0x071:'51'
	// <0x778584>|0x264|+0x071:'52'
	// <0x7785f5>|0x2d5|+0x071:'53'
	// <0x778666>|0x346|+0x07f:'54'
	// <0x7786e5>|0x3c5|+0x07f:'55'
	// <0x778764>|0x444|+0x07f:'56'
	// <0x7787e3>|0x4c3|+0x07a:'57'
	// ******
}

// STATE[STUB]
 speedtree_billboard_parameters::speedtree_billboard_parameters( )
{
	// FUNCTION BODY[0x778f90]: 7
	// <0x778f93>|0x003|+0x05a:'63'
	// <0x778fed>|0x05d|+0x059:'64'
	// <0x779046>|0x0b6|+0x059:'65'
	// <0x77909f>|0x10f|+0x059:'66'
	// <0x7790f8>|0x168|+0x059:'67'
	// <0x779151>|0x1c1|+0x058:'68'
	// <0x7791a9>|0x219|+0x059:'69'
	// ******
}

// STATE[STUB]
void speedtree_billboard_parameters::set( renderer_context* context, speedtree_tree_component* tree_component )
{
	// LOCALS
	// const float 						azimuth
	// float 							sine
	// ******

	// FUNCTION BODY[0x778a80]: 34
	// <0x778a86>|0x006|+0x016:'74'
	// <0>
	// <0x778a9c>|0x01c|+0x012:'76'
	// <0>
	// <0x778aae>|0x02e|+0x006:'78'
	// <0x778ab4>|0x034|+0x089:'79'
	// <0>
	// <1>
	// <0x778b3d>|0x0bd|+0x01d:'82'
	// <0>
	// <0x778b5a>|0x0da|+0x051:'84'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x778bab>|0x12b|+0x08b:'92'
	// <0x778c36>|0x1b6|-0x014:'92'
	// <0>
	// <0x778c22>|0x1a2|+0x00b:'94'
	// <0x778c2d>|0x1ad|+0x013:'95'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x778c40>|0x1c0|+0x04b:'102'
	// <0>
	// <1>
	// <0x778c8b>|0x20b|+0x045:'105'
	// <0x778cd0>|0x250|+0x081:'106'
	// <0x778d51>|0x2d1|+0x04a:'107'
	// ******
}

// STATE[STUB]
void speedtree_billboard_parameters::set_billboard_tangents( float camera_azimuth )
{
	// LOCALS
	// SpeedTree::Vec3 					normal
	// SpeedTree::Vec3 					tangent
	// SpeedTree::Vec3 					binormal
	// float4[3] 						billboard_tangents
	// ******

	// FUNCTION BODY[0x778870]: 22
	// <0x778870>|0x000|+0x018:'112'
	// <0>
	// <0x778888>|0x018|+0x009:'114'
	// <0x778891>|0x021|+0x014:'115'
	// <0>
	// <0x7788a5>|0x035|+0x005:'117'
	// <0x7788aa>|0x03a|+0x049:'118'
	// <0x7788f3>|0x083|+0x069:'119'
	// <0>
	// <1>
	// <2>
	// <0x77895c>|0x0ec|+0x024:'123'
	// <0x778980>|0x110|+0x049:'124'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x7789c9>|0x159|+0x00c:'129'
	// <0>
	// <1>
	// <2>
	// <0x7789d5>|0x165|+0x095:'133'
	// ******
}

// STATE[STUB]
 speedtree_common_parameters::speedtree_common_parameters( )
{
	// FUNCTION BODY[0x778e70]: 3
	// <0x778e73>|0x003|+0x059:'138'
	// <0x778ecc>|0x05c|+0x059:'139'
	// <0x778f25>|0x0b5|+0x059:'140'
	// ******
}

// STATE[STUB]
void speedtree_common_parameters::set(
	renderer_context*				context,
	speedtree_tree_component*		tree_component,
	float3 const&					lod_reference_position
)
{
	// FUNCTION BODY[0x778160]: 25
	// <0x778166>|0x006|+0x081:'145'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x7781e7>|0x087|+0x05a:'150'
	// <0>
	// <0x778241>|0x0e1|+0x017:'152'
	// <0x778258>|0x0f8|+0x006:'153'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x77825e>|0x0fe|+0x031:'163'
	// <0>
	// <0x77828f>|0x12f|+0x002:'165'
	// <0>
	// <1>
	// <0x778291>|0x131|+0x075:'168'
	// <0>
	// ******
}

// STATE[STUB]
 speedtree_tree_parameters::speedtree_tree_parameters( )
{
	// FUNCTION BODY[0x778db0]: 2
	// <0x778db3>|0x003|+0x059:'175'
	// <0x778e0c>|0x05c|+0x059:'176'
	// ******
}

// STATE[STUB]
void speedtree_tree_parameters::set(
	speedtree_tree_component*			tree_component,
	SpeedTree::CInstance const*			instance,
	SpeedTree::SInstanceLod const*		instance_lod
)
{
	// LOCALS
	// float4 							tree_pos
	// ******

	// CALL SITE INFO
	// <0x7780ed> -> SpeedTree::EGeometryType < unknown >() const
	// ******

	// FUNCTION BODY[0x778030]: 14
	// <0>
	// <0x77803c>|0x00c|+0x074:'185'
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
	// <0x7780b0>|0x080|+0x0a2:'196'
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

	// ******

} // namespace render
} // namespace vostok
