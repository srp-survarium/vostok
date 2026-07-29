////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\core\dx11\sources\shader_constant_table.h"

namespace vostok {
namespace render {

// STATE[STUB]
bool res_const_table_predicates::sort( shader_constant const& c1, shader_constant const& c2 )
{
	return false;

	// FUNCTION BODY[0x12ea70]: 1
	// <0x12ea70>|0x000|+0x018:'24'
	// ******
}

// STATE[STUB]
void shader_constant_table::destroy_impl( ) const
{
	// FUNCTION BODY[0x739360]: 0
	// ******
}

// STATE[STUB]
shader_constant* shader_constant_table::get( shared_string const& name )
{
	return NULL;

	// FUNCTION BODY[0x7390d0]: 9
	// <0>
	// <0x7390d0>|0x000|+0x003:'52'
	// <0x7390d3>|0x003|+0x004:'53'
	// <0>
	// <0x7390d7>|0x007|+0x009:'55'
	// <0x7390e0>|0x010|+0x00f:'56'
	// <0>
	// <1>
	// <0x7390ef>|0x01f|+0x003:'59'
	// ******
}

// STATE[STUB]
shader_constant* shader_constant_table::get( pcstr const name )
{
	return NULL;

	// FUNCTION BODY[0x7392e0]: 9
	// <0x7392e7>|0x007|+0x004:'64'
	// <0x7392eb>|0x00b|+0x003:'65'
	// <0>
	// <0x7392ee>|0x00e|+0x004:'67'
	// <0x7392f2>|0x012|+0x04d:'68'
	// <0>
	// <1>
	// <2>
	// <0x73933f>|0x05f|+0x002:'72'
	// ******
}

// STATE[STUB]
bool shader_constant_table::parse_constant_buffer( ID3D11ShaderReflectionConstantBuffer* src_table, u32 buffer_index )
{
	// LOCALS
	// _D3D11_SHADER_BUFFER_DESC 		shader_buffer_desc
	// bool 							do_debug_break
	// u32 								i
	// _D3D11_SHADER_TYPE_DESC 			reflection_type_desc
	// _D3D11_SHADER_VARIABLE_DESC 		variable_desc
	// u16 								r_index
	// shader_constant 					new_const
	// ******

	// STATICS
	// static < NoType > 				 = <0x7395e4>;
	// static bool 						ignore_always = <0x4c266ea>;
	// ******

	// CALL SITE INFO
	// <0x73938b> -> HRESULT < unknown >( _D3D11_SHADER_BUFFER_DESC* )
	// <0x7393fe> -> ID3D11ShaderReflectionVariable* < unknown >( u32 )
	// <0x73940c> -> HRESULT < unknown >( _D3D11_SHADER_VARIABLE_DESC* )
	// <0x739422> -> ID3D11ShaderReflectionType* < unknown >()
	// <0x73942e> -> HRESULT < unknown >( _D3D11_SHADER_TYPE_DESC* )
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7284df }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN210") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7285c0 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN47") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x728485 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN30") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x72848c }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN29") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7284b2 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN22") })
	// ******

	return false;

	// FUNCTION BODY[0x739370]: 170
	// <0>
	// <1>
	// <2>
	// <0x739379>|0x009|+0x014:'80'
	// <0x73938d>|0x01d|+0x04f:'81'
	// <0>
	// <0x7393dc>|0x06c|+0x01b:'83'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x7393f7>|0x087|+0x00b:'90'
	// <0>
	// <1>
	// <0x739402>|0x092|+0x00c:'93'
	// <0>
	// <0x73940e>|0x09e|+0x00e:'95'
	// <0>
	// <1>
	// <0x73941c>|0x0ac|+0x008:'98'
	// <0>
	// <1>
	// <0x739424>|0x0b4|+0x00c:'101'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x739430>|0x0c0|+0x01e:'108'
	// <0>
	// <1>
	// <0x73944e>|0x0de|+0x002:'111'
	// <0x739450>|0x0e0|+0x009:'112'
	// <0>
	// <0x739459>|0x0e9|-0x007:'114'
	// <0>
	// <1>
	// <0x739452>|0x0e2|+0x005:'117'
	// <0x739457>|0x0e7|+0x007:'118'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x73945e>|0x0ee|+0x005:'125'
	// <0>
	// <0x739463>|0x0f3|+0x009:'127'
	// <0>
	// <1>
	// <2>
	// <0x73946c>|0x0fc|+0x019:'131'
	// <0>
	// <1>
	// <0x739485>|0x115|+0x005:'134'
	// <0x73948a>|0x11a|+0x002:'135'
	// <0>
	// <1>
	// <0x73948c>|0x11c|+0x013:'138'
	// <0>
	// <1>
	// <0x73949f>|0x12f|+0x003:'141'
	// <0x7394a2>|0x132|+0x002:'142'
	// <0>
	// <0x7394a4>|0x134|+0x005:'144'
	// <0x7394a9>|0x139|+0x002:'145'
	// <0>
	// <0x7394ab>|0x13b|+0x005:'147'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x7394b0>|0x140|+0x002:'154'
	// <0>
	// <1>
	// <0x7394b2>|0x142|+0x007:'157'
	// <0>
	// <1>
	// <0x7394b9>|0x149|+0x021:'160'
	// <0>
	// <1>
	// <0x7394da>|0x16a|-0x007:'163'
	// <0>
	// <1>
	// <0x7394d3>|0x163|+0x005:'166'
	// <0x7394d8>|0x168|-0x00c:'167'
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
	// <0x7394cc>|0x15c|+0x005:'180'
	// <0>
	// <0x7394d1>|0x161|+0x00e:'182'
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
	// <0x7394df>|0x16f|+0x00d:'215'
	// <0>
	// <1>
	// <2>
	// <0x7394ec>|0x17c|+0x058:'219'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x739544>|0x1d4|+0x008:'225'
	// <0x73954c>|0x1dc|+0x020:'226'
	// <0x73956c>|0x1fc|+0x016:'227'
	// <0>
	// <0x739582>|0x212|+0x051:'229'
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
	// ******
}

// STATE[STUB]
bool shader_constant_table::parse( ID3D11ShaderReflection* shader_reflection, enum_shader_type destination )
{
	// LOCALS
	// _D3D11_SHADER_DESC 				shader_desc
	// u16 								buff_ind
	// _D3D11_SHADER_BUFFER_DESC 		const_buffer_Desc
	// bool 							do_debug_break
	// shader_constant_buffer_ptr 		temp_buffer
	// ******

	// STATICS
	// static bool 						ignore_always = <0x4c266eb>;
	// ******

	// CALL SITE INFO
	// <0x739665> -> HRESULT < unknown >( _D3D11_SHADER_DESC* )
	// <0x7396ae> -> ID3D11ShaderReflectionConstantBuffer* < unknown >( u32 )
	// <0x7396bc> -> HRESULT < unknown >( _D3D11_SHADER_BUFFER_DESC* )
	// ******

	return false;

	// FUNCTION BODY[0x739600]: 32
	// <0x739607>|0x007|+0x049:'252'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x739650>|0x050|+0x017:'257'
	// <0>
	// <0x739667>|0x067|+0x00f:'259'
	// <0>
	// <0x739676>|0x076|+0x00a:'261'
	// <0>
	// <1>
	// <2>
	// <0x739680>|0x080|+0x027:'265'
	// <0>
	// <0x7396a7>|0x0a7|+0x00b:'267'
	// <0>
	// <0x7396b2>|0x0b2|+0x00c:'269'
	// <0x7396be>|0x0be|+0x052:'270'
	// <0>
	// <1>
	// <2>
	// <0x739710>|0x110|+0x00f:'274'
	// <0>
	// <0x73971f>|0x11f|+0x07c:'276'
	// <0x73979b>|0x19b|+0x02d:'277'
	// <0x7397c8>|0x1c8|+0x02e:'278'
	// <0>
	// <1>
	// <2>
	// <0x7397f6>|0x1f6|+0x054:'282'
	// <0>
	// ******
}

// STATE[STUB]
void shader_constant_table::clear( )
{
	// FUNCTION BODY[0x7392a0]: 2
	// <0x7392a1>|0x001|+0x018:'328'
	// <0x7392b9>|0x019|+0x020:'329'
	// ******
}

// STATE[STUB]
s32 shader_constant_table::compare( shader_constant_table const& other ) const
{
	// LOCALS
	// u32 								size
	// ******

	return 0;

	// FUNCTION BODY[0x739190]: 16
	// <0x739193>|0x003|+0x044:'348'
	// <0x7391d7>|0x047|+0x01d:'349'
	// <0>
	// <0x7391f4>|0x064|+0x00b:'351'
	// <0x7391ff>|0x06f|+0x091:'352'
	// <0x739290>|0x100|-0x00b:'353'
	// <0>
	// <0x739285>|0x0f5|-0x06f:'355'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x739216>|0x086|+0x032:'360'
	// <0x739248>|0x0b8|+0x038:'361'
	// <0>
	// <1>
	// <0x739280>|0x0f0|+0x00b:'364'
	// <0x73928b>|0x0fb|+0x009:'364'
	// ******
}

// STATE[STUB]
void shader_constant_table::apply_bindings( shader_constant_bindings const& bindings )
{
	// FUNCTION BODY[0x739100]: 38
	// <0>
	// <0x739102>|0x002|+0x007:'369'
	// <0>
	// <0x739109>|0x009|+0x007:'371'
	// <0>
	// <0x739110>|0x010|+0x022:'373'
	// <0>
	// <0x739132>|0x032|+0x009:'375'
	// <0>
	// <1>
	// <0x73913b>|0x03b|+0x010:'378'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x73914b>|0x04b|+0x00b:'385'
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
	// <0x739156>|0x056|+0x026:'401'
	// <0>
	// <1>
	// <2>
	// <3>
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
	// 	void**
	// 	iterator_type;

	// typedef
	// 	vostok::fixed_string< 32 >*
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

	// ******

} // namespace render
} // namespace vostok
