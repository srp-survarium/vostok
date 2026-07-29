#include "pch.h"
#include "render_model_skeleton.h"

namespace vostok {
namespace render {

// STATE[STUB]
skeleton_render_surface::skeleton_render_surface( )
{
	// FUNCTION BODY[0x63b1a0]: 1
	// <0x63b1ad>|0x00d|+0x007:'22'
	// ******
}

// STATE[STUB]
skeleton_render_model::skeleton_render_model( )
{
	// FUNCTION BODY[0x63ae00]: 2
	// <0x63ae2e>|0x02e|+0x05c:'27'
	// <0x63ae8a>|0x08a|+0x05c:'28'
	// ******
}

// STATE[STUB]
void skeleton_render_model::get_bind_pose( float4x4* matrices, u32 count ) const
{
	// FUNCTION BODY[0x63aa10]: 8
	// <0>
	// <1>
	// <0x63aa1f>|0x00f|+0x011:'35'
	// <0>
	// <1>
	// <0x63aa30>|0x020|+0x00e:'38'
	// <0x63aa3e>|0x02e|+0x00e:'39'
	// <0>
	// ******
}

// STATE[STUB]
void skeleton_render_model::load_bones( memory::reader& bones_chunk )
{
	// LOCALS
	// animation::frame 				frm
	// float4x4 						frm_matrix
	// ******

	// FUNCTION BODY[0x63ac10]: 17
	// <0x63ac16>|0x006|+0x00f:'45'
	// <0>
	// <0x63ac25>|0x015|+0x023:'47'
	// <0>
	// <1>
	// <0x63ac48>|0x038|+0x01f:'50'
	// <0>
	// <0x63ac67>|0x057|+0x00e:'52'
	// <0x63ac75>|0x065|+0x011:'53'
	// <0x63ac86>|0x076|+0x018:'54'
	// <0x63ac9e>|0x08e|+0x010:'55'
	// <0>
	// <0x63acae>|0x09e|-0x05b:'57'
	// <0x63ac53>|0x043|+0x07d:'58'
	// <0x63acd0>|0x0c0|+0x00b:'58'
	// <0x63acdb>|0x0cb|+0x00a:'59'
	// <0x63ace5>|0x0d5|+0x111:'60'
	// <0>
	// ******
}

// STATE[STUB]
void skeleton_render_model::update( vector< float4x4 > const& bones )
{
	// CALL SITE INFO
	// <0x63a692> -> void < unknown >( vector< float4x4 > const& )
	// ******

	// FUNCTION BODY[0x63a670]: 5
	// <0x63a671>|0x001|+0x00f:'66'
	// <0>
	// <0x63a680>|0x010|+0x00c:'68'
	// <0x63a68c>|0x01c|+0x013:'69'
	// <0>
	// ******
}

// STATE[STUB]
skeleton_render_model_instance::skeleton_render_model_instance( )
{
	// FUNCTION BODY[0x63af90]
	// ******
}

// STATE[STUB]
skeleton_render_model_instance::~skeleton_render_model_instance( )
{
	// FUNCTION BODY[0x63aef0]: 1
	// <0x63aefa>|0x00a|+0x01b:'80'
	// ******
}

// STATE[STUB]
void skeleton_render_model_instance::assign_original( skeleton_render_model_ptr v )
{
	// FUNCTION BODY[0x63b010]: 38
	// <0x63b016>|0x006|+0x048:'85'
	// <0x63b05e>|0x04e|+0x02c:'86'
	// <0x63b08a>|0x07a|+0x023:'87'
	// <0>
	// <0x63b0ad>|0x09d|+0x002:'89'
	// <0x63b0af>|0x09f|+0x006:'90'
	// <0x63b0b5>|0x0a5|+0x00b:'91'
	// <0x63b0c0>|0x0b0|+0x00e:'92'
	// <0>
	// <1>
	// <0x63b0ce>|0x0be|+0x006:'95'
	// <0x63b0d4>|0x0c4|+0x006:'96'
	// <0x63b0da>|0x0ca|+0x006:'97'
	// <0x63b0e0>|0x0d0|+0x00e:'98'
	// <0>
	// <1>
	// <2>
	// <0x63b0ee>|0x0de|+0x012:'102'
	// <0x63b100>|0x0f0|+0x00e:'103'
	// <0>
	// <0x63b10e>|0x0fe|+0x022:'105'
	// <0>
	// <0x63b130>|0x120|+0x015:'107'
	// <0x63b145>|0x135|+0x003:'108'
	// <0x63b148>|0x138|+0x011:'109'
	// <0x63b159>|0x149|+0x003:'110'
	// <0x63b15c>|0x14c|+0x015:'111'
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
	// ******
}

// STATE[STUB]
u32 skeleton_render_model_instance::get_surfaces_count( u32 lod_id ) const
{
	return 0;

	// FUNCTION BODY[0x63a660]: 1
	// <0x63a660>|0x000|+0x005:'127'
	// ******
}

// STATE[STUB]
void skeleton_render_model_instance::get_surfaces(
	float4x4 const*							mat_vp,
	float3 const*							view_pos,
	vector< render_surface_instance* >&		list,
	bool									visible_only,
	u8										lod_id,
	u32										surface_flags
)
{
	// LOCALS
	// u32 								i
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x629c02 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN208") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x629c02 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN209") })
	// ******

	// FUNCTION BODY[0x63aa80]: 8
	// <0x63aa87>|0x007|+0x033:'132'
	// <0x63aaba>|0x03a|-0x012:'132'
	// <0>
	// <0x63aaa8>|0x028|+0x018:'134'
	// <0>
	// <0x63aac0>|0x040|+0x00a:'136'
	// <0x63aaca>|0x04a|+0x014:'137'
	// <0x63aade>|0x05e|+0x11a:'138'
	// <0x63abf8>|0x178|-0x00a:'138'
	// <0>
	// ******
}

// STATE[STUB]
void skeleton_render_model_instance::get_surface_stats( u32 surface_id, surface_stats& stats ) const
{
	// FUNCTION BODY[0x63a6a0]: 2
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void skeleton_render_model_instance::update( )
{
	// LOCALS
	// u32 								i
	// ******

	// FUNCTION BODY[0x63a7b0]: 21
	// <0>
	// <0x63a7c1>|0x011|+0x016:'151'
	// <0>
	// <0x63a7d7>|0x027|+0x210:'153'
	// <0x63a9e7>|0x237|-0x1f7:'153'
	// <0>
	// <0x63a7f0>|0x040|+0x00a:'155'
	// <0x63a7fa>|0x04a|+0x00a:'156'
	// <0>
	// <1>
	// <0x63a804>|0x054|+0x00c:'159'
	// <0x63a810>|0x060|+0x006:'160'
	// <0x63a816>|0x066|+0x00f:'161'
	// <0x63a825>|0x075|+0x031:'162'
	// <0>
	// <1>
	// <2>
	// <0x63a856>|0x0a6|+0x18a:'166'
	// <0x63a9e0>|0x230|-0x180:'166'
	// <0x63a860>|0x0b0|+0x1a4:'167'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
void skeleton_render_model_instance::set_constants( )
{
	// FUNCTION BODY[0x63a760]: 5
	// <0>
	// <1>
	// <2>
	// <0x63a764>|0x004|+0x029:'178'
	// <0x63a78d>|0x02d|+0x01f:'179'
	// ******
}

// STATE[STUB]
void skeleton_render_model_instance::update_render_matrices( float4x4 const* matrices, u32 count )
{
	// FUNCTION BODY[0x63a6c0]: 9
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x63a6cf>|0x00f|+0x011:'188'
	// <0>
	// <0x63a6e0>|0x020|+0x01a:'190'
	// <0x63a6fa>|0x03a|+0x050:'191'
	// <0>
	// ******
}

// STATE[STUB]
bool skeleton_render_model_instance::get_locator( pcstr locator_name, model_locator_item& result ) const
{
	// CALL SITE INFO
	// <0x63a6bb> -> bool < unknown >( pcstr, model_locator_item& ) const
	// ******

	return false;

	// FUNCTION BODY[0x63a6b0]: 0
	// ******
}

// STATE[STUB]
void skeleton_render_model_instance::get_bind_pose( float4x4* matrices, u32 count ) const
{
	// FUNCTION BODY[0x63aa60]: 1
	// <0x63aa60>|0x000|+0x015:'202'
	// ******
}

} // namespace render
} // namespace vostok
