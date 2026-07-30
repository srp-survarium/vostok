#include "pch.h"
#include <vostok/render/core/backend_handlers.h>

namespace vostok {
namespace render {

template < enum_shader_type shader_type >
constants_handler< shader_type >::~constants_handler( )
{
}

template < enum_shader_type shader_type >
void constants_handler< shader_type >::assign( shader_constant_table const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x12c5e0]
}

template < enum_shader_type shader_type >
void constants_handler< shader_type >::gather_data( )
{
	// STATE[STUB]
}

template < enum_shader_type shader_type >
void constants_handler< shader_type >::update_buffers( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x12cab0]
}

template < enum_shader_type shader_type >
void constants_handler< shader_type >::apply( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x73a130] for vertex shader
	// FUNCTION BODY[0x73a050] for pixel shader
	// FUNCTION BODY[0x73a0c0] for geometry shader
}

template < enum_shader_type shader_type >
void constants_handler< shader_type >::fill_changes_buffer( ID3D11Buffer**, u32& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x12c6e0]
}

template < enum_shader_type shader_type >
void constants_handler< shader_type >::check_for_unset_constants( )
{
	// STATE[STUB]
}

template < enum_shader_type shader_type >
textures_handler< shader_type >::~textures_handler( )
{
}

template < enum_shader_type shader_type >
void textures_handler< shader_type >::assign( res_texture_list const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x12c740]
}

template < enum_shader_type shader_type >
bool textures_handler< shader_type >::set_overwrite( pcstr, res_texture* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x12c7e0]
	return false;
}

template < enum_shader_type shader_type >
void textures_handler< shader_type >::apply( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x73a2c0] for vertex shader
	// FUNCTION BODY[0x73a1a0] for pixel shader
	// FUNCTION BODY[0x73a260] for geometry shader
}

template < enum_shader_type shader_type >
void textures_handler< shader_type >::fill_changes_buffer( ID3D11ShaderResourceView**, s32& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x12cd00]
}

template < enum_shader_type shader_type >
void textures_handler< shader_type >::check_for_unset_textures( )
{
	// STATE[STUB]
}

template < enum_shader_type shader_type >
samplers_handler< shader_type >::~samplers_handler( )
{
}

template < enum_shader_type shader_type >
void samplers_handler< shader_type >::assign( res_sampler_list const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x12cb20]
}

template < enum_shader_type shader_type >
void samplers_handler< shader_type >::apply( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x739ff0] for vertex shader
	// FUNCTION BODY[0x739f20] for pixel shader
	// FUNCTION BODY[0x739f80] for geometry shader
}

template < enum_shader_type shader_type >
void samplers_handler< shader_type >::fill_changes_buffer( ID3D11SamplerState**, u32& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x12cb90]
}

template < enum_shader_type shader_type >
void samplers_handler< shader_type >::check_for_unset_samplers( )
{
	// STATE[STUB]
}

template class constants_handler< enum_shader_type_vertex >;
template class constants_handler< enum_shader_type_pixel >;
template class constants_handler< enum_shader_type_geometry >;

template class textures_handler< enum_shader_type_vertex >;
template class textures_handler< enum_shader_type_pixel >;
template class textures_handler< enum_shader_type_geometry >;

template class samplers_handler< enum_shader_type_vertex >;
template class samplers_handler< enum_shader_type_pixel >;
template class samplers_handler< enum_shader_type_geometry >;

} // namespace render
} // namespace vostok
