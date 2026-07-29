////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\stage_light_propagation_volumes.h"

namespace vostok {
namespace render {

// STATE[STUB]
void `dynamic initializer for 's_lpv_dips_skipping0_cc''( )
{
	// FUNCTION BODY[0x7d6c80]
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_lpv_dips_skipping1_cc''( )
{
	// FUNCTION BODY[0x7d6cd0]
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_draw_to_rsm_cc''( )
{
	// FUNCTION BODY[0x7d6d20]
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_use_batched_lpv_geometry_cc''( )
{
	// FUNCTION BODY[0x7d6d70]
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_start_cascade_to_use_batching_cc''( )
{
	// FUNCTION BODY[0x7d6dc0]
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_lpv0''( )
{
	// FUNCTION BODY[0x7d6e20]
	// ******
}

// STATE[STUB]
void `dynamic atexit destructor for 's_lpv_dips_skipping0_cc''( )
{
	// FUNCTION BODY[0x7eeba0]
	// ******
}

// STATE[STUB]
 stage_light_propagation_volumes::stage_light_propagation_volumes( renderer* in_renderer, renderer_context* context )
{
	// LOCALS
	// float[8] 						cascade_scales
	// float[8] 						cascade_iteration_scales
	// const D3D11_INPUT_ELEMENT_DESC[2] screen_vertex_layout
	// float[8] 						cascade_cells_scales
	// float 							fs
	// u16[6] 							indices
	// float[8] 						cascade_flux_scales
	// u8[1024] 						data
	// effect_options_descriptor 		desc
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x60982f }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1827") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x60968e }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1828") })
	// ******

	// FUNCTION BODY[0x619bf0]: 111
	// <0x619c68>|0x078|+0x045:'90'
	// <0>
	// <0x619cad>|0x0bd|+0x00e:'92'
	// <0>
	// <0x619cbb>|0x0cb|+0x017:'94'
	// <0>
	// <0x619cd2>|0x0e2|+0x009:'96'
	// <0>
	// <0x619cdb>|0x0eb|+0x009:'98'
	// <0x619ce4>|0x0f4|+0x006:'99'
	// <0>
	// <0x619cea>|0x0fa|+0x01a:'101'
	// <0>
	// <0x619d04>|0x114|+0x040:'103'
	// <0x619d44>|0x154|+0x048:'104'
	// <0x619d8c>|0x19c|+0x030:'105'
	// <0x619dbc>|0x1cc|+0x033:'106'
	// <0>
	// <0x619def>|0x1ff|+0x070:'108'
	// <0>
	// <0x619e5f>|0x26f|+0x017:'110'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x619e76>|0x286|+0x0c4:'118'
	// <0x619f3a>|0x34a|+0x01f:'119'
	// <0x619f59>|0x369|+0x034:'120'
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
	// <0x619f8d>|0x39d|+0x04d:'142'
	// <0x619fda>|0x3ea|+0x04c:'143'
	// <0x61a026>|0x436|+0x04b:'144'
	// <0x61a071>|0x481|+0x04a:'145'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x61a0bb>|0x4cb|+0x05b:'154'
	// <0x61a116>|0x526|+0x05c:'155'
	// <0x61a172>|0x582|+0x05c:'156'
	// <0x61a1ce>|0x5de|+0x05b:'157'
	// <0>
	// <0x61a229>|0x639|+0x05b:'159'
	// <0x61a284>|0x694|+0x05b:'160'
	// <0x61a2df>|0x6ef|+0x05b:'161'
	// <0x61a33a>|0x74a|+0x05b:'162'
	// <0>
	// <0x61a395>|0x7a5|+0x007:'164'
	// <0x61a39c>|0x7ac|+0x006:'165'
	// <0>
	// <0x61a3a2>|0x7b2|+0x05b:'167'
	// <0>
	// <0x61a3fd>|0x80d|+0x013:'169'
	// <0>
	// <0x61a410>|0x820|+0x009:'171'
	// <0>
	// <1>
	// <2>
	// <0x61a419>|0x829|+0x017:'175'
	// <0x61a430>|0x840|+0x074:'176'
	// <0>
	// <0x61a4a4>|0x8b4|+0x023:'178'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x61a4c7>|0x8d7|+0x2c6:'188'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x61a78d>|0xb9d|+0x052:'194'
	// <0>
	// <0x61a7df>|0xbef|+0x011:'196'
	// <0x61a7f0>|0xc00|+0x011:'197'
	// <0x61a801>|0xc11|+0x011:'198'
	// <0>
	// <0x61a812>|0xc22|+0x00b:'200'
	// ******
}

// STATE[STUB]
bool stage_light_propagation_volumes::is_effects_ready( ) const
{
	return false;

	// FUNCTION BODY[0x6146e0]: 18
	// <0x6146e0>|0x000|+0x012:'205'
	// <0x6146f2>|0x012|-0x00a:'205'
	// <0>
	// <0x6146e8>|0x008|+0x005:'207'
	// <0>
	// <1>
	// <0x6146ed>|0x00d|+0x054:'210'
	// <0x614741>|0x061|-0x043:'211'
	// <0>
	// <1>
	// <0x6146fe>|0x01e|+0x012:'214'
	// <0>
	// <0x614710>|0x030|+0x010:'216'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x614720>|0x040|+0x024:'222'
	// <0x614744>|0x064|-0x004:'222'
	// <0x614740>|0x060|+0x003:'223'
	// <0x614743>|0x063|+0x003:'223'
	// ******
}

// STATE[STUB]
 stage_light_propagation_volumes::~stage_light_propagation_volumes( )
{
	// FUNCTION BODY[0x615980]: 4
	// <0x615996>|0x016|-0x00f:'226'
	// <0x615987>|0x007|+0x011:'227'
	// <0x615998>|0x018|+0x017:'228'
	// <0>
	// <0x6159af>|0x02f|+0x020:'230'
	// ******
}

// STATE[STUB]
void stage_light_propagation_volumes::set_rsm_contants( float3 const& light_direction, float3 const& grid_origin, float grid_scale )
{
	// FUNCTION BODY[0x614cd0]: 4
	// <0x614cd4>|0x004|+0x021:'237'
	// <0x614cf5>|0x025|+0x062:'238'
	// <0x614d57>|0x087|+0x05f:'239'
	// <0x614db6>|0x0e6|+0x01a:'240'
	// ******
}

// STATE[STUB]
void stage_light_propagation_volumes::register_rsm_constans( )
{
	// FUNCTION BODY[0x615510]: 4
	// <0x615513>|0x003|+0x05d:'245'
	// <0x615570>|0x060|+0x05c:'246'
	// <0x6155cc>|0x0bc|+0x05c:'247'
	// <0x615628>|0x118|+0x05c:'248'
	// ******
}

// STATE[STUB]
void stage_light_propagation_volumes::register_light_constans( )
{
	// FUNCTION BODY[0x614de0]: 24
	// <0x614de1>|0x001|+0x063:'253'
	// <0x614e44>|0x064|+0x05c:'254'
	// <0x614ea0>|0x0c0|+0x05c:'255'
	// <0x614efc>|0x11c|+0x05c:'256'
	// <0x614f58>|0x178|+0x05c:'257'
	// <0x614fb4>|0x1d4|+0x05c:'258'
	// <0x615010>|0x230|+0x05c:'259'
	// <0x61506c>|0x28c|+0x05c:'260'
	// <0x6150c8>|0x2e8|+0x05c:'261'
	// <0x615124>|0x344|+0x05c:'262'
	// <0>
	// <0x615180>|0x3a0|+0x05c:'264'
	// <0x6151dc>|0x3fc|+0x05c:'265'
	// <0>
	// <1>
	// <0x615238>|0x458|+0x05c:'268'
	// <0>
	// <0x615294>|0x4b4|+0x05c:'270'
	// <0x6152f0>|0x510|+0x05c:'271'
	// <0x61534c>|0x56c|+0x05c:'272'
	// <0x6153a8>|0x5c8|+0x05c:'273'
	// <0x615404>|0x624|+0x056:'274'
	// <0x61545a>|0x67a|+0x056:'275'
	// <0x6154b0>|0x6d0|+0x056:'276'
	// ******
}

// STATE[STUB]
void stage_light_propagation_volumes::pre_lpv_batch_render(
	float3 const&				light_color,
	const float					light_intensity,
	geometry_batch const&		batch
)
{
	// FUNCTION BODY[0x6168f0]: 6
	// <0>
	// <0x6168fe>|0x00e|+0x028:'322'
	// <0>
	// <0x616926>|0x036|+0x014:'324'
	// <0x61693a>|0x04a|+0x024:'325'
	// <0x61695e>|0x06e|+0x044:'326'
	// ******
}

// STATE[STUB]
void stage_light_propagation_volumes::post_lpv_batch_render( geometry_batch const& batch )
{
	// FUNCTION BODY[0x614750]: 2
	// <0>
	// <0x614750>|0x000|+0x00b:'332'
	// ******
}

// STATE[STUB]
void stage_light_propagation_volumes::render_to_rms(
	float3 const&			light_color,
	const float				light_intensity,
	float4x4 const&			view_matrix,
	float4x4 const&			projection_matrix,
	vector< float4x4 >		transforms,
	const u32				cascade_index
)
{
	// LOCALS
	// D3D11_VIEWPORT 					tmp_viewport
	// D3D11_VIEWPORT 					orig_viewport
	// render_surface_instance** 		it_d
	// render_surface_instance* const* 	end_d
	// vector< render_surface_instance* > visible_render_models
	// ******

	// CALL SITE INFO
	// <0x6170c4> -> < unknown >
	// <0x6170f0> -> < unknown >
	// ******

	// FUNCTION BODY[0x616c80]: 95
	// <0x616c86>|0x006|+0x014:'343'
	// <0x616c9a>|0x01a|+0x00e:'344'
	// <0>
	// <1>
	// <0x616ca8>|0x028|+0x02b:'347'
	// <0>
	// <1>
	// <0x616cd3>|0x053|+0x01f:'350'
	// <0>
	// <1>
	// <2>
	// <0x616cf2>|0x072|+0x03a:'354'
	// <0>
	// <1>
	// <2>
	// <0x616d2c>|0x0ac|+0x033:'358'
	// <0x616d5f>|0x0df|+0x02e:'359'
	// <0>
	// <0x616d8d>|0x10d|+0x02e:'361'
	// <0x616dbb>|0x13b|+0x01a:'362'
	// <0x616dd5>|0x155|+0x035:'363'
	// <0x616e0a>|0x18a|+0x031:'364'
	// <0>
	// <0x616e3b>|0x1bb|+0x06a:'366'
	// <0>
	// <1>
	// <0x616ea5>|0x225|+0x010:'369'
	// <0>
	// <0x616eb5>|0x235|+0x00b:'371'
	// <0>
	// <0x616ec0>|0x240|+0x00b:'373'
	// <0x616ecb>|0x24b|+0x029:'374'
	// <0x616ef4>|0x274|+0x015:'375'
	// <0>
	// <1>
	// <0x616f09>|0x289|+0x087:'378'
	// <0>
	// <0x616f90>|0x310|+0x00d:'380'
	// <0>
	// <0x616f9d>|0x31d|+0x009:'382'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x616fa6>|0x326|+0x157:'388'
	// <0>
	// <0x6170fd>|0x47d|+0x0ac:'390'
	// <0>
	// <1>
	// <2>
	// <0x6171a9>|0x529|+0x030:'394'
	// <0>
	// <1>
	// <0x6171d9>|0x559|+0x004:'397'
	// <0x6171dd>|0x55d|+0x00e:'398'
	// <0>
	// <0x6171eb>|0x56b|+0x008:'400'
	// <0>
	// <0x6171f3>|0x573|+0x002:'402'
	// <0x6171f5>|0x575|+0x026:'403'
	// <0>
	// <1>
	// <0x61721b>|0x59b|+0x00a:'406'
	// <0>
	// <1>
	// <0x617225>|0x5a5|+0x00e:'409'
	// <0>
	// <1>
	// <0x617233>|0x5b3|+0x00a:'412'
	// <0>
	// <1>
	// <0x61723d>|0x5bd|+0x023:'415'
	// <0>
	// <1>
	// <0x617260>|0x5e0|+0x023:'418'
	// <0x617283>|0x603|+0x050:'419'
	// <0>
	// <1>
	// <2>
	// <0x6172d3>|0x653|+0x00c:'423'
	// <0x6172df>|0x65f|+0x00a:'424'
	// <0x6172e9>|0x669|+0x008:'425'
	// <0>
	// <0x6172f1>|0x671|+0x07b:'427'
	// <0>
	// <0x61736c>|0x6ec|+0x02b:'429'
	// <0>
	// <0x617397>|0x717|-0x298:'431'
	// <0>
	// <0x6170ff>|0x47f|+0x00d:'433'
	// <0x61710c>|0x48c|+0x00c:'434'
	// <0x617118>|0x498|+0x030:'435'
	// <0x617148>|0x4c8|+0x01e:'436'
	// <0x617166>|0x4e6|+0x019:'437'
	// ******
}

// STATE[STUB]
bool remove_model_if_in_frustum_predicate::operator()( lpv_render_surface const& surface )
{
	// LOCALS
	// math::aabb 						bbox
	// ******

	// CALL SITE INFO
	// <0x7d241> -> math::aabb < unknown >()
	// ******

	return false;

	// FUNCTION BODY[0x7d230]: 11
	// <0x7d230>|0x000|+0x013:'561'
	// <0x7d243>|0x013|+0x008:'562'
	// <0x7d24b>|0x01b|+0x00d:'563'
	// <0>
	// <0x7d258>|0x028|+0x013:'565'
	// <0>
	// <0x7d26b>|0x03b|+0x00b:'567'
	// <0x7d276>|0x046|+0x008:'568'
	// <0>
	// <1>
	// <0x7d27e>|0x04e|-0x006:'571'
	// <0x7d278>|0x048|+0x008:'572'
	// ******
}

// STATE[STUB]
void stage_light_propagation_volumes::render_to_rms_smoothed2(
	float3 const&			light_color,
	const float				light_intensity,
	float4x4 const&			view_matrix,
	float4x4 const&			projection_matrix,
	vector< float4x4 >		transforms,
	const u32				cascade_index,
	const u32				render_stage_index,
	const u32				num_render_stages
)
{
	// LOCALS
	// D3D11_VIEWPORT 					tmp_viewport
	// math::frustum 					view_frustum
	// lpv_render_surface* 				begin_d
	// D3D11_VIEWPORT 					orig_viewport
	// lpv_render_surface* 				it_d
	// u32 								render_index
	// lpv_render_surface const* 		end_d
	// float4x4 						real_view_projection
	// u32 								num_render
	// vector< render_surface_instance* > caster_models
	// render_surface_instance** 		end
	// lpv_render_surface 				surface
	// math::frustum 					prev_cascade_frustum
	// render_surface_instance& 		instance
	// ******

	// CALL SITE INFO
	// <0x6160dd> -> void < unknown >( ID3D11Resource*, ID3D11Resource* )
	// <0x61610d> -> void < unknown >( ID3D11Resource*, ID3D11Resource* )
	// <0x61613d> -> void < unknown >( ID3D11Resource*, ID3D11Resource* )
	// ******

	// FUNCTION BODY[0x615f70]: 165
	// <0x615f76>|0x006|+0x012:'606'
	// <0x615f88>|0x018|+0x006:'607'
	// <0>
	// <1>
	// <0x615f8e>|0x01e|+0x028:'610'
	// <0>
	// <1>
	// <0x615fb6>|0x046|+0x01f:'613'
	// <0>
	// <1>
	// <2>
	// <0x615fd5>|0x065|+0x03a:'617'
	// <0>
	// <1>
	// <2>
	// <0x61600f>|0x09f|+0x014:'621'
	// <0x616023>|0x0b3|+0x010:'622'
	// <0>
	// <1>
	// <0x616033>|0x0c3|+0x033:'625'
	// <0x616066>|0x0f6|+0x02e:'626'
	// <0>
	// <0x616094>|0x124|+0x00e:'628'
	// <0>
	// <0x6160a2>|0x132|+0x03d:'630'
	// <0x6160df>|0x16f|+0x030:'631'
	// <0x61610f>|0x19f|+0x030:'632'
	// <0>
	// <0x61613f>|0x1cf|+0x01e:'634'
	// <0x61615d>|0x1ed|+0x01a:'635'
	// <0x616177>|0x207|+0x035:'636'
	// <0x6161ac>|0x23c|+0x005:'637'
	// <0>
	// <1>
	// <0x6161b1>|0x241|+0x030:'640'
	// <0x6161e1>|0x271|+0x018:'641'
	// <0>
	// <1>
	// <0x6161f9>|0x289|+0x004:'644'
	// <0>
	// <0x6161fd>|0x28d|+0x008:'646'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x616205>|0x295|+0x06f:'652'
	// <0x616274>|0x304|+0x02a:'653'
	// <0>
	// <1>
	// <0x61629e>|0x32e|+0x007:'656'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6162a5>|0x335|+0x06b:'664'
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
	// <0x616310>|0x3a0|+0x011:'675'
	// <0>
	// <1>
	// <2>
	// <0x616321>|0x3b1|+0x030:'679'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x616351>|0x3e1|+0x049:'687'
	// <0>
	// <0x61639a>|0x42a|+0x01a:'689'
	// <0>
	// <0x6163b4>|0x444|+0x081:'691'
	// <0x616435>|0x4c5|+0x023:'692'
	// <0>
	// <1>
	// <0x616458>|0x4e8|+0x010:'695'
	// <0>
	// <0x616468>|0x4f8|+0x008:'697'
	// <0>
	// <0x616470>|0x500|+0x00b:'699'
	// <0x61647b>|0x50b|+0x029:'700'
	// <0x6164a4>|0x534|+0x018:'701'
	// <0>
	// <1>
	// <0x6164bc>|0x54c|+0x085:'704'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x616541>|0x5d1|+0x02d:'710'
	// <0>
	// <0x61656e>|0x5fe|+0x003:'712'
	// <0>
	// <0x616571>|0x601|+0x028:'714'
	// <0>
	// <0x616599>|0x629|+0x01b:'716'
	// <0>
	// <0x6165b4>|0x644|+0x26f:'718'
	// <0x616823>|0x8b3|+0x002:'718'
	// <0>
	// <0x616825>|0x8b5|-0x267:'720'
	// <0>
	// <1>
	// <2>
	// <0x6165be>|0x64e|+0x002:'724'
	// <0x6165c0>|0x650|+0x02e:'725'
	// <0>
	// <1>
	// <0x6165ee>|0x67e|+0x00a:'728'
	// <0>
	// <1>
	// <0x6165f8>|0x688|+0x00e:'731'
	// <0>
	// <1>
	// <0x616606>|0x696|+0x014:'734'
	// <0>
	// <1>
	// <0x61661a>|0x6aa|+0x00b:'737'
	// <0x616625>|0x6b5|+0x01e:'738'
	// <0x616643>|0x6d3|+0x002:'739'
	// <0x616645>|0x6d5|+0x029:'740'
	// <0>
	// <0x61666e>|0x6fe|+0x055:'742'
	// <0x6166c3>|0x753|+0x048:'743'
	// <0>
	// <0x61670b>|0x79b|+0x010:'745'
	// <0x61671b>|0x7ab|+0x00a:'746'
	// <0>
	// <0x616725>|0x7b5|+0x009:'748'
	// <0>
	// <0x61672e>|0x7be|+0x002:'750'
	// <0x616730>|0x7c0|+0x008:'751'
	// <0>
	// <0x616738>|0x7c8|+0x005:'753'
	// <0>
	// <0x61673d>|0x7cd|+0x02a:'755'
	// <0>
	// <0x616767>|0x7f7|-0x008:'757'
	// <0x61675f>|0x7ef|-0x008:'758'
	// <0x616757>|0x7e7|+0x016:'759'
	// <0>
	// <0x61676d>|0x7fd|+0x078:'761'
	// <0>
	// <0x6167e5>|0x875|+0x066:'763'
	// <0>
	// <1>
	// <0x61684b>|0x8db|+0x007:'766'
	// <0x616852>|0x8e2|+0x00c:'767'
	// <0x61685e>|0x8ee|+0x02d:'768'
	// <0x61688b>|0x91b|+0x01e:'769'
	// <0x6168a9>|0x939|+0x019:'770'
	// ******
}

// STATE[STUB]
float3 compute_aligment( float3 const& lightXZshift, float4x4 const& light_space_transform, float smap_res )
{
	// LOCALS
	// float4x4 						light_space_transform_invert
	// float3 							align_origin
	// float4x4 						viewport_invert
	// float4x4 						viewport
	// float4 							origin_pixel
	// ******

	return vostok::math::float3(1., 1., 1.);

	// FUNCTION BODY[0x614760]: 28
	// <0>
	// <1>
	// <0x61476c>|0x00c|+0x00c:'777'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x614778>|0x018|+0x115:'782'
	// <0>
	// <0x61488d>|0x12d|+0x030:'784'
	// <0x6148bd>|0x15d|+0x098:'785'
	// <0>
	// <0x614955>|0x1f5|+0x080:'787'
	// <0>
	// <0x6149d5>|0x275|+0x0c1:'789'
	// <0>
	// <1>
	// <2>
	// <0x614a96>|0x336|+0x0a9:'793'
	// <0x614b3f>|0x3df|+0x032:'794'
	// <0x614b71>|0x411|+0x008:'795'
	// <0x614b79>|0x419|+0x008:'796'
	// <0>
	// <1>
	// <0x614b81>|0x421|+0x08b:'799'
	// <0x614c0c>|0x4ac|+0x098:'800'
	// <0>
	// <0x614ca4>|0x544|+0x019:'802'
	// ******
}

// STATE[STUB]
void stage_light_propagation_volumes::render_to_sun_rms( light* sun, const u32 cascade_index, vector< float4x4 > transforms )
{
	// LOCALS
	// float3 							sun_position
	// float4x4 						projection_matrix
	// float 							max_scale
	// float3 							adjastment
	// float4x4 						view_matrix
	// ******

	// FUNCTION BODY[0x6188a0]: 18
	// <0x6188a6>|0x006|+0x007:'810'
	// <0>
	// <1>
	// <2>
	// <0x6188ad>|0x00d|+0x0f0:'814'
	// <0>
	// <0x61899d>|0x0fd|+0x053:'816'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6189f0>|0x150|+0x033:'822'
	// <0>
	// <0x618a23>|0x183|+0x057:'824'
	// <0x618a7a>|0x1da|+0x068:'825'
	// <0>
	// <0x618ae2>|0x242|+0x04e:'827'
	// ******
}

// STATE[STUB]
void stage_light_propagation_volumes::render_to_sun_rms_smoothed(
	light*					sun,
	const u32				cascade_index,
	vector< float4x4 >		transforms,
	const u32				stage_render_index,
	const u32				num_render_stages
)
{
	// LOCALS
	// float3 							sun_position
	// float4x4 						projection_matrix
	// float 							max_scale
	// float3 							adjastment
	// float4x4 						view_matrix
	// ******

	// FUNCTION BODY[0x6169b0]: 24
	// <0x6169b6>|0x006|+0x007:'837'
	// <0>
	// <1>
	// <2>
	// <0x6169bd>|0x00d|+0x0af:'841'
	// <0>
	// <0x616a6c>|0x0bc|+0x04e:'843'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x616aba>|0x10a|+0x030:'849'
	// <0>
	// <0x616aea>|0x13a|+0x00a:'851'
	// <0>
	// <0x616af4>|0x144|+0x016:'853'
	// <0x616b0a>|0x15a|+0x014:'854'
	// <0>
	// <1>
	// <0x616b1e>|0x16e|+0x060:'857'
	// <0x616b7e>|0x1ce|+0x06a:'858'
	// <0>
	// <0x616be8>|0x238|+0x064:'860'
	// ******
}

// STATE[STUB]
void stage_light_propagation_volumes::render_to_point_rms( light* l, const u32 face_index, vector< float4x4 > transforms )
{
	// LOCALS
	// float4x4 						face_projection_matrix
	// float4x4 						face_view_matrix
	// ******

	// FUNCTION BODY[0x618750]: 9
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x618756>|0x006|+0x0b1:'887'
	// <0>
	// <0x618807>|0x0b7|+0x02d:'889'
	// <0>
	// <0x618834>|0x0e4|+0x03a:'891'
	// ******
}

// STATE[STUB]
void stage_light_propagation_volumes::render_to_sky_rms(
	light*					sun,
	const u32				face_index,
	const u32				cascade_index,
	vector< float4x4 >		transforms
)
{
	// LOCALS
	// float4x4 						projection_matrix
	// float3 							sky_position
	// float3 							light_color
	// const float 						max_scale
	// float3 							adjastment
	// float3 							up
	// float3 							direction
	// float4x4 						view_matrix
	// float4x4 						sun_rotation
	// ******

	// FUNCTION BODY[0x618200]: 28
	// <0x618206>|0x006|+0x02c:'896'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x618232>|0x032|+0x02d:'903'
	// <0>
	// <0x61825f>|0x05f|+0x068:'905'
	// <0>
	// <0x6182c7>|0x0c7|+0x11f:'907'
	// <0x6183e6>|0x1e6|+0x14d:'908'
	// <0>
	// <0x618533>|0x333|+0x0a6:'910'
	// <0>
	// <0x6185d9>|0x3d9|+0x022:'912'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6185fb>|0x3fb|+0x033:'918'
	// <0>
	// <0x61862e>|0x42e|+0x059:'920'
	// <0x618687>|0x487|+0x05f:'921'
	// <0>
	// <0x6186e6>|0x4e6|+0x038:'923'
	// ******
}

// STATE[STUB]
void stage_light_propagation_volumes::render_to_spot_rms( light* l, vector< float4x4 > transforms )
{
	// LOCALS
	// float4x4 						projection_matrix
	// float4x4 						view_matrix
	// ******

	// FUNCTION BODY[0x618010]: 9
	// <0x61801e>|0x00e|+0x01a:'928'
	// <0>
	// <0x618038>|0x028|+0x035:'930'
	// <0>
	// <1>
	// <2>
	// <0x61806d>|0x05d|+0x12f:'934'
	// <0>
	// <0x61819c>|0x18c|+0x03a:'936'
	// ******
}

// STATE[STUB]
void stage_light_propagation_volumes::propagate_lighting( const u32 cascade_index )
{
	// FUNCTION BODY[0x615f50]: 1
	// <0x615f51>|0x001|+0x011:'975'
	// ******
}

// STATE[STUB]
void stage_light_propagation_volumes::propagate_lighting_smoothed( const u32 cascade_index, const u32 propagation_iteration_index )
{
	// FUNCTION BODY[0x615f30]: 1
	// <0x615f30>|0x000|+0x015:'980'
	// ******
}

// STATE[STUB]
void stage_light_propagation_volumes::render_quad( )
{
	// LOCALS
	// u32 								offset
	// ******

	// FUNCTION BODY[0x615760]: 11
	// <0>
	// <1>
	// <0x615765>|0x005|+0x01b:'987'
	// <0x615780>|0x020|+0x05c:'988'
	// <0x6157dc>|0x07c|+0x049:'989'
	// <0x615825>|0x0c5|+0x049:'990'
	// <0x61586e>|0x10e|+0x036:'991'
	// <0x6158a4>|0x144|+0x03d:'992'
	// <0>
	// <0x6158e1>|0x181|+0x00f:'994'
	// <0x6158f0>|0x190|+0x07d:'995'
	// ******
}

// STATE[STUB]
void stage_light_propagation_volumes::downsample_rsm(
	float3 const&		light_direction,
	float3 const&		grid_origin,
	float				grid_scale,
	const u32			cascade_index
)
{
	// LOCALS
	// D3D11_VIEWPORT 					tmp_viewport
	// D3D11_VIEWPORT 					orig_viewport
	// ******

	// FUNCTION BODY[0x615d50]: 29
	// <0x615d58>|0x008|+0x02b:'1003'
	// <0x615d83>|0x033|+0x018:'1004'
	// <0>
	// <0x615d9b>|0x04b|+0x028:'1006'
	// <0x615dc3>|0x073|+0x01a:'1007'
	// <0>
	// <1>
	// <0x615ddd>|0x08d|+0x028:'1010'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x615e05>|0x0b5|+0x01f:'1016'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x615e24>|0x0d4|+0x03a:'1021'
	// <0>
	// <0x615e5e>|0x10e|+0x021:'1023'
	// <0x615e7f>|0x12f|+0x02d:'1024'
	// <0x615eac>|0x15c|+0x01f:'1025'
	// <0>
	// <0x615ecb>|0x17b|+0x00c:'1027'
	// <0>
	// <0x615ed7>|0x187|+0x00d:'1029'
	// <0x615ee4>|0x194|+0x00c:'1030'
	// <0x615ef0>|0x1a0|+0x02d:'1031'
	// ******
}

// STATE[STUB]
void stage_light_propagation_volumes::inject_lighting(
	const u32			cascade_index,
	float3 const&		light_position,
	float3 const&		light_direction,
	float				light_fov
)
{
	// FUNCTION BODY[0x615d20]: 1
	// <0x615d20>|0x000|+0x025:'1077'
	// ******
}

// STATE[STUB]
void stage_light_propagation_volumes::inject_occluders(
	const u32				cascade_index,
	float3 const&			light_position,
	float3 const&			light_direction,
	vector< float4x4 >		transforms
)
{
	// FUNCTION BODY[0x615cc0]: 2
	// <0x615ccc>|0x00c|+0x01e:'1085'
	// <0x615cea>|0x02a|+0x015:'1086'
	// ******
}

// STATE[STUB]
void stage_light_propagation_volumes::execute_smoothed_impl(
	const u32		current_cascade_index,
	const u32		stage_index,
	const u32		propagation_iteration_index,
	const u32		render_stage_index,
	const u32		num_render_stages
)
{
	// LOCALS
	// vector< float4x4 > 				box_occluder_transforms
	// std::pair< u32, float4x4 > const* end
	// std::pair< u32, float4x4 > const* it
	// D3D11_VIEWPORT 					tmp_viewport
	// D3D11_VIEWPORT 					orig_viewport
	// float3 							smaller_cascade_grid_origin
	// u32 								pass_index
	// s32 								cascade_index
	// float 							smaller_cascade_grid_cell_size
	// float 							smaller_cascade_grid_size
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x607010 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1629") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6067c7 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1633") })
	// ******

	// FUNCTION BODY[0x6173c0]: 353
	// <0x6173c9>|0x009|+0x07e:'1098'
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
	// <0x617447>|0x087|+0x012:'1114'
	// <0>
	// <0x617459>|0x099|+0x004:'1116'
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
	// <0x61745d>|0x09d|+0x005:'1137'
	// <0>
	// <0x617462>|0x0a2|+0x00b:'1139'
	// <0>
	// <0x61746d>|0x0ad|+0x00c:'1141'
	// <0x617479>|0x0b9|+0x006:'1142'
	// <0x61747f>|0x0bf|+0x014:'1143'
	// <0x617493>|0x0d3|+0x015:'1144'
	// <0x6174a8>|0x0e8|+0x315:'1145'
	// <0x6177bd>|0x3fd|-0x1d0:'1145'
	// <0>
	// <0x6175ed>|0x22d|+0x00a:'1147'
	// <0>
	// <0x6175f7>|0x237|+0x00a:'1149'
	// <0>
	// <0x617601>|0x241|+0x017:'1151'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x617618>|0x258|+0x01b:'1156'
	// <0>
	// <1>
	// <0x617633>|0x273|+0x194:'1159'
	// <0x6177c7>|0x407|-0x141:'1159'
	// <0x617686>|0x2c6|+0x03a:'1160'
	// <0x6176c0>|0x300|+0x00c:'1161'
	// <0>
	// <0x6176cc>|0x30c|+0x02c:'1163'
	// <0>
	// <0x6176f8>|0x338|+0x015:'1165'
	// <0>
	// <1>
	// <2>
	// <0x61770d>|0x34d|+0x00a:'1169'
	// <0>
	// <0x617717>|0x357|+0x01b:'1171'
	// <0>
	// <1>
	// <2>
	// <0x617732>|0x372|+0x00f:'1175'
	// <0x617741>|0x381|+0x035:'1176'
	// <0>
	// <1>
	// <0x617776>|0x3b6|+0x00b:'1179'
	// <0>
	// <1>
	// <0x617781>|0x3c1|+0x012:'1182'
	// <0x617793>|0x3d3|+0x025:'1183'
	// <0>
	// <1>
	// <2>
	// <0x6177b8>|0x3f8|+0x016:'1187'
	// <0x6177ce>|0x40e|+0x005:'1187'
	// <0>
	// <1>
	// <0x6177d3>|0x413|+0x00e:'1190'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6177e1>|0x421|+0x01d:'1196'
	// <0>
	// <1>
	// <2>
	// <0x6177fe>|0x43e|+0x007:'1200'
	// <0>
	// <1>
	// <0x617805>|0x445|+0x00f:'1203'
	// <0x617814>|0x454|+0x032:'1204'
	// <0>
	// <1>
	// <2>
	// <0x617846>|0x486|+0x004:'1208'
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
	// <57>
	// <58>
	// <59>
	// <60>
	// <61>
	// <62>
	// <63>
	// <64>
	// <65>
	// <66>
	// <67>
	// <68>
	// <69>
	// <70>
	// <71>
	// <72>
	// <73>
	// <74>
	// <75>
	// <76>
	// <77>
	// <78>
	// <79>
	// <80>
	// <81>
	// <82>
	// <83>
	// <84>
	// <85>
	// <86>
	// <87>
	// <88>
	// <89>
	// <90>
	// <91>
	// <92>
	// <93>
	// <94>
	// <95>
	// <96>
	// <97>
	// <98>
	// <99>
	// <100>
	// <101>
	// <102>
	// <103>
	// <104>
	// <105>
	// <106>
	// <107>
	// <108>
	// <109>
	// <110>
	// <111>
	// <112>
	// <113>
	// <114>
	// <115>
	// <116>
	// <117>
	// <118>
	// <119>
	// <120>
	// <121>
	// <122>
	// <123>
	// <124>
	// <125>
	// <126>
	// <127>
	// <128>
	// <129>
	// <130>
	// <131>
	// <132>
	// <133>
	// <0x61784a>|0x48a|+0x00a:'1343'
	// <0>
	// <1>
	// <0x617854>|0x494|+0x00f:'1346'
	// <0x617863>|0x4a3|+0x014:'1347'
	// <0>
	// <1>
	// <2>
	// <0x617877>|0x4b7|+0x011:'1351'
	// <0>
	// <1>
	// <0x617888>|0x4c8|+0x014:'1354'
	// <0>
	// <1>
	// <2>
	// <0x61789c>|0x4dc|+0x016:'1358'
	// <0>
	// <0x6178b2>|0x4f2|+0x00f:'1360'
	// <0>
	// <1>
	// <0x6178c1>|0x501|+0x284:'1363'
	// <0x617b45>|0x785|-0x1f1:'1363'
	// <0x617954>|0x594|+0x018:'1364'
	// <0>
	// <1>
	// <0x61796c>|0x5ac|+0x01c:'1367'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x617988>|0x5c8|+0x02f:'1374'
	// <0>
	// <1>
	// <2>
	// <0x6179b7>|0x5f7|+0x003:'1378'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6179ba>|0x5fa|+0x5f6:'1383'
	// <0x617fb0>|0xbf0|-0x5c0:'1383'
	// <0>
	// <0x6179f0>|0x630|+0x020:'1385'
	// <0>
	// <1>
	// <0x617a10>|0x650|+0x005:'1388'
	// <0>
	// <0x617a15>|0x655|+0x137:'1390'
	// <0x617b4c>|0x78c|-0x0a5:'1390'
	// <0x617aa7>|0x6e7|+0x024:'1391'
	// <0>
	// <0x617acb>|0x70b|+0x03b:'1393'
	// <0x617b06>|0x746|+0x03a:'1394'
	// <0>
	// <0x617b40>|0x780|+0x013:'1396'
	// <0>
	// <0x617b53>|0x793|+0x4b6:'1398'
	// <0x618009>|0xc49|-0x433:'1398'
	// <0x617bd6>|0x816|+0x024:'1399'
	// <0>
	// <0x617bfa>|0x83a|+0x03c:'1401'
	// <0x617c36>|0x876|+0x03c:'1402'
	// <0>
	// <1>
	// <0x617c72>|0x8b2|+0x01e:'1405'
	// <0>
	// <0x617c90>|0x8d0|+0x00c:'1407'
	// <0>
	// <0x617c9c>|0x8dc|+0x03d:'1409'
	// <0x617cd9>|0x919|+0x030:'1410'
	// <0x617d09>|0x949|+0x02a:'1411'
	// <0>
	// <0x617d33>|0x973|+0x023:'1413'
	// <0x617d56>|0x996|+0x010:'1414'
	// <0x617d66>|0x9a6|+0x08a:'1415'
	// <0>
	// <0x617df0>|0xa30|+0x038:'1417'
	// <0x617e28>|0xa68|+0x02e:'1418'
	// <0x617e56>|0xa96|+0x01f:'1419'
	// <0>
	// <0x617e75>|0xab5|+0x013:'1421'
	// <0>
	// <0x617e88>|0xac8|+0x01c:'1423'
	// <0>
	// <1>
	// <2>
	// <0x617ea4>|0xae4|+0x036:'1427'
	// <0>
	// <0x617eda>|0xb1a|+0x010:'1429'
	// <0>
	// <0x617eea>|0xb2a|+0x02f:'1431'
	// <0x617f19>|0xb59|+0x017:'1432'
	// <0x617f30>|0xb70|+0x011:'1433'
	// <0>
	// <0x617f41>|0xb81|+0x01b:'1435'
	// <0>
	// <0x617f5c>|0xb9c|+0x003:'1437'
	// <0x617f5f>|0xb9f|+0x016:'1438'
	// <0>
	// <0x617f75>|0xbb5|+0x006:'1440'
	// <0>
	// <0x617f7b>|0xbbb|+0x00d:'1442'
	// <0x617f88>|0xbc8|+0x040:'1443'
	// <0>
	// <1>
	// <2>
	// <0x617fc8>|0xc08|+0x01d:'1447'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
void stage_light_propagation_volumes::execute_impl( )
{
	// LOCALS
	// bool 							is_need_refresh
	// vectora< collision::object const* > objects
	// vector< float4x4 > 				box_occluder_transforms
	// std::pair< u32, float4x4 > const* end
	// math::frustum 					frustum
	// std::pair< u32, float4x4 > const* it
	// s32 								cascade_index
	// collision::object const* const* 	e
	// collision::object const* const* 	i
	// s32 								face_index
	// float3 							face_direction
	// D3D11_VIEWPORT 					tmp_viewport
	// D3D11_VIEWPORT 					orig_viewport
	// u32 								pass_index
	// s32 								cascade_index
	// float 							smaller_cascade_grid_cell_size
	// float 							smaller_cascade_grid_size
	// float3 							smaller_cascade_grid_origin
	// ******

	// CALL SITE INFO
	// <0x618c47> -> bool < unknown >( u32, math::cuboid const&, vectora< collision::object const* >& )
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x608b22 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2139") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x60803c }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2148") })
	// ******

	// FUNCTION BODY[0x618b60]: 332
	// <0x618b6e>|0x00e|+0x06d:'1455'
	// <0>
	// <0x618bdb>|0x07b|+0x00f:'1457'
	// <0>
	// <0x618bea>|0x08a|+0x016:'1459'
	// <0x618c00>|0x0a0|+0x012:'1460'
	// <0>
	// <0x618c12>|0x0b2|+0x014:'1462'
	// <0x618c26>|0x0c6|+0x023:'1463'
	// <0>
	// <1>
	// <0x618c49>|0x0e9|+0x01d:'1466'
	// <0>
	// <0x618c66>|0x106|+0x00b:'1468'
	// <0>
	// <0x618c71>|0x111|+0x00d:'1470'
	// <0>
	// <0x618c7e>|0x11e|+0x004:'1472'
	// <0>
	// <1>
	// <0x618c82>|0x122|+0x02e:'1475'
	// <0x618cb0>|0x150|-0x020:'1475'
	// <0>
	// <0x618c90>|0x130|+0x005:'1477'
	// <0>
	// <0x618c95>|0x135|+0x009:'1479'
	// <0>
	// <1>
	// <0x618c9e>|0x13e|+0x00e:'1482'
	// <0>
	// <1>
	// <2>
	// <0x618cac>|0x14c|+0x00b:'1486'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x618cb7>|0x157|+0x00b:'1493'
	// <0>
	// <1>
	// <2>
	// <0x618cc2>|0x162|+0x006:'1497'
	// <0x618cc8>|0x168|+0x011:'1498'
	// <0x618cd9>|0x179|+0x019:'1499'
	// <0x618cf2>|0x192|+0x00f:'1500'
	// <0x618d01>|0x1a1|+0x331:'1501'
	// <0x619032>|0x4d2|-0x1ea:'1501'
	// <0>
	// <0x618e48>|0x2e8|+0x015:'1503'
	// <0>
	// <0x618e5d>|0x2fd|+0x013:'1505'
	// <0x618e70>|0x310|+0x015:'1506'
	// <0>
	// <1>
	// <2>
	// <0x618e85>|0x325|+0x1b7:'1510'
	// <0x61903c>|0x4dc|-0x168:'1510'
	// <0x618ed4>|0x374|+0x03a:'1511'
	// <0x618f0e>|0x3ae|+0x021:'1512'
	// <0>
	// <0x618f2f>|0x3cf|+0x011:'1514'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x618f40>|0x3e0|+0x026:'1519'
	// <0>
	// <0x618f66>|0x406|+0x010:'1521'
	// <0x618f76>|0x416|+0x019:'1522'
	// <0>
	// <1>
	// <0x618f8f>|0x42f|+0x019:'1525'
	// <0>
	// <1>
	// <2>
	// <0x618fa8>|0x448|+0x028:'1529'
	// <0>
	// <1>
	// <2>
	// <0x618fd0>|0x470|+0x013:'1533'
	// <0x618fe3>|0x483|+0x060:'1534'
	// <0x619043>|0x4e3|+0x05e:'1534'
	// <0>
	// <1>
	// <2>
	// <0x6190a1>|0x541|+0x00f:'1538'
	// <0x6190b0>|0x550|+0x024:'1539'
	// <0>
	// <1>
	// <2>
	// <0x6190d4>|0x574|+0x00f:'1543'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6190e3>|0x583|+0x023:'1549'
	// <0>
	// <1>
	// <2>
	// <0x619106>|0x5a6|+0x013:'1553'
	// <0x619119>|0x5b9|+0x0d1:'1554'
	// <0>
	// <1>
	// <0x6191ea>|0x68a|+0x004:'1557'
	// <0>
	// <1>
	// <0x6191ee>|0x68e|+0x2b3:'1560'
	// <0x6194a1>|0x941|-0x29b:'1560'
	// <0>
	// <0x619206>|0x6a6|+0x009:'1562'
	// <0>
	// <0x61920f>|0x6af|+0x00d:'1564'
	// <0>
	// <0x61921c>|0x6bc|+0x01b:'1566'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x619237>|0x6d7|+0x00e:'1571'
	// <0x619245>|0x6e5|+0x016:'1572'
	// <0>
	// <1>
	// <0x61925b>|0x6fb|+0x018:'1575'
	// <0>
	// <0x619273>|0x713|+0x00b:'1577'
	// <0>
	// <1>
	// <0x61927e>|0x71e|+0x00f:'1580'
	// <0x61928d>|0x72d|+0x025:'1581'
	// <0>
	// <1>
	// <2>
	// <0x6192b2>|0x752|+0x00f:'1585'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6192c1>|0x761|+0x048:'1591'
	// <0>
	// <1>
	// <2>
	// <0x619309>|0x7a9|+0x00e:'1595'
	// <0x619317>|0x7b7|+0x03d:'1596'
	// <0>
	// <1>
	// <2>
	// <0x619354>|0x7f4|+0x004:'1600'
	// <0>
	// <0x619358>|0x7f8|+0x005:'1602'
	// <0>
	// <1>
	// <0x61935d>|0x7fd|+0x126:'1605'
	// <0x619483>|0x923|-0x113:'1605'
	// <0>
	// <0x619370>|0x810|+0x004:'1607'
	// <0>
	// <1>
	// <0x619374>|0x814|+0x020:'1610'
	// <0x619394>|0x834|+0x01a:'1611'
	// <0>
	// <1>
	// <0x6193ae>|0x84e|+0x01a:'1614'
	// <0>
	// <0x6193c8>|0x868|+0x00c:'1616'
	// <0>
	// <1>
	// <0x6193d4>|0x874|+0x00f:'1619'
	// <0x6193e3>|0x883|+0x022:'1620'
	// <0>
	// <1>
	// <2>
	// <0x619405>|0x8a5|+0x00f:'1624'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x619414>|0x8b4|+0x025:'1630'
	// <0>
	// <1>
	// <2>
	// <0x619439>|0x8d9|+0x00f:'1634'
	// <0x619448>|0x8e8|+0x04b:'1635'
	// <0>
	// <1>
	// <2>
	// <0x619493>|0x933|+0x02d:'1639'
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
	// <0x6194c0>|0x960|+0x00f:'1690'
	// <0x6194cf>|0x96f|+0x011:'1691'
	// <0x6194e0>|0x980|+0x017:'1692'
	// <0>
	// <1>
	// <0x6194f7>|0x997|+0x00f:'1695'
	// <0>
	// <1>
	// <2>
	// <0x619506>|0x9a6|+0x013:'1699'
	// <0>
	// <1>
	// <2>
	// <0x619519>|0x9b9|+0x01d:'1703'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x619536>|0x9d6|+0x029:'1710'
	// <0>
	// <1>
	// <2>
	// <0x61955f>|0x9ff|+0x003:'1714'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x619562>|0xa02|+0x54a:'1719'
	// <0x619aac>|0xf4c|-0x51c:'1719'
	// <0>
	// <0x619590>|0xa30|+0x010:'1721'
	// <0>
	// <1>
	// <0x6195a0>|0xa40|+0x005:'1724'
	// <0>
	// <0x6195a5>|0xa45|+0x102:'1726'
	// <0x6196a7>|0xb47|-0x06f:'1726'
	// <0x619638>|0xad8|+0x024:'1727'
	// <0>
	// <0x61965c>|0xafc|+0x03c:'1729'
	// <0x619698>|0xb38|+0x00a:'1730'
	// <0>
	// <0x6196a2>|0xb42|+0x00c:'1732'
	// <0>
	// <0x6196ae>|0xb4e|+0x46d:'1734'
	// <0x619b1b>|0xfbb|-0x3e7:'1734'
	// <0x619734>|0xbd4|+0x024:'1735'
	// <0>
	// <0x619758>|0xbf8|+0x03c:'1737'
	// <0x619794>|0xc34|+0x03c:'1738'
	// <0>
	// <1>
	// <0x6197d0>|0xc70|+0x01f:'1741'
	// <0>
	// <0x6197ef>|0xc8f|+0x00c:'1743'
	// <0>
	// <0x6197fb>|0xc9b|+0x03e:'1745'
	// <0x619839>|0xcd9|+0x030:'1746'
	// <0x619869>|0xd09|+0x028:'1747'
	// <0>
	// <0x619891>|0xd31|+0x020:'1749'
	// <0x6198b1>|0xd51|+0x010:'1750'
	// <0x6198c1>|0xd61|+0x046:'1751'
	// <0>
	// <0x619907>|0xda7|+0x036:'1753'
	// <0x61993d>|0xddd|+0x028:'1754'
	// <0x619965>|0xe05|+0x01e:'1755'
	// <0>
	// <0x619983>|0xe23|+0x013:'1757'
	// <0>
	// <0x619996>|0xe36|+0x01c:'1759'
	// <0>
	// <1>
	// <2>
	// <0x6199b2>|0xe52|+0x033:'1763'
	// <0>
	// <0x6199e5>|0xe85|+0x010:'1765'
	// <0>
	// <0x6199f5>|0xe95|+0x026:'1767'
	// <0x619a1b>|0xebb|+0x014:'1768'
	// <0x619a2f>|0xecf|+0x011:'1769'
	// <0>
	// <0x619a40>|0xee0|+0x01b:'1771'
	// <0>
	// <0x619a5b>|0xefb|+0x003:'1773'
	// <0x619a5e>|0xefe|+0x016:'1774'
	// <0>
	// <0x619a74>|0xf14|+0x006:'1776'
	// <0>
	// <0x619a7a>|0xf1a|+0x00d:'1778'
	// <0x619a87>|0xf27|+0x037:'1779'
	// <0>
	// <1>
	// <2>
	// <0x619abe>|0xf5e|+0x023:'1783'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
void stage_light_propagation_volumes::execute_disabled( )
{
	// FUNCTION BODY[0x615690]: 2
	// <0x615690>|0x000|+0x0ba:'1791'
	// <0x61574a>|0x0ba|-0x019:'1791'
	// <0x615731>|0x0a1|+0x018:'1792'
	// ******
}

// STATE[STUB]
void stage_light_propagation_volumes::execute( )
{
	// LOCALS
	// u32 								propagation_step_index
	// ******

	// CALL SITE INFO
	// <0x619b62> -> bool < unknown >() const
	// <0x619bdc> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x619b30]: 66
	// <0>
	// <1>
	// <0x619b3f>|0x00f|+0x00d:'1799'
	// <0>
	// <1>
	// <0x619b4c>|0x01c|+0x089:'1802'
	// <0>
	// <0x619bd5>|0x0a5|-0x06d:'1804'
	// <0>
	// <1>
	// <2>
	// <0x619b68>|0x038|+0x009:'1808'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x619b71>|0x041|+0x028:'1814'
	// <0>
	// <0x619b99>|0x069|+0x005:'1816'
	// <0x619b9e>|0x06e|+0x007:'1817'
	// <0>
	// <1>
	// <0x619ba5>|0x075|+0x005:'1820'
	// <0x619baa>|0x07a|+0x01d:'1821'
	// <0>
	// <1>
	// <0x619bc7>|0x097|-0x008:'1824'
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
	// <0x619bbf>|0x08f|+0x00e:'1863'
	// <0x619bcd>|0x09d|+0x011:'1863'
	// ******
}

// STATE[STUB]
void stage_light_propagation_volumes::draw_debug( )
{
	// FUNCTION BODY[0x6146d0]: 5
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// ******
}

	// TYPEDEFS
	// typedef
	// 	float4x4*
	// 	iterator_type;

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
	// 	vostok::render::lpv_render_surface*
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
