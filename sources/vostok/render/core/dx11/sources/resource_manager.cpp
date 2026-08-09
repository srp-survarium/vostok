#include "pch.h"
#include <ddraw.h>
#include <d3dx11tex.h>

#pragma comment( lib, "d3dx11.lib" )
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/shader_binary_source_cook.h>
#include "texture_storage.h"
#include <vostok/render/core/shader_constant_table.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/device.h>
#include <vostok/render/core/dx11/state_descriptor.h>
#include <vostok/render/core/res_state.h>
#include <vostok/render/core/dx11/res_declaration.h>
#include <vostok/render/core/dx11/res_signature.h>
#include <vostok/render/core/render_target.h>
#include <vostok/render/core/dx11/sampler_state_descriptor.h>
#include <vostok/render/core/res_sampler_list.h>
#include <vostok/render/core/res_xs.h>
#include <vostok/render/core/dx11/res_geometry.h>
#include <vostok/render/core/dx11/xs_data.h>
#include <vostok/render/core/res_texture.h>
#include <vostok/render/core/res_render_output.h>
#include <vostok/render/core/options.h>
#include <vostok/render/core/texture_cook.h>
#include "manager_common_inline.h"

#include <vostok/resources.h>
#include <vostok/resources_fs.h>

#include <vostok/tasks_system.h>

#include <algorithm>

namespace vostok {
namespace render {

bool g_enable_resource_sharing = true;
static bool s_debug_clip_texture_quality = true;

struct resource_manager_call_destructor_predicate {
	template <typename T>
	inline void operator()( T* resource ) const
	{
		resource_manager::call_resource_destructor( resource );
	}
}; // struct resource_manager_call_destructor_predicate

struct shader_hw_remove_predicate {
	explicit shader_hw_remove_predicate( pcstr name ) :
		m_name( name )
	{
	}

	template <typename T>
	bool operator()( T const& element ) const
	{
		return strings::compare( element->second, m_name ) == 0;
	}

private:
	pcstr m_name;
};

struct load_texture_delegate {
	load_texture_delegate(
		res_texture_ptr texture,
		math::rectangle< math::int2 > rectangle,
		u32 array_index,
		pcstr name
	) :
		dest_rect( rectangle ),
		dest_texture( texture ),
		user_name( name ),
		arr_ind( array_index )
	{
	}

	void execute( resources::queries_result& )
	{
	}

private:
	math::rectangle< math::int2 > dest_rect;
	res_texture_ptr dest_texture;
	pcstr user_name;
	u32 arr_ind;
};

bool resource_manager::constant_buffer_predicate::operator()(
	shader_constant_buffer const* const left,
	shader_constant_buffer const* const right
) const
{
	if( left->type() < right->type())
		return true;

	if( left->type() > right->type())
		return false;

	if( left->size() < right->size())
		return true;

	if( left->size() > right->size())
		return false;

	if( left->dest() < right->dest())
		return true;

	if( left->dest() > right->dest())
		return false;

	if( left->name() < right->name())
		return true;

	return false;
}

bool is_equal_formats( DXGI_FORMAT left, DXGI_FORMAT right )
{
	// FUNCTION BODY[0x5607b0]
	if (left == right)
		return true;

	if (left == DXGI_FORMAT_R8G8B8A8_UNORM_SRGB  && right == DXGI_FORMAT_R8G8B8A8_UNORM ||
		left == DXGI_FORMAT_BC1_UNORM_SRGB		 && right == DXGI_FORMAT_BC1_UNORM ||
		left == DXGI_FORMAT_BC2_UNORM_SRGB		 && right == DXGI_FORMAT_BC2_UNORM ||
		left == DXGI_FORMAT_BC3_UNORM_SRGB		 && right == DXGI_FORMAT_BC3_UNORM ||
		left == DXGI_FORMAT_B8G8R8A8_UNORM_SRGB  && right == DXGI_FORMAT_B8G8R8A8_UNORM ||
		left == DXGI_FORMAT_B8G8R8X8_UNORM_SRGB  && right == DXGI_FORMAT_B8G8R8X8_UNORM ||
		left == DXGI_FORMAT_BC7_UNORM_SRGB		 && right == DXGI_FORMAT_BC7_UNORM ||
		right == DXGI_FORMAT_R8G8B8A8_UNORM_SRGB && left == DXGI_FORMAT_R8G8B8A8_UNORM ||
		right == DXGI_FORMAT_BC1_UNORM_SRGB		 && left == DXGI_FORMAT_BC1_UNORM ||
		right == DXGI_FORMAT_BC2_UNORM_SRGB		 && left == DXGI_FORMAT_BC2_UNORM ||
		right == DXGI_FORMAT_BC3_UNORM_SRGB		 && left == DXGI_FORMAT_BC3_UNORM ||
		right == DXGI_FORMAT_B8G8R8A8_UNORM_SRGB && left == DXGI_FORMAT_B8G8R8A8_UNORM ||
		right == DXGI_FORMAT_B8G8R8X8_UNORM_SRGB && left == DXGI_FORMAT_B8G8R8X8_UNORM ||
		right == DXGI_FORMAT_BC7_UNORM_SRGB		 && left == DXGI_FORMAT_BC7_UNORM)
	return true;

	return false;
}

bool read_srgb_flag( pcbyte dds_ptr, u32 dds_size )
{
	// FUNCTION BODY[0x560990]
	bool is_srgb_option			= false;
	vostok::memory::copy			(
		&is_srgb_option,
		sizeof(is_srgb_option),
		dds_ptr + dds_size - sizeof(is_srgb_option),
		sizeof(is_srgb_option)
	);
	return is_srgb_option;
}

DXGI_FORMAT get_typeless_format( DXGI_FORMAT format )
{
	// FUNCTION BODY[0x560710]
	switch (format)
	{
		case DXGI_FORMAT_BC1_UNORM: return DXGI_FORMAT_BC1_TYPELESS;
		case DXGI_FORMAT_BC2_UNORM: return DXGI_FORMAT_BC2_TYPELESS;
		case DXGI_FORMAT_BC3_UNORM: return DXGI_FORMAT_BC3_TYPELESS;
		case DXGI_FORMAT_BC7_UNORM: return DXGI_FORMAT_BC7_TYPELESS;
		default:					return format;
	};
}

void begin_command_list( D3D11_QUERY_DESC& query_desc, ID3D11Query*& out_empty_query_ptr )
{
	// FUNCTION BODY[0x560970]
	vostok::render::device::ref().d3d_device()->CreateQuery(&query_desc, &out_empty_query_ptr);
}

void end_command_list( ID3D11Query*& out_empty_query_ptr )
{
	// FUNCTION BODY[0x560920]
	vostok::render::device::ref().d3d_context()->End(out_empty_query_ptr);
	while( S_OK != vostok::render::device::ref().d3d_context()->GetData(out_empty_query_ptr, 0, 0, 0) );
	out_empty_query_ptr->Release();
}

ID3D11Resource* make_copy_with_srgb_format( ID3D11Resource* in_texture )
{
	// FUNCTION BODY[0x561bd0]
	// Make copy.
	// http://www.gamedev.net/topic/605930-id3dx10font-and-srgb/
	// http://timothylottes.blogspot.com/2011/02/aliasing-srgb-and-non-srgb-on-all-apis.html

	if (!in_texture)
		return 0;

	D3D11_RESOURCE_DIMENSION	type;
	in_texture->GetType			(&type);

	if (type == D3D11_RESOURCE_DIMENSION_TEXTURE2D)
	{
		ID3D11Query*			out_empty_query_ptr;
		D3D11_QUERY_DESC		query_desc;
		query_desc.MiscFlags	= 0;
		query_desc.Query		= D3D11_QUERY_EVENT;
		begin_command_list		(query_desc, out_empty_query_ptr);

		D3D11_TEXTURE2D_DESC	texDesc;
		ID3D11Texture2D* T		= static_cast<ID3D11Texture2D*>(in_texture);
		T->GetDesc				(&texDesc);

		texDesc.Format			= get_typeless_format(texDesc.Format);
		ID3D11Texture2D* copy	= 0;
		HRESULT result			= device::ref().d3d_device()->CreateTexture2D(&texDesc, 0, &copy);
		CHECK_RESULT			(result);

		device::ref().d3d_context()->CopyResource(copy, in_texture);

		end_command_list		(out_empty_query_ptr);

		return					copy;
	}
	else
	{
		R_ASSERT				(0, "sRGB for 3d and 1d types not implemented yet.");
		return					in_texture;
	}
}

template <>
resource_manager::map_vs_hw& resource_manager::xs_hw_registry<vs_data>( )
{
	return m_vs_hw_registry;
}

template <>
resource_manager::map_gs_hw& resource_manager::xs_hw_registry<gs_data>( )
{
	return m_gs_hw_registry;
}

template <>
resource_manager::map_ps_hw& resource_manager::xs_hw_registry<ps_data>( )
{
	return m_ps_hw_registry;
}

template <typename shader_data>
res_xs_hw<shader_data>* resource_manager::create_xs_hw_impl(
	pcstr name,
	shader_configuration shader_config,
	shader_include_getter* include_getter,
	binary_shader_sources_type* shader_sources
)
{
	typedef map<shader_name_config_pair, res_xs_hw<shader_data>*> xs_registry_type;

	VOSTOK_UNREFERENCED_PARAMETER	(include_getter);

	if (!name)
		return NULL;

	xs_registry_type& registry = xs_hw_registry<shader_data>();

	// TODO: Add check of global defines to shader_configuration
	typename xs_registry_type::iterator it = registry.find(
		shader_name_config_pair( name, shader_config)
	);

	if (!m_is_shader_reloading && it != registry.end())
	{
		++sh_returned;
		return it->second;
	}

	++sh_created;

	binary_shader_sources_type::iterator found_binary_source_it = shader_sources->find(
		binary_shader_key_type(
			name,
			(enum_shader_type)shader_data::type,
			shader_config
		)
	);

	if (found_binary_source_it != shader_sources->end())
	{
		res_xs_hw<shader_data>* xs_hw	= NEW(res_xs_hw<shader_data>);
		xs_hw->mark_registered			();
		xs_hw->set_name					(name);

		registry[
			shader_name_config_pair(xs_hw->name().c_str(), shader_config)
		]								= xs_hw;

		ID3D10Blob* blob				= NULL;
		pvoid byte_code					= found_binary_source_it->second->m_shader_byte_code;
		if (!byte_code)
			byte_code = found_binary_source_it->second->m_compiled_shader_byte_code;

		if (byte_code)
		{
			CHECK_RESULT				(
				D3DCreateBlob( found_binary_source_it->second->m_shader_byte_code_size, &blob)
			);

			memcpy						(
				blob->GetBufferPointer(),
				byte_code,
				found_binary_source_it->second->m_shader_byte_code_size
			);

			if (FAILED(xs_hw->create_hw_shader( blob)))
			{
				LOG_ERROR				(
					"!%s: %s",
					shader_type_traits<(enum_shader_type)shader_data::type>::short_name(),
					name
				);
				LOG_ERROR				("shader creation failed");
			}

			if (blob)
				blob->Release			();
		}

		return xs_hw;
	}

	return 0;
}

res_xs_hw<vs_data>* resource_manager::create_vs_hw(
	pcstr name,
	shader_configuration config,
	shader_include_getter* include_getter,
	binary_shader_sources_type* sources
)
{
	return create_xs_hw_impl<vs_data>( name, config, include_getter, sources );
}

res_xs_hw<gs_data>* resource_manager::create_gs_hw(
	pcstr name,
	shader_configuration config,
	shader_include_getter* include_getter,
	binary_shader_sources_type* sources
)
{
	return create_xs_hw_impl<gs_data>( name, config, include_getter, sources );
}

res_xs_hw<ps_data>* resource_manager::create_ps_hw(
	pcstr name,
	shader_configuration config,
	shader_include_getter* include_getter,
	binary_shader_sources_type* sources
)
{
	return create_xs_hw_impl<ps_data>( name, config, include_getter, sources );
}

void resource_manager::bind_samplers_to_shaders( )
{
	for( set<res_xs<vs_data>*, compare_shader_predicate<vs_data> >::iterator it = m_v_shaders.begin(); it != m_v_shaders.end(); ++it)
		(*it)->m_samplers->rebind();

	for( set<res_xs<gs_data>*, compare_shader_predicate<gs_data> >::iterator it = m_g_shaders.begin(); it != m_g_shaders.end(); ++it)
		(*it)->m_samplers->rebind();

	for( set<res_xs<ps_data>*, compare_shader_predicate<ps_data> >::iterator it = m_p_shaders.begin(); it != m_p_shaders.end(); ++it)
		(*it)->m_samplers->rebind();
}

template <typename shader_data>
void resource_manager::release_impl( res_xs_hw<shader_data> const* xs_hw )
{
	typedef map<shader_name_config_pair, res_xs_hw<shader_data>*> xs_registry_type;

	if( !xs_hw->is_registered())
		return;

	xs_registry_type& registry = xs_hw_registry<shader_data>();
	typename xs_registry_type::iterator begin = registry.begin(), end = registry.end(), it = registry.begin();

	while( identity(true))
	{
		if ( it==end)
		{
			DELETE( xs_hw, resource_manager_call_destructor_predicate());
			break;
		}
		else if ( it->second==xs_hw)
		{
			registry.erase(it);
			DELETE( xs_hw, resource_manager_call_destructor_predicate());
			break;
		}
		++it;
	}
}

void resource_manager::release( res_xs_hw<vs_data> const* vs )
{
	release_impl<vs_data>( vs );
}

void resource_manager::release( res_xs_hw<gs_data> const* gs )
{
	release_impl<gs_data>( gs );
}

void resource_manager::release( res_xs_hw<ps_data> const* ps )
{
	release_impl<ps_data>( ps );
}

pcstr resource_manager::get_converted_shader_path( ) const
{
	// FUNCTION BODY[0x560700]
	return "resources.converted/shaders/sm_4_0";
}

pcstr get_textures_path2( )
{
	// FUNCTION BODY[0x5606f0]
	return "resources/textures";
}

// claude@NOTE: only the TOTAL size (0xfa0 bytes, from the ctor's memset) and the name are
// recovered from the target - the element type is a guess; nothing else in the exe touches it.
static ID3D11CommandList* s_command_lists[1000];

resource_manager::resource_manager( configs::binary_config_ptr const& config ) :
	sh_created					( 0),
	sh_returned					( 0),
	tl_created					( 0),
	cb_created					( 0),
	sl_created					( 0),
	m_deferred_context			( NULL),
	m_render_target_video_memory( 0),
	shader_name_to_mask_config	( config),
	m_num_bytes_of_texture_video_memory( 0),
	m_num_bytes_of_buffers_video_memory( 0),
	m_tasks_type				( tasks::create_new_task_type( "texture_create_task", 0)),
	m_loading_incomplete		( false),
	m_is_shader_reloading		( false),
	m_need_recompile_shader_if_source_reloaded( true),
	m_texture_storage			( NULL),
	m_texture_storage_staging	( NULL),
	m_compile_error_handler		( NULL)
{
	memset( s_command_lists, 0, sizeof( s_command_lists));

	static shader_binary_source_cook shader_binary_source_cooker;
	resources::register_cook(&shader_binary_source_cooker);

	m_texture_storage = NEW (texture_storage)();
	m_texture_storage_staging = NEW (texture_storage)();

#ifndef MASTER_GOLD
	fs_new::virtual_path_string textures_path	=	get_textures_path2();
	textures_path.rtrim						(fs_new::virtual_path_string::separator);
	m_watcher_subscribe_id	=	vostok::resources::subscribe_watcher(
		textures_path,
		boost::bind(& resource_manager::on_texture_source_changed, this, _1)
	);
#endif // #ifndef MASTER_GOLD
}

resource_manager::~resource_manager( )
{
	while( !m_states.empty())
	{
		res_state* state = m_states.front();
		m_states.erase( m_states.begin());
		DELETE( state, resource_manager_call_destructor_predicate());
	}

	DELETE( m_texture_storage);
	DELETE( m_texture_storage_staging);

	resources::unregister_cook(resources::shader_binary_source_class);

#ifndef MASTER_GOLD
	vostok::resources::unsubscribe_watcher(m_watcher_subscribe_id);
#endif // #ifndef MASTER_GOLD
}

void change_substring( fs_new::virtual_path_string* src_and_dest, pcstr what, pcstr to )
{
	// FUNCTION BODY[0x560c30]
	vostok::fs_new::virtual_path_string result;
	vostok::fs_new::virtual_path_string::size_type pos = src_and_dest->find(what);

	if(pos!=src_and_dest->npos)
	{
		u32 what_len = vostok::strings::length(what);
		result.assign(src_and_dest->begin(), src_and_dest->begin()+pos); // head
		result.append(to); // body
		result.append(src_and_dest->begin()+pos+what_len, src_and_dest->end()); // tail
		* src_and_dest = result;
	}
}

res_texture_ptr create_color_grading_base_lut( u32 const size )
{
	// FUNCTION BODY[0x561b20]
	struct pixel
	{
#	pragma warning(push)
#	pragma warning(disable:4201)
		union
		{
			u32		clr;
			struct
			{
				u8	r;
				u8	g;
				u8	b;
				u8	a;
			};
		};
#	pragma warning(pop)
	};

	u32 const data_size			= size * size * size * sizeof(pixel);
	pixel* temp_data			= (pixel*)ALLOCA(data_size);

	for (u32 z = 0; z < size; z++)
	{
		for (u32 y = 0; y < size; y++)
		{
			for (u32 x = 0; x < size; x++)
			{
				pixel* p	= temp_data + z * size * size + y * size + x;
				p->r		= static_cast_checked<u8>(x * size);
				p->g		= static_cast_checked<u8>(y * size);
				p->b		= static_cast_checked<u8>(z * size);
				p->a		= 255;
			}
		}
	}

	D3D11_SUBRESOURCE_DATA	data;
	data.pSysMem			= temp_data;
	data.SysMemSlicePitch	= data_size / size;
	data.SysMemPitch		= data_size / (size * size);

	return					resource_manager::ref().create_texture3d(
		"$user$test3d",
		size,
		size,
		size,
		&data,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		D3D11_USAGE_IMMUTABLE,
		1
	);
}

res_texture_ptr resource_manager::get_color_grading_base_lut( )
{
	// FUNCTION BODY[0x561fa0]
	if (!m_color_grading_base_lut)
	{
		m_color_grading_base_lut = create_color_grading_base_lut(16);
	}

	return m_color_grading_base_lut;
}

void resource_manager::reload_modified_textures( )
{
	// FUNCTION BODY[0x564640]
	vector<fs_new::virtual_path_string> textures_to_reload = m_textures_to_reload;

	for (vector<fs_new::virtual_path_string>::iterator it = textures_to_reload.begin(); it != textures_to_reload.end(); ++it)
	{
		fs_new::virtual_path_string path;
		fs_new::virtual_path_string path_add = *it;

		path.assignf("%s/%s.dds", get_textures_path2(), path_add.c_str());

		resources::query_resource(
			path.c_str(),
			resources::texture_wrapper_class,
			boost::bind(
				static_cast<void (resource_manager::*)( resources::queries_result&, u32, bool, u32 )>(&resource_manager::on_texture_loaded),
				this, _1, 0, true, u32(-1)
			),
			::vostok::render::g_allocator
		);
	}
}

void resource_manager::reload_shader_sources( bool is_recompile_shaders )
{
	m_loading_incomplete = false;
}

shader_constant_table* resource_manager::create_const_table(
	shader_constant_table const& proto
)
{
	// FUNCTION BODY[0x5622c0]
	shader_constant_table new_table( proto);

	new_table.apply_bindings( m_const_bindings);

	for ( set<shader_constant_table*, constant_table_predicate>::iterator it = m_const_tables.begin(); it != m_const_tables.end(); ++it)
		if( identity((*it)->equal( new_table) && g_enable_resource_sharing))
			return *it;

	shader_constant_table* const created_table = NEW( shader_constant_table)( new_table);
	created_table->mark_registered();
	m_const_tables.insert( created_table);

	return created_table;
}

void resource_manager::release( shader_constant_table const* const_table )
{
	// FUNCTION BODY[0x5621c0]
	if( !const_table->is_registered())
		return;

	if( reclaim( m_const_tables, const_table))
	{
		DELETE( const_table, resource_manager_call_destructor_predicate());
		return;
	}

	LOG_ERROR( "!ERROR: Failed to find compiled shader_constant-table");
}

shader_constant_host const* resource_manager::register_constant_binding(
	shader_constant_binding const& binding
)
{
	// FUNCTION BODY[0x563890]
	m_const_bindings.add( binding);

	shader_constant_host * host =  backend::ref().register_constant_host( binding.name(), binding.type() );
	host->m_source = binding.source();

	return host;
}

res_texture* resource_manager::find_texture( pcstr name )
{
	// FUNCTION BODY[0x560c00]
	map_texture::iterator it = m_texture_registry.find( name);

	if( it!=m_texture_registry.end())
		return	it->second;

	return NULL;
}

res_texture* resource_manager::create_texture(
	pcstr physical_name,
	resources::query_result_for_cook* parent,
	u32 mip_level_cut,
	bool use_pool,
	bool load_async,
	bool use_converter,
	u32 num_last_mips_used
)
{
	// FUNCTION BODY[0x564d20]
	if( physical_name && 0 == strcmp( physical_name,"null"))
		return 0;

	res_texture* res = find_texture( physical_name);
	if( res != NULL)
		return	res;
	else
		return load_texture( physical_name, parent, mip_level_cut, use_pool, load_async, use_converter, num_last_mips_used);
}

static pcstr resources_textures_converted_string	=	"resources.converted/textures/";
static pcstr resources_textures						=	"resources/textures/";

void fix_texture_name( fs_new::virtual_path_string& str )
{
	// FUNCTION BODY[0x560e90]
	change_substring(&str, resources_textures, "");
	change_substring(&str, resources_textures_converted_string, "");
	fs_new::virtual_path_string::size_type pos = str.rfind('.');
	if (pos!=fs_new::virtual_path_string::npos)
		str.set_length(pos);
}

u32 calc_bytes_per_block( DXGI_FORMAT format )
{
	switch ( format ) {
		case DXGI_FORMAT_R8_UNORM:
			return 1;
		case DXGI_FORMAT_R8G8B8A8_UNORM:
		case DXGI_FORMAT_B8G8R8A8_UNORM:
			return 4;
		case DXGI_FORMAT_BC1_UNORM:
			return 8;
		case DXGI_FORMAT_BC2_UNORM:
		case DXGI_FORMAT_BC3_UNORM:
			return 16;
		default:
			NODEFAULT( return 0 );
	}
}

u32 calc_block_size( DXGI_FORMAT format )
{
	switch ( format ) {
		case DXGI_FORMAT_R8_UNORM:
		case DXGI_FORMAT_R8G8B8A8_UNORM:
		case DXGI_FORMAT_B8G8R8A8_UNORM:
			return 1;
		case DXGI_FORMAT_BC1_UNORM:
		case DXGI_FORMAT_BC2_UNORM:
		case DXGI_FORMAT_BC3_UNORM:
			return 4;
		default:
			NODEFAULT( return 0 );
	}
}

DXGI_FORMAT find_srgb_format( DXGI_FORMAT format, bool )
{
	switch ( format ) {
		case DXGI_FORMAT_R8G8B8A8_UNORM:
			return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		case DXGI_FORMAT_B8G8R8A8_UNORM:
			return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
		case DXGI_FORMAT_BC1_UNORM:
			return DXGI_FORMAT_BC1_UNORM_SRGB;
		case DXGI_FORMAT_BC2_UNORM:
			return DXGI_FORMAT_BC2_UNORM_SRGB;
		case DXGI_FORMAT_BC3_UNORM:
			return DXGI_FORMAT_BC3_UNORM_SRGB;
		default:
			return format;
	}
}

void resource_manager::on_texture_loaded(
	resources::managed_resource_ptr data,
	pcstr in_name,
	u32 num_last_mips_used
)
{
	struct dds_header {
		u32 signature0;
		DDSURFACEDESC2 header0;
	};

	fs_new::virtual_path_string name( in_name );
	fix_texture_name( name );

	res_texture* tex = create_texture( name.c_str( ), NULL, 0, false, true, true, u32(-1) );

	resources::managed_resource_ptr managed_ptr = data;
	resources::pinned_ptr_const< texture_data_resource > managed_typed_ptr( managed_ptr );

	pcbyte const dds_ptr = static_cast< pcbyte >( managed_typed_ptr->buffer( ).c_ptr( ) );
	u32 dds_size = managed_typed_ptr->buffer( ).size( );
	bool is_srgb_option = read_srgb_flag( dds_ptr, dds_size );
	--dds_size;

	D3DX_IMAGE_INFO dds_info = { 0 };
	if ( FAILED( D3DXGetImageInfoFromMemory( dds_ptr, dds_size, NULL, &dds_info, NULL ) ) )
		return;

	pbyte copy_ptr = const_cast< pbyte >( dds_ptr ) + sizeof(dds_header);
	u32 mem_usage = 0;
	ID3D11Resource* base_tex = NULL;

	u32 orig_width;
	u32 orig_height;

	if ( dds_info.Format == DXGI_FORMAT_R8G8B8A8_UNORM )
		dds_info.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

	if ( (dds_info.Format == DXGI_FORMAT_R8G8B8A8_UNORM ||
		  dds_info.Format == DXGI_FORMAT_B8G8R8A8_UNORM) &&
		 dds_size == dds_info.Width * dds_info.Height + sizeof(dds_header) )
	{
		dds_info.Format = DXGI_FORMAT_R8_UNORM;
	}

	u32 const block_size = calc_block_size( dds_info.Format );
	u32 const block_bytes = calc_bytes_per_block( dds_info.Format );
	u32 texture_quality = options::ref( ).current.m_texture_quality;
	u32 const min_dimension = math::min( dds_info.Width, dds_info.Height );
	u32 const video_memory = device::ref( ).get_avaliable_video_memory( );
	bool const weapon_or_flora = strstr( name.c_str( ), "weapons/" ) != NULL ||
		strstr( name.c_str( ), "flora/" ) != NULL;

	if ( s_debug_clip_texture_quality && !device::ref( ).get_is_editor( ) )
	{
		if ( texture_quality == 1 )
			texture_quality = weapon_or_flora ? 1 : 0;
		else if ( texture_quality == 2 )
		{
			if ( video_memory <= 512 )
				texture_quality = 0;
			else if ( video_memory <= 1024 )
				texture_quality = weapon_or_flora ? 2 : 1;
		}
	}

	u32 mip_level_cut = 0;
	if ( min_dimension > 128 )
		mip_level_cut = 2 - texture_quality;

	if ( dds_info.ArraySize == 1 &&
		 dds_info.MipLevels > mip_level_cut &&
		 dds_info.Depth == 1 &&
		 texture_quality < 2 )
	{
		for ( u32 mip_index = 0; mip_index < mip_level_cut; ++mip_index )
		{
			u32 const width_in_blocks =
				(math::max( dds_info.Width >> mip_index, block_size ) + block_size - 1) / block_size;
			u32 const height_in_blocks =
				(math::max( dds_info.Height >> mip_index, block_size ) + block_size - 1) / block_size;
			copy_ptr += width_in_blocks * height_in_blocks * block_bytes;
		}

		dds_info.MipLevels -= mip_level_cut;
		dds_info.Width /= static_cast<u32>( math::pow( 2.f, mip_level_cut ) );
		dds_info.Height /= static_cast<u32>( math::pow( 2.f, mip_level_cut ) );
		num_last_mips_used -= mip_level_cut;
	}

	bool const use_cutting =
		(dds_info.Format == DXGI_FORMAT_BC1_UNORM || dds_info.Format == DXGI_FORMAT_BC3_UNORM) &&
		num_last_mips_used != u32(-1) &&
		num_last_mips_used < dds_info.MipLevels &&
		dds_info.Depth == 1;

	u32 cut_diff = 0;
	u32 num_orig_mips = dds_info.MipLevels;
	orig_width = dds_info.Width;
	orig_height = dds_info.Height;

	if ( use_cutting )
	{
		cut_diff = dds_info.MipLevels - num_last_mips_used;
		dds_info.Width = math::max( dds_info.Width >> cut_diff, block_size );
		dds_info.Height = math::max( dds_info.Height >> cut_diff, block_size );
		dds_info.MipLevels = num_last_mips_used;
	}

	HRESULT creation_result;
	if ( dds_info.Depth == 1 )
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory( &desc, sizeof(desc) );
		desc.ArraySize = dds_info.ArraySize;
		desc.Format = is_srgb_option ? find_srgb_format( dds_info.Format, true ) : dds_info.Format;
		desc.Width = dds_info.Width;
		desc.Height = dds_info.Height;
		desc.MipLevels = dds_info.MipLevels;
		desc.MiscFlags = dds_info.MiscFlags;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.CPUAccessFlags = 0;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.Usage = D3D11_USAGE_DEFAULT;

		if ( dds_info.ArraySize == 1 )
		{
			creation_result = device::ref( ).d3d_device( )->CreateTexture2D(
				&desc,
				NULL,
				reinterpret_cast< ID3D11Texture2D** >( &base_tex )
			);

			for ( u32 array_slice = 0; array_slice < dds_info.ArraySize; ++array_slice )
			{
				for ( u32 mip_index = 0; mip_index < num_orig_mips; ++mip_index )
				{
					u32 const width_in_blocks =
						(math::max( orig_width >> mip_index, block_size ) + block_size - 1) / block_size;
					u32 const height_in_blocks =
						(math::max( orig_height >> mip_index, block_size ) + block_size - 1) / block_size;
					u32 const mip_size = width_in_blocks * height_in_blocks * block_bytes;
					u32 const row_pitch = width_in_blocks * block_bytes;

					if ( use_cutting && num_last_mips_used < num_orig_mips - mip_index )
						copy_ptr += mip_size;
					else
					{
						device::ref( ).d3d_context( )->UpdateSubresource(
							base_tex,
							D3D11CalcSubresource( mip_index - cut_diff, array_slice, desc.MipLevels ),
							NULL,
							copy_ptr,
							row_pitch,
							0
						);
						copy_ptr += mip_size;
						mem_usage += mip_size;
					}
				}
			}
		}
		else
		{
			creation_result = device::ref( ).d3d_device( )->CreateTexture2D(
				&desc,
				NULL,
				reinterpret_cast< ID3D11Texture2D** >( &base_tex )
			);
		}

		CHECK_RESULT( creation_result, "texture creation failed: %s", name.c_str( ) );

		if ( dds_info.ArraySize != 1 )
		{
			for ( u32 array_slice = 0; array_slice < dds_info.ArraySize; ++array_slice )
			{
				for ( u32 mip_index = 0; mip_index < num_orig_mips; ++mip_index )
				{
					u32 const width_in_blocks =
						(math::max( orig_width >> mip_index, block_size ) + block_size - 1) / block_size;
					u32 const height_in_blocks =
						(math::max( orig_height >> mip_index, block_size ) + block_size - 1) / block_size;
					u32 const mip_size = width_in_blocks * height_in_blocks * block_bytes;
					u32 const row_pitch = width_in_blocks * block_bytes;

					if ( use_cutting && num_last_mips_used < num_orig_mips - mip_index )
						copy_ptr += mip_size;
					else
					{
						device::ref( ).d3d_context( )->UpdateSubresource(
							base_tex,
							D3D11CalcSubresource( mip_index - cut_diff, array_slice, desc.MipLevels ),
							NULL,
							copy_ptr,
							row_pitch,
							0
						);
						copy_ptr += mip_size;
						mem_usage += mip_size;
					}
				}
			}
		}
	}
	else
	{
		D3D11_TEXTURE3D_DESC desc;
		desc.Format = is_srgb_option ? find_srgb_format( dds_info.Format, true ) : dds_info.Format;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.Width = dds_info.Width;
		desc.Height = dds_info.Height;
		desc.Depth = dds_info.Depth;
		desc.MipLevels = dds_info.MipLevels;
		desc.MiscFlags = dds_info.MiscFlags;
		desc.Usage = D3D11_USAGE_DEFAULT;

		creation_result = device::ref( ).d3d_device( )->CreateTexture3D(
			&desc,
			NULL,
			reinterpret_cast< ID3D11Texture3D** >( &base_tex )
		);
		CHECK_RESULT( creation_result );

		for ( u32 mip_index = 0; mip_index < dds_info.MipLevels; ++mip_index )
		{
			u32 const width_in_blocks =
				((dds_info.Width >> mip_index) + block_size - 1) / block_size;
			u32 const height_in_blocks =
				((dds_info.Height >> mip_index) + block_size - 1) / block_size;
			u32 const row_pitch = width_in_blocks * block_bytes;
			u32 const mip_size =
				width_in_blocks * height_in_blocks * dds_info.Depth * block_bytes;

			device::ref( ).d3d_context( )->UpdateSubresource(
				base_tex,
				mip_index,
				NULL,
				copy_ptr,
				row_pitch,
				row_pitch * height_in_blocks
			);
			copy_ptr += mip_size;
		}

		mem_usage = dds_size + sizeof(dds_header);
	}

	tex->m_mem_usage = mem_usage;
	tex->set_hw_texture( base_tex );
	tex->m_loaded = true;

	if ( base_tex )
		base_tex->Release( );
}

void resource_manager::on_texture_loaded(
	resources::queries_result& data,
	u32 mip_level_cut,
	bool use_converter,
	u32 num_last_mips_used
)
{
	fs_new::virtual_path_string name( data[0].get_requested_path( ) );

	if ( !data.is_successful( ) )
	{
		LOG_ERROR( "Texture %s was not found!", name.c_str( ) );
		return;
	}

	fix_texture_name( name );

	vector<fs_new::virtual_path_string>::iterator to_delete =
		std::find( m_textures_to_reload.begin( ), m_textures_to_reload.end( ), name );
	if ( to_delete != m_textures_to_reload.end( ) )
		m_textures_to_reload.erase( to_delete );

	res_texture* tex = create_texture( name.c_str( ), NULL, 0, false, true, true, u32(-1) );

	if ( use_converter )
	{
		on_texture_loaded(
			data[0].get_managed_resource( ),
			data[0].get_requested_path( ),
			num_last_mips_used
		);
		return;
	}

	resources::pinned_ptr_const< u8 > ptr_man( data[0].get_managed_resource( ) );

	D3DX_IMAGE_INFO dds_info = { 0 };
	CHECK_RESULT(
		D3DXGetImageInfoFromMemory(
			ptr_man.c_ptr( ),
			ptr_man.size( ),
			NULL,
			&dds_info,
			NULL
		)
	);

	D3DX_IMAGE_LOAD_INFO load_info;
	ID3DBaseTexture* base_tex = NULL;
	CHECK_RESULT(
		D3DXCreateTextureFromMemory(
			device::ref( ).d3d_device( ),
			ptr_man.c_ptr( ),
			ptr_man.size( ),
			&load_info,
			NULL,
			&base_tex,
			NULL
		)
	);

	tex->set_hw_texture( base_tex, mip_level_cut );
	base_tex->Release( );
}

u32 resource_manager::get_texture_video_memory_size( )
{
	map_texture::iterator it = m_texture_registry.begin();
	map_texture::iterator end = m_texture_registry.end();

	u32 result = 0;

	for( ; it != end; ++it)
		result += it->second->m_mem_usage;

	return result >> 20;
}

void resource_manager::on_texture_loaded_staging(
	resources::queries_result&,
	u32,
	bool
)
{
	// claude@NOTE: the shipped body is empty - the target's 147 recorded source lines all
	// compile out of the Master Gold build (staging texture pool disabled).
}

// claude@NOTE: recovered from reload_all_textures' trailing `mov byte [s_reload_all_textures], 0`;
// the console command that sets it (s_reload_all_textures_cc, a dynamic initializer in the target)
// is not reconstructed here.
static bool s_reload_all_textures = false;

void resource_manager::reload_all_textures( )
{
	vector<fs_new::virtual_path_string> textures_to_reload;

	for( map_texture::iterator it = m_texture_registry.begin(); it != m_texture_registry.end(); ++it)
	{
		if( it->first.length() && it->first[0] != '$')
			textures_to_reload.push_back( it->first);
	}

	for( vector<fs_new::virtual_path_string>::iterator it = textures_to_reload.begin(); it != textures_to_reload.end(); ++it)
		load_texture( it->c_str(), NULL, 0, false, true, true, u32(-1));

	s_reload_all_textures = false;
}

res_texture* resource_manager::load_texture(
	pcstr texture_name,
	resources::query_result_for_cook* parent,
	u32 mip_level_cut,
	bool use_pool,
	bool load_async,
	bool use_converter,
	u32 num_last_mips_used
)
{
	resources::class_id_enum class_id = use_converter ? resources::texture_wrapper_class : resources::raw_data_class;
	fs_new::virtual_path_string path			  = "resources/textures/";

	if( !use_pool)
	{
		res_texture* tex;
		map_texture::iterator it = m_texture_registry.find( texture_name);

		if( it == m_texture_registry.end())
		{
			tex = NEW( res_texture);
			tex->set_name( texture_name );
			tex->mark_registered();
			m_texture_registry.insert( mk_pair( tex->name(), tex));
		}
		else
			tex = it->second;

		if( strstr( tex->name(), "$user$") == 0 && texture_name && texture_name[0] && num_last_mips_used)
		{
			path += tex->name();
			path += ".dds";

			if (!load_async)
				resources::query_resource_and_wait( path.c_str(),
					class_id,
					boost::bind(
						static_cast<void (resource_manager::*)( resources::queries_result&, u32, bool, u32 )>(&resource_manager::on_texture_loaded),
						this, _1, mip_level_cut, use_converter, num_last_mips_used
					),
					::vostok::render::g_allocator);
			else
				resources::query_resource( path.c_str(),
					class_id,
					boost::bind(
						static_cast<void (resource_manager::*)( resources::queries_result&, u32, bool, u32 )>(&resource_manager::on_texture_loaded),
						this, _1, mip_level_cut, use_converter, num_last_mips_used
					),
					::vostok::render::g_allocator,
					0,
					parent,
					assert_on_fail_false);
		}

		return tex;
	}
	else
	{
		res_texture* tex = NEW( res_texture)( true);
		tex->mark_registered();
		tex->set_name( texture_name );
		m_texture_registry.insert( mk_pair( tex->name(), tex));

		ASSERT( strstr( tex->name(), "$user$") == 0);
		path += tex->name();
		path += ".dds";
		ASSERT( strstr( path.c_str(), "\\")==0);
		if (!load_async)
			resources::query_resource_and_wait( path.c_str(),
				class_id,
				boost::bind( &resource_manager::on_texture_loaded_staging, this, _1, mip_level_cut, use_converter ),
				::vostok::render::g_allocator,
				0,
				parent);
		else
			resources::query_resource( path.c_str(),
				class_id,
				boost::bind( &resource_manager::on_texture_loaded_staging, this, _1, mip_level_cut, use_converter ),
				::vostok::render::g_allocator,
				0,
				parent);

		return tex;
	}
}

res_texture* resource_manager::create_texture2d_impl(
	u32 width,
	u32 height,
	D3D11_SUBRESOURCE_DATA const* data,
	DXGI_FORMAT format,
	D3D11_USAGE usage,
	u32 mip_levels,
	u32 array_size,
	bool use_for_render_target
)
{
	u32 src_row_pitch;
	D3D11_TEXTURE2D_DESC texure_desc;
	ZeroMemory( &texure_desc, sizeof(texure_desc));
	texure_desc.Width				= width;
	texure_desc.Height				= height;
	texure_desc.Format				= format;
	texure_desc.SampleDesc.Count	= 1;
	texure_desc.SampleDesc.Quality	= 0;
	texure_desc.Usage				= usage;
	texure_desc.MipLevels			= mip_levels;
	texure_desc.ArraySize			= array_size;
	texure_desc.BindFlags			= ((usage == D3D11_USAGE_STAGING) ? 0 : D3D11_BIND_SHADER_RESOURCE)
									| (use_for_render_target ? D3D11_BIND_RENDER_TARGET : 0);

	if (usage == D3D11_USAGE_DYNAMIC)
		texure_desc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	else
		texure_desc.CPUAccessFlags	= (usage == D3D11_USAGE_STAGING) ? D3D11_CPU_ACCESS_READ : 0;

	ID3D11Texture2D*	d3d_texture;
	HRESULT res = device::ref().d3d_device()->CreateTexture2D( &texure_desc, data, &d3d_texture);
	CHECK_RESULT( res);

	res_texture* tex = NEW		( res_texture);

	tex->m_mem_usage			= utils::calc_surface_size( width, height, format, 0, src_row_pitch);

	tex->set_hw_texture			( d3d_texture, 0, usage == D3D11_USAGE_STAGING);

	d3d_texture->Release		();

	return tex;
}

void resource_manager::release_impl( res_texture const* texture )
{
	// FUNCTION BODY[0x560a70]
	DELETE( texture, resource_manager_call_destructor_predicate());
}

res_texture* resource_manager::create_texture2d(
	pcstr user_name,
	u32 width,
	u32 height,
	D3D11_SUBRESOURCE_DATA const* data,
	DXGI_FORMAT format,
	D3D11_USAGE usage,
	u32 mip_levels,
	u32 array_size,
	bool use_as_render_target
)
{
	res_texture* tex;

	if( !user_name)
	{
		tex = create_texture2d_impl( width, height, data, format, usage, mip_levels, array_size, use_as_render_target);
		tex->set_name( user_name);
	}
	else
	{
		map_texture::iterator it = m_texture_registry.find( user_name);

		if( it != m_texture_registry.end())
			return it->second;

		tex = create_texture2d_impl( width, height, data, format, usage, mip_levels, array_size, use_as_render_target);
		tex->set_name( user_name);
		std::pair<map_texture::iterator, bool> res = m_texture_registry.insert	( mk_pair( tex->name(), tex));

		ASSERT( res.second, "A texture with the specified name already exists in texture registry.");
	}

	tex->mark_registered	();

	return tex;
}

res_texture* resource_manager::create_texture3d(
	pcstr user_name,
	u32 width,
	u32 height,
	u32 depth,
	D3D11_SUBRESOURCE_DATA const* data,
	DXGI_FORMAT format,
	u32 usage,
	u32 mip_levels
)
{
	u32							src_row_pitch;
	D3D11_TEXTURE3D_DESC		desc;
	desc.Width					=	width;
	desc.Height					=	height;
	desc.Depth					=	depth;
	desc.MipLevels				=	mip_levels;
	desc.Format					=	format;
	desc.Usage					=	(D3D11_USAGE)usage;

	desc.BindFlags				=	D3D11_BIND_SHADER_RESOURCE;

	if (usage == D3D11_USAGE_IMMUTABLE)
	{
		desc.CPUAccessFlags		=	0;
	}
	else
	{
		if (usage == D3D11_USAGE_DYNAMIC)
		{
			desc.CPUAccessFlags	=	D3D11_CPU_ACCESS_WRITE;
		}
	}

	desc.MiscFlags				=	0;

	ID3D11Texture3D* d3d_texture =	NULL;

	CHECK_RESULT				(device::ref().d3d_device()->CreateTexture3D( &desc, data, &d3d_texture));

	res_texture* tex = NEW		(res_texture);

	tex->m_mem_usage			=	utils::calc_surface_size( width, height, format, 0, src_row_pitch) * depth;

	tex->set_name				(user_name);
	m_texture_registry.insert	(mk_pair( tex->name(), tex));

	tex->mark_registered		();
	tex->set_hw_texture			(d3d_texture, 0, usage == D3D11_USAGE_STAGING);
	d3d_texture->Release		();

	return tex;
}

void resource_manager::release( res_texture const* texture )
{
	// FUNCTION BODY[0x560bc0]
	if( !texture->is_registered())
		return;

	pcstr const name = texture->name();
	map_texture::iterator it = m_texture_registry.find( name);

	if( it!=m_texture_registry.end())
	{
		m_texture_registry.erase( it);
		release_impl( texture);
		return;
	}

	LOG_ERROR( "! ERROR: Failed to find texture surface '%s'", texture->name() );
}

untyped_buffer* resource_manager::create_buffer(
	u32 size,
	pcvoid data,
	enum_buffer_type type,
	bool dynamic,
	bool staging
)
{
	untyped_buffer* new_buffer = NEW(untyped_buffer)( size, data, type, dynamic, staging);

	m_num_bytes_of_buffers_video_memory += size;

	m_buffers.push_back( new_buffer);

	return new_buffer;
}

void resource_manager::release( untyped_buffer const* buffer )
{
	if( reclaim( m_buffers, buffer))
	{
		m_num_bytes_of_buffers_video_memory -= buffer->size();
		DELETE( buffer, resource_manager_call_destructor_predicate());
		return;
	}

	ASSERT("The buffer not found in resource manager resources!");
}

res_state* resource_manager::create_state( state_descriptor& descriptor )
{
	// FUNCTION BODY[0x561690]
	ID3D11RasterizerState*	rasterizer_state	=	m_rs_cache.get_state ( descriptor.m_rasterizer_desc);
	ID3D11DepthStencilState* depth_stencil_state =	m_dss_cache.get_state( descriptor.m_depth_stencil_desc);
	ID3D11BlendState*		blend_state			=	m_bs_cache.get_state( descriptor.m_effect_desc);

	res_state* new_state = NEW( res_state)( rasterizer_state,
											depth_stencil_state,
											blend_state,
											descriptor.m_stencil_ref);

	new_state->mark_registered();
	m_states.push_back	( new_state);

	return new_state;
}

void resource_manager::release( res_state const* state )
{
	// FUNCTION BODY[0x5615a0]
	if( !state->is_registered())
		return;

	if( reclaim( m_states, state))
	{
		DELETE( state, resource_manager_call_destructor_predicate());
		return;
	}

	LOG_ERROR( "!ERROR: Failed to find compiled stateblock");
}

res_declaration* resource_manager::create_declaration(
	D3D11_INPUT_ELEMENT_DESC const* dcl,
	u32 count
)
{
	// FUNCTION BODY[0x563790]
	// Search equal code
	for ( set<res_declaration*, compare_predicate<res_declaration> >::iterator it = m_declarations.begin(); it != m_declarations.end(); ++it)
	{
		res_declaration* decl = *it;
		if( identity(decl->equal( dcl, count) && g_enable_resource_sharing))
			return decl;
	}

	// Create _new
	res_declaration* new_decl = NEW( res_declaration)( dcl, count);
	new_decl->mark_registered();
	m_declarations.insert( new_decl);

	return new_decl;
}

void resource_manager::release( res_declaration const* dcl )
{
	// FUNCTION BODY[0x563690]
	if( !dcl->is_registered())
		return;

	if( reclaim( m_declarations, dcl))
	{
		DELETE( dcl, resource_manager_call_destructor_predicate());
		return;
	}

	LOG_ERROR( "! ERROR: Failed to find compiled vertex-declarator");
}

res_signature* resource_manager::create_signature( ID3D10Blob* signature )
{
	// FUNCTION BODY[0x5614e0]
	for ( set<res_signature*, compare_predicate<res_signature> >::iterator it = m_signatures.begin(); it != m_signatures.end(); ++it)
	{
		if( identity((*it)->equal( signature) && g_enable_resource_sharing))
			return *it;
	}

	// Create _new
	res_signature* new_signature = NEW( res_signature)( signature);
	new_signature->mark_registered();
	m_signatures.insert( new_signature);

	return new_signature;
}

void resource_manager::release( res_signature const* signature )
{
	// FUNCTION BODY[0x5613e0]
	if( !signature->is_registered())
		return;

	if( reclaim( m_signatures, signature))
	{
		DELETE( signature, resource_manager_call_destructor_predicate());
		return;
	}

	LOG_ERROR( "! ERROR: Failed to find created signature.");
}

res_input_layout* resource_manager::create_input_layout(
	res_declaration const* decl,
	res_signature const* signature
)
{
	// FUNCTION BODY[0x562120]
	// Search equal code
	for ( set<res_input_layout*, compare_predicate<res_input_layout> >::iterator it = m_input_layouts.begin(); it != m_input_layouts.end(); ++it)
	{
		res_input_layout* layout = *it;
		if( identity(layout->equal( decl, signature) && g_enable_resource_sharing))
			return layout;
	}

	// Create _new
	res_input_layout* new_layout = NEW( res_input_layout)( decl, signature);
	new_layout->mark_registered();
	m_input_layouts.insert( new_layout);

	return new_layout;
}

void resource_manager::release( res_input_layout const* layout )
{
	// FUNCTION BODY[0x562020]
	if( !layout->is_registered())
		return;

	if( reclaim( m_input_layouts, layout))
	{
		DELETE( layout, resource_manager_call_destructor_predicate());
		return;
	}

	LOG_ERROR( "! ERROR: Failed to find created layout");
}

render_target* resource_manager::create_volume_render_target(
	pcstr name,
	u32 w,
	u32 h,
	u32 d,
	DXGI_FORMAT fmt,
	enum_rt_usage usage,
	D3D11_USAGE memory_usage
)
{
	// FUNCTION BODY[0x564fe0]
	map_rt::iterator it = m_rt_registry.find( name);

	if( name && it != m_rt_registry.end())
	{
		return it->second;
	}
	else
	{
		render_target*	rt = NEW( render_target);
		rt->set_name( name);
		rt->mark_registered();
		m_rt_registry.insert( mk_pair( rt->name().c_str(), rt));

		rt->create_3d( name, w, h, d, fmt, usage, memory_usage );

		return rt;
	}
}

render_target* resource_manager::create_render_target(
	pcstr name,
	u32 w,
	u32 h,
	DXGI_FORMAT fmt,
	enum_rt_usage usage,
	res_texture_ptr texture,
	u32 first_array_slice_index,
	D3D11_USAGE memory_usage,
	u32 sample_count,
	u32 mip_level_slice
)
{
	// FUNCTION BODY[0x564e30]
	// ***** first pass - search already created RT
	map_rt::iterator it = m_rt_registry.find( name);

	if( name && it != m_rt_registry.end())
	{
		return it->second;
	}
	else
	{
		render_target*	rt = NEW( render_target);
		rt->set_name( name);
		rt->mark_registered();
		m_rt_registry.insert( mk_pair( rt->name().c_str(), rt));

		if (texture)
		{
			rt->create( name, w, h, fmt, usage, texture, first_array_slice_index, mip_level_slice);
		}
		else
		{
			rt->create( name, w, h, fmt, usage, memory_usage, sample_count);
		}

		return rt;
	}
}

void resource_manager::release( render_target const* rt )
{
	// FUNCTION BODY[0x562360]
	if( !rt->is_registered())
		return;

	pcstr const name = rt->name().c_str();
	map_rt::iterator it	= m_rt_registry.find( name);

	if( it != m_rt_registry.end())
	{
		m_rt_registry.erase( it);
		DELETE( rt, resource_manager_call_destructor_predicate());
		return;
	}

	LOG_ERROR( "! ERROR: Failed to find render-target '%s'", rt->name().c_str());
}

shader_constant_buffer* resource_manager::create_constant_buffer(
	fixed_string<64> const& name,
	enum_shader_type dest,
	_D3D_CBUFFER_TYPE type,
	u32 size
)
{
	// FUNCTION BODY[0x561d90]
	for ( set<shader_constant_buffer*, constant_buffer_predicate>::iterator it = m_const_buffers.begin(); it != m_const_buffers.end(); ++it)
	{
		shader_constant_buffer*	buf		= *it;
		if( buf->type()		== type
			&& buf->size()		== size
			&& buf->dest()		== dest
			&& buf->name()		== name
			&& g_enable_resource_sharing)
				return buf;
	}
	shader_constant_buffer* cbuffer = NEW( shader_constant_buffer)( name, dest, type, size);

	cbuffer->mark_registered();
	m_const_buffers.insert	( cbuffer);
	return cbuffer;
}

void resource_manager::release( shader_constant_buffer const* cbuffer )
{
	// FUNCTION BODY[0x5612e0]
	if( !cbuffer->is_registered())
		return;

	if( reclaim( m_const_buffers, cbuffer))
	{
		DELETE( cbuffer, resource_manager_call_destructor_predicate());
		return;
	}

	LOG_ERROR( "!ERROR: Failed to find shader_constant buffer");
}

ID3D11SamplerState* resource_manager::create_sampler_state(
	sampler_state_descriptor const& sampler_props
)
{
	// FUNCTION BODY[0x5612c0]
	return m_sampler_cache.get_state( sampler_props.m_desc);
}

res_texture_list* resource_manager::create_texture_list(
	fixed_vector<texture_slot, 128> const& tex_list
)
{
	// FUNCTION BODY[0x561250]
	for( set<res_texture_list*, compare_member_predicate<res_texture_list> >::iterator it = m_texture_lists.begin(); it != m_texture_lists.end(); ++it)
	{
		res_texture_list* base = *it;
		if( identity(base->equal( tex_list) && g_enable_resource_sharing))
			return base;
	}

	res_texture_list* lst = NEW( res_texture_list)( tex_list);
	lst->mark_registered();
	m_texture_lists.insert( lst);
	return lst;
}

void resource_manager::release( res_texture_list const* tex_list )
{
	// FUNCTION BODY[0x561150]
	if( !tex_list->is_registered())
		return;

	if( reclaim( m_texture_lists, tex_list))
	{
		DELETE( tex_list, resource_manager_call_destructor_predicate());
		return;
	}

	LOG_ERROR( "!ERROR: Failed to find compiled list of textures");
}

res_sampler_list* resource_manager::create_sampler_list(
	fixed_vector<sampler_slot, 16> const& smp_list
)
{
	// FUNCTION BODY[0x5610e0]
	for( set<res_sampler_list*, compare_member_predicate<res_sampler_list> >::iterator it = m_sampler_lists.begin(); it != m_sampler_lists.end(); ++it)
	{
		res_sampler_list* base = *it;
		if( identity(base->equal( smp_list) && g_enable_resource_sharing))
			return base;
	}

	res_sampler_list* lst = NEW( res_sampler_list)( smp_list);
	lst->mark_registered();
	m_sampler_lists.insert( lst);
	return lst;
}

void resource_manager::release( res_sampler_list const* smp_list )
{
	// FUNCTION BODY[0x560fe0]
	if( !smp_list->is_registered())
		return;

	if( reclaim( m_sampler_lists, smp_list))
	{
		DELETE( smp_list, resource_manager_call_destructor_predicate());
		return;
	}

	LOG_ERROR( "!ERROR: Failed to find compiled list of samplers");
}

template <class shader, class binder>
bool operator == ( shader const* sh, binder const& bn )
{
	return  sh->equal( bn);
}

res_xs<vs_data>* resource_manager::create_vs( xs_descriptor<vs_data> const& binder )
{
	// FUNCTION BODY[0x5635f0]
	set<res_xs<vs_data>*, compare_shader_predicate<vs_data> >::iterator it = std::find( m_v_shaders.begin(), m_v_shaders.end(), binder);

	if( it != m_v_shaders.end())
	{
		return *it;
	}
	else
	{
		res_vs * vs = NEW(res_vs)( binder);
		vs->mark_registered();
		m_v_shaders.insert( vs);
		return vs;
	}
}

void resource_manager::release( res_xs<vs_data> const* vs )
{
	// FUNCTION BODY[0x5634f0]
	if( !vs->is_registered())
		return;

	if( reclaim( m_v_shaders, vs))
	{
		DELETE( vs, resource_manager_call_destructor_predicate());
		return;
	}

	LOG_ERROR( "!ERROR: Failed to find VS.");
}

res_xs<gs_data>* resource_manager::create_gs( xs_descriptor<gs_data> const& binder )
{
	// FUNCTION BODY[0x563450]
	set<res_xs<gs_data>*, compare_shader_predicate<gs_data> >::iterator it = std::find( m_g_shaders.begin(), m_g_shaders.end(), binder);

	if( it != m_g_shaders.end())
	{
		return *it;
	}
	else
	{
		res_gs * gs = NEW(res_gs)( binder);
		gs->mark_registered();
		m_g_shaders.insert( gs);
		return gs;
	}
}

void resource_manager::release( res_xs<gs_data> const* gs )
{
	// FUNCTION BODY[0x563350]
	if( !gs->is_registered())
		return;

	if( reclaim( m_g_shaders, gs))
	{
		DELETE( gs, resource_manager_call_destructor_predicate());
		return;
	}

	LOG_ERROR( "!ERROR: Failed to find GS.");
}

res_xs<ps_data>* resource_manager::create_ps( xs_descriptor<ps_data> const& binder )
{
	// FUNCTION BODY[0x5632b0]
	set<res_xs<ps_data>*, compare_shader_predicate<ps_data> >::iterator it = std::find( m_p_shaders.begin(), m_p_shaders.end(), binder);

	if( it != m_p_shaders.end())
	{
		return *it;
	}
	else
	{
		res_ps * ps = NEW(res_ps)( binder);
		ps->mark_registered();
		m_p_shaders.insert( ps);

		return ps;
	}
}

void resource_manager::release( res_xs<ps_data> const* ps )
{
	// FUNCTION BODY[0x5631b0]
	if( !ps->is_registered())
		return;

	if( reclaim( m_p_shaders, ps))
	{
		DELETE( ps, resource_manager_call_destructor_predicate());
		return;
	}

	LOG_ERROR( "!ERROR: Failed to find PS.");
}

res_geometry* resource_manager::create_geometry(
	D3D11_INPUT_ELEMENT_DESC const* decl,
	u32 decl_size,
	u32 vertex_stride,
	untyped_buffer& vb,
	untyped_buffer& ib
)
{
	R_ASSERT( decl);

	res_declaration_ptr dcl	= resource_manager::ref().create_declaration( decl, decl_size); return create_geometry( &*dcl, vertex_stride, vb, ib);
}

res_geometry* resource_manager::create_geometry(
	res_declaration* dcl,
	u32 vertex_stride,
	untyped_buffer& vb,
	untyped_buffer& ib
)
{
	res_geometry g( vb, ib, dcl, vertex_stride);

	set<res_geometry*, compare_member_predicate<res_geometry> >::iterator it = m_geometries.find( &g);

	if( it != m_geometries.end())
		return *it;

	res_geometry* geom = NEW( res_geometry)( vb, ib, dcl, vertex_stride);

	geom->mark_registered();
	m_geometries.insert( geom);

	return	geom;
}

void resource_manager::release( res_geometry const* geom )
{
	// FUNCTION BODY[0x564400]
	if( !geom->is_registered())
		return;

	if( reclaim( m_geometries, geom))
	{
		DELETE( geom, resource_manager_call_destructor_predicate());
		return;
	}
	LOG_ERROR( "!ERROR: Failed to find the geometry.");
}

res_render_output* resource_manager::create_render_output( HWND__* window, bool windowed )
{
	// FUNCTION BODY[0x564dc0]
	for ( u32 it=0; it<m_render_outputs.size(); it++)
	{
		if( m_render_outputs[it]->equal( window))
			return m_render_outputs[it];
	}

	res_render_output * r_output = NEW(res_render_output)( window, windowed);
	r_output->mark_registered();
	m_render_outputs.push_back( r_output);

	return r_output;
}

void resource_manager::release( res_render_output const* render_output )
{
	// FUNCTION BODY[0x560ee0]
	if( !render_output->is_registered())
		return;

	if( reclaim( m_render_outputs, render_output))
	{
		DELETE( render_output, resource_manager_call_destructor_predicate());
		return;
	}
	LOG_ERROR( "!ERROR: Failed to render output in registry.");
}

void resource_manager::copy( untyped_buffer* dest, untyped_buffer* source )
{
	// FUNCTION BODY[0x5608f0]
	device::ref().d3d_context()->CopyResource( dest->hardware_buffer(), source->hardware_buffer());
}

void resource_manager::copy2D(
	res_texture* dest,
	u32 dest_x,
	u32 dest_y,
	res_texture* source,
	u32 src_x,
	u32 src_y,
	u32 size_x,
	u32 size_y,
	u32 dest_mip,
	u32 src_mip
)
{
	// FUNCTION BODY[0x560890]
	ASSERT( src_x + size_x <= dest->width() && src_y + size_y <= dest->height() );

	D3D11_BOX box;
	box.left	= src_x;
	box.top		= src_y;
	box.front	= 0;
	box.right	= src_x + size_x;
	box.bottom	= src_y + size_y;
	box.back	= 1;

	device::ref().d3d_context()->CopySubresourceRegion( dest->hw_texture(),
														D3D11CalcSubresource( dest_mip, 0, dest->description().MipLevels),
														dest_x,
														dest_y,
														0,
														source->hw_texture(),
														src_mip,
														&box);
}

void resource_manager::register_sampler( pcstr name, ID3D11SamplerState* sampler )
{
	// FUNCTION BODY[0x560ab0]
	m_samplers_registry.push_back( mk_pair( fixed_string<64>(name), sampler) );
}

ID3D11SamplerState* resource_manager::find_registered_sampler( pcstr name )
{
	vector<std::pair<fixed_string<64>, ID3D11SamplerState*> >::const_iterator it	= m_samplers_registry.begin(),
																			 end	= m_samplers_registry.end();
	for( ; it != end; ++it)
	{
		if( it->first == name)
			return it->second;
	}

	return NULL;
}

} // namespace render
} // namespace vostok
