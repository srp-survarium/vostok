#ifndef VOSTOK_RENDER_CORE_SHADER_DEFINES_H_INCLUDED
#define VOSTOK_RENDER_CORE_SHADER_DEFINES_H_INCLUDED

#include <vostok/render/core/render_include.h>

namespace vostok {
namespace render {

enum enum_shader_type {
	enum_shader_type_vertex,
	enum_shader_type_pixel,
	enum_shader_type_geometry,
	enum_shader_types_count,
};

template < enum_shader_type shader_type >
struct shader_type_traits {
	enum { value = false };
};

struct vs_data;

template < >
struct shader_type_traits< enum_shader_type_vertex > {
	enum { value = true };
	typedef ID3D11VertexShader shader_hw_interface;
	typedef vs_data shader_data;
	static pcstr name( ) { return "vertex shader"; }
	static pcstr short_name( ) { return "vs"; }
};

struct ps_data;

template < >
struct shader_type_traits< enum_shader_type_pixel > {
	enum { value = true };
	typedef ID3D11PixelShader shader_hw_interface;
	typedef ps_data shader_data;
	static pcstr name( ) { return "pixel shader"; }
	static pcstr short_name( ) { return "ps"; }
};

struct gs_data;

template < >
struct shader_type_traits< enum_shader_type_geometry > {
	enum { value = true };
	typedef ID3D11GeometryShader shader_hw_interface;
	typedef gs_data shader_data;
	static pcstr name( ) { return "geometry shader"; }
	static pcstr short_name( ) { return "gs"; }
};

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_SHADER_DEFINES_H_INCLUDED
