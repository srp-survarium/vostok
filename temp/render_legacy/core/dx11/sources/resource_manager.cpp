////////////////////////////////////////////////////////////////////////////
//	Created		: 01.05.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

// HARVEST REMAINDER: everything portable moved to
// sources/vostok/render/core/dx11/sources/resource_manager.cpp.
// What is left below is the un-ported remainder: the removed shader-source
// subsystem (m_sources / m_previous_sources / m_files_list are not members of
// the canonical class), functions whose canonical counterpart is a header
// inline stub, and the D3DX-dependent texture-loading path whose target
// control flow diverged (on_texture_loaded split into a managed-ptr overload).

#include "pch.h"
#include <vostok/render/core/resource_manager.h>
#include "shader_binary_source_cook.h"
#include "texture_storage.h"
#include <vostok/render/core/shader_constant_table.h>
#include <vostok/render/core/backend.h>
#include "texture_cook.h"
#include "dds.h"
#include <vostok/render/core/utils.h>
#include <vostok/render/core/res_texture.h>
#include <vostok/render/core/res_xs.h>
#include <vostok/render/core/xs_data.h>
#include "include_handler.h"
#include <vostok/render/core/res_xs_hw.h>
#include "com_utils.h"
#include <vostok/render/core/effect_manager.h>
#include "manager_common_inline.h"

#include <vostok/resources_fs.h>

#include <vostok/tasks_system.h>

#pragma warning( push )
#pragma warning( disable : 4995 )
#	if !USE_DX10
#		include <d3dx11tex.h>
#		include <d3dcompiler.h>
#	else // #if !USE_DX10
#		include <d3dx10tex.h>
#	endif // #if !USE_DX10
#pragma warning( pop )

namespace vostok {
namespace render {

// UNPORTED: canonical add_shader_code is a header inline stub returning false;
// m_sources / map_shader_sources do not exist on the canonical class.
bool resource_manager::add_shader_code(char const* shader_name, char const* source_code, bool rewrite_exists)
{
	resource_manager::shader_source source;
	source.time = 0;

	u32 code_length = static_cast_checked<u32>(strlen(source_code));
	source.data.resize(code_length);
	vostok::memory::copy(&source.data[0], code_length, source_code, code_length);

	if ( rewrite_exists || m_sources.find(shader_name)==m_sources.end() )
	{
		fs_new::virtual_path_string new_path;
		new_path.appendf("%s/%s", get_shader_path(), shader_name);
		m_sources[new_path.c_str()] = source;
		return true;
	}
	return false;
}

// UNPORTED: not a member of the canonical class (shader recompilation
// subsystem removed from the shipped resource_manager).
template<typename shader_data>
void resource_manager::recompile_xs_shaders(render::vector<fs_new::virtual_path_string> const& changed_files)
{
	render::vector<res_xs_hw<shader_data>*>			xs_shaders_to_reload;
	render::vector<fs_new::virtual_path_string>		shader_names;

	for (render::vector<fs_new::virtual_path_string>::const_iterator it = changed_files.begin(); it != changed_files.end(); ++it)
	{
		fs_new::virtual_path_string file_name = *it;
		file_name = file_name.substr(file_name.rfind('/')+1);
		file_name.set_length(file_name.rfind('.'));

		shader_names.push_back(file_name);
	}
	effect_manager::ref().recompile_shaders(shader_names);
	//effect_manager::ref().recompile_effects(xs_shaders_to_reload);
}

// UNPORTED: target instantiates create_xs_hw_impl for vs_data only (carcass
// has 0x1258c0 alone), so the gs/ps forwarders were NOT ported as forwarders;
// their target bodies (0x5643a0 / 0x564370, ~0x30 bytes each) are
// matcher-phase work.
res_gs_hw*	resource_manager::create_gs_hw( char const * name, shader_configuration shader_config, shader_include_getter* include_getter, binary_shader_sources_type* shader_sources)
{
	return create_xs_hw_impl<gs_data>( name, shader_config, include_getter, shader_sources);
}

res_ps_hw* resource_manager::create_ps_hw( LPCSTR name, shader_configuration shader_config, shader_include_getter* include_getter, binary_shader_sources_type* shader_sources)
{
	return create_xs_hw_impl<ps_data>( name, shader_config, include_getter, shader_sources);
}

// UNPORTED: target release(res_gs_hw)/release(res_ps_hw) are ~0x10 bytes
// (trivial), and release_impl has no gs/ps instantiation in the target.
void	resource_manager::release	( res_gs_hw const *			gs)
{
	release_impl(gs);
}

void	resource_manager::release	( res_ps_hw const *			ps)
{
	release_impl(ps);
}

// UNPORTED: canonical get_shader_path is a header inline returning "shaders/"
// (canonical header is ground truth); legacy constant kept for reference.
pcstr resource_manager::get_shader_path() const
{
	return "resources/shaders/sm_4_0";
}

static pcstr shader_type_to_extension(enum_shader_type const type)
{
	switch(type)
	{
		case enum_shader_type_vertex:	return "vs";
		case enum_shader_type_pixel:	return "ps";
		case enum_shader_type_geometry: return "gs";
		default: NODEFAULT(return "");
	}
}

// UNPORTED: canonical get_full_shader_path is a header inline stub returning
// virtual_path_string( short_path ).
fs_new::virtual_path_string resource_manager::get_full_shader_path(pcstr short_shader_path, enum_shader_type shader_type) const
{
	fs_new::virtual_path_string				 full_shader_name;
	full_shader_name.assignf					(
		"%s/%s.%s",
		resource_manager::ref().get_shader_path(),
		short_shader_path,
		shader_type_to_extension(shader_type)
	);
	return full_shader_name;
}

// UNPORTED: not members of the canonical class (m_sources removed).
resource_manager::shader_source& resource_manager::get_shader_source_by_short_name(pcstr shader_name, enum_shader_type shader_type, bool& out_is_found)
{
	return get_shader_source_by_full_name(get_full_shader_path(shader_name, shader_type).c_str(), out_is_found);
}

resource_manager::shader_source& resource_manager::get_shader_source_by_full_name(pcstr full_shader_name, bool& out_is_found)
{
	map_shader_sources::iterator found_source	= m_sources.find(full_shader_name);
	out_is_found								= found_source != m_sources.end();
	return										found_source->second;
}

static bool is_file_extension(pcstr file_name, pcstr ext)
{
	s32 len = strings::length(file_name) - 1;

	if (len > 0)
	{
		while (len >= 0)
		{
			if (file_name[len]=='.')
				return strings::equal(&file_name[len + 1], ext);

			--len;
		}
	}

	return strings::equal("", ext);
}

// UNPORTED: canonical process_files is a header inline stub { };
// m_files_list / m_file_times_list are not members of the canonical class.
void resource_manager::process_files( vfs::vfs_iterator const &	begin)
{
	vector<vfs::vfs_iterator>	stack;
	stack.push_back( begin);
	vector<resources::request>	requests;
	while ( !stack.empty())
	{
		vfs::vfs_iterator curDir = stack.back();
		stack.pop_back();

		vfs::vfs_iterator it  = curDir.children_begin(),
							end = curDir.children_end();

		for ( ; it != end; ++it)
		{
			if( it.is_folder())
			{
				stack.push_back( it);
			}
			else
			{
				fs_new::virtual_path_string	const & file_name	=	it.get_virtual_path();

				fs_new::native_path_string const &  source_absolute_path	=	it.get_physical_path();

				fs_new::physical_path_info const & source_info	=	resources::get_physical_path_info(source_absolute_path);

				if (!is_file_extension(file_name.c_str(), "ps") &&
					!is_file_extension(file_name.c_str(), "vs") &&
					!is_file_extension(file_name.c_str(), "gs") &&
					!is_file_extension(file_name.c_str(), "h"))
					continue;

				map_shader_sources::iterator found_source = m_sources.find(file_name);
				if (found_source==m_sources.end() || found_source->second.time < source_info.last_time_of_write() )
				{
					m_files_list.push_back( file_name);
					m_file_times_list.push_back( source_info.last_time_of_write() );
				}
			}
		}
	}

	if( m_files_list.empty())
	{
		m_loading_incomplete = false;
		return;
	}

	//to ensure that list does not reallocate
	for ( u32 i = 0; i < m_files_list.size(); ++i)
	{
		resources::request	req;
		req.path = m_files_list[i].c_str();
		req.id   = resources::raw_data_class_no_reuse;
		requests.push_back( req);
	}

	resources::query_resources( &requests[0], requests.size(),
		boost::bind( &resource_manager::on_raw_files_load, this, _1),
		::vostok::render::g_allocator);
}

// UNPORTED: dev-only (compiled out of the MASTER_GOLD target); canonical
// header carries the empty inline stub.
#ifndef MASTER_GOLD
void resource_manager::on_texture_source_changed(vostok::vfs::vfs_notification const & info)
{
	if (!info.physical_new_path && !info.physical_path)
		return;

	if (( info.type==vostok::vfs::vfs_notification::type_modified ||
		  info.type==vostok::vfs::vfs_notification::type_renamed )
		  && ((info.physical_new_path && strings::length(info.physical_new_path)) || (info.physical_path && strings::length(info.physical_path))))
	{
		pstr textures_position			= (info.physical_path && strings::length(info.physical_path))
										?
										strstr(info.physical_path, "textures\\") :
										strstr(info.physical_new_path, "textures\\");

		// Photoshop temporary file
		if (info.physical_new_path && strstr(info.physical_new_path, ".tmp") ||
			info.physical_path && strstr(info.physical_path, ".tmp"))
			return;

		pstr textures_position_it		=	textures_position;
		while (*textures_position_it)
		{
			if (*textures_position_it=='\\')
				*textures_position_it = '/';

			textures_position_it++;
		}

		vostok::fs_new::virtual_path_string result	= textures_position;

		change_substring				(&result, "textures/", "");
		change_substring				(&result, ".tga", "");
		change_substring				(&result, ".options", "");

		map_texture::iterator found		= m_texture_registry.find(result.c_str());

		if (found!=m_texture_registry.end())
			if (std::find(m_textures_to_reload.begin(), m_textures_to_reload.end(), found->first)==m_textures_to_reload.end())
				m_textures_to_reload.push_back(found->first);
	}
}
#endif // #ifndef MASTER_GOLD

#if 0
void	resource_manager::initialize_texture_storage	( vostok::configs::lua_config_value const & cfg)
{
	size_t const memory_amount = device::ref().video_memory_size();

	map_texture::iterator		it = m_texture_registry.begin();
	map_texture::const_iterator	end = m_texture_registry.end();

	size_t mem_usage = 0;
	for( ; it != end; ++it)
		mem_usage += utils::calc_texture_size( it->second->width(), it->second->height(), it->second->format(), it->second->mips_count());

	size_t memory_to_use = memory_amount - mem_usage - 128*1024*1024; //mb
	float staging_percent = 2.f;

 	m_texture_storage->initialize			( cfg, size_t(memory_to_use * (100 - staging_percent)/100), D3D_USAGE_DEFAULT);
	m_texture_storage_staging->initialize	( cfg, size_t(memory_to_use * staging_percent / 100), D3D_USAGE_STAGING);
}
#endif

// UNPORTED: not a member of the canonical class (recompilation subsystem
// removed).
void resource_manager::recompile_shaders( render::vector<pcstr> const& changed_global_defines_list)
{
	render::vector<fs_new::virtual_path_string> shader_files_to_recompile;

	for (map_shader_sources::const_iterator shd_it=m_sources.begin(); shd_it!=m_sources.end(); ++shd_it)
	{
		shader_declarated_macroses_list in_shader_declared_defines_list;

		if (!found_shader_declarated_macroses(pcstr(&shd_it->second.data[0]), in_shader_declared_defines_list))
			continue;

		for (render::vector<pcstr>::const_iterator def_it=changed_global_defines_list.begin(); def_it!=changed_global_defines_list.end(); ++def_it)
		{
			if (std::find(in_shader_declared_defines_list.begin(), in_shader_declared_defines_list.end(), *def_it) != in_shader_declared_defines_list.end())
			{
				shader_files_to_recompile.push_back( shd_it->first );
			}
		}
	}

	recompile_xs_shaders<ps_data>(shader_files_to_recompile);
	//recompile_xs_shaders<vs_data>(shader_files_to_recompile);
	//recompile_xs_shaders<gs_data>(shader_files_to_recompile);
}

// UNPORTED: canonical on_raw_files_load is a header inline stub { };
// m_sources / m_previous_sources / m_files_list removed from the canonical
// class.
void resource_manager::on_raw_files_load( resources::queries_result& data)
{
	ASSERT( data.is_successful());
	ASSERT( data.size() == m_files_list.size());

	m_loading_incomplete = false;

	for ( u32 i = 0; i < data.size(); ++i)
	{
#pragma message (VOSTOK_TODO("Lain 2 IronNick: just save managed_resource_ptr here instead of using vector"))
		vostok::resources::managed_resource_ptr	shader_source	=	data[i].get_managed_resource();
		R_ASSERT								(shader_source);
		vostok::resources::pinned_ptr_const<u8>	shader_source_buffer	(shader_source);

		fs_new::virtual_path_string file_path			 = m_files_list[i];
		if (m_sources.find(file_path)!=m_sources.end())
			m_previous_sources[file_path] = m_sources[file_path];

		m_sources[file_path].data.assign(
			shader_source_buffer.c_ptr(),
			shader_source_buffer.c_ptr() + shader_source_buffer.size());

		m_sources[file_path].time = m_file_times_list[i];
	}

	if (m_is_shader_reloading && m_need_recompile_shader_if_source_reloaded)
	{
		recompile_xs_shaders<ps_data>(m_files_list);
		//recompile_xs_shaders<vs_data>(m_files_list);
		//recompile_xs_shaders<gs_data>(m_files_list);

		m_is_shader_reloading = false;
	}

	//release file names and times list
	{
		vector<fs_new::virtual_path_string>	empty;
		m_files_list.swap( empty);

		vector<u32> empty2;
		m_file_times_list.swap(empty2);
	}
}

// UNPORTED: not a member of the canonical class (shader-source loading
// subsystem removed; canonical reload_shader_sources only flips the two
// reload flags).
void resource_manager::load_shader_sources()
{
	m_files_list.clear();
	m_file_times_list.clear();

	m_loading_incomplete = true;

	resources::query_vfs_iterator( get_shader_path(),
		boost::bind( &resource_manager::process_files, this, _1 ),
		::vostok::render::g_allocator, resources::recursive_true);

	if (!m_is_shader_reloading)
	while ( m_loading_incomplete)
	{
		if( threading::g_debug_single_thread)
			resources::tick				();

		resources::dispatch_callbacks	();
	}
}

// UNPORTED: no canonical counterpart (no carcass stub for this free helper).
void on_shader_loaded2(resources::managed_resource_ptr* out_result, vostok::resources::queries_result& data)
{
	if (!data[0].is_successful())
		return;

	*out_result = data[0].get_managed_resource();
}

// UNPORTED: target control flow diverged - the canonical class splits texture
// loading into on_texture_loaded(queries_result&, u32, bool, u32) [0x562de0]
// and a new on_texture_loaded(managed_resource_ptr, pcstr, u32) [0x562400]
// (the big body) that has no legacy ancestor; this legacy body also depends
// on D3DX11 helpers and texture_data_resource. Matcher-phase work.
void resource_manager::on_texture_loaded( resources::queries_result& data, u32 mip_level_cut, bool use_converter)
{
	fs_new::virtual_path_string name( data[0].get_requested_path());

 	if( !data.is_successful())//  || !data[0].get_unmanaged_resource())
 	{
 		LOG_ERROR( "Texture %s was not found!", name.c_str());
 		return;
 	}

	fix_texture_name(name);

	textures_to_reload_vector::iterator to_delete	= std::find(m_textures_to_reload.begin(), m_textures_to_reload.end(), name);
	if (to_delete!=m_textures_to_reload.end())
		m_textures_to_reload.erase					(to_delete);

	res_texture* tex = create_texture( name.c_str());

	if (use_converter)
	{
		resources::managed_resource_ptr managed_ptr = data[0].get_managed_resource();
		resources::pinned_ptr_const< texture_data_resource > managed_typed_ptr( managed_ptr );

		render::texture_data_resource const* resource	= managed_typed_ptr.c_ptr();

		u8 const *	dds_ptr		= (u8 const *) resource->buffer().c_ptr();
		u32			dds_size	= resource->buffer().size();

		bool is_srgb_option = read_srgb_flag(dds_ptr, dds_size);

		dds_size				-= sizeof(bool);

		tex->m_rescale_min = float4(0.0f, 0.0f, 0.0f, 0.0f);
		tex->m_rescale_max = float4(1.0f, 1.0f, 1.0f, 1.0f);

		D3DX_IMAGE_INFO	dds_info = {0};
		CHECK_RESULT( D3DXGetImageInfoFromMemory( dds_ptr, dds_size, 0, &dds_info, 0));

		D3DX_IMAGE_LOAD_INFO load_info;

		bool staging = false;
		if( staging)
		{
			load_info.Usage			= D3D_USAGE_STAGING;
			load_info.BindFlags		= 0;
			load_info.CpuAccessFlags = D3D_CPU_ACCESS_WRITE;
		}
		else
		{
			load_info.BindFlags		= D3D_BIND_SHADER_RESOURCE;
			load_info.pSrcInfo		= &dds_info;
			load_info.Format		= dds_info.Format;
		}

		ID3DBaseTexture* base_tex = NULL;
		HRESULT res = D3DXCreateTextureFromMemory(
			device::ref().d3d_device(),
			dds_ptr,
			dds_size,
			&load_info,
			0,
			&base_tex,
			0
		);

		ID3DBaseTexture* base_tex_copy = NULL;

		if (is_srgb_option)
			base_tex_copy			= make_copy_with_srgb_format(base_tex);

		CHECK_RESULT(res);

		tex->set_hw_texture(is_srgb_option ? base_tex_copy : base_tex, mip_level_cut, false, is_srgb_option);

		if (base_tex)
			base_tex->Release();

		if (base_tex_copy)
			base_tex_copy->Release();
	}
	else
	{
		resources::pinned_ptr_const<u8>	ptr_man(data[0].get_managed_resource());

		D3DX_IMAGE_INFO	dds_info = {0};
		CHECK_RESULT( D3DXGetImageInfoFromMemory( ptr_man.c_ptr(), ptr_man.size(), 0, &dds_info, 0));

		D3DX_IMAGE_LOAD_INFO load_info;

		bool staging = false;
		if( staging)
		{
			load_info.Usage			= D3D_USAGE_STAGING;
			load_info.BindFlags		= 0;
			load_info.CpuAccessFlags = D3D_CPU_ACCESS_WRITE;
		}
		else
		{
			load_info.Usage		= D3D_USAGE_DEFAULT;
			load_info.BindFlags	= D3D_BIND_SHADER_RESOURCE;
		}

		ID3DBaseTexture* base_tex = NULL;
		HRESULT res = D3DXCreateTextureFromMemory( device::ref().d3d_device(),
			ptr_man.c_ptr(),
			ptr_man.size(),
			&load_info,
			0,
			&base_tex,
			0);
		CHECK_RESULT( res);
		tex->set_hw_texture( base_tex, mip_level_cut);
		base_tex->Release();
	}
}

// UNPORTED: target on_texture_loaded_staging [0x560550] is ~0x10 bytes
// (empty body - the staging texture pool is disabled in the shipped build);
// porting this large legacy body would diverge from the target. Also depends
// on dds.h / load_dds_info / texture_storage::get.
void resource_manager::on_texture_loaded_staging( resources::queries_result& data, u32 mip_level_cut, bool use_converter)
{
 	fs_new::virtual_path_string name( data[0].get_requested_path());

 	if( !data.is_successful())// || !data[0].get_unmanaged_resource())
 	{
 		LOG_ERROR( "Texture %s not found!", name.c_str());
 		return;
 	}

	fix_texture_name(name);

	res_texture* tex = create_texture( name.c_str());

	if (use_converter)
	{
		resources::managed_resource_ptr managed_ptr = data[0].get_managed_resource();
		resources::pinned_ptr_const< texture_data_resource > managed_typed_ptr( managed_ptr );

		render::texture_data_resource const* resource	= managed_typed_ptr.c_ptr();

		u8 const *	dds_ptr		= (u8 const *) resource->buffer().c_ptr();
		u32			dds_size	= resource->buffer().size();

		u16	rescale_min_max[8];
		vostok::memory::copy(rescale_min_max, sizeof(rescale_min_max), dds_ptr + dds_size - sizeof(rescale_min_max), sizeof(rescale_min_max));

		tex->m_rescale_min = float4(0.0f, 0.0f, 0.0f, 0.0f);
		tex->m_rescale_max = float4(1.0f, 1.0f, 1.0f, 1.0f);

		dds_info dds_info;
		if( !load_dds_info( dds_ptr, dds_size, dds_info))
		{
			on_texture_loaded( data);
			return;
		}

		if( dds_info.mips_count != utils::calc_mipmap_count( dds_info.width, dds_info.height))
		{
			on_texture_loaded( data);
			return;
		}

		if( mip_level_cut >= dds_info.mips_count-1 )
		{
			ASSERT( 0);
			return;
		}

		u32 resize_factor = 1 << mip_level_cut;

		u32 orig_size =  utils::calc_texture_size( dds_info.width, dds_info.height, dds_info.format, dds_info.mips_count);

		dds_info.width		/=	resize_factor;
		dds_info.height		/=	resize_factor;
		dds_info.mips_count -=	mip_level_cut;
		dds_info.data_size	=	utils::calc_texture_size( dds_info.width, dds_info.height, dds_info.format, dds_info.mips_count);
		dds_info.data_ptr	+=	( orig_size - dds_info.data_size);

		res_texture * pool_texture		= m_texture_storage->get( dds_info.width, dds_info.height, dds_info.format);
		res_texture * staging_texture	= m_texture_storage_staging->get( dds_info.width, dds_info.height, dds_info.format);

		if( !pool_texture || !staging_texture)
			return;

		tex->set_hw_texture( pool_texture->hw_texture());

		load_dds_to_texture( dds_info, staging_texture);

		device::ref().d3d_context()->CopyResource( tex->hw_texture(), staging_texture->hw_texture());

		m_texture_storage_staging->release( staging_texture);
	}
	else
	{
		resources::pinned_ptr_const<u8>	ptr_man(data[0].get_managed_resource());

		dds_info dds_info;
		if( !load_dds_info( ptr_man.c_ptr(), ptr_man.size(), dds_info))
		{
			on_texture_loaded( data);
			return;
		}


		if( dds_info.mips_count != utils::calc_mipmap_count( dds_info.width, dds_info.height))
		{
			on_texture_loaded( data);
			return;
		}

		if( mip_level_cut >= dds_info.mips_count-1 )
		{
			ASSERT( 0);
			return;
		}

		u32 resize_factor = 1 << mip_level_cut;

		u32 orig_size =  utils::calc_texture_size( dds_info.width, dds_info.height, dds_info.format, dds_info.mips_count);

		dds_info.width		/=	resize_factor;
		dds_info.height		/=	resize_factor;
		dds_info.mips_count -=	mip_level_cut;
		dds_info.data_size	=	utils::calc_texture_size( dds_info.width, dds_info.height, dds_info.format, dds_info.mips_count);
		dds_info.data_ptr	+=	( orig_size - dds_info.data_size);

		res_texture * pool_texture		= m_texture_storage->get( dds_info.width, dds_info.height, dds_info.format);
		res_texture * staging_texture	= m_texture_storage_staging->get( dds_info.width, dds_info.height, dds_info.format);

		if( !pool_texture || !staging_texture)
			return;

		tex->set_hw_texture( pool_texture->hw_texture());

		load_dds_to_texture( dds_info, staging_texture);

		device::ref().d3d_context()->CopyResource( tex->hw_texture(), staging_texture->hw_texture());

		m_texture_storage_staging->release( staging_texture);
	}
}

// UNPORTED: canonical create_texture_cube is a header inline stub returning 0
// (no target symbol).
res_texture* resource_manager::create_texture_cube( char const * user_name, u32 width, u32 height, D3D_SUBRESOURCE_DATA const * data, DXGI_FORMAT format, u32 usage, u32 mip_levels, bool use_as_depth_stencil)
{
	D3D_TEXTURE2D_DESC			desc;
	desc.Width					= width;
	desc.Height					= height;
	desc.MipLevels				= 1;
	desc.ArraySize				= 6;
	desc.SampleDesc.Count		= 1;
	desc.SampleDesc.Quality		= 0;
	desc.Usage					= (D3D_USAGE)usage;
	desc.CPUAccessFlags			= 0;
	desc.Format					= format;
	desc.MiscFlags				= D3D_RESOURCE_MISC_TEXTURECUBE;
	desc.MipLevels				= mip_levels;
	desc.BindFlags				= D3D_BIND_SHADER_RESOURCE | (use_as_depth_stencil ? D3D_BIND_DEPTH_STENCIL : D3D_BIND_RENDER_TARGET);

	ID3DTexture2D* d3d_texture  = NULL;
	CHECK_RESULT( device::ref().d3d_device()->CreateTexture2D( &desc, data, &d3d_texture));
	R_ASSERT					( d3d_texture );

	res_texture* tex = NEW		( res_texture );
	tex->set_name				( user_name );
	m_texture_registry.insert	( utils::mk_pair( tex->name(), tex));

	tex->mark_registered		();
	tex->set_hw_texture			( d3d_texture);
	d3d_texture->Release		();

	return tex;
}

// UNPORTED: the canonical load_texture_delegate::execute is an empty stub in
// the new TU (no target symbol evidence); this real body depends on D3DX11 +
// texture_data_resource.
struct load_texture_delegate
{
	load_texture_delegate( ref_texture dest_texture, math::rectangle<math::int2> dest_rect, u32 arr_ind, char const* user_name):
			dest_texture		( dest_texture),
			dest_rect			( dest_rect),
			user_name			( user_name),
			arr_ind				( arr_ind)
			{}


	void execute( resources::queries_result& data)
	{
		if ( !data.is_successful() || !data[0].get_managed_resource())
		{
			dest_texture = 0;
			DELETE ((load_texture_delegate*)this);
			return;
		}

		fs_new::virtual_path_string name(data[0].get_requested_path());
		(void)&name;

		resources::managed_resource_ptr managed_ptr = data[0].get_managed_resource();
		resources::pinned_ptr_const< texture_data_resource > managed_typed_ptr( managed_ptr );

		render::texture_data_resource const* resource	= managed_typed_ptr.c_ptr();

		u8 const *	dds_ptr		= (u8 const *) resource->buffer().c_ptr();
		u32			dds_size	= resource->buffer().size();

		bool is_srgb			= read_srgb_flag(dds_ptr, dds_size);

		dds_size				-= sizeof(is_srgb);

		D3DX_IMAGE_INFO	dds_info = {0};
		CHECK_RESULT( D3DXGetImageInfoFromMemory( dds_ptr, dds_size, 0, &dds_info, 0));

		D3D_TEXTURE2D_DESC	dest_desc	= dest_texture->description ();

		if(	dds_info.Width			> dest_desc.Width
			|| dds_info.Height		> dest_desc.Height
			|| dds_info.Depth		!= 1
			|| !is_equal_formats(dds_info.Format, dest_desc.Format))
		{
			LOG_ERROR( "The loaded image is not compatible with destination texture!" ) ;
			return;
		}

		D3DX_IMAGE_LOAD_INFO load_info;

		load_info.Usage				= D3D_USAGE_DEFAULT;
		load_info.BindFlags			= 0;
		load_info.CpuAccessFlags	= 0;

		ID3DBaseTexture*	src_tex	= NULL;

		HRESULT res = D3DXCreateTextureFromMemory(
			device::ref().d3d_device(),
			dds_ptr,
			dds_size,
			&load_info,
			0,
			&src_tex,
			0
		);

		if (is_srgb)
			src_tex					= make_copy_with_srgb_format(src_tex);

		CHECK_RESULT( res);

		u32 mip_width	= dds_info.Width;
		u32 mip_height	= dds_info.Height;

		u32 size_diff	= math::min(dest_desc.Width / dds_info.Width, dest_desc.Height / dds_info.Height);

		for (u32 i = 0; i < dest_desc.MipLevels; i++)
		{
			for (u32 y = 0; y < size_diff; y++)
			for (u32 x = 0; x < size_diff; x++)
			{
				D3D_BOX		b;
				b.left		= 0;
				b.top		= 0;
				b.right		= mip_width;
				b.bottom	= mip_height;
				b.front		= 0;
				b.back		= 1;

				device::ref().d3d_context()->CopySubresourceRegion( dest_texture->hw_texture(), D3D10CalcSubresource( i, arr_ind, dest_desc.MipLevels), x * mip_width, y * mip_height, 0, src_tex, i, &b);

				device::ref().d3d_context()->Flush();
			}

			mip_width /= 2;
			mip_height /= 2;
		}

		DELETE ((load_texture_delegate*)this);
	}

private:
	math::rectangle<math::int2>	dest_rect;
	ref_texture					dest_texture;
	char const*					user_name;
	u32							arr_ind;
};

// UNPORTED: canonical copy_texture_from_file is a header inline stub
// returning false (no target symbol).
bool resource_manager::copy_texture_from_file( res_texture * dest_texture, math::rectangle<math::int2> dest_rect, u32 arr_ind, char const* src_name)
{
	fixed_string<512> path( "resources/textures/");

	path += src_name;
	path += ".dds";

	load_texture_delegate* load_delegate = NEW ( load_texture_delegate) ( dest_texture, dest_rect, arr_ind, src_name);

	resources::query_resource( path.c_str(),
		resources::texture_wrapper_class,
		boost::bind( &load_texture_delegate::execute, load_delegate, _1 ),
		::vostok::render::g_allocator);

	dest_texture->set_name( src_name );
	m_texture_registry.insert( utils::mk_pair( src_name, dest_texture));

	return true;
}

// UNPORTED: canonical copy( untyped_buffer*, u32, untyped_buffer*, u32, u32 )
// is a header inline stub { } (no target symbol).
void resource_manager::copy		( untyped_buffer* dest, u32 dest_pos, untyped_buffer* source, u32 src_pos, u32 size)
{
	D3D_BOX box;
	box.left	= src_pos;
	box.top		= 0;
	box.front	= 0;
	box.right	= src_pos + size;
	box.bottom	= 1;
	box.back	= 1;

	device::ref().d3d_context()->CopySubresourceRegion( dest->hardware_buffer(), 0, dest_pos, 0, 0, source->hardware_buffer(), 0, &box);
}

// UNPORTED: canonical copy3D is a header inline stub { } (no target symbol).
void resource_manager::copy3D	(res_texture* dest,
								 u32 dest_x,
								 u32 dest_y,
								 u32 dest_z,
								 res_texture* source,
								 u32 src_x,
								 u32 src_y,
								 u32 src_z,
								 u32 size_x,
								 u32 size_y,
								 u32 size_z,
								 u32 dest_mip,
								 u32 src_mip)
{

#pragma message (VOSTOK_TODO("Add depth() function to ref_texture or add new type ref_texture_3d?"))

	D3D_BOX box;
	box.left	= src_x;
	box.top		= src_y;
	box.front	= src_z;
	box.right	= src_x + size_x;
	box.bottom	= src_y + size_y;
	box.back	= src_z + size_z;

	device::ref().d3d_context()->CopySubresourceRegion( dest->hw_texture(),
														D3D10CalcSubresource( dest_mip, 0, dest->description().MipLevels),
														dest_x,
														dest_y,
														dest_z,
														source->hw_texture(),
														src_mip,
														&box);
}

} // namespace render
} // namespace vostok
