#include "pch.h"
// claude@NOTE: legacy-harvest disposition: the remaining world:: stubs are absent from the legacy twin (new-in-target subsystems: clouds, tracers, environment probes, sky AO, ambient volumes, grass layers, LOD, portal system, volume fog, streaming; renderer_cook is new too); the handful with nuances carry their own notes - matcher-phase work.
#include <vostok/render/engine/world.h>
#include <vostok/render/facade/vertex_input_type.h>
#include "renderer_cook_renderer_resource.h"
#include "speedtree_tree.h"
#include <vostok/render/core/api.h>
#include <vostok/render/core/options.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/device.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/shader_macros.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/dx11/effect_constant_storage.h>
#include <vostok/render/core/untyped_buffer.h>
#include <vostok/render/facade/render_stage_types.h>
#include <vostok/render/facade/material_effects_instance_cook_data.h>
#include <vostok/render/engine/api.h>
#include <vostok/resources.h>
#include <vostok/console_command_processor.h>
#include <vostok/particle/api.h>
#include <vostok/particle/world.h>
#include "system_renderer.h"
#include "scene_manager.h"
#include "renderer_context.h"
#include "material_manager.h"
#include "stage_particles.h"
#include "decal_instance.h"
#include "renderer.h"
#include "register_samplers.h"
#include "scene.h"
#include "clouds.h"
#include "scene_view.h"
#include "render_output_window.h"
#include "speedtree.h"
#include "speedtree_instance.h"
#include "material.h"
#include "material_effects_instance.h"
#include "render_model_instance_impl.h"
#include "render_surface_instance.h"
#include "render_model_user.h"
#include "render_model_skeleton.h"
#include "model_cooker.h"
#include "render_model_cooker.h"
#include "combined_model_cooker.h"
#include "grass_cook.h"
#include "tracer_model_instance_cook.h"
#include "portal_sector_structure_cook.h"
#include "animated_model_instance_cook.h"
#include "material_cook.h"
#include "material_effects_instance_cook.h"
#include "scene_cook.h"
#include "scene_view_cook.h"
#include "render_output_window_cook.h"
#include "grass_world.h"

#include <GFx.h>

namespace vostok {
namespace render {

void register_texture_cook	( );
void unregister_texture_cook( );

struct singletons_on_preinitialize {
	// resource_manager/device ctors grew args since legacy (config, is_editor);
	// forwarding ctor is a buildability adaptation - matcher confirms wiring
	singletons_on_preinitialize( vostok::configs::binary_config_ptr const& config, bool is_editor ) :
		resource_manager( config ),
		device( is_editor )
	{
	}

	vostok::render::resource_manager		resource_manager;
	vostok::render::device					device;
	vostok::render::backend					backend;
	vostok::render::scene_manager			scene_manager;
	vostok::render::shader_macros			shader_macros;
	vostok::render::effect_manager			effect_manager;
	vostok::render::effect_constant_storage	effect_constant_storage;
}; // struct singletons_on_preinitialize

struct singletons_on_initialize {
	singletons_on_initialize( );

	// legacy 'environment' member dropped: no environment class in the new tree
	vostok::render::renderer_context						renderer_context;
	vostok::render::material_manager						material_manager;
	vostok::render::particle_shader_constants				particle_shader_constants;
	vostok::render::decal_shader_constants_and_geometry	decal_shader_constants_and_geometry;
}; // struct singletons_on_initialize

static uninitialized_reference< options >						s_options;

static uninitialized_reference< singletons_on_preinitialize >	s_singletons_on_preinitialize;
static uninitialized_reference< singletons_on_initialize >		s_singletons_on_initialize;
static uninitialized_reference< system_renderer >				s_system_renderer;

untyped_buffer_ptr		g_quad_ib;

static command_line::key	s_no_level( "no_level", "", "", "" );

renderer_cook::renderer_cook( ) :
 resources::unmanaged_cook( resources::renderer_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );
}

// claude@NOTE: inline-vs-call wall - the target keeps mutable_buffer's
// (pvoid,u32) ctor out-of-line (ICF-folded, delinked as boost::_bi::storage2)
// and pushes the size before the malloc; our build inlines it into two stores.
// Same wall on effect_cook::allocate_resource, so it is codebase-wide.
mutable_buffer renderer_cook::allocate_resource(
	resources::query_result_for_cook&		in_query,
	const_buffer							raw_file_data,
	bool									file_exist
)
{
	VOSTOK_UNREFERENCED_PARAMETERS			(&file_exist, &raw_file_data, &in_query);
	return									mutable_buffer((pvoid)MALLOC(sizeof(renderer_resource),"renderer_resource"), sizeof(renderer_resource));
}

void renderer_cook::deallocate_resource( void* buffer )
{
	FREE									(buffer);
}

// claude@NOTE: residual is one inline-vs-call - the target inlines
// query_result_for_cook::user_data() to `mov esi,[edi+14Ch]`, our link keeps it
// as a call, which then costs the aligned frame (push ebp/and esp,-8) the target
// does not have and a duplicated `engine_world = NULL` store.
void renderer_cook::create_resource(
	resources::query_result_for_cook&		in_out_query,
	const_buffer							raw_file_data,
	mutable_buffer							in_out_unmanaged_resource_buffer
)
{
	VOSTOK_UNREFERENCED_PARAMETER			(raw_file_data);

	engine::world* engine_world			=	NULL;
	in_out_query.user_data()->try_get		(engine_world);
	engine_world->reset_renderer			(true);

	renderer_resource* new_resource		=	new(in_out_unmanaged_resource_buffer.c_ptr())renderer_resource;

	in_out_query.set_unmanaged_resource		(new_resource, resources::nocache_memory, sizeof(renderer_resource));
	in_out_query.finish_query				(result_success);
}

static void register_cooks( )
{
	using resources::register_cook;

	static renderer_cook				renderer_cooker;
	static user_mesh_cook				user_mesh_cooker;
	static static_model_instance_cook	static_model_instance_cooker;
	static skeleton_model_instance_cook	skeleton_model_instance_cooker;
	static tracer_model_instance_cook	tracer_model_instance_cooker;
	static render_model_cook			skeleton_mesh_instance_cooker( resources::skeleton_render_model_class );
	static render_model_cook			render_model_class_cooker( resources::static_render_model_class );
	static grass_render_model_cook		grass_render_model_class_cooker;
	static static_render_model_instance_cook static_render_model_instance_cooker;
	static skeleton_render_model_instance_cook skeleton_render_model_instance_cooker;
	static skeleton_combined_model_cook	skeleton_combined_model_cooker;
	static skeleton_combined_render_model_instance_cook skeleton_combined_render_model_instance_cooker;
	static skeleton_combined_model_instance_cook skeleton_combined_model_instance_cooker;
	static grass_cook					grass_cooker;

	static material_cook				material_cook;
	register_cook						( &material_cook );

	static material_effects_instance_cook material_effects_instance_cooker;
	register_cook						( &material_effects_instance_cooker );

	register_texture_cook				( );

	particle::initialize				( );

	static scene_cook					scene_cook;
	register_cook						( &scene_cook );

	static scene_view_cook				scene_view_cook;
	register_cook						( &scene_view_cook );

	static render_output_window_cook	render_output_window_cook;
	register_cook						( &render_output_window_cook );

	static animated_model_instance_cook	animated_model_cook;
	register_cook						( &animated_model_cook );

	static culling::portal_sector_structure_cook s_portal_system_cook;
	register_cook						( &s_portal_system_cook );
}

static void unregister_cooks( )
{
	vostok::particle::finalize				( );
	vostok::render::unregister_texture_cook	( );
}

void initialize_options( )
{
	// <0>
	// ******

	using namespace vostok::render;
}

 singletons_on_initialize::singletons_on_initialize( )
{
	// <0>
	// ******
}

// Modified helper function from DirectX SDK
static HRESULT get_dx_version_via_dxdiag( unsigned long& major_version, unsigned long& minor_version )
{
	// LOCALS
	// IDxDiagProvider* 				dxdiag_provider
	// bool 							does_dx_version_obtained
	// bool 							should_cleanup_COM
	// bool 							does_minor_version_obtained
	// _DXDIAG_INIT_PARAMS 				dxDiagInitParam
	// IDxDiagContainer* 				pDxDiagRoot
	// IDxDiagContainer* 				pDxDiagSystemInfo
	// tagVARIANT 						var
	// ******

	// CALL SITE INFO
	// <0x653efe> -> HRESULT < unknown >( void* )
	// <0x653f25> -> HRESULT < unknown >( _GUID const&, IUnknown*, unsigned long, _GUID const&, void** )
	// <0x653f6a> -> HRESULT < unknown >( _DXDIAG_INIT_PARAMS* )
	// <0x653f8b> -> HRESULT < unknown >( IDxDiagContainer** )
	// <0x653fa9> -> HRESULT < unknown >( wchar_t const*, IDxDiagContainer** )
	// <0x653fb8> -> void < unknown >( tagVARIANT* )
	// <0x653fd2> -> HRESULT < unknown >( wchar_t const*, tagVARIANT* )
	// <0x653ff7> -> HRESULT < unknown >( tagVARIANT* )
	// <0x65400d> -> HRESULT < unknown >( wchar_t const*, tagVARIANT* )
	// <0x65402f> -> HRESULT < unknown >( tagVARIANT* )
	// <0x65404b> -> unsigned long < unknown >()
	// <0x654057> -> unsigned long < unknown >()
	// <0x654063> -> unsigned long < unknown >()
	// <0x65406c> -> void < unknown >()
	// ******

	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x653ef5>|0x005|+0x00f:'273'
	// <0x653f04>|0x014|+0x002:'274'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x653f06>|0x016|+0x025:'283'
	// <0x653f2b>|0x03b|+0x008:'284'
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
	// <0x653f33>|0x043|+0x039:'295'
	// <0x653f6c>|0x07c|+0x008:'296'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x653f74>|0x084|+0x019:'302'
	// <0x653f8d>|0x09d|+0x008:'303'
	// <0>
	// <1>
	// <0x653f95>|0x0a5|+0x016:'306'
	// <0x653fab>|0x0bb|+0x008:'307'
	// <0>
	// <1>
	// <0x653fb3>|0x0c3|+0x00b:'310'
	// <0>
	// <1>
	// <0x653fbe>|0x0ce|+0x016:'313'
	// <0x653fd4>|0x0e4|+0x00c:'314'
	// <0>
	// <0x653fe0>|0x0f0|+0x00a:'316'
	// <0x653fea>|0x0fa|+0x002:'317'
	// <0>
	// <0x653fec>|0x0fc|+0x00d:'319'
	// <0>
	// <1>
	// <0x653ff9>|0x109|+0x016:'322'
	// <0x65400f>|0x11f|+0x00c:'323'
	// <0>
	// <0x65401b>|0x12b|+0x00a:'325'
	// <0x654025>|0x135|+0x005:'326'
	// <0>
	// <0x65402a>|0x13a|+0x007:'328'
	// <0>
	// <1>
	// <0x654031>|0x141|+0x00b:'331'
	// <0x65403c>|0x14c|+0x005:'332'
	// <0>
	// <0x654041>|0x151|+0x00c:'334'
	// <0>
	// <1>
	// <0x65404d>|0x15d|+0x00c:'337'
	// <0>
	// <1>
	// <2>
	// <0x654059>|0x169|+0x00c:'341'
	// <0>
	// <1>
	// <0x654065>|0x175|+0x007:'344'
	// <0x65406c>|0x17c|+0x006:'345'
	// <0>
	// <0x654072>|0x182|+0x006:'347'
	// <0>
	// <1>
	// <2>
	// ******

    HRESULT result;
    bool should_cleanup_COM = false;

    bool does_major_version_obtained = false;
    bool does_minor_version_obtained = false;

    // Init COM.  COM may fail if its already been inited with a different
    // concurrency model.  And if it fails you shouldn't release it.
    result = CoInitialize( NULL );
    should_cleanup_COM = SUCCEEDED( result );

    // Get an IDxDiagProvider
    bool does_dx_version_obtained = false;
    IDxDiagProvider* dxdiag_provider = NULL;
    result = CoCreateInstance( CLSID_DxDiagProvider,
                           NULL,
                           CLSCTX_INPROC_SERVER,
                           IID_IDxDiagProvider,
                           ( LPVOID* )&dxdiag_provider );
    if( SUCCEEDED( result ) )
    {
        // Fill out a DXDIAG_INIT_PARAMS struct
        DXDIAG_INIT_PARAMS dxDiagInitParam;
        ZeroMemory( &dxDiagInitParam, sizeof( DXDIAG_INIT_PARAMS ) );
        dxDiagInitParam.dwSize = sizeof( DXDIAG_INIT_PARAMS );
        dxDiagInitParam.dwDxDiagHeaderVersion = DXDIAG_DX9_SDK_VERSION;
        dxDiagInitParam.bAllowWHQLChecks = false;
        dxDiagInitParam.pReserved = NULL;

        // Init the m_pDxDiagProvider
        result = dxdiag_provider->Initialize( &dxDiagInitParam );
        if( SUCCEEDED( result ) )
        {
            IDxDiagContainer* pDxDiagRoot = NULL;
            IDxDiagContainer* pDxDiagSystemInfo = NULL;

            // Get the DxDiag root container
            result = dxdiag_provider->GetRootContainer( &pDxDiagRoot );
            if( SUCCEEDED( result ) )
            {
                // Get the object called DxDiag_SystemInfo
                result = pDxDiagRoot->GetChildContainer( L"DxDiag_SystemInfo", &pDxDiagSystemInfo );
                if( SUCCEEDED( result ) )
                {
                    VARIANT var;
                    VariantInit( &var );

                    // Get the "dwDirectXVersionMajor" property
                    result = pDxDiagSystemInfo->GetProp( L"dwDirectXVersionMajor", &var );
                    if( SUCCEEDED( result ) && var.vt == VT_UI4 )
                    {
						major_version = var.ulVal;
                        does_major_version_obtained = true;
                    }
                    VariantClear( &var );

                    // Get the "dwDirectXVersionMinor" property
                    result = pDxDiagSystemInfo->GetProp( L"dwDirectXVersionMinor", &var );
                    if( SUCCEEDED( result ) && var.vt == VT_UI4 )
                    {
						minor_version = var.ulVal;
                        does_minor_version_obtained = true;
                    }
                    VariantClear( &var );

                    // If it all worked right, then mark it down
                    if( does_major_version_obtained && does_minor_version_obtained )
                        does_dx_version_obtained = true;

                    pDxDiagSystemInfo->Release();
                }

                pDxDiagRoot->Release();
            }
        }

        dxdiag_provider->Release();
    }

    if( should_cleanup_COM )
        CoUninitialize();

    if( does_dx_version_obtained )
        return S_OK;
    else
        return E_FAIL;
}

static bool does_os_support_dx11( )
{
	// LOCALS
	// _OSVERSIONINFOEXA 				OsVersionInfo
	// unsigned long 					major_version
	// unsigned long 					minor_version
	// ******

	// CALL SITE INFO
	// <0x6542f7> -> int < unknown >( _OSVERSIONINFOA* )
	// ******

	// <0>
	// <1>
	// <0x6542d6>|0x006|+0x014:'358'
	// <0>
	// <1>
	// <0x6542ea>|0x01a|+0x015:'361'
	// <0x6542ff>|0x02f|+0x002:'362'
	// <0>
	// <0x654301>|0x031|+0x007:'364'
	// <0x654308>|0x038|+0x002:'365'
	// <0>
	// <0x65430a>|0x03a|+0x002:'367'
	// <0>
	// <0x65430c>|0x03c|+0x005:'369'
	// <0x654311>|0x041|+0x002:'370'
	// <0>
	// <0x654313>|0x043|+0x00b:'372'
	// <0>
	// <1>
	// <2>
	// <0x65431e>|0x04e|+0x016:'376'
	// <0>
	// <0x654334>|0x064|+0x007:'378'
	// <0x65433b>|0x06b|+0x009:'379'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x654344>|0x074|-0x007:'385'
	// <0x65433d>|0x06d|+0x009:'386'
	// ******

	OSVERSIONINFOEX OsVersionInfo;

	ZeroMemory( &OsVersionInfo , sizeof( OSVERSIONINFOEX ) );
	OsVersionInfo.dwOSVersionInfoSize = sizeof( OSVERSIONINFOEX );

	if ( GetVersionEx( (LPOSVERSIONINFO) &OsVersionInfo ) == 0 )
		return true; // failed to get OS version information for some reason

	if ( OsVersionInfo.dwMajorVersion > 6 )
		return true; // unknown OS greater than Win7

	if ( OsVersionInfo.dwMajorVersion == 6 )
	{
		 if ( OsVersionInfo.dwMinorVersion >= 1 )
			 return true; // Win7 + optional update
		 // Vista here
		 if ( OsVersionInfo.wServicePackMajor >= 2 )
		 {
			// SP2 or greater, check for DX11 ( KB971512 + KB971644 )
			DWORD major_version, minor_version;
			if ( SUCCEEDED( get_dx_version_via_dxdiag( major_version, minor_version ) ) )
			{
				if ( major_version >= 10 )
					return true;
			}
			else
				return true; // failed to get DX version for some reason
		 }
	}
	return false;
}

void engine::world::apply_render_options_changes( )
{
	// <0>
	// <1>
	// <0x656e10>|0x000|+0x007:'392'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******

	reset_renderer								( false );
#ifndef MASTER_GOLD
	options::ref().save_current_configuration	( );
#endif // #ifndef MASTER_GOLD
}

static void on_material_loaded( resources::queries_result& data, long volatile* waiting_for )
{
	static u32 num_materials_processed = 0;

	u32 const pending_queries_count = resources::pending_queries_count();
	LOG_ERROR					( "pending qc:%d", pending_queries_count );
	num_materials_processed			+= data.size();
	LOG_ERROR					( "num nmtls:%d", num_materials_processed );

	if ( waiting_for )
		threading::interlocked_exchange( *waiting_for, 0 );
}

static void on_fs_iterator_materials_ready_children(
	vector< fs_new::virtual_path_string >&	out_material_names,
	pcstr						materials_path,
	vfs::vfs_iterator const&	fs_it
)
{
	vfs::vfs_iterator it = fs_it;
	for ( ; it; ++it )
	{
		pcstr mname = it.get_name();

		if ( it.is_folder() )
		{
			fs_new::virtual_path_string new_materials_path;
			new_materials_path.assignf( "%s/%s", materials_path, mname );
			on_fs_iterator_materials_ready_children(
				out_material_names,
				new_materials_path.c_str(),
				it.children_begin()
			);
		}
		else if ( strstr( mname, ".orig" ) )
		{
			continue;
		}
		else
		{
			fs_new::virtual_path_string request_path;
			request_path.assignf( "%s/%s", materials_path, mname );
			request_path.rtrim();
			request_path.replace( "resources/material_instances/", "" );
			// 49 target lines are retail-compiled-out source.
			out_material_names.push_back( fs_new::virtual_path_string( request_path.c_str() ) );
		}
	}
}

static void query_materials_and_wait( vector< fs_new::virtual_path_string > const& in_material_names )
{
	fs_new::virtual_path_string const* name_it = in_material_names.begin();
	fs_new::virtual_path_string const* name_end = in_material_names.end();
	u32 const num_resuests = in_material_names.size() * num_vertex_input_types;

	resources::request* requests = (resources::request*)MALLOC( sizeof( resources::request ) * num_resuests, "" );
	resources::user_data_variant* user_data_variants = (resources::user_data_variant*)MALLOC( sizeof( resources::user_data_variant ) * num_resuests, "" );
	resources::user_data_variant** user_data_variants_ptrs = (resources::user_data_variant**)MALLOC( sizeof( resources::user_data_variant* ) * num_resuests, "" );

	u32 request_index = 0;
	for ( ; name_it != name_end; ++name_it )
	{
		for ( u32 vi_type = 0; vi_type < num_vertex_input_types; ++vi_type, ++request_index )
		{
			requests[request_index].set( name_it->c_str(), resources::material_effects_instance_class );

			user_data_variants_ptrs[request_index] = new(
				&user_data_variants[request_index]
			) resources::user_data_variant;
			user_data_variants_ptrs[request_index]->set(
				NEW( material_effects_instance_cook_data )(
					(enum_vertex_input_type)vi_type,
					0
				)
			);
		}
	}

	volatile long waiting_for = 1;
	resources::query_resources(
		requests,
		num_resuests,
		boost::bind( &on_material_loaded, _1, &waiting_for ),
		g_allocator,
		(resources::user_data_variant const**)user_data_variants_ptrs,
		NULL,
		assert_on_fail_true
	);

	FREE( user_data_variants_ptrs );
	FREE( user_data_variants );
	FREE( requests );

	while ( waiting_for )
	{
		resources::dispatch_callbacks();
		threading::yield( 1 );
	}
}

static void on_fs_iterator_materials_ready(
	pcstr								materials_path,
	vfs::vfs_locked_iterator const&		fs_it,
	long volatile*						waiting_for
)
{
	if ( !fs_it.get_children_count() || !fs_it )
		return;

	vector< fs_new::virtual_path_string > out_material_names;
	vfs::vfs_iterator it = fs_it.children_begin();
	vfs::vfs_iterator end = fs_it.children_end();
	for ( ; it != end; ++it )
	{
		pcstr mname = it.get_name();
		if ( it.is_folder() )
		{
			fs_new::virtual_path_string new_materials_path;
			new_materials_path.assignf( "%s/%s", materials_path, mname );
			on_fs_iterator_materials_ready_children(
				out_material_names,
				new_materials_path.c_str(),
				it.children_begin()
			);
		}
		else
		{
			on_fs_iterator_materials_ready_children(
				out_material_names,
				materials_path,
				it
			);
		}
	}

	vector< fs_new::virtual_path_string > query_names;
	fs_new::virtual_path_string const* name_end = out_material_names.end();
	for (
		fs_new::virtual_path_string const* name_it = out_material_names.begin();
		name_it != name_end;
		++name_it
	)
	{
		fs_new::virtual_path_string name = *name_it;
		if (
			name.length() < strings::length( "post_process/" ) ||
			!strstr( name.c_str(), "post_process/" )
		)
			query_names.push_back( name );

		if ( query_names.size() == 50 )
		{
			query_materials_and_wait( query_names );
			query_names.clear();
		}
	}

	if ( query_names.size() )
		query_materials_and_wait( query_names );

	if ( waiting_for )
		threading::interlocked_exchange( *waiting_for, 0 );

	LOG_ERROR( "pending qc:%d", resources::pending_queries_count() );
}

 engine::world::world( configs::binary_config_ptr const& in_config, bool is_editor ) :
	m_frame_id						( 0 ),
	m_initialized					( false ),
	m_enable_terrain_debug_mode		( false ),
	m_renderer						( NULL )
{
	CHECK_OR_EXIT					(
		does_os_support_dx11(),
		"Your operating system doesn't support DirectX 11.\r\n"
		"Please upgrade your OS to Windows Vista Service Pack 2 + Platform Update or later."
	);

	VOSTOK_CONSTRUCT_REFERENCE		( s_options, options )( );

	initialize_speedtree			( );

	register_cooks					( );

	VOSTOK_CONSTRUCT_REFERENCE		( s_singletons_on_preinitialize, singletons_on_preinitialize )( in_config, is_editor );
}

void engine::world::on_renderer_configuration_config_loaded( bool async_effects, resources::queries_result& data )
{
	// <0x656d43>|0x003|+0x017:'787'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x656d5a>|0x01a|+0x083:'793'
	// <0>
	// <0x656ddd>|0x09d|+0x00e:'795'
	// <0>
	// <0x656deb>|0x0ab|+0x01a:'797'
	// <0>
	// ******

	if (data.is_successful())
	{
		options::ref().load_from_config(
			vostok::static_cast_resource_ptr<vostok::configs::binary_config_ptr>(
				data[0].get_unmanaged_resource()
			)->get_root()["options"]
		);
	}
	reset_renderer					( async_effects );
	console_commands::save			( "user.cfg", console_commands::command_type_user_specific, memory::g_mt_allocator );
}

// claude@NOTE: LOG_INFO embeds __LINE__, so the two log sites push our physical
// line numbers (0x43E/0x...) where the target pushes 0x323/0x32E - the original
// .cpp had these statements at lines 803 and 814. That residual only closes when
// this file's line numbering matches the original (the remaining carcass blocks
// above are what shifts it).
void engine::world::reset_renderer( bool async_effects )
{
	LOG_INFO						("Renderer creating started...");

	if (m_renderer)
 		DELETE						(m_renderer);

 	effect_manager::ref().force_sync = !async_effects;

 	m_renderer						 = NEW( render::renderer )( &s_singletons_on_initialize->renderer_context );

 	effect_manager::ref().force_sync = false;

	LOG_INFO						("Renderer creating finished.");
}

void engine::world::initialize( bool is_editor )
{
	VOSTOK_UNREFERENCED_PARAMETER	( is_editor );

	// model cooker uses material_manager
	VOSTOK_CONSTRUCT_REFERENCE		( s_singletons_on_initialize, singletons_on_initialize )( );

	if ( !s_no_level )
		while ( !s_singletons_on_preinitialize->scene_manager.scene_count() ) {
			resources::dispatch_callbacks	( );
			threading::yield				( 10 );
		}

	R_ASSERT						( !m_initialized );
	m_initialized					= true;

	VOSTOK_CONSTRUCT_REFERENCE		( s_system_renderer, system_renderer ) ( &s_singletons_on_initialize->renderer_context );
}

 engine::world::~world( )
{
	material::finalize_nomaterial_material	( );
	finalize_speedtree						( );

	VOSTOK_DESTROY_REFERENCE		( s_system_renderer );
	DELETE							( m_renderer );
	VOSTOK_DESTROY_REFERENCE		( s_singletons_on_initialize );
	VOSTOK_DESTROY_REFERENCE		( s_singletons_on_preinitialize );
	VOSTOK_DESTROY_REFERENCE		( s_options );

	unregister_cooks				( );

	particle::finalize				( );
}

void engine::world::reload_shaders( )
{
	resource_manager::ref().reload_shader_sources	( true );
}

void engine::world::reload_modified_textures( )
{
	// <0x655d40>|0x000|+0x00b:'892'
	// ******

	resource_manager::ref().reload_modified_textures( );
}

void engine::world::build_lpv_geometry( base_scene_ptr const& scene )
{
	static_cast_checked<vostok::render::scene*>(scene.c_ptr())->build_lpv_geometry	( );
}

void engine::world::remove_unused_environment_cubemaps( base_scene_ptr const& scene )
{
	static_cast_checked<vostok::render::scene*>(scene.c_ptr())->remove_unused_environment_cubemaps( );
}

void engine::world::clear_resources( )
{
	m_renderer->m_renderer_context->clear_resources	( );
}

void engine::world::begin_render_options_changing( long volatile* waiting_for )
{
	options::ref().previous = options::ref().current;
	if ( waiting_for )
		threading::interlocked_exchange( *waiting_for, 0 );
}

void engine::world::end_render_options_changing(
	base_scene_ptr const&		scene,
	base_output_window_ptr		output_window,
	bool						reload_all_materials,
	bool						shaders_recompile,
	long volatile*				waiting_for
)
{
	vector<fs_new::virtual_path_string> changed_defines;
	enum_options_changes_result const changes = options::ref().end_render_options_changing( changed_defines );

	if ( !reload_all_materials )
	{
		if ( shaders_recompile && changes != ocr_need_nothing )
			effect_manager::ref().recompile_shaders_async( changed_defines );

		if ( (changes & ocr_need_reset_renderer) || (changes & ocr_need_reload_shaders) )
			reset_renderer( true );
		else if ( changes & (ocr_need_reset_postprocess | ocr_need_reset_lighting | ocr_need_reset_rain) )
		{
			m_renderer->recreate_stage( post_process_render_stage );
			m_renderer->recreate_stage( lighting_render_stage );
			m_renderer->recreate_stage( light_propagation_volumes_render_stage );
			m_renderer->recreate_stage( deferred_lighting_render_stage );
			m_renderer->recreate_stage( sun_shadows_accumulate_render_stage );
			m_renderer->recreate_stage( sun_render_stage );
			m_renderer->recreate_stage( translucency_render_stage );
			m_renderer->recreate_stage( resolve_lighting_render_stage );
			m_renderer->recreate_stage( ambient_lighting_render_stage );
			m_renderer->recreate_stage( ambient_occlusion_render_stage );
			m_renderer->recreate_stage( pre_rain_normal_modify_render_stage );
			m_renderer->recreate_stage( rain_render_stage );
		}

		if ( (changes & ocr_need_recreate_samplers) && !(changes & ocr_need_reset_renderer) )
			register_samplers( );

		if ( changes & ocr_need_reload_textures )
			resource_manager::ref().reload_all_textures( );

		if ( changes & ocr_need_resize_window )
		{
			render_output_window* const window = static_cast_checked<render_output_window*>( output_window.c_ptr( ) );
			window->set_size(
				options::ref().current.m_resolution_x,
				options::ref().current.m_resolution_y,
				options::ref().current.m_fullscreen,
				true
			);
			m_renderer->m_renderer_context->set_target_context( &window->target_context( ), true );
		}

		if ( waiting_for )
			threading::interlocked_exchange( *waiting_for, 0 );
	}
	else if ( changes != ocr_need_nothing )
	{
		resources::query_vfs_iterator(
			fs_new::virtual_path_string( "resources.sources/material_instances" ),
			boost::bind( &on_fs_iterator_materials_ready, "resources/material_instances", _1, waiting_for ),
			&memory::g_mt_allocator,
			resources::recursive_true
		);
	}
}

void engine::world::set_view_matrix( base_scene_view_ptr const& scene_view, float4x4 const& view_and_culling_matrix )
{
	// <0x654b01>|0x001|+0x016:'1004'
	// ******

	static_cast_checked< render::scene_view* >( scene_view.c_ptr() )->camera_set_view		( view_and_culling_matrix );
}

void engine::world::set_projection_matrix( base_scene_view_ptr const& scene_view, float4x4 const& projection_matrix )
{
	// <0x654490>|0x000|+0x01b:'1016'
	// ******

	static_cast_checked< render::scene_view* >(scene_view.c_ptr())->camera_set_projection	( projection_matrix );
}

// claude@NOTE: LTCG drops the two constant bool arguments to scene::flush in the base call site.
void engine::world::draw_scene(
	base_scene_ptr const&				scene,
	base_scene_view_ptr const&			view,
	base_output_window_ptr const&		output_window,
	math::rectangle< float2 > const&	viewport,
	boost::function< void( bool ) > const&	on_draw_scene,
	vostok::ui::font const*				default_font
)
{
	float4x4 identity;
	identity.identity			( );
	system_renderer::ref().set_w( identity );

	if (m_renderer)
		m_renderer->render	( scene, view, output_window, viewport, on_draw_scene, m_enable_terrain_debug_mode, default_font );
	else
		static_cast_checked<vostok::render::scene*>(scene.c_ptr())->flush	( on_draw_scene, true, true );

}

void engine::world::end_frame( )
{
	// <0x653ee0>|0x000|+0x003:'1111'
	// ******

	++m_frame_id;
}

u32 engine::world::frame_id( )
{
	// <0x653ed0>|0x000|+0x003:'1116'
	// ******

	return				( m_frame_id);
}

void engine::world::draw_ui_vertices(
	ui::vertex const*		vertices,
	u32 const&				count,
	s32						prim_type,
	s32						point_type
)
{
	// <0x655c10>|0x000|+0x018:'1132'
	// ******

	render::system_renderer::ref().draw_ui_vertices( (vertex_formats::TL const*)vertices, count, prim_type, point_type );
}

void engine::world::add_speedtree_instance(
	base_scene_ptr const&				in_scene,
	speedtree_instance_ptr const&		v,
	float4x4 const&						transform,
	bool								populate_forest
)
{
	// <0>
	// <1>
	// <0x655bd0>|0x000|+0x039:'1227'
	// ******

	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	scene->add_speedtree_instance(v, transform, populate_forest);
}

void engine::world::remove_speedtree_instance(
	base_scene_ptr const&				in_scene,
	speedtree_instance_ptr const&		v,
	bool								populate_forest
)
{
	// <0>
	// <1>
	// <0x655b90>|0x000|+0x034:'1234'
	// ******

	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	scene->remove_speedtree_instance(v, populate_forest);
}

void engine::world::update_speedtree_instance(
	base_scene_ptr const&				in_scene,
	speedtree_instance_ptr const&		v,
	float4x4 const&						transform,
	bool								populate_forest
)
{
	// <0>
	// <1>
	// <0x655b50>|0x000|+0x039:'1241'
	// ******

	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	scene->set_speedtree_instance_transform(v, transform, populate_forest);
}

void engine::world::populate_speedtree_forest( base_scene_ptr const& in_scene )
{
	// <0>
	// <1>
	// ******

	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	scene->populate_speedtree_forest( );
}

void engine::world::add_model(
	base_scene_ptr const&				in_scene,
	render_model_instance_ptr const&	v,
	float4x4 const&						transform,
	bool								apply_transform
)
{
	// CALL SITE INFO
	// <0x655868> -> void < unknown >( float4x4 const& )
	// ******

	// <0x655831>|0x001|+0x004:'1253'
	// <0>
	// <0x655835>|0x005|+0x020:'1255'
	// <0>
	// <0x655855>|0x025|+0x007:'1257'
	// <0x65585c>|0x02c|+0x00e:'1258'
	// <0>
	// <0x65586a>|0x03a|+0x023:'1260'
	// ******

	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	render_model_instance_impl_ptr model = static_cast_resource_ptr<render_model_instance_impl_ptr>(v);

	if (apply_transform)
		model->set_transform( transform );

	scene->add_model( model);
}

void engine::world::update_model(
	base_scene_ptr const&				in_scene,
	render_model_instance_ptr const&	v,
	float4x4 const&						transform
)
{
	// CALL SITE INFO
	// <0x6555f2> -> void < unknown >( float4x4 const& )
	// ******

	// <0x6555c1>|0x001|+0x004:'1265'
	// <0>
	// <0x6555c5>|0x005|+0x021:'1267'
	// <0x6555e6>|0x026|+0x00e:'1268'
	// <0x6555f4>|0x034|+0x021:'1269'
	// ******

	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	render_model_instance_impl_ptr model = static_cast_resource_ptr<render_model_instance_impl_ptr>(v);
	model->set_transform				( transform );
	scene->modify_model					( model );
}

void engine::world::remove_model( base_scene_ptr const& in_scene, render_model_instance_ptr const& v )
{
	// <0x6557c0>|0x000|+0x004:'1274'
	// <0>
	// <0x6557c4>|0x004|+0x01f:'1276'
	// <0x6557e3>|0x023|+0x024:'1277'
	// ******

	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	render_model_instance_impl_ptr model = static_cast_resource_ptr<render_model_instance_impl_ptr>(v);
	scene->remove_model( model);
}

enum_vertex_input_type speedtree_subsurface_name_to_vertex_input_type( fs_new::virtual_path_string const& subsurface_name )
{
	if ( subsurface_name == "branch" ||
		 subsurface_name == "frond" ||
		 subsurface_name == "leafmesh" ||
		 subsurface_name == "leafcard" ||
		 subsurface_name == "billboard" )
		return unknown_vertex_input_type;

	return null_vertex_input_type;
}

static speedtree_tree::component_type vertex_input_type_to_speedtree_component_type( enum_vertex_input_type type )
{
	return speedtree_tree::branch;
}

static void on_speedtree_material_effects_instance_ready(
	resources::queries_result&		data,
	speedtree_tree*					tree,
	enum_vertex_input_type			vertex_input_type
)
{
	if (data[0].is_successful())
		tree->set_material_effects(
			vostok::static_cast_resource_ptr<material_effects_instance_ptr>(data[0].get_unmanaged_resource()),
			vertex_input_type_to_speedtree_component_type(vertex_input_type),
			data[0].get_requested_path()
		);
}

void engine::world::set_speedtree_instance_material(
	speedtree_instance_ptr const&			v,
	fs_new::virtual_path_string const&		subsurface_name,
	resources::unmanaged_resource_ptr		in_mtl_ptr
)
{
	// LOCALS
	// speedtree_tree* 					tree
	// variant< 32 > 					data_variant
	// ******

	speedtree_tree*		tree				= static_cast_checked<speedtree_tree*>(v->m_speedtree_tree_ptr.c_ptr());
	material_ptr		mtl_ptr				= static_cast_resource_ptr<material_ptr>(in_mtl_ptr);

	if (!mtl_ptr)
		return;

	resources::user_data_variant			data_variant;

	data_variant.set						(NEW(material_effects_instance_cook_data)(
		speedtree_subsurface_name_to_vertex_input_type(subsurface_name),
		static_cast_resource_ptr<resources::unmanaged_resource_ptr>(mtl_ptr))
	);

	// Must be query_resource_and_wait!
	resources::query_resource_and_wait		(
		mtl_ptr->get_material_name(),
		resources::material_effects_instance_class,
		boost::bind(
			&on_speedtree_material_effects_instance_ready,
			_1,
			tree,
			speedtree_subsurface_name_to_vertex_input_type(subsurface_name)
		),
		vostok::render::g_allocator,
		&data_variant
	);
}

static void on_model_material_effects_instance_ready( resources::queries_result& in_data, render_surface* in_render_surface )
{
	// <0x655723>|0x003|+0x00c:'1349'
	// <0>
	// <1>
	// <0x65572f>|0x00f|+0x016:'1352'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x655745>|0x025|+0x06c:'1357'
	// <0>
	// ******

	if (!in_render_surface)
		return;

 	if (in_data[0].is_successful())
	{
 		in_render_surface->set_material_effects(
 			vostok::static_cast_resource_ptr<material_effects_instance_ptr>(in_data[0].get_unmanaged_resource()),
			in_data[0].get_requested_path()
 		);
	}
}

void engine::world::set_model_material(
	render_model_instance_ptr const&		v,
	fs_new::virtual_path_string const&		subsurface_name,
	resources::unmanaged_resource_ptr		m
)
{
	// LOCALS
	// bool 							check_name
	// render_model_instance_impl_ptr 	model
	// vector< render_surface_instance* > list
	// material_ptr 					mtl_ptr
	// render_surface_instance** 		it
	// variant< 32 > 					data_variant
	// ******

	// CALL SITE INFO
	// <0x656581> -> void < unknown >( float4x4 const*, float3 const*, vector< render_surface_instance* >&, bool, u8, u32 )
	// ******

	// <0x656539>|0x009|+0x024:'1365'
	// <0>
	// <1>
	// <0x65655d>|0x02d|+0x026:'1368'
	// <0>
	// <0x656583>|0x053|+0x008:'1370'
	// <0>
	// <0x65658b>|0x05b|+0x022:'1372'
	// <0>
	// <0x6565ad>|0x07d|+0x017:'1374'
	// <0>
	// <0x6565c4>|0x094|+0x002:'1376'
	// <0>
	// <0x6565c6>|0x096|+0x05c:'1378'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x656622>|0x0f2|+0x006:'1383'
	// <0>
	// <0x656628>|0x0f8|+0x032:'1385'
	// <0x65665a>|0x12a|+0x005:'1386'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x65665f>|0x12f|+0x085:'1394'
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
	// <0x6566e4>|0x1b4|+0x075:'1407'
	// <0x656759>|0x229|+0x02c:'1408'
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
	// ******

	render_model_instance_impl_ptr model = static_cast_resource_ptr<render_model_instance_impl_ptr>(v);

	vector< render_surface_instance* >	list;
	model->get_surfaces				( NULL, NULL, list, false, 0xaa, 3 );

	bool check_name = (subsurface_name.length()!=0);

	material_ptr mtl_ptr = static_cast_resource_ptr<material_ptr>(m);

	for (vector< render_surface_instance* >::iterator it = list.begin(); it != list.end(); ++it)
	{
		render_surface_instance& inst = *(*it);

		if (!check_name || (inst.m_render_surface->m_render_geometry.shading_group_name == subsurface_name.c_str()))
		{
			if (!mtl_ptr)
			{
				inst.m_render_surface->m_materail_effects_instance = 0;
			}
			else
			{
				resources::user_data_variant data_variant;

				data_variant.set						(NEW(material_effects_instance_cook_data)(
					inst.m_render_surface->get_vertex_input_type(),
					static_cast_resource_ptr<resources::unmanaged_resource_ptr>(mtl_ptr))
				);

				resources::query_resource_and_wait		(
					mtl_ptr->get_material_name(),
					resources::material_effects_instance_class,
					boost::bind(
						&on_model_material_effects_instance_ready,
						_1,
						inst.m_render_surface
					),
					vostok::render::g_allocator,
					&data_variant
				);
			}
		}
	}
}

void engine::world::set_model_lod_params(
	render_model_instance_ptr const&	v,
	u8									type,
	bool								use_default,
	float								p0,
	float								p1,
	float								p2
)
{
	render_model_instance_impl_ptr model = static_cast_resource_ptr<render_model_instance_impl_ptr>( v );
	model->set_lod_params( type, use_default, p0, p1, p2 );
}

void engine::world::set_model_visible(
	render_model_instance_ptr const&		v,
	fs_new::virtual_path_string const&		subsurface_name,
	u32										flags
)
{
	render_model_instance_impl_ptr model = static_cast_resource_ptr<render_model_instance_impl_ptr>( v );

	vector< render_surface_instance* > list;
	model->get_surfaces( NULL, NULL, list, false, 0xaa, 3 );

	bool check_name = subsurface_name.length( ) != 0;

	for ( vector< render_surface_instance* >::iterator it = list.begin( ); it != list.end( ); ++it )
	{
		render_surface_instance& inst = **it;

		if ( !check_name || inst.m_render_surface->m_render_geometry.shading_group_name == subsurface_name.c_str( ) )
		{
			inst.m_flags = flags;
		}
	}
}

void engine::world::set_model_visible_by_id( render_model_instance_ptr const& v, u32 subsurface_id, u32 flags )
{
	render_model_instance_impl_ptr model = static_cast_resource_ptr<render_model_instance_impl_ptr>( v );

	vector< render_surface_instance* > list;
	model->get_surfaces( NULL, NULL, list, false, 0xaa, 3 );

	if ( subsurface_id >= list.size( ) )
	{
		LOG_ERROR( "There is no surface with id[%d]!", subsurface_id );
		return;
	}

	list[subsurface_id]->m_flags = flags;
}

void engine::world::set_model_ghost_mode( render_model_instance_ptr const& v, bool value )
{
	// <0>
	// <1>
	// <2>
	// <3>
	// ******

	VOSTOK_UNREFERENCED_PARAMETERS( v, value );
	//render_model_instance_impl_ptr model = static_cast_resource_ptr<render_model_instance_impl_ptr>(v);
	//polymorph_vector_base<render_model_instance> const* render_models = model->get_models_();
	//system_renderer::ref().set_model_ghost_mode( *render_models, value);
}

void engine::world::update_model_vertex_buffer( render_model_instance_ptr const& v, vectora< buffer_fragment > const& fragments )
{
	// CALL SITE INFO
	// <0x654438> -> HRESULT < unknown >( ID3D11Resource*, u32, D3D11_MAP, u32, D3D11_MAPPED_SUBRESOURCE* )
	// <0x654481> -> void < unknown >( ID3D11Resource*, u32 )
	// ******

	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x654400>|0x000|+0x03a:'1510'
	// <0>
	// <1>
	// <0x65443a>|0x03a|+0x002:'1513'
	// <0x65443c>|0x03c|+0x007:'1514'
	// <0>
	// <0x654443>|0x043|+0x004:'1516'
	// <0x654447>|0x047|+0x01c:'1517'
	// <0>
	// <0x654463>|0x063|+0x024:'1519'
	// ******

	user_render_model_instance* model = static_cast_checked<user_render_model_instance*>(v.c_ptr());

	ASSERT( fragments.size() > 0 );

	user_render_surface_editable* surface = static_cast_checked<user_render_surface_editable*>(model->m_surface);
	u8* lock_data	= (u8*)surface->m_vb->map( D3D_MAP_WRITE_DISCARD);

	vectora<buffer_fragment>::const_iterator	it	= fragments.begin();
	vectora<buffer_fragment>::const_iterator	end	= fragments.end();

	for( ; it != end; ++it)
		memory::copy		( lock_data + it->start, it->size, it->buffer, it->size );

	surface->m_vb->unmap	( );
}

void engine::world::add_light( base_scene_ptr const& in_scene, u32 id, light_props* props )
{
	// <0>
	// <1>
	// <0x656e70>|0x000|+0x01b:'1539'
	// ******

	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	scene->add_light( id, props);
}

void engine::world::update_light( base_scene_ptr const& in_scene, u32 id, light_props* props )
{
	// <0>
	// <1>
	// <0x656e23>|0x003|+0x047:'1546'
	// ******

	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	scene->update_light	( id, props);
}

void engine::world::remove_light( base_scene_ptr const& in_scene, u32 id )
{
	// <0>
	// <1>
	// <0x655a11>|0x001|+0x018:'1553'
	// ******

	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	scene->remove_light	( id);
}

void engine::world::add_clouds( base_scene_ptr const& in_scene, cloud_parameters const& parameters )
{
	static_cast_checked<vostok::render::scene*>(in_scene.c_ptr())->add_clouds	( parameters );
}

void engine::world::update_clouds( base_scene_ptr const& in_scene, cloud_parameters const& parameters )
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());
	scene->update_clouds	( parameters );
}

void engine::world::remove_clouds( base_scene_ptr const& in_scene )
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());
	scene->remove_clouds	( );
}

void engine::world::set_num_clouds_keys( base_scene_ptr const& in_scene, const u32 num_keys )
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	if ( scene->has_clouds() )
		scene->get_clouds()->set_num_keys( num_keys );
}

void engine::world::set_clouds_key(
	base_scene_ptr const&			in_scene,
	const u32						index,
	cloud_key_parameters const&		parameters
)
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	if ( scene->has_clouds() )
		scene->get_clouds()->set_key( index, parameters );
}

void engine::world::set_clouds_time( base_scene_ptr const& in_scene, const float time )
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	if ( scene->has_clouds() )
		scene->get_clouds()->set_time( time );
}

void engine::world::set_editor_mode( base_scene_ptr const& in_scene, bool is_editor_mode )
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	if ( scene->has_clouds() )
		scene->get_clouds()->set_editor_mode( is_editor_mode );
}

void engine::world::apply_clouds_changes( base_scene_ptr const& in_scene )
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	if ( scene->has_clouds() )
		scene->get_clouds()->invalidate();
}

void engine::world::add_tracer(
	base_scene_ptr const&				in_scene,
	tracer_model_instance_ptr const&	instance,
	float4x4 const&						initialize_transform
)
{
	static_cast_checked<vostok::render::scene*>(in_scene.c_ptr())->add_tracer	( instance, initialize_transform );
}

void engine::world::update_tracer(
	base_scene_ptr const&				in_scene,
	tracer_model_instance_ptr const&	instance,
	float4x4 const&						new_transform
)
{
	static_cast_checked<vostok::render::scene*>(in_scene.c_ptr())->update_tracer	( instance, new_transform );
}

void engine::world::remove_tracer( base_scene_ptr const& in_scene, tracer_model_instance_ptr const& instance )
{
	static_cast_checked<vostok::render::scene*>(in_scene.c_ptr())->remove_tracer	( instance );
}

void engine::world::add_decal( base_scene_ptr const& in_scene, u32 id, decal_properties const& properties )
{
	// <0>
	// <1>
	// ******

	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	scene->add_decal(id, properties);
}

void engine::world::update_decal( base_scene_ptr const& in_scene, u32 id, decal_properties const& properties )
{
	// <0>
	// <1>
	// <0x6558b0>|0x000|+0x016:'1640'
	// ******

	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	scene->update_decal	( id, properties);
}

void engine::world::remove_decal( base_scene_ptr const& in_scene, u32 id )
{
	// <0>
	// <1>
	// <0x654c60>|0x000|+0x00f:'1647'
	// ******

	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	scene->remove_decal	( id);
}

void engine::world::update_environment_probe(
	base_scene_ptr const&					in_scene,
	u32										id,
	environment_probe_properties const&		properties
)
{
	static_cast_checked<vostok::render::scene*>(in_scene.c_ptr())->update_environment_probe	( id, properties );
}

void engine::world::remove_environment_probe( base_scene_ptr const& in_scene, u32 id )
{
	static_cast_checked<vostok::render::scene*>(in_scene.c_ptr())->remove_environment_probe	( id );
}

void engine::world::update_sky_ambient_occlusion(
	base_scene_ptr const&		in_scene,
	u32							id,
	sky_ambient_occlusion_properties const&	properties
)
{
	static_cast_checked<vostok::render::scene*>(in_scene.c_ptr())->update_sky_ambient_occlusion	( id, properties );
}

void engine::world::remove_sky_ambient_occlusion( base_scene_ptr const& in_scene, u32 id )
{
	static_cast_checked<vostok::render::scene*>(in_scene.c_ptr())->remove_sky_ambient_occlusion	( id );
}

void engine::world::update_ambient_volume(
	base_scene_ptr const&				in_scene,
	u32									id,
	ambient_volume_properties const&	properties
)
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	scene->update_ambient_volume	( id, properties);
}

void engine::world::add_vegetation_trample( base_scene_ptr const& in_scene, trample_desc const& desc )
{
	static_cast_checked<vostok::render::scene*>(in_scene.c_ptr())->add_trample	( desc );
}

void engine::world::remove_ambient_volume( base_scene_ptr const& in_scene, u32 id )
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	scene->remove_ambient_volume	( id);
}

void engine::world::update_lpv_occluder( base_scene_ptr const& in_scene, u32 id, float4x4 const& transform )
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	scene->update_lpv_occluder	( id, transform);
}

void engine::world::remove_lpv_occluder( base_scene_ptr const& in_scene, u32 id )
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	scene->remove_lpv_occluder	( id);
}

void engine::world::add_volume_fog(
	base_scene_ptr const&			in_scene,
	u32								id,
	volume_fog_parameters const&	in_parameters
)
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	scene->add_volume_fog( id, in_parameters );
}

void engine::world::update_volume_fog(
	base_scene_ptr const&			in_scene,
	u32								id,
	volume_fog_parameters const&	in_parameters
)
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	scene->update_volume_fog	( id, in_parameters);
}

void engine::world::remove_volume_fog( base_scene_ptr const& in_scene, u32 id )
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	scene->remove_volume_fog	( id);
}

void engine::world::update_skeleton( render_model_instance_ptr const& v, float4x4* matrices, u32 count )
{
	// <0>
	// <0x6543d0>|0x000|+0x00d:'1725'
	// ******

	skeleton_render_model_instance* skeleton	= static_cast<skeleton_render_model_instance*>(v.c_ptr());
	skeleton->update_render_matrices	( matrices, count );
}

void engine::world::play_particle_system(
	base_scene_ptr const&					in_scene,
	resources::unmanaged_resource_ptr		in_instance,
	bool									use_transform,
	bool									always_looping,
	float4x4 const&							transform
)
{
	// CALL SITE INFO
	// <0x654686> -> void < unknown >( resources::unmanaged_resource_ptr, float4x4 const&, bool, bool )
	// ******

	// <0>
	// <1>
	// <0x654650>|0x000|+0x038:'1767'
	// ******

	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	scene->particle_world()->play(in_instance, transform, use_transform, always_looping);
}

void engine::world::stop_particle_system( base_scene_ptr const& in_scene, resources::unmanaged_resource_ptr in_instance )
{
	// CALL SITE INFO
	// <0x65461f> -> void < unknown >( resources::unmanaged_resource_ptr, float )
	// ******

	// <0>
	// <0x6545f0>|0x000|+0x004:'1773'
	// <0>
	// <0x6545f4>|0x004|+0x02d:'1775'
	// ******

	// TODO: time
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	scene->particle_world()->stop(in_instance, 0.0f);
}

void engine::world::remove_particle_system_instance(
	resources::unmanaged_resource_ptr		particle_system_instance,
	base_scene_ptr const&					in_scene
)
{
	// CALL SITE INFO
	// <0x6545b7> -> void < unknown >( resources::unmanaged_resource_ptr )
	// ******

	// <0x654590>|0x000|+0x007:'1780'
	// <0>
	// <1>
	// <0x654597>|0x007|+0x022:'1783'
	// ******

	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	R_ASSERT						( particle_system_instance );
	scene->particle_world()->remove	( particle_system_instance );
}

void engine::world::update_particle_system_instance(
	resources::unmanaged_resource_ptr		particle_system_instance,
	base_scene_ptr const&					in_scene,
	float4x4 const&							transform,
	bool									visible,
	bool									paused
)
{
	// CALL SITE INFO
	// <0x65451c> -> void < unknown >( resources::unmanaged_resource_ptr, float4x4 const& )
	// <0x65453d> -> void < unknown >( resources::unmanaged_resource_ptr, bool )
	// <0x65455e> -> void < unknown >( resources::unmanaged_resource_ptr, bool )
	// ******

	// <0>
	// <1>
	// <2>
	// <0x6544f0>|0x000|+0x02e:'1791'
	// <0x65451e>|0x02e|+0x021:'1792'
	// <0x65453f>|0x04f|+0x021:'1793'
	// ******

	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());

	particle::world& world			= *scene->particle_world();
	world.set_transform				( particle_system_instance, transform );
	world.set_visible				( particle_system_instance, visible );
	world.set_paused				( particle_system_instance, paused );
}

void engine::world::resize_render_output_window(
	base_output_window_ptr const&		output_window,
	const u32							width,
	const u32							height,
	const bool							fullscreen
)
{
	// <0x656e90>|0x000|+0x01b:'1798'
	// ******

	static_cast_checked< render::render_output_window* >( output_window.c_ptr() )->set_size(
		width,
		height,
		fullscreen,
		false
	);
}

void engine::world::goto_fullscreen( base_output_window_ptr const& output_window )
{
	// <0x654ca0>|0x000|+0x00f:'1803'
	// ******

	static_cast_checked< render::render_output_window* >( output_window.c_ptr() )->goto_fullscreen( );
}

static uninitialized_reference< engine::world > s_world;

engine::world* engine::create_world( configs::binary_config_ptr const& in_config, bool is_editor )
{
	R_ASSERT					( !s_world.initialized() );
	VOSTOK_CONSTRUCT_REFERENCE	( s_world, engine::world ) ( in_config, is_editor );
	return						s_world.c_ptr( );
}

void engine::destroy( engine::world*& engine_world )
{
	// <0>
	// <1>
	// <0x655b10>|0x000|+0x00b:'1926'
	// <0x655b1b>|0x00b|+0x014:'1927'
	// ******

	R_ASSERT					( s_world.initialized() );
	R_ASSERT					( s_world.c_ptr() == engine_world );
	VOSTOK_DESTROY_REFERENCE	( s_world );
	engine_world				= 0;
}

void engine::world::set_slomo( base_scene_ptr const& scene, float time_multiplier )
{
	// <0>
	// <0x654160>|0x000|+0x014:'1933'
	// ******

	render::scene* const scene_ptr	= static_cast_checked< render::scene* >( scene.c_ptr() );
	scene_ptr->set_slomo			( time_multiplier );
}

void engine::world::toggle_render_stage( enum_render_stage_type stage_type, bool toggle )
{
	// <0>
	// <0x654130>|0x000|+0x006:'1939'
	// <0x654136>|0x006|+0x01c:'1940'
	// ******

	if (m_renderer)
		m_renderer->toggle_render_stage(stage_type, toggle);
}

void engine::world::set_view_mode( base_scene_view_ptr view_ptr, scene_view_mode view_mode )
{
	// <0>
	// <0x655100>|0x000|+0x00e:'1946'
	// ******

	scene_view* view = static_cast_checked<scene_view*>(view_ptr.c_ptr());
	view->set_view_mode(view_mode);
}

void engine::world::set_particles_render_mode( base_scene_view_ptr view_ptr, particle::enum_particle_render_mode render_mode )
{
	// <0>
	// <0x6550c0>|0x000|+0x00e:'1953'
	// ******

	scene_view* view = static_cast_checked<scene_view*>(view_ptr.c_ptr());
	view->set_particles_render_mode(render_mode);
}

void engine::world::set_sky_material( base_scene_ptr const& in_scene, resources::unmanaged_resource_ptr mtl_ptr )
{
	vostok::render::scene* scn	= static_cast_checked<vostok::render::scene*>(in_scene.c_ptr());
	scn->set_sky_material		(static_cast_resource_ptr<material_effects_instance_ptr>(mtl_ptr));
}

void engine::world::enable_post_process( base_scene_view_ptr view_ptr, bool enable )
{
	// <0>
	// <0x655080>|0x000|+0x00e:'2065'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******

	scene_view* view = static_cast_checked<scene_view*>(view_ptr.c_ptr());
	view->set_use_post_process(enable);
}

void engine::world::set_post_process(
	base_scene_view_ptr						view_ptr,
	resources::unmanaged_resource_ptr		post_process_resource
)
{
	if (!post_process_resource)
		return;

	scene_view* view = static_cast_checked<scene_view*>(view_ptr.c_ptr());

	material_ptr mtl = static_cast_checked<vostok::render::material*>(post_process_resource.c_ptr());

	vostok::render::material_manager::ref().initialize_post_process_parameters(&view->m_post_process_parameters, mtl, false);
}

particle::world& engine::world::particle_world( base_scene_ptr const& scene )
{
	// <0x6551f1>|0x001|+0x038:'2086'
	// ******

	return	*static_cast_resource_ptr< resources::resource_ptr<vostok::render::scene, resources::unmanaged_resource> >( scene )->particle_world();
}

void engine::world::draw_lines(
	base_scene_ptr const&				scene,
	vectora< vertex_colored > const&	vertices,
	vectora< u16 > const&				indices
)
{
	// <0x655c80>|0x000|+0x040:'2096'
	// ******

	static_cast_resource_ptr< resources::resource_ptr<vostok::render::scene, resources::unmanaged_resource> >( scene )->draw_lines	( vertices, indices );
}

void engine::world::draw_triangles(
	base_scene_ptr const&				scene,
	vectora< vertex_colored > const&	vertices,
	vectora< u16 > const&				indices
)
{
	// <0x655c30>|0x000|+0x040:'2101'
	// ******

	static_cast_resource_ptr< resources::resource_ptr<vostok::render::scene, resources::unmanaged_resource> >( scene )->draw_triangles( vertices, indices );
}

void make_ui_vertices(
	vostok::vectora<vostok::render::ui::vertex>& out_vertices,
	pcstr in_text,
	vostok::ui::font const& in_font,
	vostok::math::float2 const& in_position,
	vostok::math::color const& in_color,
	vostok::math::color const& in_selection_color,
	u32 max_line_width,
	bool is_multiline,
	u32 start_selection_index,
	u32 end_selection_index
);

void engine::world::draw_text(
	vectora< ui::vertex >&		output,
	pcstr const&				text,
	vostok::ui::font const&		font,
	float2 const&				position,
	math::color const&			text_color,
	math::color const&			selection_color,
	const u32					max_line_width,
	const bool					is_multiline,
	const u32					start_selection,
	const u32					end_selection
)
{
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
	// <0x6544b1>|0x001|+0x034:'2141'
	// ******

	make_ui_vertices(
		output,
		text,
		font,
		position,
		text_color,
		selection_color,
		max_line_width,
		is_multiline,
		start_selection,
		end_selection
	);
}

void engine::world::show_movie( base_scene_view_ptr const& scene_view, survarium::flash_movie_resource_ptr movie )
{
	// <0>
	// <0x655651>|0x001|+0x00f:'2146'
	// ******

	static_cast_checked< render::scene_view* >( scene_view.c_ptr( ) )->add_movie( movie );
}

void engine::world::hide_movie( base_scene_view_ptr const& scene_view, survarium::flash_movie_resource_ptr movie )
{
	// <0x655190>|0x000|+0x008:'2151'
	// <0x655198>|0x008|+0x021:'2152'
	// ******

	static_cast_checked< render::scene_view* >( scene_view.c_ptr( ) )->remove_movie( movie );
}

void engine::world::show_text_manager( base_scene_view_ptr const& scene_view, survarium::flash_text_manager* tm )
{
	// <0>
	// <0x654110>|0x000|+0x010:'2158'
	// ******

	static_cast_checked< render::scene_view* >( scene_view.c_ptr( ) )->add_text_manager( tm );
}

void engine::world::hide_text_manager( base_scene_view_ptr const& scene_view, survarium::flash_text_manager* tm )
{
	// <0>
	// <0x6540f0>|0x000|+0x010:'2164'
	// ******

	static_cast_checked< render::scene_view* >( scene_view.c_ptr( ) )->remove_text_manager( tm );
}

void engine::world::execute_scaleform_command( survarium::scaleform_render_command command )
{
	// <0x653eb0>|0x000|+0x00b:'2169'
	// ******

	command.execute	( );
}

void engine::world::set_portal_system( base_scene_ptr const& scene, resources::unmanaged_resource_ptr pss_ptr )
{
	vostok::render::scene* s = static_cast_checked<vostok::render::scene*>(scene.c_ptr());
	s->set_portal_system	( pss_ptr );
}

void engine::world::test_action_portal_system( base_scene_ptr const& scene )
{
	vostok::render::scene* s = static_cast_checked<vostok::render::scene*>(scene.c_ptr());
	s->test_action_portal_system	( );
}

void engine::world::clear_grass( base_scene_ptr const& s )
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>( s.c_ptr( ) );
	scene->get_grass( )->clear( );
}

void engine::world::populate_grass( base_scene_ptr const& s )
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>( s.c_ptr( ) );
	scene->get_grass( )->populate( 16.f );
}

void engine::world::add_grass_layer( grass_layer_desc* desc, grass_layer_data* data, base_scene_ptr const& s )
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>( s.c_ptr( ) );
	scene->get_grass( )->update_grass_layer( desc, data, true, false, false );
}

void engine::world::update_grass_layer(
	grass_layer_desc*			desc,
	grass_layer_data*			data,
	base_scene_ptr const&		s,
	bool						is_set
)
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>( s.c_ptr( ) );
	scene->get_grass( )->update_grass_layer( desc, data, is_set, true, false );
}

void engine::world::remove_grass_layer( u8 id, base_scene_ptr const& s )
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>( s.c_ptr( ) );
	scene->get_grass( )->remove_grass_layer( id, true );
}

void engine::world::set_grass( resources::unmanaged_resource_ptr grass, base_scene_ptr const& s )
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(s.c_ptr());

	scene->set_grass	( static_cast_checked<grass_world*>(grass.c_ptr()) );
}

void engine::world::reset_grass( resources::unmanaged_resource_ptr grass, base_scene_ptr const& s )
{
	vostok::render::scene* scene = static_cast_checked<vostok::render::scene*>(s.c_ptr());

	scene->reset_grass	( static_cast_checked<grass_world*>(grass.c_ptr()) );
}

bool engine::world::is_playing( resources::unmanaged_resource_ptr const& instance )
{
	return particle::is_playing( instance );
}

void engine::world::set_gamma_correction_factor( const float value )
{
	options::ref( ).current.m_gamma_correction_factor = value;
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
	// 	vostok::fs_new::virtual_path_string*
	// 	iterator_type;

	// typedef
	// 	vostok::render::data_indexer*
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
