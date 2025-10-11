////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "ladder_cook.h"

namespace survarium {

// STATE[STUB]
// survarium::ladder_cook::ladder_cook()
ladder_cook::ladder_cook( )
{
}

// STATE[STUB]
// void survarium::ladder_cook::~ladder_cook()
void ladder_cook::~ladder_cook( )
{
}

// STATE[STUB]
// void survarium::ladder_cook::translate_query(vostok::resources::query_result_for_cook&)
void ladder_cook::translate_query( vostok::resources::query_result_for_cook& parent )
{
	// LOCALS
	// vostok::configs::binary_config_value const* it_point_end
	// bool 						result
	// vostok::configs::binary_config_value const& points
	// vostok::variant<32>* 		user_data
	// vostok::configs::binary_config_value const* it_point
	// vostok::configs::binary_config_value config
	// vostok::vectora<vostok::resources::request> requests
	// pcstr 						end_animation<1>
	// pcstr 						start_animation<1>
	// vostok::configs::binary_config_value const& point<1>
	// ******

	// FUNCTION BODY
	// <0x70162b>|0x000|0x000:'25'
	// <0x701636>|0x00b|0x00b:'26'
	// 1
	// <0x701642>|0x017|0x00c:'28'
	// <0x70164a>|0x01f|0x008:'29'
	// <0x701658>|0x02d|0x00e:'30'
	// 1
	// <0x701664>|0x039|0x00c:'32'
	// 1
	// <0x70166c>|0x041|0x008:'34'
	// 1
	// <0x701684>|0x059|0x018:'36'
	// <0x701704>|0x0d9|0x080:'37'
	// <0x701710>|0x0e5|0x00c:'38'
	// 1
	// 2
	// <0x70171d>|0x0f2|0x00d:'41'
	// 1
	// <0x70174a>|0x11f|0x02d:'43'
	// 1
	// <0x701756>|0x12b|0x00c:'45'
	// <0x701766>|0x13b|0x010:'46'
	// <0x701771>|0x146|0x00b:'47'
	// 1
	// <0x70177c>|0x151|0x00b:'49'
	// 1
	// <0x701793>|0x168|0x017|[1]:'51'
	// 1
	// <0x701799>|0x16e|0x006:'53'
	// 1
	// <0x7017a5>|0x17a|0x00c:'55'
	// <0x7017ba>|0x18f|0x015:'56'
	// <0x7017ce>|0x1a3|0x014:'57'
	// 1
	// <0x7017f2>|0x1c7|0x024:'59'
	// <0x701807>|0x1dc|0x015:'60'
	// <0x70181b>|0x1f0|0x014:'61'
	// <0x701842>|0x217|0x027:'62'
	// 1
	// <0x701847>|0x21c|0x005:'64'
	// 1
	// <0x701858>|0x22d|0x011:'66'
	// <0x7018e1>|0x2b6|0x089:'67'
	// <0x7018ed>|0x2c2|0x00c:'68'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// <0x7018fa>|0x2cf|0x00d:'78'
	// ******
}

// STATE[STUB]
// void survarium::ladder_cook::on_animations_loaded(vostok::resources::queries_result&, vostok::configs::binary_config_value const&)
void ladder_cook::on_animations_loaded( vostok::resources::queries_result& data, vostok::configs::binary_config_value const& config )
{
	// LOCALS
	// vostok::configs::binary_config_value const* it_point_end
	// vostok::math::float4x4 const& ladder_transform
	// vostok::configs::binary_config_value const& points
	// vostok::math::plane const& 	ladder_plane
	// u32 							resource_index
	// vostok::resources::query_result_for_cook* parent
	// vostok::configs::binary_config_value const* it_point
	// ladder* 						new_ladder
	// pcstr 						end_animation<1>
	// vostok::math::float4x4 const& point_tansform<1>
	// pcstr 						start_animation<1>
	// landing_point* 				new_point<1>
	// vostok::configs::binary_config_value const& point<1>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6f01a0 }, type_index: TypeIndex(0x42116) })
	// ******

	// FUNCTION BODY
	// <0x700fab>|0x000|0x000:'83'
	// <0x700fb6>|0x00b|0x00b:'84'
	// <0x700fc8>|0x01d|0x012:'85'
	// 1
	// <0x700fd7>|0x02c|0x00f:'87'
	// <0x700fe9>|0x03e|0x012:'88'
	// <0x700ff5>|0x04a|0x00c:'89'
	// 1
	// 2
	// <0x700ffa>|0x04f|0x005:'92'
	// <0x701001>|0x056|0x007:'93'
	// <0x701059>|0x0ae|0x058:'94'
	// <0x701087>|0x0dc|0x02e:'95'
	// <0x701187>|0x1dc|0x100:'96'
	// <0x7011a2>|0x1f7|0x01b:'97'
	// <0x7011b2>|0x207|0x010:'98'
	// <0x7011bd>|0x212|0x00b:'99'
	// 1
	// <0x7011c8>|0x21d|0x00b:'101'
	// 1
	// <0x7011df>|0x234|0x017|[1]:'103'
	// <0x7011e5>|0x23a|0x006:'104'
	// <0x701257>|0x2ac|0x072:'105'
	// 1
	// <0x701359>|0x3ae|0x102:'107'
	// <0x70136e>|0x3c3|0x015:'108'
	// <0x701382>|0x3d7|0x014:'109'
	// 1
	// <0x701400>|0x455|0x07e:'111'
	// <0x701418>|0x46d|0x018:'112'
	// <0x70142f>|0x484|0x017:'113'
	// 1
	// <0x7014ad>|0x502|0x07e:'115'
	// 1
	// <0x7014fd>|0x552|0x050:'117'
	// <0x701586>|0x5db|0x089:'118'
	// <0x701587>|0x5dc|0x001:'119'
	// <0x7015b9>|0x60e|0x032:'120'
	// 1
	// 2
	// <0x7015be>|0x613|0x005:'123'
	// <0x7015ca>|0x61f|0x00c:'124'
	// 1
	// <0x7015cf>|0x624|0x005:'126'
	// <0x7015ed>|0x642|0x01e:'127'
	// ******
}

// STATE[STUB]
// void survarium::ladder_cook::delete_resource(vostok::resources::resource_base*)
void ladder_cook::delete_resource( vostok::resources::resource_base* resource )
{
	// LOCALS
	// ladder* 						ladder_res
	// landing_point* 				point<1>
	// ******

	// FUNCTION BODY
	// <0x700f29>|0x000|0x000:'132'
	// 1
	// <0x700f2f>|0x006|0x006|[1]:'134'
	// <0x700f40>|0x017|0x011:'135'
	// 1
	// <0x700f59>|0x030|0x019:'137'
	// ******
}

	/* TYPEDEFS

	typedef
		vostok::resources::request*
		iterator_type;

} // namespace survarium
