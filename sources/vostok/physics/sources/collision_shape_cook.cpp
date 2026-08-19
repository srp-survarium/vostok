////////////////////////////////////////////////////////////////////////////
//	Created 	: 05.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "./collision_shape_cook.h"

#include <vostok/physics/collision_shapes.h>
#include "bullet_include.h"
#include <vostok/physics/bullet_utils.h>
#include <vostok/render/engine/model_format.h>

namespace vostok {
namespace physics {

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

void collision_shape_cook::on_collision_sources_loaded( resources::queries_result& data, collision_shape_cook::cook_data* cd )
{
	configs::binary_config_ptr primitives_cfg = static_cast_resource_ptr<configs::binary_config_ptr>( data[0].get_unmanaged_resource( ) );
	configs::binary_config_ptr model_settings_cfg = static_cast_resource_ptr<configs::binary_config_ptr>( data[1].get_unmanaged_resource( ) );
	configs::binary_config_value mtl_bind_root;

	bt_collision_shape* result = NULL;
	if ( primitives_cfg )
	{
		configs::binary_config_value primitives_config_root = primitives_cfg->get_root( );
		result = create_primitives_shape( primitives_config_root["primitives"], cd );
		u32 size = primitives_config_root["primitives"].size( );

		if ( model_settings_cfg && model_settings_cfg->get_root( ).value_exists( "game_material_settings" ) )
		{
			mtl_bind_root = model_settings_cfg->get_root( )["game_material_settings"];
			for ( u32 i = 0 ; i < size ; ++i )
			{
				u16 shape_mtl_idx = result->m_shapes_face_data[i];
				pcstr maya_sg = (pcstr)primitives_config_root["mtl_list"] + 24 * shape_mtl_idx;
				if ( mtl_bind_root.value_exists( maya_sg ) )
					result->m_shapes_face_data[i] = (u16)mtl_bind_root[maya_sg]["game_material_id"];
				else
					result->m_shapes_face_data[i] = 0;
			}
		} else
			memset( result->m_shapes_face_data, 0, size * sizeof( u16 ) );
	}

	if ( data[2].is_successful( ) )
	{
		resources::pinned_ptr_const<u8> vertices_ptr    ( data[2].get_managed_resource() );
		resources::pinned_ptr_const<u8> indices_ptr		( data[3].get_managed_resource() );
		resources::pinned_ptr_const<u8> face_data_ptr   ( data[4].get_managed_resource() );

		memory::chunk_reader vertices_chunk_reader	( vertices_ptr.c_ptr(), vertices_ptr.size(), memory::chunk_reader::chunk_type_sequential );
		memory::chunk_reader indices_chunk_reader	( indices_ptr.c_ptr(), indices_ptr.size(), memory::chunk_reader::chunk_type_sequential );
		memory::chunk_reader face_data_chunk_reader	( face_data_ptr.c_ptr(), face_data_ptr.size(), memory::chunk_reader::chunk_type_sequential );

		memory::reader vertices_reader = vertices_chunk_reader.open_reader( render::model_chunk_collision_v );
		memory::reader indices_reader  = indices_chunk_reader.open_reader( render::model_chunk_collision_i );

		u32 const vcount = vertices_reader.length( ) / sizeof( float3 );
		u32 const icount = indices_reader.length( ) / sizeof( u32 );
		u32 const tcount = icount / 3;
		u16* face_data = NULL;

		if ( model_settings_cfg )
		{
			memory::reader face_data_reader = face_data_chunk_reader.open_reader( render::model_chunk_collision_face_data_hdr );
			u16 mtl_count = face_data_reader.r_u16( );

			struct remap {
				u16 game_mtl;
			};

			remap* remap_table = VOSTOK_ALLOC_IMPL( g_ph_allocator, remap, mtl_count );
			memory::zero( remap_table, mtl_count * sizeof( remap ) );
			for ( u16 i = 0; i < mtl_count; ++i )
				remap_table[i].game_mtl = u16(-1);

			configs::binary_config_value root = model_settings_cfg->get_root( );
			if ( root.value_exists( "game_material_settings" ) )
			{
				mtl_bind_root = root["game_material_settings"];
				for ( u16 i = 0; i < mtl_count; ++i )
				{
					pcstr maya_sg = face_data_reader.r_string( );
					fixed_string<260> game_mtl;
					if ( mtl_bind_root.value_exists( maya_sg ) )
					{
						configs::binary_config_value t = mtl_bind_root[maya_sg];
						u16 game_mtl_id = u16(-1);
						if ( t.value_exists( "game_material_id" ) )
							game_mtl_id = (u16)t["game_material_id"];

						remap_table[i].game_mtl = game_mtl_id;
					}
				}
			}

			face_data_reader = face_data_chunk_reader.open_reader( render::model_chunk_collision_face_data );
			face_data = VOSTOK_ALLOC_IMPL( g_ph_allocator, u16, tcount );
			for ( u32 i = 0; i < tcount; ++i )
				face_data[i] = remap_table[face_data_reader.r_u16( )].game_mtl;

			VOSTOK_FREE_IMPL( g_ph_allocator, remap_table );
		}

		if ( !result )
		{
			result = create_static_triangle_mesh_shape(
				(float3*)vertices_reader.pointer( ),
				(u32*)indices_reader.pointer( ),
				vcount,
				icount,
				face_data,
				cd->scale_,
				data[2].get_managed_resource( ),
				data[3].get_managed_resource( )
			);
			result->m_tri_face_data = face_data;
		} else
		{
			btBvhTriangleMeshShape* tri_shape = create_btBvhTriangleMeshShape(
				(float3*)vertices_reader.pointer( ),
				(u32*)indices_reader.pointer( ),
				vcount,
				icount,
				face_data,
				cd->scale_,
				data[2].get_managed_resource( ),
				data[3].get_managed_resource( )
			);

			btCompoundShape* compound_shape = (btCompoundShape*)result->get_bt_shape( );
			btTransform child_local_transform( from_vostok( float4x4().identity() ) );
			compound_shape->addChildShape( child_local_transform, tri_shape );
			result->m_tri_face_data = face_data;
		}
	}

	if ( result )
	{
		cd->parent_query->set_unmanaged_resource( result, resources::nocache_memory, sizeof( bt_collision_shape ) );
		cd->parent_query->finish_query( result_success );
	} else
		cd->parent_query->finish_query( result_error );

	VOSTOK_DELETE_IMPL( g_ph_allocator, cd );
}

void collision_shape_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( g_ph_allocator, resource );
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
