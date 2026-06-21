////////////////////////////////////////////////////////////////////////////
//	Created 	: 05.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "./collision_shape_cook.h"

#include <vostok/physics/collision_shapes.h>

namespace vostok {
namespace physics {

// claude@NOTE: STRUCTURE MATCH (3 stmts). Residual is non-steerable: optimized-COMDAT
// convention (this-in-esi, ret 4); register_cook() is an out-of-line call in the target
// but /GL inlines its body in our base; and the translate_query_cook base-ctor resolves
// thread_id_unset to a direct GetCurrentThreadId() in the target vs a cached global here.
collision_shape_cook::collision_shape_cook( bool static_object ):
	resources::translate_query_cook			(
		static_object ? resources::collision_bt_shape_class_static : resources::collision_bt_shape_class_dynamic,
		reuse_true,
		thread_id_unset,
		flag_last
	),
	m_static_object	( static_object )
{
	register_cook( this );
}

// * `set_length` didn't inline.
// * virtual_path vs path differences (possibly also inlining problems).
// * LTCG for `query_resources`.
void collision_shape_cook::translate_query( resources::query_result_for_cook& parent )
{
	fs_new::virtual_path_string req_path = parent.get_requested_path( );
	u32 found = req_path.find( "#[" );
	collision_shape_cook::cook_data* cd = VOSTOK_NEW_IMPL( g_ph_allocator, collision_shape_cook::cook_data );
	cd->parent_query = &parent;

	if ( found != -1 )
	{
		cd->model_path = req_path.substr( 0, found );
		float sx, sy, sz;
		sscanf_s( req_path.begin( ) + found, "#[%f][%f][%f]", &sx, &sy, &sz );

		cd->scale_.set( sx, sy, sz );
	} else
	{
		cd->model_path = req_path;
		cd->scale_.set( 1.0f, 1.0f, 1.0f );
	}

	pcstr exported_primitives_path = 0;
	STR_JOINA( exported_primitives_path, cd->model_path.c_str(), "/exported_primitives" );

	fs_new::virtual_path_string model_config_path = cd->model_path;

	fs_new::virtual_path_string::size_type pos = model_config_path.find( ".model" );
	model_config_path.set_length( pos + strlen(".model") );
	model_config_path.append("/settings");

	pcstr verticies_path = 0;
	pcstr indices_path = 0;
	pcstr face_data_path = 0;
	STR_JOINA( verticies_path, cd->model_path.c_str(), "/vertices" );
	STR_JOINA( indices_path, cd->model_path.c_str(), "/indices" );
	STR_JOINA( face_data_path, cd->model_path.c_str(), "/face_data" );

	resources::request const requests[] = {
		{ exported_primitives_path,		resources::binary_config_class_impl },
		{ model_config_path.c_str( ),	resources::binary_config_class_impl },
		{ verticies_path,				resources::raw_data_class },
		{ indices_path,					resources::raw_data_class },
		{ face_data_path,				resources::raw_data_class }
	};

	resources::query_resources			(
		requests,
		array_size( requests ),
		boost::bind( &collision_shape_cook::on_collision_sources_loaded, this, _1, cd ),
		g_ph_allocator,
		0,
		&parent
	);
}

// claude@NOTE: PARTIAL reconstruction (~35%). Target is 69 stmts / 28 locals / 0xc81
// bytes (release-optimized: intrusive_ptr::set + lock-xadd refcounting all inlined).
// The vertices/indices/face_data chunk-reader -> btBvhTriangleMeshShape / btCompoundShape
// build (target source lines ~119-184) is NOT yet reconstructed: missing locals tri_shape,
// compound_shape, remap_table, child_local_transform (btTransform), game_mtl
// (fixed_string<260>), icount/tcount/vcount. The carcass-locals list below was the
// recovery seed. Next step: read --view target 0x71d420 offset 0x23a..0x6f1 to recover
// the triangle-mesh build + material remap loop, then re-measure. Parked to finish the
// rest of the unit.
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
}

void collision_shape_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( g_ph_allocator, resource );

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