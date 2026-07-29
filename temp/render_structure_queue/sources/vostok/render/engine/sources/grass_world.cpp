////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\grass_world.h"

namespace vostok {
namespace render {

// STATE[STUB]
void `dynamic initializer for 's_draw_grass_debug''( )
{
	// FUNCTION BODY[0x7d64c0]
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_use_grass_patches_sorting''( )
{
	// FUNCTION BODY[0x7d6510]
	// ******
}

// STATE[STUB]
void `dynamic atexit destructor for 's_draw_grass_debug''( )
{
	// FUNCTION BODY[0x7ee390]
	// ******
}

// STATE[STUB]
 grass_world::grass_world( )
{
	// CALL SITE INFO
	// <0x6365b6> -> unsigned long < unknown >()
	// ******

	// FUNCTION BODY[0x6364e0]: 15
	// <0x6364ec>|0x00c|+0x06f:'54'
	// <0>
	// <1>
	// <2>
	// <0x63655b>|0x07b|+0x05b:'58'
	// <0x6365b6>|0x0d6|+0x006:'59'
	// <0x6365bc>|0x0dc|+0x079:'60'
	// <0x636635>|0x155|+0x05b:'61'
	// <0x636690>|0x1b0|+0x05b:'62'
	// <0x6366eb>|0x20b|+0x05b:'63'
	// <0x636746>|0x266|+0x05b:'64'
	// <0x6367a1>|0x2c1|+0x05b:'65'
	// <0x6367fc>|0x31c|+0x05b:'66'
	// <0x636857>|0x377|+0x05c:'67'
	// <0x6368b3>|0x3d3|+0x05b:'68'
	// ******
}

// STATE[STUB]
void grass_world::set_wind_parameters( float2 const& dir, const float strength )
{
	// FUNCTION BODY[0x635d10]: 1
	// <0x635d13>|0x003|+0x073:'73'
	// ******
}

// STATE[STUB]
void grass_world::set_patch_parameters( grass_patch* patch )
{
	// FUNCTION BODY[0x635c70]: 1
	// <0x635c73>|0x003|+0x088:'78'
	// ******
}

// STATE[STUB]
void grass_world::set_trample_parameters( trample_desc& desc )
{
	// FUNCTION BODY[0x635c40]: 1
	// <0x635c41>|0x001|+0x020:'83'
	// ******
}

// STATE[STUB]
void grass_world::set_shadow_parameters( const u32 cascade_index )
{
	// FUNCTION BODY[0x635be0]: 1
	// <0x635be1>|0x001|+0x057:'88'
	// ******
}

// STATE[STUB]
 grass_world::~grass_world( )
{
	// FUNCTION BODY[0x6370f0]: 2
	// <0x6370f8>|0x008|+0x00d:'93'
	// <0x637105>|0x015|+0x02f:'94'
	// ******
}

// STATE[STUB]
u32 grass_world::add_template( grass_render_model_ptr const& in_render_model )
{
	return 0;

	// FUNCTION BODY[0x636920]: 5
	// <0>
	// <1>
	// <2>
	// <0x636921>|0x001|+0x055:'103'
	// <0x636976>|0x056|-0x008:'103'
	// <0x63696e>|0x04e|+0x002:'104'
	// <0x636970>|0x050|+0x015:'105'
	// ******
}

// STATE[STUB]
void grass_world::add_trample( trample_desc const& desc )
{
	// FUNCTION BODY[0x635f10]: 1
	// <0x635f11>|0x001|+0x031:'109'
	// <0x635f42>|0x032|-0x001:'109'
	// <0x635f41>|0x031|+0x008:'110'
	// ******
}

// STATE[STUB]
u32 grass_world::add_instance(
	const u32				in_template_id,
	math::color const&		in_color,
	float4x4 const&			in_transform,
	const u8				in_layer,
	const float				in_wind_scale
)
{
	// LOCALS
	// grass_instance* 					instance
	// ******

	return 0;

	// FUNCTION BODY[0x636090]: 16
	// <0>
	// <0x636094>|0x004|+0x00a:'142'
	// <0>
	// <1>
	// <0x63609e>|0x00e|+0x006:'145'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6360a4>|0x014|+0x04c:'152'
	// <0>
	// <0x6360f0>|0x060|+0x01e:'154'
	// <0x63610e>|0x07e|-0x009:'154'
	// <0>
	// <0x636105>|0x075|+0x002:'156'
	// <0x636107>|0x077|+0x016:'157'
	// ******
}

// STATE[STUB]
void grass_world::remove_instance( const u32 in_id )
{
	// FUNCTION BODY[0x635d90]: 28
	// <0x635d91>|0x001|+0x007:'161'
	// <0x635d98>|0x008|+0x009:'162'
	// <0>
	// <0x635da1>|0x011|+0x020:'164'
	// <0x635dc1>|0x031|-0x01a:'164'
	// <0>
	// <0x635da7>|0x017|+0x002:'166'
	// <0>
	// <0x635da9>|0x019|+0x003:'168'
	// <0x635dac>|0x01c|+0x003:'169'
	// <0>
	// <0x635daf>|0x01f|+0x004:'171'
	// <0>
	// <0x635db3>|0x023|+0x002:'173'
	// <0>
	// <0x635db5>|0x025|+0x019:'175'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x635dce>|0x03e|+0x013:'183'
	// <0x635de1>|0x051|-0x015:'184'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x635dcc>|0x03c|+0x03b:'189'
	// ******
}

// STATE[STUB]
grass_template* grass_world::id_to_template( const u32 id ) const
{
	return NULL;

	// FUNCTION BODY[0x635a70]: 15
	// <0>
	// <0x635a7a>|0x00a|+0x00e:'194'
	// <0x635a88>|0x018|+0x011:'195'
	// <0>
	// <0x635a99>|0x029|+0x007:'197'
	// <0>
	// <1>
	// <0x635aa0>|0x030|+0x011:'200'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x635ab1>|0x041|+0x009:'207'
	// <0x635aba>|0x04a|-0x006:'207'
	// <0x635ab4>|0x044|+0x009:'208'
	// ******
}

// STATE[STUB]
grass_template* grass_world::find_template( grass_render_model_ptr const& model ) const
{
	return NULL;

	// FUNCTION BODY[0x635a20]: 13
	// <0>
	// <0x635a25>|0x005|+0x00e:'213'
	// <0x635a33>|0x013|+0x011:'214'
	// <0>
	// <0x635a44>|0x024|+0x00c:'216'
	// <0>
	// <0x635a50>|0x030|+0x00f:'218'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x635a5f>|0x03f|+0x008:'224'
	// <0x635a67>|0x047|-0x005:'224'
	// <0x635a62>|0x042|+0x008:'225'
	// ******
}

// STATE[STUB]
grass_patch* grass_world::find_patch( float3 const& point )
{
	return NULL;

	// FUNCTION BODY[0x6359c0]: 15
	// <0x6359c0>|0x000|+0x006:'266'
	// <0x6359c6>|0x006|+0x006:'267'
	// <0>
	// <0x6359cc>|0x00c|+0x03b:'269'
	// <0x635a07>|0x047|-0x033:'269'
	// <0>
	// <0x6359d4>|0x014|+0x002:'271'
	// <0>
	// <1>
	// <2>
	// <0x6359d6>|0x016|+0x038:'275'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x635a0e>|0x04e|+0x002:'280'
	// ******
}

// STATE[STUB]
void grass_world::remove_patches( )
{
	// LOCALS
	// grass_patch* const* 				end_p
	// ******

	// FUNCTION BODY[0x636ee0]: 10
	// <0>
	// <0x636ee3>|0x003|+0x013:'286'
	// <0>
	// <0x636ef6>|0x016|+0x00a:'288'
	// <0>
	// <0x636f00>|0x020|+0x002:'290'
	// <0x636f02>|0x022|+0x029:'291'
	// <0>
	// <0x636f2b>|0x04b|+0x03a:'293'
	// <0x636f65>|0x085|+0x03a:'294'
	// ******
}

// STATE[STUB]
void grass_world::populate( const float patch_size_ground )
{
	// LOCALS
	// grass_template** 				end
	// grass_template** 				it
	// grass_instance** 				it_instance
	// grass_instance** 				end_instance
	// grass_template* 					templ
	// float3 							origin
	// float3 							origin_aligned
	// grass_instance* 					instance
	// grass_patch* 					new_patch
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x627140 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN713") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x627140 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN716") })
	// ******

	// FUNCTION BODY[0x637b90]: 61
	// <0x637b9f>|0x00f|+0x005:'299'
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
	// <0x637ba4>|0x014|+0x006:'312'
	// <0x637baa>|0x01a|+0x00e:'313'
	// <0>
	// <1>
	// <2>
	// <0x637bb8>|0x028|+0x538:'317'
	// <0x6380f0>|0x560|-0x510:'317'
	// <0>
	// <0x637be0>|0x050|+0x006:'319'
	// <0>
	// <0x637be6>|0x056|+0x007:'321'
	// <0x637bed>|0x05d|+0x00b:'322'
	// <0>
	// <1>
	// <2>
	// <0x637bf8>|0x068|+0x008:'326'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x637c00>|0x070|+0x117:'335'
	// <0>
	// <0x637d17>|0x187|+0x072:'337'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x637d89>|0x1f9|+0x042:'346'
	// <0>
	// <0x637dcb>|0x23b|+0x36b:'348'
	// <0x638136>|0x5a6|-0x24c:'348'
	// <0>
	// <0x637eea>|0x35a|+0x0c4:'350'
	// <0>
	// <1>
	// <2>
	// <0x637fae>|0x41e|+0x157:'354'
	// <0x638105>|0x575|+0x003:'354'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x638108>|0x578|+0x025:'359'
	// ******
}

// STATE[STUB]
void grass_world::merge_patches( )
{
	// FUNCTION BODY[0x6370c0]: 9
	// <0>
	// <0x6370c1>|0x001|+0x00e:'365'
	// <0>
	// <0x6370cf>|0x00f|+0x004:'367'
	// <0>
	// <0x6370d3>|0x013|+0x002:'369'
	// <0x6370d5>|0x015|+0x006:'370'
	// <0x6370db>|0x01b|+0x00d:'371'
	// <0>
	// ******
}

// STATE[STUB]
void grass_world::render_debug( renderer_context* context )
{
	// LOCALS
	// math::random32 					r
	// grass_patch* const* 				end
	// float 							fpatch_index
	// grass_patch* const* 				it
	// const float 						fnum_patches
	// float 							finstance_index
	// grass_patch* 					patch
	// grass_instance** 				it_instance
	// grass_instance** 				end_instance
	// const float 						fnum_instances
	// float3 							origin
	// vertex_colored[2] 				vertices
	// u16[2] 							indices
	// ******

	// FUNCTION BODY[0x637270]: 51
	// <0x637276>|0x006|+0x011:'377'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x637287>|0x017|+0x00c:'383'
	// <0>
	// <0x637293>|0x023|+0x021:'385'
	// <0x6372b4>|0x044|+0x00d:'386'
	// <0>
	// <0x6372c1>|0x051|+0x008:'388'
	// <0>
	// <0x6372c9>|0x059|+0x646:'390'
	// <0x63790f>|0x69f|-0x60d:'390'
	// <0>
	// <0x637302>|0x092|+0x002:'392'
	// <0>
	// <1>
	// <2>
	// <0x637304>|0x094|+0x235:'396'
	// <0>
	// <1>
	// <0x637539>|0x2c9|+0x00c:'399'
	// <0>
	// <0x637545>|0x2d5|+0x021:'401'
	// <0x637566>|0x2f6|+0x02e:'402'
	// <0x637594>|0x324|-0x027:'402'
	// <0>
	// <0x63756d>|0x2fd|+0x031:'404'
	// <0>
	// <0x63759e>|0x32e|+0x002:'406'
	// <0x6375a0>|0x330|+0x008:'407'
	// <0>
	// <0x6375a8>|0x338|+0x004:'409'
	// <0>
	// <1>
	// <2>
	// <0x6375ac>|0x33c|+0x214:'413'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6377c0>|0x550|+0x0ab:'418'
	// <0x63786b>|0x5fb|+0x002:'419'
	// <0x63786d>|0x5fd|+0x079:'420'
	// <0>
	// <1>
	// <0x6378e6>|0x676|+0x02d:'423'
	// <0>
	// <1>
	// <0x637913>|0x6a3|+0x021:'426'
	// <0>
	// ******
}

// STATE[STUB]
bool sort_grass_patch_predicate::operator()( grass_patch const* left, grass_patch const* right ) const
{
	return false;

	// FUNCTION BODY[0x70670]: 2
	// <0>
	// <0x70674>|0x004|+0x09d:'438'
	// <0x70711>|0x0a1|-0x003:'438'
	// <0x7070e>|0x09e|+0x005:'439'
	// ******
}

// STATE[STUB]
void grass_world::process_sorting( float3 const& viewer_position, bool sort_instances )
{
	// FUNCTION BODY[0x636130]: 26
	// <0x636131>|0x001|+0x013:'446'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x636144>|0x014|+0x028:'451'
	// <0>
	// <0x63616c>|0x03c|+0x010:'453'
	// <0>
	// <0x63617c>|0x04c|+0x006:'455'
	// <0x636182>|0x052|+0x006:'456'
	// <0>
	// <0x636188>|0x058|+0x002:'458'
	// <0x63618a>|0x05a|+0x006:'459'
	// <0>
	// <0x636190>|0x060|+0x005:'461'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x636195>|0x065|+0x00c:'467'
	// <0>
	// <0x6361a1>|0x071|+0x005:'469'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void grass_world::process_culling( renderer_context* context, const float first_lod_distance )
{
	// LOCALS
	// vectora< collision::object const* > objects
	// math::frustum 					view_frustum
	// collision::object const* const* 	end
	// collision::object const* const* 	it
	// const float 						to_aabb_center_squared
	// ******

	// CALL SITE INFO
	// <0x636269> -> bool < unknown >( u32, math::cuboid const&, vectora< collision::object const* >& )
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6254dc }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN351") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6254dc }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN353") })
	// ******

	// FUNCTION BODY[0x6361b0]: 39
	// <0x6361b0>|0x000|+0x043:'477'
	// <0>
	// <0x6361f3>|0x043|+0x01b:'479'
	// <0>
	// <1>
	// <0x63620e>|0x05e|+0x030:'482'
	// <0>
	// <0x63623e>|0x08e|+0x016:'484'
	// <0>
	// <0x636254>|0x0a4|+0x017:'486'
	// <0>
	// <1>
	// <2>
	// <0x63626b>|0x0bb|+0x20e:'490'
	// <0x636479>|0x2c9|-0x1e9:'490'
	// <0>
	// <0x636290>|0x0e0|-0x00b:'492'
	// <0>
	// <0x636285>|0x0d5|+0x010:'494'
	// <0x636295>|0x0e5|+0x07d:'494'
	// <0>
	// <0x636312>|0x162|+0x013:'496'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x636325>|0x175|+0x02b:'501'
	// <0x636350>|0x1a0|+0x00c:'502'
	// <0x63635c>|0x1ac|+0x00c:'503'
	// <0x636368>|0x1b8|+0x00a:'504'
	// <0>
	// <0x636372>|0x1c2|+0x023:'506'
	// <0>
	// <1>
	// <0x636395>|0x1e5|+0x0eb:'509'
	// <0x636480>|0x2d0|+0x052:'509'
	// <0x6364d2>|0x322|-0x042:'509'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x636490>|0x2e0|+0x022:'515'
	// ******
}

// STATE[STUB]
void grass_world::accumulate_trample( renderer* in_renderer, renderer_context* in_context )
{
	// LOCALS
	// grass_patch* const* 				end_patch
	// grass_patch* const* 				it_patch
	// ******

	// FUNCTION BODY[0x6371c0]: 20
	// <0x6371c6>|0x006|+0x006:'520'
	// <0x6371cc>|0x00c|+0x011:'521'
	// <0>
	// <0x6371dd>|0x01d|+0x004:'523'
	// <0>
	// <1>
	// <2>
	// <0x6371e1>|0x021|+0x01a:'527'
	// <0>
	// <1>
	// <0x6371fb>|0x03b|+0x006:'530'
	// <0x637201>|0x041|+0x006:'531'
	// <0>
	// <0x637207>|0x047|+0x009:'533'
	// <0>
	// <0x637210>|0x050|+0x027:'535'
	// <0>
	// <1>
	// <2>
	// <0x637237>|0x077|+0x029:'539'
	// ******
}

// STATE[STUB]
void grass_world::remove_trample( )
{
	// FUNCTION BODY[0x635bb0]: 7
	// <0x635bb1>|0x001|+0x007:'544'
	// <0x635bb8>|0x008|+0x006:'545'
	// <0>
	// <0x635bbe>|0x00e|+0x004:'547'
	// <0>
	// <0x635bc2>|0x012|+0x010:'549'
	// <0>
	// ******
}

// STATE[STUB]
void grass_world::render(
	renderer_context*			context,
	float3 const&				viewer_position,
	enum_render_stage_type		stage_type,
	const u32					tech_index,
	const float					draw_distance,
	bool						stencil_mask,
	res_effect*					debug_effect,
	bool						shadow_pass,
	const u32					cascade_index
)
{
	// LOCALS
	// grass_patch* const* 				end_patch
	// math::frustum 					shadow_frustum
	// ******

	// FUNCTION BODY[0x638150]: 64
	// <0x638150>|0x000|+0x023:'563'
	// <0>
	// <1>
	// <2>
	// <0x638173>|0x023|+0x02b:'567'
	// <0x63819e>|0x04e|+0x007:'568'
	// <0>
	// <0x6381a5>|0x055|+0x009:'570'
	// <0>
	// <0x6381ae>|0x05e|+0x006:'572'
	// <0x6381b4>|0x064|+0x007:'573'
	// <0>
	// <1>
	// <2>
	// <0x6381bb>|0x06b|+0x010:'577'
	// <0>
	// <1>
	// <2>
	// <0x6381cb>|0x07b|+0x015:'581'
	// <0>
	// <1>
	// <2>
	// <0x6381e0>|0x090|+0x019:'585'
	// <0>
	// <1>
	// <0x6381f9>|0x0a9|+0x00a:'588'
	// <0>
	// <0x638203>|0x0b3|+0x015:'590'
	// <0x638218>|0x0c8|+0x013:'591'
	// <0>
	// <1>
	// <2>
	// <0x63822b>|0x0db|+0x03d:'595'
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
	// ******
}

// STATE[STUB]
void grass_world::add_grass_layer(
	grass_layer_desc*		desc,
	grass_layer_data*		data,
	bool					populate,
	bool					from_cook
)
{
	// FUNCTION BODY[0x637b70]: 1
	// <0x637b70>|0x000|+0x017:'697'
	// ******
}

// STATE[STUB]
void grass_world::update_grass_layer(
	grass_layer_desc*		desc,
	grass_layer_data*		layer_data,
	bool					is_set,
	bool					do_populate,
	bool					from_cook
)
{
	// LOCALS
	// buffer_vector< resources::request > r
	// float2 							cell_pos_rb
	// float2 							cell_pos_lt
	// u16 								x
	// ******

	// FUNCTION BODY[0x637940]: 39
	// <0x637946>|0x006|+0x012:'706'
	// <0>
	// <0x637958>|0x018|+0x019:'708'
	// <0>
	// <1>
	// <0x637971>|0x031|+0x014:'711'
	// <0x637985>|0x045|+0x00b:'712'
	// <0x637990>|0x050|+0x010:'713'
	// <0>
	// <0x6379a0>|0x060|+0x019:'715'
	// <0x6379b9>|0x079|+0x00f:'716'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6379c8>|0x088|+0x0ce:'725'
	// <0>
	// <1>
	// <0x637a96>|0x156|+0x0ab:'728'
	// <0x637b41>|0x201|-0x09a:'728'
	// <0x637aa7>|0x167|+0x01a:'729'
	// <0>
	// <1>
	// <2>
	// <0x637ac1>|0x181|+0x042:'733'
	// <0>
	// <0x637b03>|0x1c3|+0x01f:'735'
	// <0x637b22>|0x1e2|+0x02d:'736'
	// <0>
	// <1>
	// <0x637b4f>|0x20f|+0x006:'739'
	// <0>
	// <0x637b55>|0x215|-0x0cb:'741'
	// <0>
	// <1>
	// <2>
	// <0x637a8a>|0x14a|+0x0d5:'745'
	// ******
}

// STATE[STUB]
void grass_world::clear( )
{
	// LOCALS
	// grass_template** 				it_t
	// grass_template** 				end_t
	// ******

	// FUNCTION BODY[0x636fb0]: 22
	// <0x636fb5>|0x005|+0x009:'749'
	// <0>
	// <0x636fbe>|0x00e|+0x006:'751'
	// <0x636fc4>|0x014|+0x00e:'752'
	// <0>
	// <0x636fd2>|0x022|+0x012:'754'
	// <0>
	// <0x636fe4>|0x034|+0x002:'756'
	// <0>
	// <1>
	// <0x636fe6>|0x036|+0x003:'759'
	// <0x636fe9>|0x039|+0x003:'760'
	// <0>
	// <0x636fec>|0x03c|+0x004:'762'
	// <0>
	// <0x636ff0>|0x040|+0x002:'764'
	// <0x636ff2>|0x042|+0x01d:'765'
	// <0>
	// <0x63700f>|0x05f|+0x02e:'767'
	// <0x63703d>|0x08d|+0x035:'768'
	// <0>
	// <0x637072>|0x0c2|+0x040:'770'
	// ******
}

// STATE[STUB]
void grass_world::remove_grass_layer( u8 id, bool do_populate )
{
	// LOCALS
	// grass_template** 				end
	// vector< u32 > 					instances_to_remove
	// grass_instance** 				end_instance
	// ******

	// FUNCTION BODY[0x635e40]: 26
	// <0x635e43>|0x003|+0x004:'775'
	// <0x635e47>|0x007|+0x00f:'776'
	// <0>
	// <0x635e56>|0x016|+0x015:'778'
	// <0x635e6b>|0x02b|+0x056:'779'
	// <0x635ec1>|0x081|-0x051:'779'
	// <0>
	// <0x635e70>|0x030|+0x002:'781'
	// <0>
	// <1>
	// <0x635e72>|0x032|+0x00a:'784'
	// <0>
	// <0x635e7c>|0x03c|+0x041:'786'
	// <0x635ebd>|0x07d|-0x03d:'786'
	// <0>
	// <0x635e80>|0x040|+0x003:'788'
	// <0x635e83>|0x043|+0x009:'789'
	// <0x635e8c>|0x04c|+0x040:'790'
	// <0>
	// <1>
	// <2>
	// <0x635ecc>|0x08c|+0x018:'794'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// ******
}

// STATE[STUB]
void grass_world::remove_instances( vector< u32 > const& v )
{
	// FUNCTION BODY[0x635e10]: 4
	// <0x635e16>|0x006|+0x003:'805'
	// <0x635e19>|0x009|+0x003:'806'
	// <0x635e1c>|0x00c|+0x004:'807'
	// <0x635e20>|0x010|+0x013:'808'
	// ******
}

// STATE[STUB]
void setup_seed_clk( )
{
	// CALL SITE INFO
	// <0x635990> -> unsigned long < unknown >()
	// ******

	// FUNCTION BODY[0x635990]: 7
	// <0x635990>|0x000|+0x006:'831'
	// <0x635996>|0x006|+0x005:'832'
	// <0x63599b>|0x00b|+0x005:'833'
	// <0x6359a0>|0x010|+0x005:'834'
	// <0x6359a5>|0x015|+0x005:'835'
	// <0x6359aa>|0x01a|+0x005:'836'
	// <0x6359af>|0x01f|+0x005:'837'
	// ******
}

// STATE[STUB]
u8 select_model_template( float* values, float sum, const u8 count )
{
	// LOCALS
	// float 							p
	// ******

	return 0;

	// FUNCTION BODY[0x635b50]: 8
	// <0x635b51>|0x001|+0x02e:'855'
	// <0>
	// <0x635b7f>|0x02f|+0x014:'857'
	// <0>
	// <0x635b93>|0x043|+0x013:'859'
	// <0>
	// <1>
	// <0x635ba6>|0x056|+0x004:'862'
	// ******
}

// STATE[STUB]
float select_model_orientation( )
{
	return 0.0f;

	// FUNCTION BODY[0x635b10]: 1
	// <0x635b11>|0x001|+0x034:'867'
	// ******
}

// STATE[STUB]
float select_model_scale( const float delta, const float base_scale )
{
	return 0.0f;

	// FUNCTION BODY[0x635ac0]: 1
	// <0x635ac1>|0x001|+0x040:'877'
	// ******
}

// STATE[STUB]
void grass_world::grass_layer_resources_ready_from_cook(
	resources::queries_result&		data,
	grass_layer_desc*				desc,
	grass_layer_data*				layer_data,
	bool							do_populate
)
{
	// LOCALS
	// resources::pinned_ptr_const< u8 > pdata
	// memory::chunk_reader 			chunk
	// ******

	// FUNCTION BODY[0x636da0]: 25
	// <0x636dac>|0x00c|+0x03b:'935'
	// <0x636de7>|0x047|+0x00c:'936'
	// <0x636df3>|0x053|+0x025:'937'
	// <0>
	// <0x636e18>|0x078|+0x011:'939'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x636e29>|0x089|+0x01f:'944'
	// <0>
	// <0x636e48>|0x0a8|+0x00f:'946'
	// <0x636e57>|0x0b7|+0x007:'947'
	// <0>
	// <1>
	// <0x636e5e>|0x0be|+0x013:'950'
	// <0x636e71>|0x0d1|+0x004:'951'
	// <0>
	// <1>
	// <2>
	// <0x636e75>|0x0d5|+0x01c:'955'
	// <0>
	// <1>
	// <0x636e91>|0x0f1|+0x01e:'958'
	// <0x636eaf>|0x10f|+0x01c:'959'
	// ******
}

// STATE[STUB]
void grass_world::grass_layer_resources_ready(
	resources::queries_result&		data,
	grass_layer_desc*				desc,
	grass_layer_data*				layer_data,
	bool							do_populate
)
{
	// LOCALS
	// u8 								models_count
	// math::color 						clr
	// float 							prob_sum
	// u32 								i
	// float4x4 						m
	// float 							orient
	// grass_render_model_ptr 			model
	// ******

	// FUNCTION BODY[0x636990]: 61
	// <0x636999>|0x009|+0x022:'967'
	// <0x6369bb>|0x02b|+0x01c:'968'
	// <0>
	// <1>
	// <0x6369d7>|0x047|+0x0be:'971'
	// <0x636a95>|0x105|-0x0a5:'971'
	// <0>
	// <1>
	// <0x6369f0>|0x060|+0x04a:'974'
	// <0>
	// <0x636a3a>|0x0aa|+0x00e:'976'
	// <0x636a48>|0x0b8|+0x004:'977'
	// <0>
	// <0x636a4c>|0x0bc|+0x008:'979'
	// <0x636a54>|0x0c4|+0x002:'980'
	// <0>
	// <0x636a56>|0x0c6|+0x00f:'982'
	// <0>
	// <0x636a65>|0x0d5|+0x033:'984'
	// <0>
	// <0x636a98>|0x108|+0x007:'986'
	// <0>
	// <0x636a9f>|0x10f|+0x00d:'988'
	// <0x636aac>|0x11c|+0x014:'989'
	// <0>
	// <0x636ac0>|0x130|+0x027:'991'
	// <0x636ae7>|0x157|-0x011:'991'
	// <0x636ad6>|0x146|+0x02e:'992'
	// <0>
	// <1>
	// <0x636b04>|0x174|-0x018:'995'
	// <0>
	// <0x636aec>|0x15c|+0x02a:'997'
	// <0>
	// <1>
	// <2>
	// <0x636b16>|0x186|+0x095:'1001'
	// <0x636bab>|0x21b|-0x036:'1001'
	// <0>
	// <1>
	// <0x636b75>|0x1e5|+0x03a:'1004'
	// <0x636baf>|0x21f|+0x00d:'1004'
	// <0x636bbc>|0x22c|+0x00f:'1005'
	// <0>
	// <1>
	// <0x636bcb>|0x23b|+0x01f:'1008'
	// <0>
	// <0x636bea>|0x25a|+0x009:'1010'
	// <0x636bf3>|0x263|+0x07d:'1011'
	// <0x636c70>|0x2e0|+0x057:'1012'
	// <0>
	// <1>
	// <0x636cc7>|0x337|+0x018:'1015'
	// <0x636cdf>|0x34f|+0x031:'1016'
	// <0x636d10>|0x380|+0x021:'1017'
	// <0>
	// <0x636d31>|0x3a1|+0x03b:'1019'
	// <0>
	// <1>
	// <2>
	// <0x636d6c>|0x3dc|+0x010:'1023'
	// <0>
	// <0x636d7c>|0x3ec|+0x007:'1025'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void grass_world::remove_layer_instances( u8 id, float2 const& cell_lt, float2 const& cell_rb )
{
	// LOCALS
	// float3 							pt
	// vector< u32 > 					instances_to_remove
	// float3 							p
	// ******

	// FUNCTION BODY[0x635f50]: 29
	// <0x635f53>|0x003|+0x008:'1032'
	// <0>
	// <0x635f5b>|0x00b|+0x03b:'1034'
	// <0>
	// <0x635f96>|0x046|+0x00a:'1036'
	// <0>
	// <1>
	// <2>
	// <0x635fa0>|0x050|+0x006:'1040'
	// <0x635fa6>|0x056|+0x012:'1041'
	// <0>
	// <0x635fb8>|0x068|+0x008:'1043'
	// <0>
	// <0x635fc0>|0x070|+0x003:'1045'
	// <0x635fc3>|0x073|+0x009:'1046'
	// <0>
	// <0x635fcc>|0x07c|+0x00e:'1048'
	// <0>
	// <0x635fda>|0x08a|+0x032:'1050'
	// <0x63600c>|0x0bc|+0x031:'1051'
	// <0>
	// <1>
	// <2>
	// <0x63603d>|0x0ed|+0x008:'1055'
	// <0>
	// <0x636045>|0x0f5|+0x01d:'1057'
	// <0x636062>|0x112|+0x00b:'1058'
	// <0>
	// <1>
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
	// 	u32*
	// 	iterator_type;

	// typedef
	// 	void**
	// 	iterator_type;

	// typedef
	// 	vostok::collision::ray_triangle_result*
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

	// typedef
	// 	vostok::render::trample_desc*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok
