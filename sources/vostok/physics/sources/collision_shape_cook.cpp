////////////////////////////////////////////////////////////////////////////
//	Created 	: 05.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "./collision_shape_cook.h"

#include <vostok/physics/collision_shapes.h>

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

// * `set_length` didn't inline.
// * virtual_path vs path differences (possibly also inlining problems).
// * LTCG for `query_resources`.
void collision_shape_cook::translate_query( resources::query_result_for_cook& parent )
{
	fs_new::virtual_path_string req_path = parent.get_requested_path( );
	u32 found = req_path.find( "#[" );
	collision_shape_cook::cook_data* cd = VOSTOK_NEW_IMPL( g_ph_allocator, collision_shape_cook::cook_data );
	cd->parent_query = &parent;																// <0x72e15b>|0x09f|0x046:'37'

	if ( found != -1 )																		// <0x72e15d>|0x0a1|0x002:'39'
	{
		cd->model_path = req_path.substr( 0, found );										// <0x72e162>|0x0a6|0x005:'41'
		float sx, sy, sz;
		sscanf_s( req_path.begin( ) + found, "#[%f][%f][%f]", &sx, &sy, &sz );				// <0x72e17f>|0x0c3|0x01d:'43'

		cd->scale_.set( sx, sy, sz );														// <0x72e1a1>|0x0e5|0x022:'45' !
	} else																					// <0x72e1c3>|0x107|0x022:'46'
	{
		cd->model_path = req_path;															// <0x72e1c5>|0x109|0x002:'48'
		cd->scale_.set( 1.0f, 1.0f, 1.0f );													// <0x72e1d6>|0x11a|0x011:'49' !
	}

	pcstr exported_primitives_path = 0;
	STR_JOINA( exported_primitives_path, cd->model_path.c_str(), "/exported_primitives" );	// <0x72e1f6>|0x13a|0x020:'53'

	fs_new::virtual_path_string model_config_path = cd->model_path;							// <0x72e229>|0x16d|0x033:'55'

	fs_new::virtual_path_string::size_type pos = model_config_path.find( ".model" );		// <0x72e26c>|0x1b0|0x043:'57' !
	model_config_path.set_length( pos + strlen(".model") );									// <0x72e297>|0x1db|0x02b:'58' ! `models\level\03\flora\gra_green_01.model\settings`
	model_config_path.append("/settings");													// <0x72e2b5>|0x1f9|0x01e:'59' !

	pcstr verticies_path = 0;
	pcstr indices_path = 0;
	pcstr face_data_path = 0;
	STR_JOINA( verticies_path, cd->model_path.c_str(), "/vertices" );						// <0x72e2e7>|0x22b|0x032:'64'
	STR_JOINA( indices_path, cd->model_path.c_str(), "/indices" );							// <0x72e31a>|0x25e|0x033:'65'
	STR_JOINA( face_data_path, cd->model_path.c_str(), "/face_data" );						// <0x72e34d>|0x291|0x033:'66'

	resources::request const requests[] = {
		{ exported_primitives_path,		resources::binary_config_class_impl },				// <0x72e37d>|0x2c1|0x030:'69'
		{ model_config_path.c_str( ),	resources::binary_config_class_impl },				// <0x72e380>|0x2c4|0x003:'70'
		{ verticies_path,				resources::raw_data_class },						// <0x72e389>|0x2cd|0x009:'71'
		{ indices_path,					resources::raw_data_class },						// <0x72e394>|0x2d8|0x00b:'72'
		{ face_data_path,				resources::raw_data_class }
	};

	resources::query_resources			(
		requests,
		array_size( requests ),
		boost::bind( &collision_shape_cook::on_collision_sources_loaded, this, _1, cd ),
		g_ph_allocator,
		0,
		&parent
	);																						// <0x72e3a5>|0x2e9|0x011:'83'
}

void collision_shape_cook::on_collision_sources_loaded( resources::queries_result& data, collision_shape_cook::cook_data* cd )
{
	configs::binary_config_ptr primitives_cfg = static_cast_resource_ptr<configs::binary_config_ptr>( data[0].get_unmanaged_resource( ) );
	configs::binary_config_ptr model_settings_cfg = static_cast_resource_ptr<configs::binary_config_ptr>( data[1].get_unmanaged_resource( ) );

	bt_collision_shape* shape = NULL;
	if ( primitives_cfg.c_ptr( ) ) // sushi@TODO: Really?
	{
		configs::binary_config_value primitives_config_root = primitives_cfg->get_root( );
		shape = create_primitives_shape( primitives_config_root["primitives"], cd );
		u32 size = primitives_config_root["primitives"].size( );

		if ( model_settings_cfg.c_ptr() && model_settings_cfg->get_root( ).value_exists( "game_material_settings" ) )
		{
			configs::binary_config_value game_mtl_settings = model_settings_cfg->get_root( )["game_material_settings"];
			for ( u32 i = 0 ; i < size ; ++i )
			{
				u16 face_data = shape->m_shapes_face_data[i];	// sushi@TODO: Should be private? // sushi@TODO: Structure has u16*
				pcstr key = (pcstr)primitives_config_root["mtl_list"] + 6 * face_data; // sushi@TODO: 6?
				if ( game_mtl_settings.value_exists( key ) )
				{
					shape->m_shapes_face_data[i] = (u16)game_mtl_settings[key]["game_material_id"];
				} else
				{
					shape->m_shapes_face_data[i] = 0;
				}
			}
		} else
		{
			memset( shape->m_shapes_face_data, 0, size );
		}
	}

	if ( data[2].is_successful( ) )
	{
		resources::pinned_ptr_const<u8> vertices_ptr    ( data[2].get_managed_resource() );
		resources::pinned_ptr_const<u8> indices_ptr		( data[3].get_managed_resource() );
		resources::pinned_ptr_const<u8> face_data_ptr   ( data[4].get_managed_resource() );

		memory::chunk_reader vertices_chunk_reader	( vertices_ptr.c_ptr(), vertices_ptr.size(), memory::chunk_reader::chunk_type_sequential );
		memory::chunk_reader indices_chunk_reader	( indices_ptr.c_ptr(), indices_ptr.size(), memory::chunk_reader::chunk_type_sequential );
		memory::chunk_reader face_data_chunk_reader	( face_data_ptr.c_ptr(), face_data_ptr.size(), memory::chunk_reader::chunk_type_sequential );

		memory::reader vertices_reader = vertices_chunk_reader.open_reader( 0x19u );
		memory::reader indices_reader  = indices_chunk_reader.open_reader( 0x1Au ); // sushi@TODO

		if ( model_settings_cfg.c_ptr( ) ) // sushi@TODO
		{
			memory::reader face_data_reader  = face_data_chunk_reader.open_reader( 0x1Bu );

		}
	}

	if ( shape )
	{
		cd->parent_query->set_unmanaged_resource( shape, resources::nocache_memory, sizeof( bt_collision_shape ) );
		cd->parent_query->finish_query( result_success );
	} else
		cd->parent_query->finish_query( result_error );

	VOSTOK_DELETE_IMPL( g_ph_allocator, cd );

	//
	// configs::binary_config_value mtl_bind_root
	// u16 							shape_mtl_idx
	// pcstr 						maya_sg
	// u32 							icount
	// u32 							tcount
	// u32 							vcount
	// u16* 						face_data
	// configs::binary_config_value root
	// remap*						remap_table
	// configs::binary_config_value t
	// fixed_string<260> 			game_mtl
	// u16 							game_mtl_id
	// btBvhTriangleMeshShape* 		tri_shape
	// btCompoundShape* 			compound_shape
	// btTransform 					child_local_transform


	struct remap {
		u16		game_mtl;
	};


	// FUNCTION BODY
	// <0x72d42f>|0x000|0x000:'88'
	// <0x72d479>|0x04a|0x04a:'89'
	// <1>
	// <2>
	// <3>
	// <0x72d4ba>|0x08b|0x041:'93'
	// <1>
	// <0x72d4d7>|0x0a8|0x01d:'95'
	// <0x72d4fd>|0x0ce|0x026:'96'
	// <1>
	// <0x72d521>|0x0f2|0x024:'98'
	// <1>
	// <0x72d54a>|0x11b|0x029:'100'
	// <1>
	// <0x72d57a>|0x14b|0x030:'102'
	// <1>
	// <0x72d5ae>|0x17f|0x034:'104'
	// <1>
	// <0x72d5c1>|0x192|0x013:'106'
	// <0x72d5cf>|0x1a0|0x00e:'107'
	// <0x72d5f3>|0x1c4|0x024:'108'
	// <1>
	// <0x72d608>|0x1d9|0x015:'110'
	// <0x72d63c>|0x20d|0x034:'111'
	// <0x72d63e>|0x20f|0x002:'112'
	// <0x72d657>|0x228|0x019:'113'
	// <1>
	// <0x72d984>|0x555|0x32d:'115'
	// <1>
	// <2>
	// <3>
	// <0x72d65a>|0x22b|-0x32a:'119' if ( data[2].is_successful( ) )
	// <1>							 {
	// <2>
	// <3>
	// <0x72d673>|0x244|0x019:'123'
	// <0x72d6af>|0x280|0x03c:'124'
	// <0x72d6e8>|0x2b9|0x039:'125'
	// <1>
	// <0x72d71d>|0x2ee|0x035:'127'
	// <0x72d724>|0x2f5|0x007:'128'
	// <0x72d728>|0x2f9|0x004:'129'
	// <1>
	// <0x72d76c>|0x33d|0x044:'131'
	// <0x72d7e9>|0x3ba|0x07d:'132'
	// <1>
	// <0x72d804>|0x3d5|0x01b:'134'
	// <0x72d80b>|0x3dc|0x007:'135'
	// <0x72d810>|0x3e1|0x005:'136'
	// <1>
	// <0x72d82a>|0x3fb|0x01a:'138'
	// <0x72d82e>|0x3ff|0x004:'139'
	// <1>
	// <2>
	// <0x72d845>|0x416|0x017:'142'
	// <0x72d862>|0x433|0x01d:'143'
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x72d873>|0x444|0x011:'148'
	// <0x72d890>|0x461|0x01d:'149'
	// <0x72d89c>|0x46d|0x00c:'150'
	// <1>
	// <2>
	// <0x72d8cf>|0x4a0|0x033:'153'
	// <1>
	// <0x72d8f9>|0x4ca|0x02a:'155'
	// <1>
	// <0x72d91b>|0x4ec|0x022:'157'
	// <1>
	// <0x72d93e>|0x50f|0x023:'159'
	// <1>
	// <0x72d9a5>|0x576|0x067:'161'
	// <0x72d9ba>|0x58b|0x015:'162'
	// <1>
	// <0x72d9d5>|0x5a6|0x01b:'164'
	// <1>
	// <2>
	// <3>
	// <0x72d9fb>|0x5cc|0x026:'168'
	// <0x72da20>|0x5f1|0x025:'169'
	// <1>
	// <0x72da4b>|0x61c|0x02b:'171'
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x72da62>|0x633|0x017:'176'
	// <0x72dace>|0x69f|0x06c:'177'
	// <1>
	// <0x72dae5>|0x6b6|0x017:'179'
	// <1>
	// <0x72daf3>|0x6c4|0x00e:'181'
	// <0x72daf6>|0x6c7|0x003:'182'
	// <1>
	// <0x72db11>|0x6e2|0x01b:'184'
	// <1>
	// <2>
	// <0x72db2f>|0x700|0x01e:'187'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x72db3b>|0x70c|0x00c:'196'
	// <0x72dbc3>|0x794|0x088:'197'
	// <0x72dbcd>|0x79e|0x00a:'198'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x72dbd2>|0x7a3|0x005:'207'
	// <1>
	// <0x72dc5a>|0x82b|0x088:'209'
	// <0x72dc60>|0x831|0x006:'210'
	// <0x72dda1>|0x972|0x141:'211'
	// <0x72df1a>|0xaeb|0x179:'212'
	// <1>
	// <0x72df28>|0xaf9|0x00e:'214'
	// <1>
	// <0x72e000>|0xbd1|0x0d8:'216' if ( shape )
	// <1>							{
	// <0x72e008>|0xbd9|0x008:'218'		cd->parent_query->set_unmanaged_resource( shape, resources::nocache_memory, sizeof( bt_collision_shape ) );
	// <0x72e030>|0xc01|0x028:'219'		cd->parent_query->finish_query( result_success );
	// <0x72e037>|0xc08|0x007:'220' } else
	// <0x72e039>|0xc0a|0x002:'221'		cd->parent_query->finish_query( result_error );
	// <1>
	// <0x72e049>|0xc1a|0x010:'223' VOSTOK_DELETE_IMPL( g_ph_allocator, cd );
	// ******
}

void collision_shape_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( g_ph_allocator, resource ); // <0x72d261>|0x000|0x000:'229'

	// bt_collision_shape* s = static_cast_checked<bt_collision_shape*>(resource);
	// destroy_shape			( *g_ph_allocator, s );

}

bt_collision_shape* collision_shape_cook::create_primitives_shape( configs::binary_config_value const& primitives_t, collision_shape_cook::cook_data* cd )
{
	float3 p;
	bool single_shape = primitives_t.size() == 1;
	if(single_shape)
	{
		p = primitives_t[0]["position"];
		single_shape = p.is_similar(float3(0,0,0));
	}

	if(!single_shape)
	{
		return create_compound_shape( primitives_t, cd->scale_, cd->model_path.c_str() );
	}else
	{// single primitive shape
		configs::binary_config_value cfg = primitives_t[0];
		collision::primitive_type type = (collision::primitive_type)(int)cfg["type"];
		return create_primitive_shape		( type, (float3)cfg["scale"], cd->scale_ );
	}
}

} // namespace physics
} // namespace vostok