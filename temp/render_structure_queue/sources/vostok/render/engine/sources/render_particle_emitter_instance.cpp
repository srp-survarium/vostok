////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\render_particle_emitter_instance.h"

namespace vostok {
namespace render {

// STATE[STUB]
void particle_sprite_vertex::set(
	float3		in_position,
	float4		in_color,
	float2		in_uv,
	float2		in_size,
	float		in_rotation,
	float		in_gravity,
	float3		in_old_position
)
{
	// FUNCTION BODY[0x88620]: 7
	// <0x88620>|0x000|+0x00a:'51'
	// <0>
	// <0x8862a>|0x00a|+0x01e:'53'
	// <0>
	// <0x88648>|0x028|+0x019:'55'
	// <0x88661>|0x041|+0x019:'56'
	// <0x8867a>|0x05a|+0x011:'57'
	// ******
}

// STATE[STUB]
void* render_particle_emitter_instance::`scalar deleting destructor'( u32 arg_0 )
{
	return NULL;

	// FUNCTION BODY[0x88750]: 3
	// <0x88690>|0x000|+0x091:'74'
	// <0>
	// <0x88721>|0x091|+0x024:'76'
	// ******
}

// STATE[STUB]
 render_particle_emitter_instance::render_particle_emitter_instance(
	particle::world&						particle_world,
	particle::particle_emitter_instance&	particle_emitter_instance,
	particle::base_particle_list const&		particle_list,
	particle::billboard_parameters*			billboard_parameters,
	particle::beamtrail_parameters*			beamtrail_parameters,
	particle::enum_particle_locked_axis		locked_axis,
	particle::enum_particle_screen_alignment	screen_alignment,
	float4x4 const&							transform,
	float4 const&							instance_color
)
{
	// FUNCTION BODY[0x5fe370]: 0
	// ******
}

// STATE[STUB]
 render_particle_emitter_instance::~render_particle_emitter_instance( )
{
	// FUNCTION BODY[0x5fe260]: 5
	// <0x5fe263>|0x003|+0x011:'143'
	// <0x5fe274>|0x014|+0x029:'144'
	// <0>
	// <0x5fe29d>|0x03d|+0x00a:'146'
	// <0x5fe2a7>|0x047|+0x029:'147'
	// ******
}

// STATE[STUB]
bool render_particle_emitter_instance::is_occluded( ) const
{
	return false;

	// FUNCTION BODY[0x5fc780]: 1
	// <0x5fc780>|0x000|+0x01d:'152'
	// <0x5fc79d>|0x01d|-0x001:'152'
	// <0x5fc79c>|0x01c|+0x003:'153'
	// ******
}

// STATE[STUB]
math::aabb const& render_particle_emitter_instance::get_aabb( ) const
{
	// FUNCTION BODY[0x5fc680]: 1
	// <0x5fc680>|0x000|+0x005:'157'
	// ******
}

// STATE[STUB]
void render_particle_emitter_instance::update_render_buffers(
	particle::enum_particle_data_type		datatype,
	bool									use_subuv,
	u32										in_num_max_particles,
	u32										beamtrail_parameters_num_sheets
)
{
	// STATICS
	// static < NoType > 				 = <0x5fe87c>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5ed572 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN14") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5ed738 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN7") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5ed856 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// ******

	// FUNCTION BODY[0x5fe4a0]: 93
	// <0x5fe4ae>|0x00e|+0x028:'167'
	// <0x5fe4d6>|0x036|+0x026:'168'
	// <0x5fe4fc>|0x05c|+0x026:'169'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5fe522>|0x082|+0x01c:'174'
	// <0>
	// <1>
	// <0x5fe53e>|0x09e|+0x034:'177'
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
	// <0x5fe572>|0x0d2|+0x03e:'188'
	// <0x5fe5b0>|0x110|+0x00d:'189'
	// <0x5fe5bd>|0x11d|+0x01e:'190'
	// <0>
	// <0x5fe5db>|0x13b|+0x010:'192'
	// <0x5fe5eb>|0x14b|+0x00b:'193'
	// <0x5fe5f6>|0x156|+0x01c:'194'
	// <0>
	// <0x5fe612>|0x172|+0x032:'196'
	// <0x5fe644>|0x1a4|+0x004:'197'
	// <0>
	// <1>
	// <0x5fe648>|0x1a8|+0x005:'200'
	// <0>
	// <0x5fe64d>|0x1ad|+0x006:'202'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5fe653>|0x1b3|+0x004:'207'
	// <0x5fe657>|0x1b7|+0x00b:'208'
	// <0x5fe662>|0x1c2|+0x01c:'209'
	// <0>
	// <0x5fe67e>|0x1de|+0x010:'211'
	// <0x5fe68e>|0x1ee|+0x00b:'212'
	// <0x5fe699>|0x1f9|+0x01c:'213'
	// <0>
	// <0x5fe6b5>|0x215|+0x032:'215'
	// <0x5fe6e7>|0x247|+0x020:'216'
	// <0x5fe707>|0x267|+0x031:'217'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5fe738>|0x298|+0x005:'224'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5fe73d>|0x29d|+0x01a:'229'
	// <0x5fe757>|0x2b7|+0x012:'230'
	// <0>
	// <0x5fe769>|0x2c9|+0x011:'232'
	// <0x5fe77a>|0x2da|+0x00c:'233'
	// <0>
	// <1>
	// <0x5fe786>|0x2e6|+0x018:'236'
	// <0x5fe79e>|0x2fe|+0x00b:'237'
	// <0x5fe7a9>|0x309|+0x01c:'238'
	// <0>
	// <0x5fe7c5>|0x325|+0x010:'240'
	// <0x5fe7d5>|0x335|+0x00b:'241'
	// <0x5fe7e0>|0x340|+0x01c:'242'
	// <0>
	// <0x5fe7fc>|0x35c|+0x031:'244'
	// <0x5fe82d>|0x38d|+0x020:'245'
	// <0x5fe84d>|0x3ad|+0x004:'246'
	// <0x5fe851>|0x3b1|+0x005:'247'
	// <0>
	// <1>
	// <2>
	// <0x5fe856>|0x3b6|+0x013:'251'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5fe869>|0x3c9|-0x13a:'256'
	// <0>
	// <1>
	// <2>
	// <0x5fe72f>|0x28f|+0x131:'260'
	// <0x5fe860>|0x3c0|+0x013:'260'
	// ******
}

// STATE[STUB]
u32 render_particle_emitter_instance::get_num_particles( ) const
{
	return 0;

	// FUNCTION BODY[0x5fc660]: 10
	// <0x5fc660>|0x000|+0x009:'264'
	// <0>
	// <0x5fc669>|0x009|+0x002:'266'
	// <0x5fc66b>|0x00b|+0x005:'267'
	// <0>
	// <0x5fc670>|0x010|+0x006:'269'
	// <0x5fc676>|0x016|+0x005:'270'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
void render_particle_emitter_instance::render_sprites( )
{
	// LOCALS
	// u32 								v_offset
	// u16* 							pib
	// u32 								i_offset
	// u32 								p
	// float4 							final_color
	// u16 								_v
	// ******

	// FUNCTION BODY[0x5fdd10]: 54
	// <0x5fdd1f>|0x00f|+0x026:'291'
	// <0>
	// <0x5fdd45>|0x035|+0x0bb:'293'
	// <0>
	// <1>
	// <0x5fde00>|0x0f0|-0x0b3:'296'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5fdd4d>|0x03d|+0x021:'301'
	// <0>
	// <0x5fdd6e>|0x05e|+0x023:'303'
	// <0>
	// <1>
	// <0x5fdd91>|0x081|+0x011:'306'
	// <0x5fdda2>|0x092|+0x341:'307'
	// <0x5fe0e3>|0x3d3|-0x2df:'307'
	// <0>
	// <0x5fde04>|0x0f4|+0x008:'309'
	// <0x5fde0c>|0x0fc|+0x003:'310'
	// <0>
	// <0x5fde0f>|0x0ff|+0x007:'312'
	// <0x5fde16>|0x106|+0x007:'313'
	// <0x5fde1d>|0x10d|+0x010:'314'
	// <0x5fde2d>|0x11d|+0x00e:'315'
	// <0x5fde3b>|0x12b|+0x006:'316'
	// <0x5fde41>|0x131|+0x00b:'317'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5fde4c>|0x13c|+0x007:'324'
	// <0>
	// <0x5fde53>|0x143|+0x008:'326'
	// <0x5fde5b>|0x14b|+0x059:'327'
	// <0x5fdeb4>|0x1a4|-0x10a:'328'
	// <0>
	// <0x5fddaa>|0x09a|+0x10d:'330'
	// <0x5fdeb7>|0x1a7|-0x10a:'330'
	// <0x5fddad>|0x09d|+0x181:'331'
	// <0x5fdf2e>|0x21e|-0x15a:'331'
	// <0x5fddd4>|0x0c4|+0x1df:'332'
	// <0x5fdfb3>|0x2a3|-0x1d3:'332'
	// <0x5fdde0>|0x0d0|+0x25a:'333'
	// <0x5fe03a>|0x32a|+0x010:'333'
	// <0>
	// <0x5fe04a>|0x33a|+0x078:'335'
	// <0x5fe0c2>|0x3b2|+0x028:'336'
	// <0>
	// <1>
	// <0x5fe0ea>|0x3da|+0x02c:'339'
	// <0x5fe116>|0x406|+0x028:'340'
	// <0>
	// <0x5fe13e>|0x42e|+0x00b:'342'
	// <0x5fe149>|0x439|+0x079:'343'
	// <0x5fe1c2>|0x4b2|+0x021:'344'
	// ******
}

// STATE[STUB]
float frac( float f )
{
	return 0.0f;

	// FUNCTION BODY[0x5fc700]: 1
	// <0x5fc709>|0x009|+0x069:'349'
	// ******
}

// STATE[STUB]
void render_particle_emitter_instance::render_subuv_sprites( )
{
	// LOCALS
	// u32 								v_offset
	// u16* 							pib
	// u32 								i_offset
	// float4 							final_color
	// float 							sizeV
	// float 							subimage_index
	// float 							next_posV
	// float2 							size
	// float 							next_posU
	// float 							u_offset
	// float 							next_subimage_index
	// float 							posU
	// float 							sizeU
	// float 							posV
	// const float 						f_time
	// ******

	// FUNCTION BODY[0x5fd360]: 93
	// <0x5fd366>|0x006|+0x030:'354'
	// <0>
	// <0x5fd396>|0x036|+0x09a:'356'
	// <0>
	// <1>
	// <0x5fd430>|0x0d0|-0x092:'359'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5fd39e>|0x03e|+0x021:'364'
	// <0>
	// <0x5fd3bf>|0x05f|+0x02c:'366'
	// <0>
	// <1>
	// <0x5fd3eb>|0x08b|+0x00c:'369'
	// <0x5fd3f7>|0x097|+0x040:'370'
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
	// <0x5fd437>|0x0d7|+0x067:'389'
	// <0>
	// <0x5fd49e>|0x13e|+0x048:'391'
	// <0x5fd4e6>|0x186|+0x016:'392'
	// <0>
	// <0x5fd4fc>|0x19c|+0x040:'394'
	// <0x5fd53c>|0x1dc|+0x016:'395'
	// <0>
	// <0x5fd552>|0x1f2|+0x018:'397'
	// <0x5fd56a>|0x20a|+0x035:'398'
	// <0>
	// <1>
	// <0x5fd59f>|0x23f|+0x06e:'401'
	// <0>
	// <0x5fd60d>|0x2ad|+0x01a:'403'
	// <0>
	// <0x5fd627>|0x2c7|+0x047:'405'
	// <0x5fd66e>|0x30e|+0x00d:'406'
	// <0>
	// <0x5fd67b>|0x31b|+0x072:'408'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5fd6ed>|0x38d|+0x006:'414'
	// <0x5fd6f3>|0x393|+0x009:'415'
	// <0x5fd6fc>|0x39c|+0x006:'416'
	// <0x5fd702>|0x3a2|+0x007:'417'
	// <0x5fd709>|0x3a9|+0x00a:'418'
	// <0x5fd713>|0x3b3|+0x004:'419'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5fd717>|0x3b7|+0x02f:'426'
	// <0>
	// <1>
	// <0x5fd746>|0x3e6|+0x03b:'429'
	// <0x5fd781>|0x421|-0x382:'430'
	// <0>
	// <0x5fd3ff>|0x09f|+0x38b:'432'
	// <0x5fd78a>|0x42a|-0x37d:'432'
	// <0x5fd40d>|0x0ad|+0x485:'433'
	// <0x5fd892>|0x532|-0x47c:'433'
	// <0x5fd416>|0x0b6|+0x5a8:'434'
	// <0x5fd9be>|0x65e|-0x59f:'434'
	// <0x5fd41f>|0x0bf|+0x6c8:'435'
	// <0x5fdae7>|0x787|+0x107:'435'
	// <0>
	// <1>
	// <0x5fdbee>|0x88e|+0x019:'438'
	// <0>
	// <1>
	// <0x5fdc07>|0x8a7|+0x02c:'441'
	// <0x5fdc33>|0x8d3|+0x02b:'442'
	// <0>
	// <0x5fdc5e>|0x8fe|+0x00b:'444'
	// <0x5fdc69>|0x909|+0x077:'445'
	// <0x5fdce0>|0x980|+0x023:'446'
	// ******
}

// STATE[STUB]
void render_particle_emitter_instance::render( float3 const& view_location, const u32 num_particles )
{
	// STATICS
	// static < NoType > 				 = <0x5fe244>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5ed208 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5ed213 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5ed21e }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5ed235 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1") })
	// ******

	// FUNCTION BODY[0x5fe1f0]: 34
	// <0x5fe1f6>|0x006|+0x012:'451'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5fe208>|0x018|+0x00b:'456'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5fe213>|0x023|+0x00b:'464'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5fe21e>|0x02e|+0x017:'472'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5fe235>|0x045|-0x027:'480'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5fe20e>|0x01e|+0x00b:'485'
	// <0x5fe219>|0x029|+0x017:'485'
	// <0x5fe230>|0x040|+0x00c:'485'
	// ******
}

// STATE[STUB]
void render_particle_emitter_instance::render_beams( float3 const& view_location, u32 num_particles )
{
	// FUNCTION BODY[0x5fd300]: 16
	// <0x5fd300>|0x000|+0x01c:'489'
	// <0x5fd31c>|0x01c|-0x008:'489'
	// <0>
	// <0x5fd314>|0x014|+0x00a:'491'
	// <0>
	// <0x5fd31e>|0x01e|+0x009:'493'
	// <0x5fd327>|0x027|+0x002:'494'
	// <0x5fd329>|0x029|+0x007:'495'
	// <0>
	// <0x5fd330>|0x030|+0x004:'497'
	// <0>
	// <1>
	// <2>
	// <0x5fd334>|0x034|+0x00b:'501'
	// <0>
	// <0x5fd33f>|0x03f|+0x01b:'503'
	// <0>
	// ******
}

// STATE[STUB]
void render_particle_emitter_instance::render_trails(
	float3 const&				view_location,
	particle::base_particle*	start_particle,
	u32							num_particles
)
{
	// LOCALS
	// const u32 						num_sheets
	// const float 						angle_incrase
	// float 							current_angle
	// u32 								v_offset
	// u16* 							pib
	// const u32 						texture_tiles
	// const u32 						num_quads
	// u32 								i_offset
	// float 							distance_from_first_to_last
	// u32 								p
	// float 							current_uv_start
	// float3 							prev_particle_dir
	// particle::base_particle* 		prevP
	// u32 								p
	// particle::base_particle* 		nextP
	// float3 							right_a
	// float4 							final_color
	// const float 						scaleUV
	// const float 						current_v_pos
	// float3 							particle_dir
	// float3 							view_dir
	// float4x4 						rot_matrix
	// u16 								s
	// ******

	// FUNCTION BODY[0x5fc820]: 126
	// <0x5fc829>|0x009|+0x018:'509'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5fc841>|0x021|+0x012:'514'
	// <0>
	// <1>
	// <0x5fc853>|0x033|+0x021:'517'
	// <0>
	// <1>
	// <0x5fc874>|0x054|+0x020:'520'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5fc894>|0x074|+0x034:'526'
	// <0x5fc8c8>|0x0a8|+0x025:'527'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5fc8ed>|0x0cd|+0x0e3:'532'
	// <0x5fc9d0>|0x1b0|+0x004:'532'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5fc9d4>|0x1b4|-0x0e0:'541'
	// <0x5fc8f4>|0x0d4|+0x121:'542'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5fca15>|0x1f5|+0x036:'547'
	// <0>
	// <0x5fca4b>|0x22b|+0x714:'549'
	// <0x5fd15f>|0x93f|-0x700:'549'
	// <0>
	// <1>
	// <2>
	// <0x5fca5f>|0x23f|+0x007:'553'
	// <0x5fca66>|0x246|+0x00c:'554'
	// <0>
	// <1>
	// <0x5fca72>|0x252|+0x097:'557'
	// <0>
	// <0x5fcb09>|0x2e9|+0x006:'559'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5fcb0f>|0x2ef|+0x029:'567'
	// <0>
	// <0x5fcb38>|0x318|+0x01d:'569'
	// <0>
	// <0x5fcb55>|0x335|+0x05a:'571'
	// <0x5fcbaf>|0x38f|+0x05b:'572'
	// <0>
	// <0x5fcc0a>|0x3ea|+0x00b:'574'
	// <0>
	// <0x5fcc15>|0x3f5|+0x096:'576'
	// <0>
	// <1>
	// <0x5fccab>|0x48b|-0x22b:'579'
	// <0>
	// <1>
	// <2>
	// <0x5fca80>|0x260|+0x240:'583'
	// <0x5fccc0>|0x4a0|+0x0d2:'583'
	// <0>
	// <1>
	// <2>
	// <0x5fcd92>|0x572|+0x018:'587'
	// <0>
	// <0x5fcdaa>|0x58a|+0x01a:'589'
	// <0>
	// <0x5fcdc4>|0x5a4|+0x022:'591'
	// <0x5fcde6>|0x5c6|+0x075:'592'
	// <0>
	// <1>
	// <0x5fce5b>|0x63b|+0x0f5:'595'
	// <0x5fcf50>|0x730|+0x01d:'596'
	// <0>
	// <0x5fcf6d>|0x74d|+0x091:'598'
	// <0>
	// <1>
	// <0x5fcffe>|0x7de|+0x09f:'601'
	// <0x5fd09d>|0x87d|+0x083:'602'
	// <0>
	// <1>
	// <0x5fd120>|0x900|+0x006:'605'
	// <0x5fd126>|0x906|+0x011:'606'
	// <0x5fd137>|0x917|+0x004:'607'
	// <0x5fd13b>|0x91b|+0x029:'608'
	// <0>
	// <0x5fd164>|0x944|+0x024:'610'
	// <0>
	// <1>
	// <0x5fd188>|0x968|+0x04a:'613'
	// <0x5fd1d2>|0x9b2|-0x040:'613'
	// <0>
	// <0x5fd192>|0x972|+0x006:'615'
	// <0>
	// <0x5fd198>|0x978|+0x008:'617'
	// <0>
	// <0x5fd1a0>|0x980|+0x006:'619'
	// <0x5fd1a6>|0x986|+0x009:'620'
	// <0x5fd1af>|0x98f|+0x009:'621'
	// <0x5fd1b8>|0x998|+0x006:'622'
	// <0x5fd1be>|0x99e|+0x006:'623'
	// <0x5fd1c4>|0x9a4|+0x025:'624'
	// <0>
	// <1>
	// <2>
	// <0x5fd1e9>|0x9c9|+0x033:'628'
	// <0x5fd21c>|0x9fc|+0x028:'629'
	// <0>
	// <0x5fd244>|0xa24|+0x00b:'631'
	// <0>
	// <0x5fd24f>|0xa2f|+0x07d:'633'
	// <0x5fd2cc>|0xaac|+0x021:'634'
	// ******
}

// STATE[STUB]
enum_vertex_input_type render_particle_emitter_instance::get_vertex_input_type( )
{
	// STATICS
	// static < NoType > 				 = <0x5fc648>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5eb632 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5eb638 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5eb63e }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1") })
	// ******

	// FUNCTION BODY[0x5fc620]: 11
	// <0x5fc620>|0x000|+0x012:'643'
	// <0>
	// <1>
	// <0x5fc632>|0x012|+0x006:'646'
	// <0>
	// <0x5fc638>|0x018|+0x006:'648'
	// <0>
	// <1>
	// <0x5fc63e>|0x01e|+0x006:'651'
	// <0>
	// <0x5fc644>|0x024|-0x00d:'653'
	// <0x5fc637>|0x017|+0x006:'654'
	// <0x5fc63d>|0x01d|+0x006:'654'
	// <0x5fc643>|0x023|+0x003:'654'
	// ******
}

// STATE[STUB]
material_effects& render_particle_emitter_instance::get_material_effects( )
{
	// STATICS
	// static < NoType > 				 = <0x5fc6e4>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5eb6c6 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN7") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5eb6b9 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN8") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5eb6ac }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN9") })
	// ******

	// FUNCTION BODY[0x5fc690]: 9
	// <0>
	// <0x5fc690>|0x000|+0x00a:'659'
	// <0>
	// <0x5fc69a>|0x00a|+0x01f:'661'
	// <0x5fc6b9>|0x029|+0x00d:'661'
	// <0x5fc6c6>|0x036|+0x00d:'661'
	// <0x5fc6d3>|0x043|+0x00a:'661'
	// <0>
	// <1>
	// <2>
	// <0x5fc6dd>|0x04d|-0x025:'665'
	// <0>
	// <0x5fc6b8>|0x028|+0x00d:'667'
	// <0x5fc6c5>|0x035|+0x00d:'667'
	// <0x5fc6d2>|0x042|+0x00a:'667'
	// <0x5fc6dc>|0x04c|+0x006:'667'
	// ******
}

// STATE[STUB]
void render_particle_emitter_instance::change_material( resources::unmanaged_resource_ptr const& material )
{
	// FUNCTION BODY[0x5fc7a0]: 17
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
	// <0x5fc7a0>|0x000|+0x07b:'686'
	// <0>
	// ******
}

// STATE[STUB]
void render_particle_emitter_instance::set_transform( float4x4 const& transform )
{
	// FUNCTION BODY[0x5fc600]: 1
	// <0x5fc601>|0x001|+0x014:'694'
	// ******
}

// STATE[STUB]
void render_particle_emitter_instance::set_aabb( math::aabb const& bbox )
{
	// FUNCTION BODY[0x5fc5d0]: 1
	// <0x5fc5d0>|0x000|+0x02a:'699'
	// ******
}

// STATE[STUB]
void render_particle_emitter_instance::draw_debug( float4x4 const& view_matrix, particle::enum_particle_render_mode debug_mode )
{
	// LOCALS
	// float4x4 						camera_to_world
	// particle::base_particle* 		P
	// float3[2] 						points
	// ******

	// FUNCTION BODY[0x5fe890]: 43
	// <0>
	// <0x5fe89c>|0x00c|+0x014:'705'
	// <0>
	// <0x5fe8b0>|0x020|+0x0e8:'707'
	// <0x5fe998>|0x108|+0x0f9:'708'
	// <0>
	// <1>
	// <2>
	// <0x5fea91>|0x201|+0x6f7:'712'
	// <0>
	// <1>
	// <2>
	// <0x5ff188>|0x8f8|+0x00d:'716'
	// <0x5ff195>|0x905|+0x013:'717'
	// <0>
	// <0x5ff1a8>|0x918|+0x227:'719'
	// <0x5ff3cf>|0xb3f|-0x8f5:'720'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5feada>|0x24a|+0x009:'727'
	// <0x5feae3>|0x253|+0x027:'728'
	// <0>
	// <1>
	// <2>
	// <0x5feb0a>|0x27a|+0x079:'732'
	// <0x5feb83>|0x2f3|+0x00c:'733'
	// <0>
	// <0x5feb8f>|0x2ff|+0x2a2:'735'
	// <0>
	// <0x5fee31>|0x5a1|+0x057:'737'
	// <0x5fee88>|0x5f8|+0x0a5:'738'
	// <0>
	// <0x5fef2d>|0x69d|+0x240:'740'
	// <0>
	// <0x5ff16d>|0x8dd|+0x00e:'742'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5ff17b>|0x8eb|+0x266:'747'
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
