////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "items_dictionary_cook.h"

namespace survarium {

// STATE[STUB]
// survarium::items_dictionary_cook::items_dictionary_cook()
items_dictionary_cook::items_dictionary_cook( )
{
	// FUNCTION BODY
	// <0x7645e5>|0x000|0x000:'17'
	// ******
}

// STATE[STUB]
// void survarium::items_dictionary_cook::translate_query(vostok::resources::query_result_for_cook&)
void items_dictionary_cook::translate_query( vostok::resources::query_result_for_cook& parent )
{
	// FUNCTION BODY
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// <0x764d2c>|0x000|0x000:'28'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::items_dictionary_cook::delete_resource(vostok::resources::resource_base*)
void items_dictionary_cook::delete_resource( vostok::resources::resource_base* resource )
{
	// FUNCTION BODY
	// <0x764609>|0x000|0x000:'34'
	// ******
}

// STATE[STUB]
// void survarium::items_dictionary_cook::on_items_dictionary_config_loaded(vostok::resources::queries_result&)
void items_dictionary_cook::on_items_dictionary_config_loaded( vostok::resources::queries_result& data )
{
	// LOCALS
	// u32 							requests_count
	// vostok::resources::query_result_for_cook* parent
	// vostok::configs::binary_config_value const* items_it_e
	// u32* 						item_dict_ids
	// items_dictionary* 			cooked_resource
	// vostok::vectora<vostok::resources::request> requests
	// vostok::configs::binary_config_value const& dict_cfg
	// vostok::configs::binary_config_value const* items_it
	// u32 							i<1>
	// dictionary_item 				item_dict<2>
	// bool 						is_stack<2>
	// bool 						is_premium<2>
	// char* 						item_cfg_path<2>
	// u32 							item_dict_id<2>
	// u8 							item_category_id<2>
	// vostok::strings::detail::tuples STR_JOINA_tuples_unique_identifier<3>
	// ******

	// SKIPPED BLOCKS
	// <0x764a4f><3>
	// ******

	// FUNCTION BODY
	// <0x7648c1>|0x000|0x000:'40'
	// 1
	// <0x7648cc>|0x00b|0x00b:'42'
	// 1
	// <0x76493b>|0x07a|0x06f:'44'
	// 1
	// <0x764980>|0x0bf|0x045:'46'
	// 1
	// <0x764995>|0x0d4|0x015:'48'
	// 1
	// <0x76499d>|0x0dc|0x008:'50'
	// 1
	// <0x7649b4>|0x0f3|0x017:'52'
	// 1
	// <0x7649d6>|0x115|0x022:'54'
	// <0x7649ed>|0x12c|0x017:'55'
	// 1
	// <0x764a04>|0x143|0x017|[1]:'57'
	// 1
	// <0x764a2b>|0x16a|0x027|[2]:'59'
	// <0x764a32>|0x171|0x007:'60'
	// 1
	// 2
	// 3
	// 4
	// <0x764b47>|0x286|0x115:'65'
	// 1
	// 2
	// <0x764b6e>|0x2ad|0x027:'68'
	// <0x764b83>|0x2c2|0x015:'69'
	// <0x764b98>|0x2d7|0x015:'70'
	// <0x764baf>|0x2ee|0x017:'71'
	// 1
	// <0x764bc6>|0x305|0x017:'73'
	// 1
	// <0x764bd2>|0x311|0x00c:'75'
	// 1
	// <0x764be8>|0x327|0x016:'77'
	// <0x764bf1>|0x330|0x009:'78'
	// <0x764bf7>|0x336|0x006:'79'
	// <0x764c0f>|0x34e|0x018:'80'
	// <0x764c15>|0x354|0x006:'81'
	// 1
	// <0x764c1b>|0x35a|0x006:'83'
	// <0x764c3e>|0x37d|0x023:'84'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// <0x764c4e>|0x38d|0x010:'93'
	// ******
}

// STATE[STUB]
// void survarium::items_dictionary_cook::on_subresources_loaded(vostok::resources::queries_result&, survarium::items_dictionary*, unsigned int*)
void items_dictionary_cook::on_subresources_loaded( vostok::resources::queries_result& data, items_dictionary* cooked_resource, u32* item_dict_ids )
{
	// LOCALS
	// vostok::resources::query_result_for_cook* parent
	// u32 							i<1>
	// vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base> item_cfg<2>
	// dictionary_item& 			current_item<2>
	// u32 							clip_size<3>
	// float 						clip_weight<3>
	// ******

	// SKIPPED BLOCKS
	// <0x76467e><2>
	// <0x7646f6><3>
	// ******

	// FUNCTION BODY
	// <0x76464a>|0x000|0x000:'98'
	// 1
	// <0x764655>|0x00b|0x00b:'100'
	// 1
	// <0x764661>|0x017|0x00c|[1]:'102'
	// 1
	// <0x764684>|0x03a|0x023:'104'
	// <0x7646a8>|0x05e|0x024:'105'
	// 1
	// <0x7646ce>|0x084|0x026:'107'
	// 1
	// <0x7646e9>|0x09f|0x01b:'109'
	// 1
	// <0x7646fc>|0x0b2|0x013:'111'
	// <0x764729>|0x0df|0x02d:'112'
	// <0x764758>|0x10e|0x02f:'113'
	// 1
	// <0x76477d>|0x133|0x025:'115'
	// <0x76477f>|0x135|0x002:'116'
	// 1
	// <0x7647b4>|0x16a|0x035:'118'
	// <0x76482e>|0x1e4|0x07a:'119'
	// 1
	// 2
	// 3
	// 4
	// <0x76483b>|0x1f1|0x00d:'124'
	// <0x76486e>|0x224|0x033:'125'
	// 1
	// <0x76487a>|0x230|0x00c:'127'
	// ******
}

	/* TYPEDEFS

	typedef
		vostok::resources::request*
		iterator_type;

} // namespace survarium
