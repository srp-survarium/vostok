////////////////////////////////////////////////////////////////////////////
//	Created 	: 05.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "./collision_shape_cook.h"

namespace vostok {
namespace physics {

// STATE[STUB]
collision_shape_cook::collision_shape_cook( bool static_object ):
	resources::translate_query_cook			(
		static_object ? resources::collision_bt_shape_class_static : resources::collision_bt_shape_class_dynamic,
		reuse_true,
		thread_id_unset,
		flag_last
	),
	m_static_object	( static_object )
{
	register_cook( this );	// <0x72d2e5>|0x000|0x000:'29'
}

// STATE[STUB]
void collision_shape_cook::translate_query( resources::query_result_for_cook& parent )
{
	// LOCALS
	// fs_new::virtual_path_string     model_config_path
	// collision_shape_cook::cook_data* cd
	// fs_new::virtual_path_string     req_path
	// resources::request[5]           requests
	// u32                             found
	// float                           sz
	// float                           sx
	// float                           sy
	// strings::detail::tuples         STR_JOINA_tuples_unique_identifier
	// strings::detail::tuples         STR_JOINA_tuples_unique_identifier
	// strings::detail::tuples         STR_JOINA_tuples_unique_identifier
	// strings::detail::tuples         STR_JOINA_tuples_unique_identifier
	// ******

	// FUNCTION BODY
	// <0x72e0bc>|0x000|0x000:'34'
	// <0x72e0e6>|0x02a|0x02a:'35'
	// <0x72e115>|0x059|0x02f:'36'
	// <0x72e15b>|0x09f|0x046:'37'

	// <0x72e15d>|0x0a1|0x002:'39'

	// <0x72e162>|0x0a6|0x005:'41'

	// <0x72e17f>|0x0c3|0x01d:'43'

	// <0x72e1a1>|0x0e5|0x022:'45'
	// <0x72e1c3>|0x107|0x022:'46'

	// <0x72e1c5>|0x109|0x002:'48'
	// <0x72e1d6>|0x11a|0x011:'49'



	// <0x72e1f6>|0x13a|0x020:'53'

	// <0x72e229>|0x16d|0x033:'55'

	// <0x72e26c>|0x1b0|0x043:'57'
	// <0x72e297>|0x1db|0x02b:'58'
	// <0x72e2b5>|0x1f9|0x01e:'59'




	// <0x72e2e7>|0x22b|0x032:'64'
	// <0x72e31a>|0x25e|0x033:'65'
	// <0x72e34d>|0x291|0x033:'66'


	// <0x72e37d>|0x2c1|0x030:'69'
	// <0x72e380>|0x2c4|0x003:'70'
	// <0x72e389>|0x2cd|0x009:'71'
	// <0x72e394>|0x2d8|0x00b:'72'










	// <0x72e3a5>|0x2e9|0x011:'83'
	// ******
}

// STATE[STUB]
void collision_shape_cook::on_collision_sources_loaded( resources::queries_result& data, collision_shape_cook::cook_data* cd )
{
	// LOCALS
	// bt_collision_shape*             result
	// resources::resource_ptr<configs::binary_config,resources::unmanaged_intrusive_base> primitives_cfg
	// resources::resource_ptr<configs::binary_config,resources::unmanaged_intrusive_base> model_settings_cfg
	// configs::binary_config_value    primitives_config_root
	// configs::binary_config_value    mtl_bind_root
	// u16                             shape_mtl_idx
	// pcstr                           maya_sg
	// u32                             icount
	// memory::chunk_reader            indices_chunk_reader
	// memory::chunk_reader            vertices_chunk_reader
	// memory::chunk_reader            face_data_chunk_reader
	// u32                             tcount
	// u32                             vcount
	// resources::pinned_ptr_const<u8> face_data_ptr
	// resources::pinned_ptr_const<u8> indices_ptr
	// u16*                            face_data
	// resources::pinned_ptr_const<u8> vertices_ptr
	// configs::binary_config_value    root
	// collision_shape_cook::on_collision_sources_loaded::__l29::remap* remap_table
	// memory::reader                  face_data_reader
	// configs::binary_config_value    t
	// fixed_string<260>               game_mtl
	// u16                             game_mtl_id
	// btBvhTriangleMeshShape*         tri_shape
	// btCompoundShape*                compound_shape
	// btTransform                     child_local_transform
	// ******


	struct remap {
		u16		game_mtl;
	};

	// FUNCTION BODY
	// <0x72d42f>|0x000|0x000:'88'
	// <0x72d479>|0x04a|0x04a:'89'



	// <0x72d4ba>|0x08b|0x041:'93'

	// <0x72d4d7>|0x0a8|0x01d:'95'
	// <0x72d4fd>|0x0ce|0x026:'96'

	// <0x72d521>|0x0f2|0x024:'98'

	// <0x72d54a>|0x11b|0x029:'100'

	// <0x72d57a>|0x14b|0x030:'102'

	// <0x72d5ae>|0x17f|0x034:'104'

	// <0x72d5c1>|0x192|0x013:'106'
	// <0x72d5cf>|0x1a0|0x00e:'107'
	// <0x72d5f3>|0x1c4|0x024:'108'

	// <0x72d608>|0x1d9|0x015:'110'
	// <0x72d63c>|0x20d|0x034:'111'
	// <0x72d63e>|0x20f|0x002:'112'
	// <0x72d657>|0x228|0x019:'113'

	// <0x72d984>|0x555|0x32d:'115'



	// <0x72d65a>|0x22b|-0x32a:'119'



	// <0x72d673>|0x244|0x019:'123'
	// <0x72d6af>|0x280|0x03c:'124'
	// <0x72d6e8>|0x2b9|0x039:'125'

	// <0x72d71d>|0x2ee|0x035:'127'
	// <0x72d724>|0x2f5|0x007:'128'
	// <0x72d728>|0x2f9|0x004:'129'

	// <0x72d76c>|0x33d|0x044:'131'
	// <0x72d7e9>|0x3ba|0x07d:'132'

	// <0x72d804>|0x3d5|0x01b:'134'
	// <0x72d80b>|0x3dc|0x007:'135'
	// <0x72d810>|0x3e1|0x005:'136'

	// <0x72d82a>|0x3fb|0x01a:'138'
	// <0x72d82e>|0x3ff|0x004:'139'


	// <0x72d845>|0x416|0x017:'142'
	// <0x72d862>|0x433|0x01d:'143'




	// <0x72d873>|0x444|0x011:'148'
	// <0x72d890>|0x461|0x01d:'149'
	// <0x72d89c>|0x46d|0x00c:'150'


	// <0x72d8cf>|0x4a0|0x033:'153'

	// <0x72d8f9>|0x4ca|0x02a:'155'

	// <0x72d91b>|0x4ec|0x022:'157'

	// <0x72d93e>|0x50f|0x023:'159'

	// <0x72d9a5>|0x576|0x067:'161'
	// <0x72d9ba>|0x58b|0x015:'162'

	// <0x72d9d5>|0x5a6|0x01b:'164'



	// <0x72d9fb>|0x5cc|0x026:'168'
	// <0x72da20>|0x5f1|0x025:'169'

	// <0x72da4b>|0x61c|0x02b:'171'




	// <0x72da62>|0x633|0x017:'176'
	// <0x72dace>|0x69f|0x06c:'177'

	// <0x72dae5>|0x6b6|0x017:'179'

	// <0x72daf3>|0x6c4|0x00e:'181'
	// <0x72daf6>|0x6c7|0x003:'182'

	// <0x72db11>|0x6e2|0x01b:'184'


	// <0x72db2f>|0x700|0x01e:'187'








	// <0x72db3b>|0x70c|0x00c:'196'
	// <0x72dbc3>|0x794|0x088:'197'
	// <0x72dbcd>|0x79e|0x00a:'198'








	// <0x72dbd2>|0x7a3|0x005:'207'

	// <0x72dc5a>|0x82b|0x088:'209'
	// <0x72dc60>|0x831|0x006:'210'
	// <0x72dda1>|0x972|0x141:'211'
	// <0x72df1a>|0xaeb|0x179:'212'

	// <0x72df28>|0xaf9|0x00e:'214'

	// <0x72e000>|0xbd1|0x0d8:'216'

	// <0x72e008>|0xbd9|0x008:'218'
	// <0x72e030>|0xc01|0x028:'219'
	// <0x72e037>|0xc08|0x007:'220'
	// <0x72e039>|0xc0a|0x002:'221'

	// <0x72e049>|0xc1a|0x010:'223'
	// ******
}

// STATE[100%|DONE]
void collision_shape_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( g_ph_allocator, resource ); // <0x72d261>|0x000|0x000:'229'

	// bt_collision_shape* s = static_cast_checked<bt_collision_shape*>(resource);
	// destroy_shape			( *g_ph_allocator, s );

}

// STATE[STUB]
bt_collision_shape* collision_shape_cook::create_primitives_shape( configs::binary_config_value const& primitives_t, collision_shape_cook::cook_data* cd )
{
	return NULL;

	// LOCALS
	// float3                          p
	// configs::binary_config_value    cfg
	// ******

	// FUNCTION BODY

	// <0x72d319>|0x000|0x000:'235'
	// <0x72d33d>|0x024|0x024:'236'

	// <0x72d341>|0x028|0x004:'238'
	// <0x72d34d>|0x034|0x00c:'239'


	// <0x72d38d>|0x074|0x040:'242'

	// <0x72d391>|0x078|0x004:'244'


	// <0x72d3a8>|0x08f|0x017:'247'
	// <0x72d3c4>|0x0ab|0x01c:'248'
	// <0x72d39f>|0x086|-0x025:'249'



	// ******
}

} // namespace physics
} // namespace vostok