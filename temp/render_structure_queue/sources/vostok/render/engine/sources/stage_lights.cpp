////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\stage_lights.h"

namespace vostok {
namespace render {

// STATE[STUB]
void `dynamic initializer for 's_shadow_z_near''( )
{
	// FUNCTION BODY[0x7d6ad0]
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_one_light_dip''( )
{
	// FUNCTION BODY[0x7d6b40]
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_lights_cull''( )
{
	// FUNCTION BODY[0x7d6b90]
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_draw_lights''( )
{
	// FUNCTION BODY[0x7d6be0]
	// ******
}

// STATE[STUB]
void `dynamic atexit destructor for 's_shadow_z_near''( )
{
	// FUNCTION BODY[0x7ee980]
	// ******
}

// STATE[STUB]
void stage_lights::new_sphere_geometry( )
{
	// LOCALS
	// D3D11_INPUT_ELEMENT_DESC[1] 		desc
	// ******

	// FUNCTION BODY[0x61bcd0]: 7
	// <0x61bcd0>|0x000|+0x04a:'125'
	// <0x61bd1a>|0x04a|+0x044:'126'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x61bd5e>|0x08e|+0x070:'131'
	// ******
}

// STATE[STUB]
void stage_lights::create_pyramid_geometry( )
{
	// LOCALS
	// float3[5] 						vertices
	// u16[18] 							indices
	// D3D11_INPUT_ELEMENT_DESC[1] 		desc
	// ******

	// FUNCTION BODY[0x61bac0]: 24
	// <0>
	// <0x61bac3>|0x003|+0x003:'137'
	// <0x61bac6>|0x006|+0x008:'138'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x61bace>|0x00e|+0x00f:'144'
	// <0x61badd>|0x01d|+0x013:'145'
	// <0x61baf0>|0x030|+0x012:'146'
	// <0x61bb02>|0x042|+0x021:'147'
	// <0x61bb23>|0x063|+0x007:'148'
	// <0>
	// <1>
	// <2>
	// <0x61bb2a>|0x06a|+0x0e7:'152'
	// <0x61bc11>|0x151|+0x045:'153'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x61bc56>|0x196|+0x06f:'159'
	// ******
}

// STATE[STUB]
void stage_lights::create_obb_geometry( )
{
	// LOCALS
	// D3D11_INPUT_ELEMENT_DESC[1] 		desc
	// ******

	// FUNCTION BODY[0x61b9a0]: 22
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x61b9a3>|0x003|+0x048:'170'
	// <0>
	// <0x61b9eb>|0x04b|+0x00c:'172'
	// <0x61b9f7>|0x057|+0x00d:'173'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x61ba04>|0x064|+0x043:'180'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x61ba47>|0x0a7|+0x068:'185'
	// ******
}

// STATE[STUB]
 stage_lights::stage_lights( renderer* in_renderer, renderer_context* context, bool is_forward_lighting_pass )
{
	// LOCALS
	// const D3D11_INPUT_ELEMENT_DESC[2] screen_vertex_layout
	// u16[6] 							indices
	// ******

	// FUNCTION BODY[0x624e70]: 307
	// <0>
	// <0x6250a4>|0x234|+0x015:'209'
	// <0>
	// <0x6250b9>|0x249|+0x004:'211'
	// <0>
	// <0x6250bd>|0x24d|+0x004:'213'
	// <0x6250c1>|0x251|+0x009:'214'
	// <0x6250ca>|0x25a|+0x002:'215'
	// <0x6250cc>|0x25c|+0x009:'216'
	// <0>
	// <1>
	// <0x6250d5>|0x265|+0x00c:'219'
	// <0>
	// <0x6250e1>|0x271|+0x049:'221'
	// <0x62512a>|0x2ba|+0x04a:'222'
	// <0x625174>|0x304|+0x049:'223'
	// <0x6251bd>|0x34d|+0x037:'224'
	// <0x6251f4>|0x384|+0x03f:'225'
	// <0x625233>|0x3c3|+0x03f:'226'
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
	// <0x625272>|0x402|+0x011:'348'
	// <0x625283>|0x413|+0x011:'349'
	// <0x625294>|0x424|+0x011:'350'
	// <0x6252a5>|0x435|+0x011:'351'
	// <0x6252b6>|0x446|+0x011:'352'
	// <0x6252c7>|0x457|+0x011:'353'
	// <0x6252d8>|0x468|+0x011:'354'
	// <0x6252e9>|0x479|+0x011:'355'
	// <0x6252fa>|0x48a|+0x011:'356'
	// <0x62530b>|0x49b|+0x011:'357'
	// <0x62531c>|0x4ac|+0x011:'358'
	// <0x62532d>|0x4bd|+0x011:'359'
	// <0x62533e>|0x4ce|+0x011:'360'
	// <0x62534f>|0x4df|+0x011:'361'
	// <0x625360>|0x4f0|+0x011:'362'
	// <0x625371>|0x501|+0x011:'363'
	// <0>
	// <0x625382>|0x512|+0x007:'365'
	// <0x625389>|0x519|+0x005:'366'
	// <0x62538e>|0x51e|+0x005:'367'
	// <0>
	// <0x625393>|0x523|+0x05b:'369'
	// <0x6253ee>|0x57e|+0x05b:'370'
	// <0x625449>|0x5d9|+0x05b:'371'
	// <0x6254a4>|0x634|+0x05b:'372'
	// <0>
	// <0x6254ff>|0x68f|+0x05b:'374'
	// <0x62555a>|0x6ea|+0x05b:'375'
	// <0x6255b5>|0x745|+0x05b:'376'
	// <0x625610>|0x7a0|+0x05b:'377'
	// <0x62566b>|0x7fb|+0x05b:'378'
	// <0x6256c6>|0x856|+0x051:'379'
	// <0x625717>|0x8a7|+0x052:'380'
	// <0>
	// <0x625769>|0x8f9|+0x051:'382'
	// <0x6257ba>|0x94a|+0x051:'383'
	// <0>
	// <0x62580b>|0x99b|+0x051:'385'
	// <0>
	// <0x62585c>|0x9ec|+0x051:'387'
	// <0x6258ad>|0xa3d|+0x051:'388'
	// <0>
	// <0x6258fe>|0xa8e|+0x051:'390'
	// <0x62594f>|0xadf|+0x051:'391'
	// <0>
	// <0x6259a0>|0xb30|+0x052:'393'
	// <0>
	// <0x6259f2>|0xb82|+0x051:'395'
	// <0x625a43>|0xbd3|+0x051:'396'
	// <0>
	// <0x625a94>|0xc24|+0x051:'398'
	// <0x625ae5>|0xc75|+0x051:'399'
	// <0>
	// <0x625b36>|0xcc6|+0x051:'401'
	// <0>
	// <0x625b87>|0xd17|+0x051:'403'
	// <0x625bd8>|0xd68|+0x051:'404'
	// <0>
	// <0x625c29>|0xdb9|+0x051:'406'
	// <0x625c7a>|0xe0a|+0x051:'407'
	// <0>
	// <0x625ccb>|0xe5b|+0x051:'409'
	// <0>
	// <0x625d1c>|0xeac|+0x051:'411'
	// <0x625d6d>|0xefd|+0x051:'412'
	// <0>
	// <0x625dbe>|0xf4e|+0x052:'414'
	// <0>
	// <0x625e10>|0xfa0|+0x051:'416'
	// <0>
	// <0x625e61>|0xff1|+0x051:'418'
	// <0>
	// <0x625eb2>|0x1042|+0x051:'420'
	// <0x625f03>|0x1093|+0x051:'421'
	// <0>
	// <1>
	// <2>
	// <0x625f54>|0x10e4|+0x051:'425'
	// <0x625fa5>|0x1135|+0x051:'426'
	// <0x625ff6>|0x1186|+0x051:'427'
	// <0x626047>|0x11d7|+0x051:'428'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x626098>|0x1228|+0x00d:'433'
	// <0>
	// <1>
	// <0x6260a5>|0x1235|+0x002:'436'
	// <0x6260a7>|0x1237|+0x096:'437'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x62613d>|0x12cd|+0x02d:'443'
	// <0>
	// <0x62616a>|0x12fa|+0x012:'445'
	// <0x62617c>|0x130c|+0x051:'446'
	// <0x6261cd>|0x135d|+0x025:'447'
	// <0>
	// <0x6261f2>|0x1382|+0x01c:'449'
	// <0x62620e>|0x139e|+0x012:'450'
	// <0x626220>|0x13b0|+0x020:'451'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x626240>|0x13d0|+0x045:'457'
	// <0x626285>|0x1415|+0x014:'458'
	// <0>
	// <0x626299>|0x1429|+0x03f:'460'
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
	// ******
}

// STATE[STUB]
bool stage_lights::is_effects_ready( ) const
{
	return false;

	// FUNCTION BODY[0x61a8f0]: 17
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
	// <0x61a8f0>|0x000|+0x09e:'690'
	// <0x61a98e>|0x09e|-0x001:'690'
	// <0x61a98d>|0x09d|+0x003:'691'
	// ******
}

// STATE[STUB]
void stage_lights::make_spot_light_shadowmap( u32 shadow_quality, light* l )
{
	// LOCALS
	// float4x4 						projection_matrix
	// float4x4 						view_matrix
	// ******

	// FUNCTION BODY[0x6206d0]: 8
	// <0x6206d0>|0x000|+0x01f:'743'
	// <0>
	// <0x6206ef>|0x01f|+0x035:'745'
	// <0>
	// <0x620724>|0x054|+0x12c:'747'
	// <0>
	// <0x620850>|0x180|+0x028:'749'
	// <0x620878>|0x1a8|+0x023:'750'
	// ******
}

// STATE[STUB]
void stage_lights::make_plane_spot_light_shadowmap( u32 shadow_quality, light* l )
{
	// LOCALS
	// float3 							new_position
	// float4x4 						projection_matrix
	// const float 						max_angle
	// const float 						max_scale
	// float3 							up
	// const float 						inv_distance
	// float4x4 						view_matrix
	// ******

	// FUNCTION BODY[0x620430]: 15
	// <0x620430>|0x000|+0x01f:'755'
	// <0x62044f>|0x01f|+0x029:'756'
	// <0>
	// <0x620478>|0x048|+0x017:'758'
	// <0>
	// <0x62048f>|0x05f|+0x0a9:'760'
	// <0>
	// <1>
	// <2>
	// <0x620538>|0x108|+0x0e6:'764'
	// <0>
	// <0x62061e>|0x1ee|+0x053:'766'
	// <0>
	// <0x620671>|0x241|+0x028:'768'
	// <0x620699>|0x269|+0x026:'769'
	// ******
}

// STATE[STUB]
void stage_lights::render_to_hw_shadowmap(
	light*				l,
	u32					shadow_quality,
	const float			z_bias,
	u32					smap_size,
	u32					smap_size_index,
	float4x4 const&		view_matrix,
	float4x4 const&		projection_matrix,
	u32					marge
)
{
	// LOCALS
	// D3D11_VIEWPORT 					tmp_viewport
	// vector< render_surface_instance* > m_dynamic_visuals_to_shadow
	// D3D11_VIEWPORT 					orig_viewport
	// float3 							view_position
	// render_surface_instance** 		it_d
	// render_surface_instance* const* 	end_d
	// float4x4 						shadow_transform
	// float4x4 						shadow_transform
	// vector< speedtree_forest::tree_render_info > visible_trees
	// ******

	// CALL SITE INFO
	// <0x62019b> -> SpeedTree::EGeometryType < unknown >() const
	// ******

	// FUNCTION BODY[0x61fb80]: 163
	// <0x61fb9a>|0x01a|+0x00b:'783'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x61fba5>|0x025|+0x029:'791'
	// <0>
	// <0x61fbce>|0x04e|+0x05b:'793'
	// <0x61fc29>|0x0a9|+0x009:'794'
	// <0>
	// <0x61fc32>|0x0b2|+0x026:'796'
	// <0x61fc58>|0x0d8|+0x005:'797'
	// <0>
	// <0x61fc5d>|0x0dd|+0x002:'799'
	// <0>
	// <0x61fc5f>|0x0df|+0x02b:'801'
	// <0x61fc8a>|0x10a|+0x02e:'802'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x61fcb8>|0x138|+0x003:'807'
	// <0>
	// <0x61fcbb>|0x13b|+0x01e:'809'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x61fcd9>|0x159|+0x03d:'814'
	// <0>
	// <0x61fd16>|0x196|+0x003:'816'
	// <0>
	// <0x61fd19>|0x199|+0x048:'818'
	// <0x61fd61>|0x1e1|+0x032:'819'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x61fd93>|0x213|+0x00e:'824'
	// <0x61fda1>|0x221|+0x02a:'825'
	// <0x61fdcb>|0x24b|+0x002:'826'
	// <0x61fdcd>|0x24d|+0x021:'827'
	// <0>
	// <0x61fdee>|0x26e|+0x02b:'829'
	// <0>
	// <0x61fe19>|0x299|+0x024:'831'
	// <0x61fe3d>|0x2bd|+0x02c:'832'
	// <0x61fe69>|0x2e9|+0x02f:'833'
	// <0>
	// <0x61fe98>|0x318|+0x00b:'835'
	// <0x61fea3>|0x323|+0x007:'836'
	// <0x61feaa>|0x32a|+0x002:'837'
	// <0x61feac>|0x32c|+0x002:'838'
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
	// <0x61feae>|0x32e|+0x006:'851'
	// <0>
	// <1>
	// <0x61feb4>|0x334|+0x004:'854'
	// <0x61feb8>|0x338|+0x00c:'855'
	// <0>
	// <0x61fec4>|0x344|+0x00c:'857'
	// <0>
	// <0x61fed0>|0x350|+0x002:'859'
	// <0x61fed2>|0x352|+0x026:'860'
	// <0>
	// <1>
	// <0x61fef8>|0x378|+0x024:'863'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x61ff1c>|0x39c|+0x019:'868'
	// <0>
	// <1>
	// <0x61ff35>|0x3b5|+0x006:'871'
	// <0>
	// <0x61ff3b>|0x3bb|+0x00c:'873'
	// <0>
	// <1>
	// <2>
	// <0x61ff47>|0x3c7|+0x005:'877'
	// <0>
	// <1>
	// <0x61ff4c>|0x3cc|+0x00c:'880'
	// <0x61ff58>|0x3d8|+0x00a:'881'
	// <0x61ff62>|0x3e2|+0x008:'882'
	// <0>
	// <0x61ff6a>|0x3ea|+0x095:'884'
	// <0>
	// <1>
	// <0x61ffff>|0x47f|+0x054:'887'
	// <0>
	// <1>
	// <0x620053>|0x4d3|-0x029:'890'
	// <0x62002a>|0x4aa|+0x01b:'891'
	// <0>
	// <0x620045>|0x4c5|+0x30a:'893'
	// <0x62034f>|0x7cf|-0x2f9:'893'
	// <0>
	// <1>
	// <2>
	// <0x620056>|0x4d6|+0x080:'897'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6200d6>|0x556|+0x042:'904'
	// <0>
	// <0x620118>|0x598|+0x025:'906'
	// <0x62013d>|0x5bd|+0x020:'907'
	// <0x62015d>|0x5dd|+0x007:'908'
	// <0>
	// <1>
	// <0x620164>|0x5e4|+0x01a:'911'
	// <0x62017e>|0x5fe|+0x002:'912'
	// <0x620180>|0x600|+0x014:'913'
	// <0>
	// <1>
	// <0x620194>|0x614|+0x009:'916'
	// <0>
	// <0x62019d>|0x61d|+0x025:'918'
	// <0x6201c2>|0x642|+0x00a:'919'
	// <0x6201cc>|0x64c|+0x08d:'920'
	// <0>
	// <0x620259>|0x6d9|+0x005:'922'
	// <0>
	// <0x62025e>|0x6de|+0x027:'924'
	// <0x620285>|0x705|+0x00a:'925'
	// <0x62028f>|0x70f|+0x07c:'926'
	// <0>
	// <0x62030b>|0x78b|+0x00d:'928'
	// <0x620318>|0x798|+0x03b:'929'
	// <0>
	// <1>
	// <0x620353>|0x7d3|+0x019:'932'
	// <0>
	// <0x62036c>|0x7ec|+0x014:'934'
	// <0>
	// <0x620380>|0x800|+0x01b:'936'
	// <0x62039b>|0x81b|+0x019:'937'
	// <0>
	// <1>
	// <0x6203b4>|0x834|+0x011:'940'
	// <0>
	// <1>
	// <0x6203c5>|0x845|+0x042:'943'
	// <0>
	// <0x620407>|0x887|+0x01a:'945'
	// ******
}

// STATE[STUB]
 stage_lights::~stage_lights( )
{
	// FUNCTION BODY[0x61b090]: 2
	// <0x61b09e>|0x00e|+0x01d:'993'
	// <0>
	// ******
}

// STATE[STUB]
void stage_lights::render_particle_lighting( render_particle_emitter_instance* instance, light* l, u32 num_particles )
{
	// LOCALS
	// float 							light_range
	// float3 							light_direction
	// float3 							light_color
	// float3 							light_position
	// const float 						penumbra_half_angle_cosine
	// float4x4 						obb_world
	// const float 						penumbra_half_angle_cosine
	// const float 						umbra_half_angle_cosine
	// ******

	// STATICS
	// static < NoType > 				 = <0x61fb70>;
	// static < NoType > 				 = <0x61fb54>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x60e57b }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1670") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x60e574 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1666") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x60e56d }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1667") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x60e566 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1668") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x60ddc3 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN13") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x60de94 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN12") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x60e0da }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN11") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x60e2fb }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN10") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x60e50a }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN9") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x60e615 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x60e6fe }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// ******

	// FUNCTION BODY[0x61ec00]: 164
	// <0x61ec10>|0x010|+0x007:'999'
	// <0x61ec17>|0x017|+0x00e:'1000'
	// <0x61ec25>|0x025|+0x0a0:'1001'
	// <0x61ecc5>|0x0c5|+0x022:'1002'
	// <0>
	// <1>
	// <2>
	// <0x61ece7>|0x0e7|+0x0dc:'1006'
	// <0>
	// <0x61edc3>|0x1c3|+0x019:'1008'
	// <0x61eddc>|0x1dc|+0x04b:'1009'
	// <0x61ee27>|0x227|+0x04f:'1010'
	// <0x61ee76>|0x276|+0x019:'1011'
	// <0>
	// <1>
	// <0x61ee8f>|0x28f|+0x005:'1014'
	// <0>
	// <1>
	// <0x61ee94>|0x294|+0x019:'1017'
	// <0x61eead>|0x2ad|+0x04b:'1018'
	// <0x61eef8>|0x2f8|+0x04f:'1019'
	// <0x61ef47>|0x347|+0x04c:'1020'
	// <0x61ef93>|0x393|+0x01f:'1021'
	// <0>
	// <1>
	// <0x61efb2>|0x3b2|+0x01f:'1024'
	// <0x61efd1>|0x3d1|+0x04f:'1025'
	// <0>
	// <1>
	// <0x61f020>|0x420|+0x098:'1028'
	// <0x61f0b8>|0x4b8|+0x019:'1029'
	// <0>
	// <0x61f0d1>|0x4d1|+0x009:'1031'
	// <0>
	// <1>
	// <0x61f0da>|0x4da|+0x019:'1034'
	// <0>
	// <0x61f0f3>|0x4f3|+0x04b:'1036'
	// <0x61f13e>|0x53e|+0x04f:'1037'
	// <0x61f18d>|0x58d|+0x01f:'1038'
	// <0>
	// <0x61f1ac>|0x5ac|+0x00e:'1040'
	// <0x61f1ba>|0x5ba|+0x016:'1041'
	// <0x61f1d0>|0x5d0|+0x06a:'1042'
	// <0>
	// <0x61f23a>|0x63a|+0x042:'1044'
	// <0x61f27c>|0x67c|+0x020:'1045'
	// <0x61f29c>|0x69c|+0x01d:'1046'
	// <0>
	// <0x61f2b9>|0x6b9|+0x003:'1048'
	// <0x61f2bc>|0x6bc|+0x015:'1049'
	// <0x61f2d1>|0x6d1|+0x01c:'1050'
	// <0>
	// <0x61f2ed>|0x6ed|+0x00e:'1052'
	// <0>
	// <1>
	// <0x61f2fb>|0x6fb|+0x019:'1055'
	// <0>
	// <0x61f314>|0x714|+0x04b:'1057'
	// <0x61f35f>|0x75f|+0x04f:'1058'
	// <0x61f3ae>|0x7ae|+0x04c:'1059'
	// <0x61f3fa>|0x7fa|+0x01d:'1060'
	// <0>
	// <0x61f417>|0x817|+0x019:'1062'
	// <0x61f430>|0x830|+0x01b:'1063'
	// <0>
	// <0x61f44b>|0x84b|+0x04a:'1065'
	// <0x61f495>|0x895|+0x019:'1066'
	// <0x61f4ae>|0x8ae|+0x01b:'1067'
	// <0>
	// <0x61f4c9>|0x8c9|+0x003:'1069'
	// <0x61f4cc>|0x8cc|+0x019:'1070'
	// <0x61f4e5>|0x8e5|+0x01c:'1071'
	// <0>
	// <0x61f501>|0x901|+0x009:'1073'
	// <0>
	// <1>
	// <0x61f50a>|0x90a|+0x026:'1076'
	// <0>
	// <0x61f530>|0x930|+0x012:'1078'
	// <0>
	// <0x61f542>|0x942|+0x01d:'1080'
	// <0x61f55f>|0x95f|+0x04c:'1081'
	// <0>
	// <0x61f5ab>|0x9ab|-0x063:'1083'
	// <0x61f548>|0x948|+0x07c:'1084'
	// <0x61f5c4>|0x9c4|+0x048:'1084'
	// <0>
	// <1>
	// <0x61f60c>|0xa0c|+0x009:'1087'
	// <0>
	// <1>
	// <0x61f615>|0xa15|+0x019:'1090'
	// <0x61f62e>|0xa2e|+0x020:'1091'
	// <0x61f64e>|0xa4e|+0x016:'1092'
	// <0x61f664>|0xa64|+0x015:'1093'
	// <0>
	// <0x61f679>|0xa79|+0x015:'1095'
	// <0>
	// <0x61f68e>|0xa8e|+0x013:'1097'
	// <0x61f6a1>|0xaa1|+0x015:'1098'
	// <0x61f6b6>|0xab6|+0x015:'1099'
	// <0>
	// <0x61f6cb>|0xacb|+0x008:'1101'
	// <0x61f6d3>|0xad3|+0x00f:'1102'
	// <0x61f6e2>|0xae2|+0x017:'1103'
	// <0>
	// <0x61f6f9>|0xaf9|+0x005:'1105'
	// <0>
	// <1>
	// <0x61f6fe>|0xafe|+0x019:'1108'
	// <0x61f717>|0xb17|+0x020:'1109'
	// <0x61f737>|0xb37|+0x016:'1110'
	// <0x61f74d>|0xb4d|+0x039:'1111'
	// <0x61f786>|0xb86|+0x015:'1112'
	// <0>
	// <0x61f79b>|0xb9b|+0x01f:'1114'
	// <0x61f7ba>|0xbba|+0x016:'1115'
	// <0x61f7d0>|0xbd0|+0x01f:'1116'
	// <0x61f7ef>|0xbef|+0x014:'1117'
	// <0>
	// <1>
	// <0x61f803>|0xc03|+0x043:'1120'
	// <0x61f846>|0xc46|+0x015:'1121'
	// <0>
	// <1>
	// <0x61f85b>|0xc5b|+0x02d:'1124'
	// <0>
	// <0x61f888>|0xc88|+0x013:'1126'
	// <0x61f89b>|0xc9b|+0x015:'1127'
	// <0x61f8b0>|0xcb0|+0x015:'1128'
	// <0>
	// <0x61f8c5>|0xcc5|+0x003:'1130'
	// <0x61f8c8>|0xcc8|+0x015:'1131'
	// <0x61f8dd>|0xcdd|+0x023:'1132'
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
	// <0x61f900>|0xd00|+0x013:'1143'
	// <0x61f913>|0xd13|+0x015:'1144'
	// <0x61f928>|0xd28|+0x015:'1145'
	// <0x61f93d>|0xd3d|+0x020:'1146'
	// <0x61f95d>|0xd5d|+0x015:'1147'
	// <0x61f972>|0xd72|+0x015:'1148'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x61f987>|0xd87|+0x185:'1156'
	// <0x61fb0c>|0xf0c|+0x014:'1157'
	// <0>
	// <0x61fb20>|0xf20|+0x00f:'1159'
	// <0>
	// <0x61fb2f>|0xf2f|+0x016:'1161'
	// <0>
	// ******
}

// STATE[STUB]
float compute_gaussian_value( float x, float __formal, float std_deviation )
{
	return 0.0f;

	// FUNCTION BODY[0x61a9a0]: 2
	// <0>
	// <0x61a9a3>|0x003|+0x058:'1168'
	// ******
}

// STATE[STUB]
void get_gaussain_weights_offsets(
	float*		out_weights,
	float*		out_offsets,
	u32			buffer_size,
	float		blur_scale,
	float		blur_intencity,
	u32			num_samples
)
{
	// FUNCTION BODY[0x61aa00]: 6
	// <0x61aa05>|0x005|+0x07b:'1173'
	// <0>
	// <0x61aa80>|0x080|+0x010:'1175'
	// <0x61aa90>|0x090|-0x072:'1176'
	// <0x61aa1e>|0x01e|+0x084:'1177'
	// <0x61aaa2>|0x0a2|+0x037:'1177'
	// <0>
	// ******
}

// STATE[STUB]
void stage_lights::fill_surface( render_target_ptr surf )
{
	// LOCALS
	// u32 								offset
	// ******

	// FUNCTION BODY[0x61add0]: 15
	// <0x61add0>|0x000|+0x070:'1183'
	// <0x61ae40>|0x070|+0x006:'1184'
	// <0>
	// <1>
	// <2>
	// <0x61ae46>|0x076|+0x023:'1188'
	// <0x61ae69>|0x099|+0x05c:'1189'
	// <0x61aec5>|0x0f5|+0x049:'1190'
	// <0x61af0e>|0x13e|+0x049:'1191'
	// <0x61af57>|0x187|+0x036:'1192'
	// <0x61af8d>|0x1bd|+0x03c:'1193'
	// <0>
	// <0x61afc9>|0x1f9|+0x00f:'1195'
	// <0>
	// <0x61afd8>|0x208|+0x082:'1197'
	// ******
}

// STATE[STUB]
void stage_lights::make_skin_scattering_texture( render_surface_instance* instance, light* l )
{
	// LOCALS
	// D3D11_VIEWPORT 					tmp_viewport
	// float 							light_range
	// D3D11_VIEWPORT 					orig_viewport
	// float3 							light_direction
	// float3 							light_color
	// float3 							light_position
	// const float 						penumbra_half_angle_cosine
	// const float 						umbra_half_angle_cosine
	// float[9] 						offsets_h
	// float4[8] 						kernel_offsets
	// float[9] 						weights_v
	// float[9] 						offsets_v
	// float[9] 						weights_h
	// float4[9] 						blur_offsets_weights
	// ******

	// FUNCTION BODY[0x61d2d0]: 284
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
	// <0x61d2df>|0x00f|+0x07b:'1238'
	// <0x61d35a>|0x08a|+0x006:'1239'
	// <0x61d360>|0x090|+0x02c:'1240'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x61d38c>|0x0bc|+0x02d:'1245'
	// <0>
	// <1>
	// <2>
	// <0x61d3b9>|0x0e9|+0x032:'1249'
	// <0x61d3eb>|0x11b|+0x00e:'1250'
	// <0>
	// <1>
	// <2>
	// <0x61d3f9>|0x129|+0x040:'1254'
	// <0>
	// <1>
	// <2>
	// <0x61d439>|0x169|+0x0a7:'1258'
	// <0x61d4e0>|0x210|+0x0f1:'1259'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x61d5d1>|0x301|+0x5a2:'1265'
	// <0>
	// <0x61db73>|0x8a3|+0x048:'1267'
	// <0>
	// <0x61dbbb>|0x8eb|+0x050:'1269'
	// <0x61dc0b>|0x93b|+0x045:'1270'
	// <0x61dc50>|0x980|+0x01c:'1271'
	// <0x61dc6c>|0x99c|+0x056:'1272'
	// <0>
	// <0x61dcc2>|0x9f2|+0x022:'1274'
	// <0>
	// <0x61dce4>|0xa14|+0x016:'1276'
	// <0x61dcfa>|0xa2a|+0x027:'1277'
	// <0x61dd21>|0xa51|+0x016:'1278'
	// <0x61dd37>|0xa67|+0x01b:'1279'
	// <0x61dd52>|0xa82|+0x049:'1280'
	// <0x61dd9b>|0xacb|+0x022:'1281'
	// <0>
	// <0x61ddbd>|0xaed|+0x008:'1283'
	// <0>
	// <0x61ddc5>|0xaf5|-0x758:'1285'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x61d66d>|0x39d|+0x048:'1291'
	// <0>
	// <0x61d6b5>|0x3e5|+0x059:'1293'
	// <0>
	// <0x61d70e>|0x43e|+0x04a:'1295'
	// <0x61d758>|0x488|+0x04a:'1296'
	// <0x61d7a2>|0x4d2|+0x06b:'1297'
	// <0x61d80d>|0x53d|+0x01c:'1298'
	// <0>
	// <0x61d829>|0x559|+0x016:'1300'
	// <0x61d83f>|0x56f|+0x016:'1301'
	// <0>
	// <0x61d855>|0x585|+0x01f:'1303'
	// <0x61d874>|0x5a4|+0x050:'1304'
	// <0x61d8c4>|0x5f4|+0x01f:'1305'
	// <0x61d8e3>|0x613|+0x04d:'1306'
	// <0>
	// <1>
	// <0x61d930>|0x660|+0x07d:'1309'
	// <0x61d9ad>|0x6dd|+0x016:'1310'
	// <0>
	// <0x61d9c3>|0x6f3|+0x050:'1312'
	// <0x61da13>|0x743|+0x016:'1313'
	// <0x61da29>|0x759|+0x016:'1314'
	// <0>
	// <0x61da3f>|0x76f|+0x008:'1316'
	// <0x61da47>|0x777|+0x010:'1317'
	// <0>
	// <1>
	// <0x61da57>|0x787|+0x022:'1320'
	// <0>
	// <0x61da79>|0x7a9|+0x016:'1322'
	// <0x61da8f>|0x7bf|+0x027:'1323'
	// <0x61dab6>|0x7e6|+0x016:'1324'
	// <0x61dacc>|0x7fc|+0x01b:'1325'
	// <0x61dae7>|0x817|+0x04f:'1326'
	// <0x61db36>|0x866|+0x014:'1327'
	// <0>
	// <0x61db4a>|0x87a|+0x00b:'1329'
	// <0>
	// <0x61db55>|0x885|+0x019:'1331'
	// <0>
	// <1>
	// <0x61db6e>|0x89e|-0x571:'1334'
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
	// <0x61d5fd>|0x32d|+0x012:'1351'
	// <0x61d60f>|0x33f|+0x020:'1352'
	// <0x61d62f>|0x35f|+0x01f:'1353'
	// <0x61d64e>|0x37e|+0x01a:'1354'
	// <0>
	// <0x61d668>|0x398|+0x7aa:'1356'
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
	// <0x61de12>|0xb42|+0x06a:'1371'
	// <0x61de7c>|0xbac|+0x01f:'1372'
	// <0x61de9b>|0xbcb|+0x017:'1373'
	// <0x61deb2>|0xbe2|+0x016:'1374'
	// <0x61dec8>|0xbf8|+0x016:'1375'
	// <0x61dede>|0xc0e|+0x021:'1376'
	// <0x61deff>|0xc2f|+0x016:'1377'
	// <0x61df15>|0xc45|+0x01b:'1378'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x61df30>|0xc60|+0x05b:'1386'
	// <0>
	// <0x61df8b>|0xcbb|+0x00f:'1388'
	// <0x61df9a>|0xcca|+0x013:'1389'
	// <0x61dfad>|0xcdd|+0x015:'1390'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x61dfc2>|0xcf2|+0x00d:'1396'
	// <0x61dfcf>|0xcff|+0x01f:'1397'
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
	// <0x61dfee>|0xd1e|+0x04e:'1410'
	// <0x61e03c>|0xd6c|+0x01c:'1411'
	// <0>
	// <0x61e058>|0xd88|+0x016:'1413'
	// <0x61e06e>|0xd9e|+0x016:'1414'
	// <0>
	// <1>
	// <0x61e084>|0xdb4|+0x27f:'1417'
	// <0>
	// <0x61e303>|0x1033|+0x0d3:'1419'
	// <0x61e3d6>|0x1106|+0x023:'1420'
	// <0x61e3f9>|0x1129|+0x05b:'1421'
	// <0x61e454>|0x1184|+0x01a:'1422'
	// <0>
	// <0x61e46e>|0x119e|+0x052:'1424'
	// <0x61e4c0>|0x11f0|+0x023:'1425'
	// <0x61e4e3>|0x1213|+0x06a:'1426'
	// <0x61e54d>|0x127d|+0x017:'1427'
	// <0x61e564>|0x1294|+0x01a:'1428'
	// <0>
	// <0x61e57e>|0x12ae|+0x00b:'1430'
	// <0x61e589>|0x12b9|+0x057:'1431'
	// <0x61e5e0>|0x1310|+0x023:'1432'
	// <0x61e603>|0x1333|+0x06a:'1433'
	// <0x61e66d>|0x139d|+0x017:'1434'
	// <0x61e684>|0x13b4|+0x01a:'1435'
	// <0>
	// <0x61e69e>|0x13ce|+0x00c:'1437'
	// <0x61e6aa>|0x13da|+0x057:'1438'
	// <0x61e701>|0x1431|+0x023:'1439'
	// <0x61e724>|0x1454|+0x06a:'1440'
	// <0x61e78e>|0x14be|+0x017:'1441'
	// <0x61e7a5>|0x14d5|+0x01a:'1442'
	// <0>
	// <0x61e7bf>|0x14ef|+0x00b:'1444'
	// <0x61e7ca>|0x14fa|+0x057:'1445'
	// <0x61e821>|0x1551|+0x023:'1446'
	// <0x61e844>|0x1574|+0x06a:'1447'
	// <0x61e8ae>|0x15de|+0x017:'1448'
	// <0x61e8c5>|0x15f5|+0x01a:'1449'
	// <0>
	// <0x61e8df>|0x160f|+0x00c:'1451'
	// <0x61e8eb>|0x161b|+0x057:'1452'
	// <0x61e942>|0x1672|+0x023:'1453'
	// <0x61e965>|0x1695|+0x023:'1454'
	// <0x61e988>|0x16b8|+0x01a:'1455'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x61e9a2>|0x16d2|+0x055:'1460'
	// <0x61e9f7>|0x1727|+0x00c:'1461'
	// <0x61ea03>|0x1733|+0x030:'1462'
	// <0>
	// <0x61ea33>|0x1763|+0x057:'1464'
	// <0x61ea8a>|0x17ba|+0x01d:'1465'
	// <0x61eaa7>|0x17d7|+0x01c:'1466'
	// <0x61eac3>|0x17f3|+0x017:'1467'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x61eada>|0x180a|+0x05e:'1475'
	// <0>
	// <0x61eb38>|0x1868|+0x00c:'1477'
	// <0x61eb44>|0x1874|+0x013:'1478'
	// <0x61eb57>|0x1887|+0x015:'1479'
	// <0>
	// <1>
	// <2>
	// <0x61eb6c>|0x189c|+0x048:'1483'
	// <0x61ebb4>|0x18e4|+0x00c:'1484'
	// <0x61ebc0>|0x18f0|+0x030:'1485'
	// ******
}

// STATE[STUB]
void stage_lights::render_particle_probe_lighting(
	render_particle_emitter_instance*		instance,
	environment_probe*						probe,
	u32										num_particles
)
{
	// FUNCTION BODY[0x61cec0]: 34
	// <0x61cec0>|0x000|+0x011:'1490'
	// <0>
	// <0x61ced1>|0x011|+0x00e:'1492'
	// <0>
	// <1>
	// <0x61cedf>|0x01f|+0x023:'1495'
	// <0>
	// <0x61cf02>|0x042|+0x026:'1497'
	// <0>
	// <1>
	// <2>
	// <0x61cf28>|0x068|+0x09a:'1501'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x61cfc2>|0x102|+0x097:'1510'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x61d059>|0x199|+0x232:'1518'
	// <0x61d28b>|0x3cb|+0x014:'1519'
	// <0>
	// <0x61d29f>|0x3df|+0x00f:'1521'
	// <0>
	// <0x61d2ae>|0x3ee|+0x013:'1523'
	// ******
}

// STATE[STUB]
void stage_lights::render_model_probe_lighting(
	render_surface_instance*	instance,
	environment_probe*			probe,
	float						min_probe_scale
)
{
	// FUNCTION BODY[0x61cc70]: 19
	// <0x61cc78>|0x008|+0x02c:'1528'
	// <0>
	// <0x61cca4>|0x034|+0x029:'1530'
	// <0>
	// <0x61cccd>|0x05d|+0x020:'1532'
	// <0>
	// <0x61cced>|0x07d|+0x004:'1534'
	// <0>
	// <0x61ccf1>|0x081|+0x0af:'1536'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x61cda0>|0x130|+0x091:'1545'
	// <0x61ce31>|0x1c1|+0x07b:'1546'
	// ******
}

// STATE[STUB]
void stage_lights::render_model_lighting( render_surface_instance* instance, light* l )
{
	// LOCALS
	// float 							light_range
	// float3 							light_direction
	// float3 							light_color
	// float3 							light_position
	// const float 						penumbra_half_angle_cosine
	// float4x4 						obb_world
	// const float 						penumbra_half_angle_cosine
	// const float 						umbra_half_angle_cosine
	// ******

	// STATICS
	// static < NoType > 				 = <0x6225d4>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x610aa4 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN13") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x610b6c }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN12") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x610da6 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN11") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x610fbe }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN10") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6111b1 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN9") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x611287 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6112ec }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// ******

	// FUNCTION BODY[0x621850]: 213
	// <0x621861>|0x011|+0x007:'1551'
	// <0>
	// <1>
	// <2>
	// <0x621868>|0x018|+0x00e:'1555'
	// <0x621876>|0x026|+0x0a0:'1556'
	// <0x621916>|0x0c6|+0x022:'1557'
	// <0>
	// <0x621938>|0x0e8|+0x0fa:'1559'
	// <0>
	// <1>
	// <2>
	// <0x621a32>|0x1e2|+0x006:'1563'
	// <0>
	// <0x621a38>|0x1e8|+0x016:'1565'
	// <0>
	// <0x621a4e>|0x1fe|+0x01a:'1567'
	// <0x621a68>|0x218|+0x00d:'1568'
	// <0>
	// <0x621a75>|0x225|+0x016:'1570'
	// <0>
	// <1>
	// <2>
	// <0x621a8b>|0x23b|+0x002:'1574'
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
	// <0x621a8d>|0x23d|+0x002:'1609'
	// <0>
	// <0x621a8f>|0x23f|+0x003:'1611'
	// <0>
	// <1>
	// <0x621a92>|0x242|+0x002:'1614'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x621a94>|0x244|+0x010:'1619'
	// <0>
	// <0x621aa4>|0x254|+0x00d:'1621'
	// <0x621ab1>|0x261|+0x050:'1622'
	// <0x621b01>|0x2b1|+0x04a:'1623'
	// <0x621b4b>|0x2fb|+0x01c:'1624'
	// <0>
	// <1>
	// <0x621b67>|0x317|+0x005:'1627'
	// <0>
	// <1>
	// <0x621b6c>|0x31c|+0x00d:'1630'
	// <0x621b79>|0x329|+0x050:'1631'
	// <0x621bc9>|0x379|+0x04a:'1632'
	// <0x621c13>|0x3c3|+0x04a:'1633'
	// <0x621c5d>|0x40d|+0x01c:'1634'
	// <0>
	// <0x621c79>|0x429|+0x01f:'1636'
	// <0x621c98>|0x448|+0x04d:'1637'
	// <0>
	// <1>
	// <0x621ce5>|0x495|+0x0a0:'1640'
	// <0x621d85>|0x535|+0x01c:'1641'
	// <0>
	// <0x621da1>|0x551|+0x005:'1643'
	// <0>
	// <1>
	// <0x621da6>|0x556|+0x00d:'1646'
	// <0>
	// <0x621db3>|0x563|+0x050:'1648'
	// <0x621e03>|0x5b3|+0x04a:'1649'
	// <0x621e4d>|0x5fd|+0x01c:'1650'
	// <0>
	// <0x621e69>|0x619|+0x00e:'1652'
	// <0x621e77>|0x627|+0x016:'1653'
	// <0x621e8d>|0x63d|+0x069:'1654'
	// <0>
	// <0x621ef6>|0x6a6|+0x042:'1656'
	// <0x621f38>|0x6e8|+0x023:'1657'
	// <0x621f5b>|0x70b|+0x01f:'1658'
	// <0>
	// <0x621f7a>|0x72a|+0x003:'1660'
	// <0x621f7d>|0x72d|+0x016:'1661'
	// <0x621f93>|0x743|+0x01f:'1662'
	// <0>
	// <0x621fb2>|0x762|+0x00c:'1664'
	// <0>
	// <1>
	// <0x621fbe>|0x76e|+0x00d:'1667'
	// <0>
	// <0x621fcb>|0x77b|+0x050:'1669'
	// <0x62201b>|0x7cb|+0x04a:'1670'
	// <0x622065>|0x815|+0x04a:'1671'
	// <0x6220af>|0x85f|+0x01c:'1672'
	// <0>
	// <0x6220cb>|0x87b|+0x016:'1674'
	// <0x6220e1>|0x891|+0x016:'1675'
	// <0>
	// <0x6220f7>|0x8a7|+0x04d:'1677'
	// <0x622144>|0x8f4|+0x016:'1678'
	// <0x62215a>|0x90a|+0x016:'1679'
	// <0>
	// <0x622170>|0x920|+0x008:'1681'
	// <0x622178>|0x928|+0x015:'1682'
	// <0x62218d>|0x93d|+0x01f:'1683'
	// <0>
	// <0x6221ac>|0x95c|+0x005:'1685'
	// <0>
	// <1>
	// <0x6221b1>|0x961|+0x01b:'1688'
	// <0>
	// <0x6221cc>|0x97c|+0x007:'1690'
	// <0>
	// <0x6221d3>|0x983|+0x012:'1692'
	// <0x6221e5>|0x995|+0x03c:'1693'
	// <0>
	// <0x622221>|0x9d1|+0x01c:'1695'
	// <0x62223d>|0x9ed|+0x045:'1696'
	// <0>
	// <1>
	// <0x622282>|0xa32|+0x005:'1699'
	// <0>
	// <1>
	// <0x622287>|0xa37|+0x00d:'1702'
	// <0x622294>|0xa44|+0x018:'1703'
	// <0x6222ac>|0xa5c|+0x010:'1704'
	// <0x6222bc>|0xa6c|+0x012:'1705'
	// <0>
	// <0x6222ce>|0xa7e|+0x012:'1707'
	// <0>
	// <0x6222e0>|0xa90|+0x007:'1709'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6222e7>|0xa97|+0x005:'1717'
	// <0>
	// <1>
	// <0x6222ec>|0xa9c|+0x00d:'1720'
	// <0x6222f9>|0xaa9|+0x018:'1721'
	// <0x622311>|0xac1|+0x010:'1722'
	// <0x622321>|0xad1|+0x036:'1723'
	// <0x622357>|0xb07|+0x012:'1724'
	// <0>
	// <0x622369>|0xb19|+0x01f:'1726'
	// <0x622388>|0xb38|+0x013:'1727'
	// <0x62239b>|0xb4b|+0x01f:'1728'
	// <0x6223ba>|0xb6a|+0x013:'1729'
	// <0>
	// <1>
	// <0x6223cd>|0xb7d|+0x03e:'1732'
	// <0x62240b>|0xbbb|+0x012:'1733'
	// <0>
	// <1>
	// <0x62241d>|0xbcd|+0x029:'1736'
	// <0>
	// <0x622446>|0xbf6|+0x010:'1738'
	// <0x622456>|0xc06|+0x012:'1739'
	// <0x622468>|0xc18|+0x012:'1740'
	// <0>
	// <1>
	// <0x62247a>|0xc2a|+0x014:'1743'
	// <0x62248e>|0xc3e|+0x014:'1744'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6224a2>|0xc52|+0x05b:'1753'
	// <0x6224fd>|0xcad|+0x01e:'1754'
	// <0x62251b>|0xccb|+0x014:'1755'
	// <0x62252f>|0xcdf|+0x016:'1756'
	// <0x622545>|0xcf5|+0x016:'1757'
	// <0x62255b>|0xd0b|+0x021:'1758'
	// <0x62257c>|0xd2c|+0x016:'1759'
	// <0x622592>|0xd42|+0x016:'1760'
	// <0x6225a8>|0xd58|-0xb2a:'1761'
	// <0>
	// <1>
	// <0x621a7e>|0x22e|+0xb46:'1764'
	// ******
}

// STATE[STUB]
void stage_lights::render_speedtree_lighting(
	lod_entry const*					lod,
	SpeedTree::CInstance const*			instance,
	SpeedTree::SInstanceLod const*		instance_lod,
	speedtree_tree_component*			tree_component,
	light*								l
)
{
	// LOCALS
	// float 							light_range
	// float3 							light_direction
	// float3 							light_color
	// float3 							light_position
	// const float 						penumbra_half_angle_cosine
	// float4x4 						obb_world
	// const float 						penumbra_half_angle_cosine
	// const float 						umbra_half_angle_cosine
	// ******

	// STATICS
	// static < NoType > 				 = <0x61cc50>;
	// ******

	// CALL SITE INFO
	// <0x61cbc7> -> SpeedTree::EGeometryType < unknown >() const
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x60afa4 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN14") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x60b08c }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN13") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x60b2e6 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN12") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x60b521 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN11") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x60b71a }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN10") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x60b78b }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN8") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x60b81b }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN7") })
	// ******

	// FUNCTION BODY[0x61bde0]: 172
	// <0x61bdf1>|0x011|+0x007:'1772'
	// <0x61bdf8>|0x018|+0x00e:'1773'
	// <0x61be06>|0x026|+0x0a0:'1774'
	// <0x61bea6>|0x0c6|+0x03a:'1775'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x61bee0>|0x100|+0x0c4:'1783'
	// <0>
	// <0x61bfa4>|0x1c4|+0x030:'1785'
	// <0x61bfd4>|0x1f4|+0x050:'1786'
	// <0x61c024>|0x244|+0x04a:'1787'
	// <0x61c06e>|0x28e|+0x019:'1788'
	// <0>
	// <0x61c087>|0x2a7|+0x005:'1790'
	// <0>
	// <1>
	// <0x61c08c>|0x2ac|+0x030:'1793'
	// <0x61c0bc>|0x2dc|+0x050:'1794'
	// <0x61c10c>|0x32c|+0x04a:'1795'
	// <0x61c156>|0x376|+0x04a:'1796'
	// <0x61c1a0>|0x3c0|+0x01c:'1797'
	// <0>
	// <0x61c1bc>|0x3dc|+0x01f:'1799'
	// <0x61c1db>|0x3fb|+0x04d:'1800'
	// <0>
	// <1>
	// <0x61c228>|0x448|+0x0a0:'1803'
	// <0x61c2c8>|0x4e8|+0x019:'1804'
	// <0>
	// <0x61c2e1>|0x501|+0x005:'1806'
	// <0>
	// <1>
	// <0x61c2e6>|0x506|+0x030:'1809'
	// <0>
	// <0x61c316>|0x536|+0x050:'1811'
	// <0x61c366>|0x586|+0x04a:'1812'
	// <0x61c3b0>|0x5d0|+0x01c:'1813'
	// <0>
	// <0x61c3cc>|0x5ec|+0x00e:'1815'
	// <0x61c3da>|0x5fa|+0x016:'1816'
	// <0x61c3f0>|0x610|+0x069:'1817'
	// <0>
	// <0x61c459>|0x679|+0x042:'1819'
	// <0x61c49b>|0x6bb|+0x023:'1820'
	// <0x61c4be>|0x6de|+0x01f:'1821'
	// <0>
	// <0x61c4dd>|0x6fd|+0x003:'1823'
	// <0x61c4e0>|0x700|+0x016:'1824'
	// <0x61c4f6>|0x716|+0x01f:'1825'
	// <0>
	// <0x61c515>|0x735|+0x00c:'1827'
	// <0>
	// <1>
	// <0x61c521>|0x741|+0x030:'1830'
	// <0>
	// <0x61c551>|0x771|+0x04b:'1832'
	// <0x61c59c>|0x7bc|+0x045:'1833'
	// <0x61c5e1>|0x801|+0x045:'1834'
	// <0x61c626>|0x846|+0x01c:'1835'
	// <0>
	// <0x61c642>|0x862|+0x016:'1837'
	// <0x61c658>|0x878|+0x016:'1838'
	// <0>
	// <0x61c66e>|0x88e|+0x04d:'1840'
	// <0x61c6bb>|0x8db|+0x016:'1841'
	// <0x61c6d1>|0x8f1|+0x016:'1842'
	// <0>
	// <0x61c6e7>|0x907|+0x008:'1844'
	// <0x61c6ef>|0x90f|+0x015:'1845'
	// <0x61c704>|0x924|+0x011:'1846'
	// <0>
	// <0x61c715>|0x935|+0x005:'1848'
	// <0>
	// <1>
	// <0x61c71a>|0x93a|+0x03f:'1851'
	// <0>
	// <0x61c759>|0x979|+0x015:'1853'
	// <0x61c76e>|0x98e|+0x018:'1854'
	// <0>
	// <1>
	// <0x61c786>|0x9a6|+0x005:'1857'
	// <0>
	// <1>
	// <0x61c78b>|0x9ab|+0x01a:'1860'
	// <0x61c7a5>|0x9c5|+0x020:'1861'
	// <0x61c7c5>|0x9e5|+0x014:'1862'
	// <0x61c7d9>|0x9f9|+0x016:'1863'
	// <0>
	// <0x61c7ef>|0xa0f|+0x016:'1865'
	// <0>
	// <0x61c805>|0xa25|+0x011:'1867'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x61c816>|0xa36|+0x005:'1875'
	// <0>
	// <1>
	// <0x61c81b>|0xa3b|+0x01a:'1878'
	// <0x61c835>|0xa55|+0x020:'1879'
	// <0x61c855>|0xa75|+0x014:'1880'
	// <0x61c869>|0xa89|+0x03a:'1881'
	// <0x61c8a3>|0xac3|+0x016:'1882'
	// <0>
	// <0x61c8b9>|0xad9|+0x01f:'1884'
	// <0x61c8d8>|0xaf8|+0x017:'1885'
	// <0x61c8ef>|0xb0f|+0x01f:'1886'
	// <0x61c90e>|0xb2e|+0x014:'1887'
	// <0>
	// <1>
	// <0x61c922>|0xb42|+0x045:'1890'
	// <0x61c967>|0xb87|+0x016:'1891'
	// <0>
	// <1>
	// <0x61c97d>|0xb9d|+0x02e:'1894'
	// <0>
	// <0x61c9ab>|0xbcb|+0x014:'1896'
	// <0x61c9bf>|0xbdf|+0x016:'1897'
	// <0x61c9d5>|0xbf5|+0x016:'1898'
	// <0>
	// <0x61c9eb>|0xc0b|+0x003:'1900'
	// <0x61c9ee>|0xc0e|+0x01b:'1901'
	// <0x61ca09>|0xc29|+0x01e:'1902'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x61ca27>|0xc47|+0x05b:'1911'
	// <0x61ca82>|0xca2|+0x01f:'1912'
	// <0x61caa1>|0xcc1|+0x014:'1913'
	// <0x61cab5>|0xcd5|+0x016:'1914'
	// <0x61cacb>|0xceb|+0x016:'1915'
	// <0x61cae1>|0xd01|+0x016:'1916'
	// <0x61caf7>|0xd17|+0x016:'1917'
	// <0>
	// <0x61cb0d>|0xd2d|+0x02a:'1919'
	// <0x61cb37>|0xd57|+0x020:'1920'
	// <0>
	// <0x61cb57>|0xd77|+0x02a:'1922'
	// <0>
	// <0x61cb81>|0xda1|+0x00a:'1924'
	// <0>
	// <1>
	// <0x61cb8b>|0xdab|+0x023:'1927'
	// <0x61cbae>|0xdce|+0x002:'1928'
	// <0x61cbb0>|0xdd0|+0x010:'1929'
	// <0>
	// <0x61cbc0>|0xde0|+0x00e:'1931'
	// <0>
	// <0x61cbce>|0xdee|+0x01a:'1933'
	// <0x61cbe8>|0xe08|+0x008:'1934'
	// <0x61cbf0>|0xe10|+0x007:'1935'
	// <0>
	// <0x61cbf7>|0xe17|+0x002:'1937'
	// <0>
	// <0x61cbf9>|0xe19|+0x023:'1939'
	// <0x61cc1c>|0xe3c|+0x008:'1940'
	// <0x61cc24>|0xe44|+0x01c:'1941'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void stage_lights::execute_disabled( )
{
	// LOCALS
	// float 							packed_color
	// ******

	// FUNCTION BODY[0x61aae0]: 11
	// <0x61aae1>|0x001|+0x0da:'1948'
	// <0x61abbb>|0x0db|+0x006:'1949'
	// <0>
	// <0x61abc1>|0x0e1|+0x030:'1951'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x61abf1>|0x111|+0x045:'1958'
	// ******
}

// STATE[STUB]
void stage_lights::execute( )
{
	// LOCALS
	// vector< light_data > const& 		e_lights
	// environment_probe* 				found_probe
	// D3D11_VIEWPORT 					tmp_viewport
	// vectora< particle::render_particle_emitter_instance* > emitters
	// environment_probe** 				probe_it
	// render_surface_instance** 		it_d
	// D3D11_VIEWPORT 					orig_viewport
	// float 							max_rad
	// render_surface_instance* const* 	end_d
	// float 							min_dist
	// vector< render_surface_instance* > m_dynamic_visuals
	// render_surface_instance& 		instance
	// math::aabb 						bbox
	// const float 						model_max_scale
	// float3 							model_location
	// const float 						model_probe_sqdist
	// vector< speedtree_forest::tree_render_info > visible_trees
	// particle::render_particle_emitter_instance* const* it
	// particle::enum_particle_render_mode particle_render_mode
	// math::aabb 						bbox
	// ******

	// TYPEDEFS
	// typedef
	// 	stage_lights::execute::__l15::sort_by_size_predicate
	// 	stage_lights::execute::__l15::sort_by_size_predicate;

	// ******

	// CALL SITE INFO
	// <0x6240f0> -> bool < unknown >() const
	// <0x6240fd> -> void < unknown >()
	// <0x624428> -> math::aabb < unknown >()
	// <0x624bbc> -> void < unknown >( float4x4 const&, vectora< particle::render_particle_emitter_instance* >& )
	// ******

	// FUNCTION BODY[0x6240c0]: 290
	// <0>
	// <1>
	// <0x6240d4>|0x014|+0x017:'1965'
	// <0>
	// <1>
	// <0x6240eb>|0x02b|+0x00b:'1968'
	// <0>
	// <0x6240f6>|0x036|+0x011:'1970'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x624107>|0x047|+0x020:'1979'
	// <0>
	// <0x624127>|0x067|+0x014:'1981'
	// <0x62413b>|0x07b|+0x082:'1982'
	// <0x6241bd>|0x0fd|+0x01b:'1983'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6241d8>|0x118|+0x00d:'1989'
	// <0>
	// <0x6241e5>|0x125|+0x01b:'1991'
	// <0>
	// <1>
	// <2>
	// <0x624200>|0x140|+0x024:'1995'
	// <0>
	// <1>
	// <0x624224>|0x164|+0x00f:'1998'
	// <0>
	// <1>
	// <0x624233>|0x173|+0x01f:'2001'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x624252>|0x192|+0x144:'2007'
	// <0x624396>|0x2d6|-0x0bc:'2007'
	// <0>
	// <1>
	// <0x6242da>|0x21a|+0x009:'2010'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6242e3>|0x223|+0x00b:'2019'
	// <0x6242ee>|0x22e|+0x003:'2020'
	// <0>
	// <0x6242f1>|0x231|+0x017:'2022'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x624308>|0x248|+0x04e:'2032'
	// <0>
	// <0x624356>|0x296|+0x004:'2034'
	// <0x62435a>|0x29a|+0x00c:'2035'
	// <0>
	// <0x624366>|0x2a6|+0x4e5:'2037'
	// <0x62484b>|0x78b|-0x4db:'2037'
	// <0>
	// <0x624370>|0x2b0|+0x006:'2039'
	// <0x624376>|0x2b6|+0x027:'2040'
	// <0x62439d>|0x2dd|+0x00a:'2040'
	// <0>
	// <1>
	// <0x6243a7>|0x2e7|+0x00d:'2043'
	// <0>
	// <1>
	// <0x6243b4>|0x2f4|+0x00c:'2046'
	// <0x6243c0>|0x300|+0x008:'2047'
	// <0>
	// <1>
	// <2>
	// <0x6243c8>|0x308|+0x377:'2051'
	// <0x62473f>|0x67f|-0x35f:'2051'
	// <0>
	// <1>
	// <2>
	// <0x6243e0>|0x320|+0x03b:'2055'
	// <0>
	// <1>
	// <0x62441b>|0x35b|+0x00f:'2058'
	// <0x62442a>|0x36a|+0x00c:'2059'
	// <0>
	// <0x624436>|0x376|+0x008:'2061'
	// <0x62443e>|0x37e|+0x04c:'2062'
	// <0x62448a>|0x3ca|+0x026:'2063'
	// <0>
	// <0x6244b0>|0x3f0|+0x03e:'2065'
	// <0>
	// <1>
	// <0x6244ee>|0x42e|+0x013:'2068'
	// <0>
	// <0x624501>|0x441|+0x006:'2070'
	// <0>
	// <1>
	// <2>
	// <0x624507>|0x447|+0x005:'2074'
	// <0>
	// <0x62450c>|0x44c|+0x056:'2076'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x624562>|0x4a2|+0x0ca:'2086'
	// <0x62462c>|0x56c|+0x00c:'2087'
	// <0x624638>|0x578|+0x009:'2088'
	// <0>
	// <0x624641>|0x581|+0x0c8:'2090'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x624709>|0x649|+0x021:'2095'
	// <0>
	// <0x62472a>|0x66a|+0x00b:'2097'
	// <0>
	// <0x624735>|0x675|+0x006:'2099'
	// <0x62473b>|0x67b|+0x019:'2100'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x624754>|0x694|+0x008:'2107'
	// <0>
	// <1>
	// <0x62475c>|0x69c|+0x007:'2110'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x624763>|0x6a3|+0x064:'2118'
	// <0>
	// <0x6247c7>|0x707|+0x009:'2120'
	// <0x6247d0>|0x710|+0x00c:'2121'
	// <0>
	// <0x6247dc>|0x71c|+0x00b:'2123'
	// <0>
	// <0x6247e7>|0x727|+0x00d:'2125'
	// <0>
	// <0x6247f4>|0x734|+0x021:'2127'
	// <0x624815>|0x755|+0x00c:'2128'
	// <0x624821>|0x761|+0x03f:'2129'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x624860>|0x7a0|+0x02b:'2134'
	// <0>
	// <0x62488b>|0x7cb|+0x040:'2136'
	// <0x6248cb>|0x80b|-0x032:'2136'
	// <0>
	// <0x624899>|0x7d9|+0x022:'2138'
	// <0>
	// <0x6248bb>|0x7fb|+0x10d:'2140'
	// <0x6249c8>|0x908|-0x0f8:'2140'
	// <0>
	// <0x6248d0>|0x810|+0x01b:'2142'
	// <0>
	// <0x6248eb>|0x82b|+0x056:'2144'
	// <0>
	// <1>
	// <0x624941>|0x881|+0x027:'2147'
	// <0>
	// <0x624968>|0x8a8|+0x00d:'2149'
	// <0x624975>|0x8b5|+0x01a:'2150'
	// <0>
	// <0x62498f>|0x8cf|+0x011:'2152'
	// <0>
	// <0x6249a0>|0x8e0|+0x036:'2154'
	// <0>
	// <1>
	// <0x6249d6>|0x916|+0x019:'2157'
	// <0>
	// <0x6249ef>|0x92f|+0x00d:'2159'
	// <0x6249fc>|0x93c|+0x002:'2160'
	// <0>
	// <1>
	// <0x6249fe>|0x93e|+0x013:'2163'
	// <0>
	// <0x624a11>|0x951|+0x004:'2165'
	// <0>
	// <0x624a15>|0x955|+0x014:'2167'
	// <0x624a29>|0x969|+0x026:'2168'
	// <0>
	// <1>
	// <0x624a4f>|0x98f|+0x02b:'2171'
	// <0>
	// <1>
	// <0x624a7a>|0x9ba|+0x003:'2174'
	// <0>
	// <0x624a7d>|0x9bd|+0x050:'2176'
	// <0x624acd>|0xa0d|+0x03e:'2177'
	// <0x624b0b>|0xa4b|+0x003:'2178'
	// <0>
	// <0x624b0e>|0xa4e|+0x03b:'2180'
	// <0>
	// <0x624b49>|0xa89|+0x042:'2182'
	// <0>
	// <1>
	// <0x624b8b>|0xacb|+0x018:'2185'
	// <0>
	// <0x624ba3>|0xae3|+0x01b:'2187'
	// <0>
	// <0x624bbe>|0xafe|+0x201:'2189'
	// <0x624dbf>|0xcff|-0x1ed:'2189'
	// <0>
	// <0x624bd2>|0xb12|+0x002:'2191'
	// <0x624bd4>|0xb14|+0x022:'2192'
	// <0>
	// <0x624bf6>|0xb36|+0x008:'2194'
	// <0>
	// <1>
	// <0x624bfe>|0xb3e|+0x00d:'2197'
	// <0>
	// <0x624c0b>|0xb4b|+0x025:'2199'
	// <0>
	// <1>
	// <2>
	// <0x624c30>|0xb70|+0x02c:'2203'
	// <0>
	// <1>
	// <0x624c5c>|0xb9c|+0x02a:'2206'
	// <0x624c86>|0xbc6|+0x01b:'2207'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x624ca1>|0xbe1|+0x00f:'2215'
	// <0>
	// <0x624cb0>|0xbf0|+0x006:'2217'
	// <0x624cb6>|0xbf6|+0x017:'2218'
	// <0>
	// <1>
	// <2>
	// <0x624ccd>|0xc0d|+0x007:'2222'
	// <0>
	// <0x624cd4>|0xc14|+0x00f:'2224'
	// <0>
	// <1>
	// <0x624ce3>|0xc23|+0x07a:'2227'
	// <0>
	// <0x624d5d>|0xc9d|+0x009:'2229'
	// <0x624d66>|0xca6|+0x013:'2230'
	// <0>
	// <0x624d79>|0xcb9|+0x017:'2232'
	// <0>
	// <0x624d90>|0xcd0|+0x010:'2234'
	// <0>
	// <0x624da0>|0xce0|+0x006:'2236'
	// <0x624da6>|0xce6|+0x02e:'2237'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x624dd4>|0xd14|+0x020:'2244'
	// <0>
	// <0x624df4>|0xd34|+0x02e:'2246'
	// <0>
	// <0x624e22>|0xd62|+0x01b:'2248'
	// <0>
	// <1>
	// <0x624e3d>|0xd7d|+0x00d:'2251'
	// <0x624e4a>|0xd8a|-0xd4b:'2252'
	// <0x6240ff>|0x03f|+0x14b:'2253'
	// <0x62424a>|0x18a|+0x7fd:'2253'
	// <0x624a47>|0x987|+0x41e:'2253'
	// ******
}

// STATE[STUB]
bool `vostok::render::stage_lights::execute'::`15'::sort_by_size_predicate::operator()(
	environment_probe const*	arg_0 /* vostok::render::environment_probe const* left */,
	environment_probe const*	arg_1 /* vostok::render::environment_probe const* right */
) const
{
	return false;

	// FUNCTION BODY[0x61a8d0]: 1
	// <0x61a8d0>|0x000|+0x017:'2015'
	// <0x61a8e7>|0x017|-0x001:'2015'
	// <0x61a8e6>|0x016|+0x003:'2016'
	// ******
}

// STATE[STUB]
u32 stage_lights::index_to_shadow_size( u32 size_index ) const
{
	return 0;

	// FUNCTION BODY[0x61a8b0]: 10
	// <0x61a8b0>|0x000|+0x016:'2258'
	// <0>
	// <1>
	// <0x61a8c6>|0x016|-0x006:'2261'
	// <0>
	// <0x61a8c0>|0x010|-0x006:'2263'
	// <0>
	// <0x61a8ba>|0x00a|+0x005:'2265'
	// <0>
	// <1>
	// <0x61a8bf>|0x00f|+0x006:'2268'
	// <0x61a8c5>|0x015|+0x006:'2268'
	// ******
}

// STATE[STUB]
void stage_lights::render_shadowed_light( light* l )
{
	// LOCALS
	// float 							light_range
	// res_geometry_ptr 				geometry
	// float3 							light_color
	// float3 							light_position
	// u32 								face_index
	// float4x4 						local_to_world
	// const float 						range_X_tan_penumbra_angle_div_2
	// float4x4 						face_view_matrix
	// const bool 						distribute_shadow
	// float4x4 						scale_matrix
	// float4x4 						face_projection_matrix
	// u32 								tech_index
	// u32 								tech_index
	// float4x4 						obb_world
	// u32 								tech_index
	// ******

	// FUNCTION BODY[0x6208b0]: 179
	// <0>
	// <0x6208bd>|0x00d|+0x00e:'2273'
	// <0x6208cb>|0x01b|+0x0c9:'2274'
	// <0>
	// <1>
	// <0x620994>|0x0e4|+0xe7d:'2277'
	// <0>
	// <0x621811>|0xf61|-0xe5c:'2279'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x6209b5>|0x105|+0x156:'2289'
	// <0x620b0b>|0x25b|+0x029:'2290'
	// <0>
	// <0x620b34>|0x284|+0x00e:'2292'
	// <0x620b42>|0x292|+0x02c:'2293'
	// <0x620b6e>|0x2be|+0x01d:'2294'
	// <0>
	// <0x620b8b>|0x2db|+0x020:'2296'
	// <0>
	// <0x620bab>|0x2fb|+0x02e:'2298'
	// <0>
	// <0x620bd9>|0x329|+0x02c:'2300'
	// <0>
	// <1>
	// <0x620c05>|0x355|+0x00c:'2303'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x620c11>|0x361|+0x02a:'2313'
	// <0>
	// <1>
	// <0x620c3b>|0x38b|+0x010:'2316'
	// <0>
	// <1>
	// <2>
	// <0x620c4b>|0x39b|+0x0d5:'2320'
	// <0x620d20>|0x470|+0x01b:'2321'
	// <0>
	// <1>
	// <0x620d3b>|0x48b|+0x02e:'2324'
	// <0x620d69>|0x4b9|+0x009:'2325'
	// <0x620d72>|0x4c2|+0x07c:'2326'
	// <0>
	// <1>
	// <2>
	// <0x620dee>|0x53e|+0x023:'2330'
	// <0x620e11>|0x561|+0x51f:'2330'
	// <0>
	// <1>
	// <2>
	// <0x621330>|0xa80|+0x00b:'2334'
	// <0>
	// <0x62133b>|0xa8b|+0x024:'2336'
	// <0x62135f>|0xaaf|+0x054:'2337'
	// <0x6213b3>|0xb03|+0x058:'2338'
	// <0x62140b>|0xb5b|+0x047:'2339'
	// <0x621452>|0xba2|+0x047:'2340'
	// <0x621499>|0xbe9|+0x029:'2341'
	// <0>
	// <0x6214c2>|0xc12|+0x002:'2343'
	// <0>
	// <0x6214c4>|0xc14|+0x024:'2345'
	// <0>
	// <1>
	// <0x6214e8>|0xc38|+0x04b:'2348'
	// <0x621533>|0xc83|+0x05f:'2349'
	// <0x621592>|0xce2|+0x04f:'2350'
	// <0>
	// <0x6215e1>|0xd31|+0x04f:'2352'
	// <0x621630>|0xd80|-0x311:'2353'
	// <0x62131f>|0xa6f|+0x360:'2354'
	// <0x62167f>|0xdcf|+0x045:'2354'
	// <0>
	// <0x6216c4>|0xe14|+0x045:'2356'
	// <0x621709>|0xe59|+0x04e:'2357'
	// <0x621757>|0xea7|+0x04e:'2358'
	// <0>
	// <0x6217a5>|0xef5|+0x009:'2360'
	// <0x6217ae>|0xefe|+0x016:'2361'
	// <0x6217c4>|0xf14|+0x01c:'2362'
	// <0x6217e0>|0xf30|+0x00c:'2363'
	// <0x6217ec>|0xf3c|-0x76c:'2364'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x621080>|0x7d0|+0x004:'2369'
	// <0>
	// <0x621084>|0x7d4|+0x00b:'2371'
	// <0>
	// <0x62108f>|0x7df|+0x024:'2373'
	// <0x6210b3>|0x803|+0x028:'2374'
	// <0x6210db>|0x82b|+0x027:'2375'
	// <0x621102>|0x852|+0x016:'2376'
	// <0x621118>|0x868|+0x016:'2377'
	// <0x62112e>|0x87e|+0x029:'2378'
	// <0>
	// <0x621157>|0x8a7|+0x002:'2380'
	// <0>
	// <0x621159>|0x8a9|+0x024:'2382'
	// <0>
	// <1>
	// <0x62117d>|0x8cd|+0x01d:'2385'
	// <0x62119a>|0x8ea|+0x01c:'2386'
	// <0x6211b6>|0x906|+0x01d:'2387'
	// <0>
	// <0x6211d3>|0x923|+0x016:'2389'
	// <0x6211e9>|0x939|+0x016:'2390'
	// <0>
	// <0x6211ff>|0x94f|+0x011:'2392'
	// <0x621210>|0x960|+0x019:'2393'
	// <0x621229>|0x979|+0x037:'2394'
	// <0>
	// <0x621260>|0x9b0|+0x019:'2396'
	// <0x621279>|0x9c9|+0x01c:'2397'
	// <0x621295>|0x9e5|+0x01f:'2398'
	// <0>
	// <0x6212b4>|0xa04|+0x009:'2400'
	// <0x6212bd>|0xa0d|+0x010:'2401'
	// <0x6212cd>|0xa1d|+0x01c:'2402'
	// <0x6212e9>|0xa39|+0x00c:'2403'
	// <0x6212f5>|0xa45|+0x01a:'2404'
	// <0>
	// <0x62130f>|0xa5f|-0x50f:'2406'
	// <0>
	// <1>
	// <0x620e00>|0x550|+0x01a:'2409'
	// <0x620e1a>|0x56a|+0x00a:'2409'
	// <0>
	// <0x620e24>|0x574|+0x00b:'2411'
	// <0>
	// <0x620e2f>|0x57f|+0x024:'2413'
	// <0x620e53>|0x5a3|+0x028:'2414'
	// <0x620e7b>|0x5cb|+0x027:'2415'
	// <0x620ea2>|0x5f2|+0x016:'2416'
	// <0x620eb8>|0x608|+0x016:'2417'
	// <0x620ece>|0x61e|+0x029:'2418'
	// <0>
	// <0x620ef7>|0x647|+0x002:'2420'
	// <0>
	// <0x620ef9>|0x649|+0x024:'2422'
	// <0>
	// <0x620f1d>|0x66d|+0x020:'2424'
	// <0x620f3d>|0x68d|+0x011:'2425'
	// <0x620f4e>|0x69e|+0x01c:'2426'
	// <0>
	// <0x620f6a>|0x6ba|+0x01c:'2428'
	// <0x620f86>|0x6d6|+0x01c:'2429'
	// <0>
	// <0x620fa2>|0x6f2|+0x01f:'2431'
	// <0>
	// <0x620fc1>|0x711|+0x011:'2433'
	// <0x620fd2>|0x722|+0x01c:'2434'
	// <0x620fee>|0x73e|+0x01f:'2435'
	// <0>
	// <0x62100d>|0x75d|+0x008:'2437'
	// <0x621015>|0x765|+0x010:'2438'
	// <0x621025>|0x775|+0x01c:'2439'
	// <0x621041>|0x791|+0x00c:'2440'
	// <0x62104d>|0x79d|+0x025:'2441'
	// <0>
	// <0x621072>|0x7c2|+0x7b2:'2443'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// ******
}

// STATE[STUB]
void stage_lights::render_light( light* l, bool shadowers_pass )
{
	// LOCALS
	// float 							light_range
	// res_geometry_ptr 				geometry
	// float3 							light_direction
	// float3 							light_color
	// float3 							light_position
	// u32 								tech_index
	// u32 								tech_index
	// const float 						penumbra_half_angle_cosine
	// const float 						umbra_half_angle_cosine
	// u32 								tech_index
	// float4x4 						obb_world
	// u32 								tech_index
	// u32 								tech_index
	// u32 								tech_index
	// const float 						penumbra_half_angle_cosine
	// const float 						umbra_half_angle_cosine
	// ******

	// STATICS
	// static < NoType > 				 = <0x624098>;
	// static < NoType > 				 = <0x62407c>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x611e78 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN52") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x611e59 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN51") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x611e61 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN50") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x611e69 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN45") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x61200a }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN35") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x612276 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN29") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x612780 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN21") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x612979 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN17") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x612b37 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN12") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x612cc7 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN8") })
	// ******

	// FUNCTION BODY[0x622c20]: 324
	// <0x622c38>|0x018|+0x00d:'2455'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x622c45>|0x025|+0x012:'2461'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x622c57>|0x037|+0x027:'2466'
	// <0>
	// <0x622c7e>|0x05e|+0x014:'2468'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x622c92>|0x072|+0x0a5:'2475'
	// <0x622d37>|0x117|+0x0ee:'2476'
	// <0>
	// <1>
	// <0x622e25>|0x205|+0x010:'2479'
	// <0x622e35>|0x215|+0x00c:'2480'
	// <0>
	// <1>
	// <0x622e41>|0x221|+0x018:'2483'
	// <0>
	// <0x622e59>|0x239|+0x006:'2485'
	// <0x622e5f>|0x23f|+0x002:'2486'
	// <0>
	// <1>
	// <0x622e61>|0x241|+0x006:'2489'
	// <0x622e67>|0x247|+0x002:'2490'
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
	// <0x622e69>|0x249|+0x00f:'2506'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x622e78>|0x258|+0x039:'2512'
	// <0>
	// <0x622eb1>|0x291|+0x019:'2514'
	// <0x622eca>|0x2aa|-0x03b:'2515'
	// <0>
	// <1>
	// <2>
	// <0x622e8f>|0x26f|+0x01a:'2519'
	// <0x622ea9>|0x289|+0x006:'2520'
	// <0x622eaf>|0x28f|+0x021:'2521'
	// <0>
	// <1>
	// <2>
	// <0x622ed0>|0x2b0|+0x00c:'2525'
	// <0>
	// <1>
	// <2>
	// <0x622edc>|0x2bc|+0x0cd:'2529'
	// <0x622fa9>|0x389|+0x01b:'2530'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x622fc4>|0x3a4|+0x026:'2535'
	// <0x622fea>|0x3ca|+0x009:'2536'
	// <0x622ff3>|0x3d3|+0x007:'2537'
	// <0>
	// <1>
	// <0x622ffa>|0x3da|+0x010:'2540'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x62300a>|0x3ea|+0x00c:'2547'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x623016>|0x3f6|+0x009:'2555'
	// <0x62301f>|0x3ff|+0x006:'2556'
	// <0x623025>|0x405|+0x002:'2557'
	// <0x623027>|0x407|+0x00b:'2558'
	// <0>
	// <1>
	// <0x623032>|0x412|+0x052:'2561'
	// <0x623084>|0x464|+0x04c:'2562'
	// <0x6230d0>|0x4b0|+0x01c:'2563'
	// <0>
	// <0x6230ec>|0x4cc|+0x01c:'2565'
	// <0x623108>|0x4e8|+0x01c:'2566'
	// <0>
	// <0x623124>|0x504|+0x068:'2568'
	// <0>
	// <0x62318c>|0x56c|+0x04c:'2570'
	// <0x6231d8>|0x5b8|+0x01c:'2571'
	// <0x6231f4>|0x5d4|+0x01c:'2572'
	// <0x623210>|0x5f0|+0x009:'2573'
	// <0x623219>|0x5f9|+0x016:'2574'
	// <0x62322f>|0x60f|+0x01c:'2575'
	// <0x62324b>|0x62b|+0x00c:'2576'
	// <0x623257>|0x637|+0x01a:'2577'
	// <0>
	// <0x623271>|0x651|+0x0cb:'2579'
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
	// <0x62333c>|0x71c|+0x022:'2598'
	// <0>
	// <1>
	// <0x62335e>|0x73e|+0x002:'2601'
	// <0x623360>|0x740|+0x00f:'2602'
	// <0>
	// <0x62336f>|0x74f|+0x052:'2604'
	// <0x6233c1>|0x7a1|+0x04c:'2605'
	// <0x62340d>|0x7ed|+0x073:'2606'
	// <0x623480>|0x860|+0x01c:'2607'
	// <0>
	// <0x62349c>|0x87c|+0x01c:'2609'
	// <0x6234b8>|0x898|-0x242:'2610'
	// <0x623276>|0x656|+0x25e:'2611'
	// <0x6234d4>|0x8b4|+0x04d:'2611'
	// <0>
	// <0x623521>|0x901|+0x01f:'2613'
	// <0x623540>|0x920|+0x050:'2614'
	// <0x623590>|0x970|+0x01f:'2615'
	// <0x6235af>|0x98f|+0x04d:'2616'
	// <0>
	// <1>
	// <0x6235fc>|0x9dc|+0x07d:'2619'
	// <0x623679>|0xa59|+0x01c:'2620'
	// <0>
	// <0x623695>|0xa75|+0x04d:'2622'
	// <0x6236e2>|0xac2|+0x01c:'2623'
	// <0x6236fe>|0xade|+0x01c:'2624'
	// <0>
	// <0x62371a>|0xafa|+0x009:'2626'
	// <0x623723>|0xb03|+0x016:'2627'
	// <0x623739>|0xb19|+0x01c:'2628'
	// <0x623755>|0xb35|+0x00c:'2629'
	// <0x623761>|0xb41|+0x01a:'2630'
	// <0>
	// <1>
	// <0x62377b>|0xb5b|+0x005:'2633'
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
	// <0x623780>|0xb60|+0x0e6:'2646'
	// <0>
	// <0x623866>|0xc46|+0x00e:'2648'
	// <0x623874>|0xc54|+0x016:'2649'
	// <0x62388a>|0xc6a|+0x037:'2650'
	// <0>
	// <0x6238c1>|0xca1|+0x019:'2652'
	// <0x6238da>|0xcba|+0x01c:'2653'
	// <0x6238f6>|0xcd6|+0x01f:'2654'
	// <0>
	// <0x623915>|0xcf5|+0x008:'2656'
	// <0x62391d>|0xcfd|+0x010:'2657'
	// <0x62392d>|0xd0d|+0x01c:'2658'
	// <0x623949>|0xd29|+0x00c:'2659'
	// <0x623955>|0xd35|+0x01f:'2660'
	// <0>
	// <0x623974>|0xd54|+0x005:'2662'
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
	// <0x623979>|0xd59|+0x0d3:'2676'
	// <0>
	// <0x623a4c>|0xe2c|+0x01c:'2678'
	// <0x623a68>|0xe48|+0x01f:'2679'
	// <0>
	// <0x623a87>|0xe67|+0x011:'2681'
	// <0x623a98>|0xe78|+0x01c:'2682'
	// <0x623ab4>|0xe94|+0x01f:'2683'
	// <0>
	// <0x623ad3>|0xeb3|+0x003:'2685'
	// <0x623ad6>|0xeb6|+0x016:'2686'
	// <0x623aec>|0xecc|+0x01b:'2687'
	// <0x623b07>|0xee7|+0x00c:'2688'
	// <0x623b13>|0xef3|+0x01f:'2689'
	// <0>
	// <0x623b32>|0xf12|+0x005:'2691'
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
	// <0x623b37>|0xf17|+0x0c1:'2707'
	// <0>
	// <0x623bf8>|0xfd8|+0x01f:'2709'
	// <0>
	// <0x623c17>|0xff7|+0x011:'2711'
	// <0x623c28>|0x1008|+0x01c:'2712'
	// <0x623c44>|0x1024|+0x01f:'2713'
	// <0>
	// <0x623c63>|0x1043|+0x008:'2715'
	// <0x623c6b>|0x104b|+0x010:'2716'
	// <0x623c7b>|0x105b|+0x01c:'2717'
	// <0x623c97>|0x1077|+0x00c:'2718'
	// <0x623ca3>|0x1083|+0x01f:'2719'
	// <0>
	// <0x623cc2>|0x10a2|+0x0cc:'2721'
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
	// <0x623d8e>|0x116e|+0x00d:'2736'
	// <0>
	// <0x623d9b>|0x117b|+0x020:'2738'
	// <0x623dbb>|0x119b|+0x011:'2739'
	// <0x623dcc>|0x11ac|+0x041:'2740'
	// <0x623e0d>|0x11ed|+0x01c:'2741'
	// <0>
	// <0x623e29>|0x1209|+0x01c:'2743'
	// <0x623e45>|0x1225|-0x17e:'2744'
	// <0x623cc7>|0x10a7|+0x19d:'2745'
	// <0x623e64>|0x1244|+0x011:'2745'
	// <0>
	// <0x623e75>|0x1255|+0x029:'2747'
	// <0x623e9e>|0x127e|+0x014:'2748'
	// <0x623eb2>|0x1292|+0x02c:'2749'
	// <0x623ede>|0x12be|+0x011:'2750'
	// <0>
	// <1>
	// <0x623eef>|0x12cf|+0x042:'2753'
	// <0x623f31>|0x1311|+0x01f:'2754'
	// <0>
	// <1>
	// <0x623f50>|0x1330|+0x046:'2757'
	// <0>
	// <0x623f96>|0x1376|+0x011:'2759'
	// <0x623fa7>|0x1387|+0x01c:'2760'
	// <0x623fc3>|0x13a3|+0x01f:'2761'
	// <0>
	// <0x623fe2>|0x13c2|+0x008:'2763'
	// <0x623fea>|0x13ca|+0x010:'2764'
	// <0x623ffa>|0x13da|+0x01c:'2765'
	// <0x624016>|0x13f6|+0x00c:'2766'
	// <0x624022>|0x1402|+0x01f:'2767'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x624041>|0x1421|+0x009:'2775'
	// <0>
	// <0x62404a>|0x142a|-0x13c5:'2777'
	// <0>
	// <0x622c85>|0x065|+0x13d0:'2779'
	// ******
}

// STATE[STUB]
void stage_lights::draw_geometry( light* l )
{
	// STATICS
	// static < NoType > 				 = <0x61adac>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x609c53 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x609ccc }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x609d25 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// ******

	// FUNCTION BODY[0x61ac40]: 18
	// <0x61ac40>|0x000|+0x013:'2783'
	// <0>
	// <1>
	// <0x61ac53>|0x013|+0x074:'2786'
	// <0x61acc7>|0x087|+0x005:'2787'
	// <0>
	// <1>
	// <0x61accc>|0x08c|+0x057:'2790'
	// <0x61ad23>|0x0e3|+0x002:'2791'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x61ad25>|0x0e5|+0x07c:'2796'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******
}

// STATE[STUB]
void stage_lights::debug_render( )
{
	// LOCALS
	// light_data const* 				e
	// float4x4 						transform
	// vertex_colored[8] 				vertices
	// ******

	// STATICS
	// static bool 						s_debug_render = <0x4c265b4>;
	// ******

	// TYPEDEFS
	// typedef
	// 	vector< light_data >
	// 	lights_type;

	// ******

	// FUNCTION BODY[0x6225f0]: 71
	// <0>
	// <0x6225fc>|0x00c|+0x013:'2806'
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
	// <0x62260f>|0x01f|+0x5fb:'2819'
	// <0x622c0a>|0x61a|-0x56e:'2819'
	// <0>
	// <1>
	// <0x62269c>|0x0ac|+0x00f:'2822'
	// <0x6226ab>|0x0bb|+0x011:'2823'
	// <0x6226bc>|0x0cc|+0x02f:'2824'
	// <0>
	// <0x6226eb>|0x0fb|+0x014:'2826'
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
	// <0x6226ff>|0x10f|+0x494:'2867'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x622b93>|0x5a3|+0x06f:'2874'
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
	// 	vostok::render::speedtree_forest::tree_render_info*
	// 	iterator_type;

	// typedef
	// 	vostok::render::texture_named_instance*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok
