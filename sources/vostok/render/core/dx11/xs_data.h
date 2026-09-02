// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_DX11_XS_DATA_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_XS_DATA_H_INCLUDED
#include <vostok/fixed_vector.h>
#include <vostok/render/core/shader_data_base.h>
#include <vostok/render/core/shader_defines.h>
#include <vostok/render/core/shader_constant_table.h>
#include <vostok/render/core/dx11/res_signature.h>
#include <vostok/render/core/dx11/sampler_slot.h>
#include <vostok/render/core/dx11/texture_slot.h>

namespace vostok {
namespace render {

struct vs_data : public shader_data_base {
	typedef ID3D11VertexShader hw_interface;
	enum { type = enum_shader_type_vertex };

	vs_data( ) : hardware_shader( 0 ) { }

	ID3D11VertexShader*				hardware_shader;
	shader_constant_table			constants;
	fixed_vector<sampler_slot, 16>	samplers;
	fixed_vector<texture_slot, 128>	textures;
	signature_const_ptr				signature;
};

struct gs_data : public shader_data_base {
	typedef ID3D11GeometryShader hw_interface;
	enum { type = enum_shader_type_geometry };

	gs_data( ) : hardware_shader( 0 ) { }

	ID3D11GeometryShader*			hardware_shader;
	shader_constant_table			constants;
	fixed_vector<sampler_slot, 16>	samplers;
	fixed_vector<texture_slot, 128>	textures;
};

struct ps_data : public shader_data_base {
	typedef ID3D11PixelShader hw_interface;
	enum { type = enum_shader_type_pixel };

	ps_data( ) : hardware_shader( 0 ) { }

	ID3D11PixelShader*				hardware_shader;
	shader_constant_table			constants;
	fixed_vector<sampler_slot, 16>	samplers;
	fixed_vector<texture_slot, 128>	textures;
};

STATIC_SIZE_ASSERT( vs_data, 0x2F7C );
STATIC_SIZE_ASSERT( gs_data, 0x2F78 );
STATIC_SIZE_ASSERT( ps_data, 0x2F78 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_XS_DATA_H_INCLUDED
