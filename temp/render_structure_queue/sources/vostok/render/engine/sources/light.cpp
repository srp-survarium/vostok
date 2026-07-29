////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\light.h"

namespace vostok {
namespace render {

// STATE[STUB]
explicit light::light( collision::space_partitioning_tree* tree )
{
	// FUNCTION BODY[0x601350]: 15
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x601475>|0x125|+0x01f:'52'
	// <0x601494>|0x144|+0x030:'53'
	// <0x6014c4>|0x174|+0x030:'54'
	// <0x6014f4>|0x1a4|+0x010:'55'
	// <0x601504>|0x1b4|+0x018:'56'
	// <0x60151c>|0x1cc|+0x018:'57'
	// <0>
	// <1>
	// <0x601534>|0x1e4|+0x006:'60'
	// ******
}

// STATE[STUB]
 light::~light( )
{
	// FUNCTION BODY[0x6012d0]: 1
	// <0x6012d1>|0x001|+0x006:'65'
	// ******
}

// STATE[STUB]
float frac( float f )
{
	return 0.0f;

	// FUNCTION BODY[0x5ff670]: 1
	// <0x5ff671>|0x001|+0x02a:'70'
	// ******
}

// STATE[STUB]
void light::tick_color_animation( const float time_delta )
{
	// FUNCTION BODY[0x6000c0]: 9
	// <0x6000c8>|0x008|+0x00f:'75'
	// <0>
	// <1>
	// <0x6000d7>|0x017|+0x02a:'78'
	// <0>
	// <0x600101>|0x041|+0x011:'80'
	// <0x600112>|0x052|+0x036:'81'
	// <0>
	// <0x600148>|0x088|+0x063:'83'
	// ******
}

// STATE[STUB]
void light::remove_collision( )
{
	// CALL SITE INFO
	// <0x5ff533> -> void < unknown >( collision::object* )
	// ******

	// FUNCTION BODY[0x5ff510]: 10
	// <0x5ff515>|0x005|+0x00e:'88'
	// <0>
	// <1>
	// <0x5ff523>|0x013|+0x00a:'91'
	// <0x5ff52d>|0x01d|+0x00a:'92'
	// <0>
	// <0x5ff537>|0x027|+0x037:'94'
	// <0x5ff56e>|0x05e|+0x038:'95'
	// <0>
	// <0x5ff5a6>|0x096|+0x00c:'97'
	// ******
}

// STATE[STUB]
bool light::is_occluded( ) const
{
	return false;

	// FUNCTION BODY[0x5ff650]: 1
	// <0x5ff650>|0x000|+0x01d:'102'
	// <0x5ff66d>|0x01d|-0x001:'102'
	// <0x5ff66c>|0x01c|+0x003:'103'
	// ******
}

// STATE[STUB]
bool light::is_cast_shadows( ) const
{
	return false;

	// FUNCTION BODY[0x5ff4b0]: 17
	// <0x5ff4b0>|0x000|+0x00b:'107'
	// <0>
	// <0x5ff4bb>|0x00b|+0x003:'109'
	// <0>
	// <1>
	// <2>
	// <0x5ff4be>|0x00e|+0x005:'113'
	// <0>
	// <1>
	// <2>
	// <0x5ff4c3>|0x013|+0x039:'117'
	// <0x5ff4fc>|0x04c|+0x006:'117'
	// <0>
	// <1>
	// <2>
	// <0x5ff502>|0x052|-0x045:'121'
	// <0>
	// <1>
	// <0x5ff4bd>|0x00d|+0x03e:'124'
	// <0x5ff4fb>|0x04b|+0x006:'124'
	// <0x5ff501>|0x051|+0x003:'124'
	// ******
}

// STATE[STUB]
void light::destroy_impl( ) const
{
	// FUNCTION BODY[0x601540]: 2
	// <0>
	// <0x601542>|0x002|+0x020:'194'
	// ******
}

// STATE[STUB]
void light::set_position( float3 const& P )
{
	// FUNCTION BODY[0x5ff6a0]: 3
	// <0>
	// <0x5ff6a0>|0x000|+0x01f:'214'
	// <0x5ff6bf>|0x01f|+0x00f:'215'
	// ******
}

// STATE[STUB]
void light::set_color( math::color const& c, const float intensity )
{
	// FUNCTION BODY[0x5ff5c0]: 5
	// <0x5ff5c0>|0x000|+0x00f:'220'
	// <0x5ff5cf>|0x00f|+0x010:'221'
	// <0x5ff5df>|0x01f|+0x010:'222'
	// <0x5ff5ef>|0x02f|+0x03c:'223'
	// <0x5ff62b>|0x06b|+0x016:'224'
	// ******
}

// STATE[STUB]
void light::set_range( float R )
{
	// FUNCTION BODY[0x5ff4a0]: 3
	// <0>
	// <1>
	// <0x5ff4a0>|0x000|+0x008:'231'
	// ******
}

// STATE[STUB]
void light::set_orientation( float3 const& direction, float3 const& right )
{
	// FUNCTION BODY[0x601230]: 2
	// <0x601233>|0x003|+0x035:'236'
	// <0x601268>|0x038|+0x058:'237'
	// ******
}

// STATE[STUB]
void light::on_properties_changed( )
{
	// LOCALS
	// float4x4 						transform
	// float3 							L_up
	// float4x4 						rotation_X_translation
	// float3 							L_right
	// float3 							L_dir
	// float3 							box_half_length
	// const float 						radius
	// float3 							box_half_length
	// float3 							box_half_length
	// const float 						scale_x_plus_range
	// ******

	// STATICS
	// static < NoType > 				 = <0x601208>;
	// ******

	// CALL SITE INFO
	// <0x6001fc> -> void < unknown >( collision::object* )
	// <0x6011ee> -> void < unknown >( collision::object*, float4x4 const& )
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5ef7f2 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN7") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5ef982 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN6") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5efb96 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN5") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5efc74 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5efdae }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5eff2e }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// ******

	// FUNCTION BODY[0x6001c0]: 153
	// <0>
	// <0x6001d2>|0x012|+0x010:'243'
	// <0>
	// <1>
	// <0x6001e2>|0x022|+0x014:'246'
	// <0x6001f6>|0x036|+0x008:'247'
	// <0>
	// <0x6001fe>|0x03e|+0x03c:'249'
	// <0x60023a>|0x07a|+0x03c:'250'
	// <0>
	// <1>
	// <0x600276>|0x0b6|+0x003:'253'
	// <0x600279>|0x0b9|+0x08b:'254'
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
	// <0x600304>|0x144|+0x05e:'265'
	// <0>
	// <1>
	// <0x600362>|0x1a2|+0x064:'268'
	// <0x6003c6>|0x206|+0x0bc:'269'
	// <0x600482>|0x2c2|+0x0f8:'270'
	// <0x60057a>|0x3ba|+0x005:'271'
	// <0>
	// <0x60057f>|0x3bf|+0x047:'273'
	// <0x6005c6>|0x406|+0x08b:'274'
	// <0x600651>|0x491|+0x0f8:'275'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x600749>|0x589|+0x00a:'280'
	// <0x600753>|0x593|+0x00a:'281'
	// <0x60075d>|0x59d|+0x012:'282'
	// <0x60076f>|0x5af|+0x00d:'283'
	// <0>
	// <1>
	// <0x60077c>|0x5bc|+0x076:'286'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6007f2>|0x632|+0x0b3:'295'
	// <0x6008a5>|0x6e5|+0x0d8:'296'
	// <0x60097d>|0x7bd|+0x005:'297'
	// <0>
	// <1>
	// <2>
	// <0x600982>|0x7c2|+0x03e:'301'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6009c0>|0x800|+0x00e:'306'
	// <0>
	// <1>
	// <2>
	// <0x6009ce>|0x80e|+0x152:'310'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x600b20>|0x960|+0x071:'318'
	// <0>
	// <0x600b91>|0x9d1|+0x005:'320'
	// <0>
	// <1>
	// <0x600b96>|0x9d6|+0x026:'323'
	// <0>
	// <1>
	// <2>
	// <0x600bbc>|0x9fc|+0x0b3:'327'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x600c6f>|0xaaf|+0x005:'335'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x600c74>|0xab4|+0x014:'340'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x600c88>|0xac8|+0x0bb:'349'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x600d43>|0xb83|+0x06b:'357'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x600dae>|0xbee|+0x0a9:'367'
	// <0>
	// <1>
	// <0x600e57>|0xc97|+0x0d2:'370'
	// <0x600f29>|0xd69|+0x005:'371'
	// <0>
	// <1>
	// <0x600f2e>|0xd6e|+0x02d:'374'
	// <0x600f5b>|0xd9b|+0x014:'375'
	// <0>
	// <1>
	// <2>
	// <0x600f6f>|0xdaf|+0x1dd:'379'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x60114c>|0xf8c|+0x089:'386'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6011d5>|0x1015|+0x01b:'393'
	// <0x6011f0>|0x1030|+0x00e:'394'
	// ******
}

// STATE[STUB]
void light::xform_calc( )
{
	// LOCALS
	// float3 							L_up
	// float4x4 						rotation_X_translation
	// float3 							L_right
	// float3 							L_dir
	// const float 						range
	// const float 						range_X_tan_penumbra_angle_div_2
	// const float 						range
	// float4x4 						rotation
	// const float 						range_X_tan_penumbra_angle_div_2
	// ******

	// STATICS
	// static < NoType > 				 = <0x6000a4>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5eebae }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN7") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5eec35 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN6") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5eecb2 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN5") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5eed38 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5eedab }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5eee06 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// ******

	// FUNCTION BODY[0x5ff6d0]: 82
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
	// <0x5ff6dc>|0x00c|+0x063:'412'
	// <0>
	// <1>
	// <0x5ff73f>|0x06f|+0x062:'415'
	// <0x5ff7a1>|0x0d1|+0x0b4:'416'
	// <0x5ff855>|0x185|+0x0e2:'417'
	// <0x5ff937>|0x267|+0x005:'418'
	// <0>
	// <0x5ff93c>|0x26c|+0x04f:'420'
	// <0x5ff98b>|0x2bb|+0x086:'421'
	// <0x5ffa11>|0x341|+0x0e2:'422'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5ffaf3>|0x423|+0x00a:'427'
	// <0x5ffafd>|0x42d|+0x00a:'428'
	// <0x5ffb07>|0x437|+0x00d:'429'
	// <0x5ffb14>|0x444|+0x019:'430'
	// <0>
	// <0x5ffb2d>|0x45d|+0x081:'432'
	// <0>
	// <0x5ffbae>|0x4de|+0x010:'434'
	// <0x5ffbbe>|0x4ee|+0x029:'435'
	// <0x5ffbe7>|0x517|+0x04e:'436'
	// <0>
	// <1>
	// <2>
	// <0x5ffc35>|0x565|+0x02d:'440'
	// <0x5ffc62>|0x592|+0x036:'441'
	// <0x5ffc98>|0x5c8|+0x015:'442'
	// <0x5ffcad>|0x5dd|+0x005:'443'
	// <0>
	// <1>
	// <0x5ffcb2>|0x5e2|+0x054:'446'
	// <0x5ffd06>|0x636|+0x02d:'447'
	// <0x5ffd33>|0x663|+0x005:'448'
	// <0>
	// <1>
	// <0x5ffd38>|0x668|+0x06e:'451'
	// <0>
	// <0x5ffda6>|0x6d6|+0x005:'453'
	// <0>
	// <1>
	// <0x5ffdab>|0x6db|+0x018:'456'
	// <0x5ffdc3>|0x6f3|+0x01d:'457'
	// <0x5ffde0>|0x710|+0x021:'458'
	// <0x5ffe01>|0x731|+0x005:'459'
	// <0>
	// <1>
	// <2>
	// <0x5ffe06>|0x736|+0x030:'463'
	// <0x5ffe36>|0x766|+0x06a:'464'
	// <0x5ffea0>|0x7d0|+0x066:'465'
	// <0>
	// <1>
	// <2>
	// <0x5fff06>|0x836|+0x016:'469'
	// <0x5fff1c>|0x84c|+0x062:'470'
	// <0x5fff7e>|0x8ae|+0x01b:'471'
	// <0>
	// <0x5fff99>|0x8c9|+0x0f9:'473'
	// <0>
	// <1>
	// <2>
	// <0x600092>|0x9c2|-0x466:'477'
	// <0>
	// <1>
	// <2>
	// <0x5ffc2c>|0x55c|+0x45d:'481'
	// <0x600089>|0x9b9|+0x011:'481'
	// ******
}

// STATE[STUB]
void light::set_attenuation_power( float value )
{
	// FUNCTION BODY[0x5ff490]: 1
	// <0x5ff490>|0x000|+0x008:'485'
	// ******
}

// STATE[STUB]
void light::set_scale( float3 const& scale )
{
	// FUNCTION BODY[0x5ff470]: 1
	// <0x5ff470>|0x000|+0x015:'490'
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
	// 	vostok::render::shader_constant_binding*
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
