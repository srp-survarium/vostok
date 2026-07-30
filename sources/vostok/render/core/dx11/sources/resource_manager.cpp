#include "pch.h"
#include <vostok/render/core/resource_manager.h>

namespace vostok {
namespace render {

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
	// STATE[STUB]
	// FUNCTION BODY[0x124a20]
	return false;
}

bool is_equal_formats( DXGI_FORMAT, DXGI_FORMAT )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5607b0]
	return false;
}

bool read_srgb_flag( pcbyte, u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x560990]
	return false;
}

DXGI_FORMAT get_typeless_format( DXGI_FORMAT format )
{
	// STATE[STUB]
	// FUNCTION BODY[0x560710]
	return format;
}

void begin_command_list( D3D11_QUERY_DESC&, ID3D11Query*& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x560970]
}

void end_command_list( ID3D11Query*& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x560920]
}

ID3D11Resource* make_copy_with_srgb_format( ID3D11Resource* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x561bd0]
	return 0;
}

template <>
res_xs_hw<vs_data>* resource_manager::create_xs_hw_impl<vs_data>(
	pcstr,
	shader_configuration,
	shader_include_getter*,
	binary_shader_sources_type*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x1258c0]
	return 0;
}

res_xs_hw<vs_data>* resource_manager::create_vs_hw(
	pcstr name,
	shader_configuration config,
	shader_include_getter* include_getter,
	binary_shader_sources_type* sources
)
{
	// STATE[STUB]
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
	// STATE[STUB]
	// FUNCTION BODY[0x564370]
	return 0;
}

void resource_manager::bind_samplers_to_shaders( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x560d30]
}

template <>
void resource_manager::release_impl<vs_data>( res_xs_hw<vs_data> const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x1264b0]
}

void resource_manager::release( res_xs_hw<vs_data> const* shader )
{
	// STATE[STUB]
	// FUNCTION BODY[0x562dd0]
	release_impl<vs_data>( shader );
}

void resource_manager::release( res_xs_hw<gs_data> const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x562dc0]
}

void resource_manager::release( res_xs_hw<ps_data> const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x562db0]
}

pcstr resource_manager::get_converted_shader_path( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x560700]
	return 0;
}

pcstr get_textures_path2( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5606f0]
	return 0;
}

resource_manager::resource_manager( configs::binary_config_ptr const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x563e20]
}

resource_manager::~resource_manager( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5638f0]
}

void change_substring( fs_new::virtual_path_string*, pcstr, pcstr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x560c30]
}

res_texture_ptr create_color_grading_base_lut( u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x561b20]
	return res_texture_ptr( );
}

res_texture_ptr resource_manager::get_color_grading_base_lut( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x561fa0]
	return m_color_grading_base_lut;
}

void resource_manager::reload_modified_textures( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x564640]
}

void resource_manager::reload_shader_sources( bool )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5606e0]
}

shader_constant_table* resource_manager::create_const_table(
	shader_constant_table const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5622c0]
	return 0;
}

void resource_manager::release( shader_constant_table const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5621c0]
}

shader_constant_host const* resource_manager::register_constant_binding(
	shader_constant_binding const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x563890]
	return 0;
}

res_texture* resource_manager::find_texture( pcstr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x560c00]
	return 0;
}

res_texture* resource_manager::create_texture(
	pcstr,
	resources::query_result_for_cook*,
	u32,
	bool,
	bool,
	bool,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x564d20]
	return 0;
}

void fix_texture_name( fs_new::virtual_path_string& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x560e90]
}

u32 calc_bytes_per_block( DXGI_FORMAT )
{
	// STATE[STUB]
	// FUNCTION BODY[0x560660]
	return 0;
}

u32 calc_block_size( DXGI_FORMAT )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5605f0]
	return 0;
}

DXGI_FORMAT find_srgb_format( DXGI_FORMAT format, bool )
{
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
	// STATE[STUB]
	// FUNCTION BODY[0x562de0]
}

u32 resource_manager::get_texture_video_memory_size( )
{
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
	// STATE[STUB]
	// FUNCTION BODY[0x560550]
}

void resource_manager::reload_all_textures( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x564c50]
}

res_texture* resource_manager::load_texture(
	pcstr,
	resources::query_result_for_cook*,
	u32,
	bool,
	bool,
	bool,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x564810]
	return 0;
}

res_texture* resource_manager::create_texture2d_impl(
	u32,
	u32,
	D3D11_SUBRESOURCE_DATA const*,
	DXGI_FORMAT,
	D3D11_USAGE,
	u32,
	u32,
	bool
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5619b0]
	return 0;
}

void resource_manager::release_impl( res_texture const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x560a70]
}

res_texture* resource_manager::create_texture2d(
	pcstr,
	u32,
	u32,
	D3D11_SUBRESOURCE_DATA const*,
	DXGI_FORMAT,
	D3D11_USAGE,
	u32,
	u32,
	bool
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x561e60]
	return 0;
}

res_texture* resource_manager::create_texture3d(
	pcstr,
	u32,
	u32,
	u32,
	D3D11_SUBRESOURCE_DATA const*,
	DXGI_FORMAT,
	u32,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5617c0]
	return 0;
}

void resource_manager::release( res_texture const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x560bc0]
}

untyped_buffer* resource_manager::create_buffer(
	u32,
	pcvoid,
	enum_buffer_type,
	bool,
	bool
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x561740]
	return 0;
}

void resource_manager::release( untyped_buffer const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x560a20]
}

res_state* resource_manager::create_state( state_descriptor& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x561690]
	return 0;
}

void resource_manager::release( res_state const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5615a0]
}

res_declaration* resource_manager::create_declaration(
	D3D11_INPUT_ELEMENT_DESC const*,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x563790]
	return 0;
}

void resource_manager::release( res_declaration const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x563690]
}

res_signature* resource_manager::create_signature( ID3D10Blob* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5614e0]
	return 0;
}

void resource_manager::release( res_signature const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5613e0]
}

res_input_layout* resource_manager::create_input_layout(
	res_declaration const*,
	res_signature const*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x562120]
	return 0;
}

void resource_manager::release( res_input_layout const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x562020]
}

render_target* resource_manager::create_volume_render_target(
	pcstr,
	u32,
	u32,
	u32,
	DXGI_FORMAT,
	enum_rt_usage,
	D3D11_USAGE
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x564fe0]
	return 0;
}

render_target* resource_manager::create_render_target(
	pcstr,
	u32,
	u32,
	DXGI_FORMAT,
	enum_rt_usage,
	res_texture_ptr,
	u32,
	D3D11_USAGE,
	u32,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x564e30]
	return 0;
}

void resource_manager::release( render_target const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x562360]
}

shader_constant_buffer* resource_manager::create_constant_buffer(
	fixed_string<64> const&,
	enum_shader_type,
	_D3D_CBUFFER_TYPE,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x561d90]
	return 0;
}

void resource_manager::release( shader_constant_buffer const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5612e0]
}

ID3D11SamplerState* resource_manager::create_sampler_state(
	sampler_state_descriptor const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5612c0]
	return 0;
}

res_texture_list* resource_manager::create_texture_list(
	fixed_vector<texture_slot, 128> const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x561250]
	return 0;
}

void resource_manager::release( res_texture_list const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x561150]
}

res_sampler_list* resource_manager::create_sampler_list(
	fixed_vector<sampler_slot, 16> const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5610e0]
	return 0;
}

void resource_manager::release( res_sampler_list const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x560fe0]
}

res_xs<vs_data>* resource_manager::create_vs( xs_descriptor<vs_data> const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5635f0]
	return 0;
}

void resource_manager::release( res_xs<vs_data> const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5634f0]
}

res_xs<gs_data>* resource_manager::create_gs( xs_descriptor<gs_data> const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x563450]
	return 0;
}

void resource_manager::release( res_xs<gs_data> const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x563350]
}

res_xs<ps_data>* resource_manager::create_ps( xs_descriptor<ps_data> const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5632b0]
	return 0;
}

void resource_manager::release( res_xs<ps_data> const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5631b0]
}

res_geometry* resource_manager::create_geometry(
	D3D11_INPUT_ELEMENT_DESC const*,
	u32,
	u32,
	untyped_buffer&,
	untyped_buffer&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5645e0]
	return 0;
}

res_geometry* resource_manager::create_geometry(
	res_declaration*,
	u32,
	untyped_buffer&,
	untyped_buffer&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x564500]
	return 0;
}

void resource_manager::release( res_geometry const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x564400]
}

res_render_output* resource_manager::create_render_output( HWND__*, bool )
{
	// STATE[STUB]
	// FUNCTION BODY[0x564dc0]
	return 0;
}

void resource_manager::release( res_render_output const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x560ee0]
}

void resource_manager::copy( untyped_buffer*, untyped_buffer* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5608f0]
}

void resource_manager::copy2D(
	res_texture*,
	u32,
	u32,
	res_texture*,
	u32,
	u32,
	u32,
	u32,
	u32,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x560890]
}

void resource_manager::register_sampler( pcstr, ID3D11SamplerState* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x560ab0]
}

ID3D11SamplerState* resource_manager::find_registered_sampler( pcstr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5609a0]
	return 0;
}

} // namespace render
} // namespace vostok
