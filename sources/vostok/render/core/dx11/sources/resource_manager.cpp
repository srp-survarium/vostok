#include "pch.h"
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
#include "manager_common_inline.h"

#include <vostok/resources.h>
#include <vostok/resources_fs.h>

#include <vostok/tasks_system.h>

#include <algorithm>

namespace vostok {
namespace render {

bool g_enable_resource_sharing = true;

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
	shader_constant_buffer const* const,
	shader_constant_buffer const* const
) const
{
	// claude@NOTE: no legacy ancestor - constant_buffer_predicate is new-in-target (legacy kept a plain vector with linear search); matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x124a20]
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
		return					0;
	}
}

template <>
res_xs_hw<vs_data>* resource_manager::create_xs_hw_impl<vs_data>(
	pcstr name,
	shader_configuration shader_config,
	shader_include_getter* include_getter,
	binary_shader_sources_type* shader_sources
)
{
	// FUNCTION BODY[0x1258c0]
	VOSTOK_UNREFERENCED_PARAMETER	(include_getter);

	if (!name)
		return NULL;

	fs_new::virtual_path_string	reg_name			(name);
	map_vs_hw::iterator it				= m_vs_hw_registry.find( shader_name_config_pair( reg_name.c_str(), shader_config));

	// TODO: Add check of global defines to shader_configuration
//	if (!m_is_shader_reloading && it != xs_hw_registry.end())
//	{
//		return it->second;
//	}
//	else
	{
		binary_shader_sources_type::iterator found_binary_source_it = shader_sources->find(
			binary_shader_key_type(
				name,
				(enum_shader_type)vs_data::type,
				shader_config
			)
		);

		if (found_binary_source_it != shader_sources->end())
		{
			res_xs_hw<vs_data>* xs_hw	= NEW(res_xs_hw<vs_data>);
			xs_hw->mark_registered		();
			xs_hw->set_name				(reg_name.c_str() );

			m_vs_hw_registry[
				shader_name_config_pair(xs_hw->name().c_str(), shader_config)
			]							= xs_hw;

			if (FAILED(xs_hw->create_hw_shader((ID3D10Blob*)found_binary_source_it->second->m_shader_byte_code)))
			{
				LOG_ERROR				(
					"!%s: %s",
					shader_type_traits<(enum_shader_type)vs_data::type>::short_name(),
					name
				);
				LOG_ERROR				("shader creation failed");
				FATAL					("!Shader is not created");
			}
			return xs_hw;
		}
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
	// FUNCTION BODY[0x5643d0]
	return create_xs_hw_impl<vs_data>( name, config, include_getter, sources );
}

res_xs_hw<gs_data>* resource_manager::create_gs_hw(
	pcstr,
	shader_configuration,
	shader_include_getter*,
	binary_shader_sources_type*
)
{
	// claude@NOTE: legacy forwards to create_xs_hw_impl<gs_data>; the target DOES carry that
	// instantiation (??$create_xs_hw_impl@Ugs_data@... is in match.db), so the shipped source
	// had ONE generic create_xs_hw_impl template instantiated for vs/gs/ps rather than the
	// vs_data-only explicit specialization this TU currently holds; genericising it (registry
	// + shader_type via traits) and restoring the forwards is matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x5643a0]
	return 0;
}

res_xs_hw<ps_data>* resource_manager::create_ps_hw(
	pcstr,
	shader_configuration,
	shader_include_getter*,
	binary_shader_sources_type*
)
{
	// claude@NOTE: legacy forwards to create_xs_hw_impl<ps_data>; the target DOES carry that
	// instantiation (??$create_xs_hw_impl@Ups_data@... is in match.db) - see create_gs_hw above;
	// matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x564370]
	return 0;
}

void resource_manager::bind_samplers_to_shaders( )
{
	// claude@NOTE: no legacy ancestor - bind_samplers_to_shaders has no legacy ancestor; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x560d30]
}

template <>
void resource_manager::release_impl<vs_data>( res_xs_hw<vs_data> const* xs_hw )
{
	// FUNCTION BODY[0x1264b0]
	if( !xs_hw->is_registered())
		return;

	map_vs_hw::iterator begin = m_vs_hw_registry.begin(), end = m_vs_hw_registry.end(), it = m_vs_hw_registry.begin();

	while( identity(true))
	{
		if ( it==end)
		{
			DELETE( xs_hw, resource_manager_call_destructor_predicate());
			break;
		}
		else if ( it->second==xs_hw)
		{
			m_vs_hw_registry.erase(it);
			DELETE( xs_hw, resource_manager_call_destructor_predicate());
			break;
		}
		++it;
	}
}

void resource_manager::release( res_xs_hw<vs_data> const* shader )
{
	// FUNCTION BODY[0x562dd0]
	release_impl<vs_data>( shader );
}

void resource_manager::release( res_xs_hw<gs_data> const* )
{
	// claude@NOTE: legacy body diverged - legacy release_impl has no gs instantiation in the target (~0x10-byte target body, see legacy UNPORTED note); matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x562dc0]
}

void resource_manager::release( res_xs_hw<ps_data> const* )
{
	// claude@NOTE: legacy body diverged - legacy release_impl has no ps instantiation in the target; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x562db0]
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

resource_manager::resource_manager( configs::binary_config_ptr const& config ) :
	shader_name_to_mask_config	( config),
	m_loading_incomplete		( false),
	m_is_shader_reloading		( false),
	m_need_recompile_shader_if_source_reloaded( true),
	m_texture_storage			( NULL),
	m_texture_storage_staging	( NULL),
	m_compile_error_handler		( NULL)
{
	// FUNCTION BODY[0x563e20]
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
	// FUNCTION BODY[0x5638f0]
	for( u32 i = 0; i< m_states.size(); ++i)
		DELETE( m_states[i], resource_manager_call_destructor_predicate());

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
				this, _1, 0, true, 0
			),
			::vostok::render::g_allocator
		);
	}
}

void resource_manager::reload_shader_sources( bool is_recompile_shaders )
{
	// FUNCTION BODY[0x5606e0]
	m_is_shader_reloading					   = true;
	m_need_recompile_shader_if_source_reloaded = is_recompile_shaders;
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

u32 calc_bytes_per_block( DXGI_FORMAT )
{
	// claude@NOTE: no legacy ancestor - calc_bytes_per_block has no legacy ancestor; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x560660]
	return 0;
}

u32 calc_block_size( DXGI_FORMAT )
{
	// claude@NOTE: no legacy ancestor - calc_block_size has no legacy ancestor; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x5605f0]
	return 0;
}

DXGI_FORMAT find_srgb_format( DXGI_FORMAT format, bool )
{
	// claude@NOTE: no legacy ancestor - find_srgb_format has no legacy ancestor; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x560560]
	return format;
}

void resource_manager::on_texture_loaded(
	resources::managed_resource_ptr,
	pcstr,
	u32
)
{
	// claude@NOTE: no legacy ancestor - the managed-ptr on_texture_loaded split has no legacy ancestor (see legacy UNPORTED note); matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x562400]
}

void resource_manager::on_texture_loaded(
	resources::queries_result&,
	u32,
	bool,
	u32
)
{
	// claude@NOTE: legacy body diverged - legacy is the 3-param D3DX11/texture_data_resource path; the target gained a fourth u32 and split the flow; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x562de0]
}

u32 resource_manager::get_texture_video_memory_size( )
{
	// claude@NOTE: no legacy ancestor - get_texture_video_memory_size has no legacy ancestor; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x560850]
	return 0;
}

void resource_manager::on_texture_loaded_staging(
	resources::queries_result&,
	u32,
	bool
)
{
	// claude@NOTE: legacy body diverged - signature matches but the shipped body is empty (staging pool disabled in the shipped build, see legacy UNPORTED note); matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x560550]
}

void resource_manager::reload_all_textures( )
{
	// claude@NOTE: no legacy ancestor - reload_all_textures has no legacy ancestor; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x564c50]
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
	// FUNCTION BODY[0x564810]
	resources::class_id_enum class_id = use_converter ? resources::texture_wrapper_class : resources::raw_data_class;
	fs_new::virtual_path_string path			  = "resources/textures/";

	if( !use_pool)
	{
		res_texture* tex = NEW( res_texture);
		tex->set_name( texture_name );
		tex->mark_registered();
		m_texture_registry.insert( mk_pair( tex->name(), tex));

		if( strstr( tex->name(), "$user$") == 0 && texture_name && texture_name[0])
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
					::vostok::render::g_allocator,
					0,
					parent);
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
	bool
)
{
	// FUNCTION BODY[0x5619b0]
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
	texure_desc.BindFlags			= (usage == D3D11_USAGE_STAGING) ? 0 : D3D11_BIND_SHADER_RESOURCE;
	texure_desc.CPUAccessFlags		= ((usage == D3D11_USAGE_DYNAMIC) || (usage == D3D11_USAGE_STAGING)) ? D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ : 0;

	ID3D11Texture2D*	d3d_texture;
	HRESULT res = device::ref().d3d_device()->CreateTexture2D( &texure_desc, data, &d3d_texture);
	CHECK_RESULT( res);

	res_texture* tex = NEW		( res_texture);

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
	// FUNCTION BODY[0x561e60]
	res_texture* tex = create_texture2d_impl( width, height, data, format, usage, mip_levels, array_size, use_as_render_target);
	tex->set_name( user_name);
	std::pair<map_texture::iterator, bool> res = m_texture_registry.insert	( mk_pair( tex->name(), tex));

	ASSERT( res.second, "A texture with the specified name already exists in texture registry.");

	if( !res.second)
		return NULL;

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
	// FUNCTION BODY[0x5617c0]
	D3D11_TEXTURE3D_DESC		desc;
	desc.Width					=	width;
	desc.Height					=	height;
	desc.Depth					=	depth;
	desc.MipLevels				=	mip_levels;
	desc.Format					=	format;
	desc.Usage					=	(D3D11_USAGE)usage;

	desc.BindFlags				=	(usage == D3D11_USAGE_IMMUTABLE) ?
									 D3D11_BIND_SHADER_RESOURCE : ((usage == D3D11_USAGE_STAGING) ? 0 : D3D11_BIND_SHADER_RESOURCE);

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
		else if (usage == D3D11_USAGE_STAGING)
		{
			desc.CPUAccessFlags	=	D3D11_CPU_ACCESS_READ;
		}
	}

	desc.MiscFlags				=	0;

	ID3D11Texture3D* d3d_texture =	NULL;

	CHECK_RESULT				(device::ref().d3d_device()->CreateTexture3D( &desc, data, &d3d_texture));
	R_ASSERT					(d3d_texture);

	res_texture* tex = NEW		(res_texture);
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
	// FUNCTION BODY[0x561740]
	untyped_buffer* new_buffer = NEW(untyped_buffer)( size, data, type, dynamic, staging);

	m_buffers.push_back( new_buffer);

	return new_buffer;
}

void resource_manager::release( untyped_buffer const* buffer )
{
	// FUNCTION BODY[0x560a20]
	if( reclaim( m_buffers, buffer))
	{
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
	// FUNCTION BODY[0x5645e0]
	R_ASSERT( decl);

	res_declaration_ptr dcl	= resource_manager::ref().create_declaration( decl, decl_size);

	return create_geometry( &*dcl, vertex_stride, vb, ib);
}

res_geometry* resource_manager::create_geometry(
	res_declaration* dcl,
	u32 vertex_stride,
	untyped_buffer& vb,
	untyped_buffer& ib
)
{
	// FUNCTION BODY[0x564500]
	u32 vb_stride = vertex_stride;
	res_geometry g( vb, ib, dcl, vb_stride);

	set<res_geometry*, compare_member_predicate<res_geometry> >::const_iterator it = m_geometries.begin();
	set<res_geometry*, compare_member_predicate<res_geometry> >::const_iterator end = m_geometries.end();
	for( ; it != end ; ++it)
	{
		if( identity((*it)->equal( g) && g_enable_resource_sharing))
			return *it;
	}

	res_geometry* geom = NEW( res_geometry)( vb, ib, dcl, vb_stride);
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
	// FUNCTION BODY[0x5609a0]
	vector<std::pair<fixed_string<64>, ID3D11SamplerState*> >::const_iterator it	= m_samplers_registry.begin();
	vector<std::pair<fixed_string<64>, ID3D11SamplerState*> >::const_iterator end	= m_samplers_registry.end();

	for( ; it != end; ++it)
	{
		if( it->first == name)
			return it->second;
	}

	return NULL;
}

} // namespace render
} // namespace vostok
