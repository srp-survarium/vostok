#ifndef VOSTOK_RENDER_CORE_SHADER_CONSTANT_DEFINES_H_INCLUDED
#define VOSTOK_RENDER_CORE_SHADER_CONSTANT_DEFINES_H_INCLUDED

#include <vostok/math_float2.h>
#include <vostok/math_float3.h>
#include <vostok/math_float4.h>
#include <vostok/math_float4x4.h>
#include <vostok/math_int2.h>
#include <vostok/math_uint2.h>
#include <vostok/render/core/shader_defines.h>

namespace vostok {
namespace render {

enum enum_constant_type {
	rc_float = 0,
	rc_int = 1,
	rc_bool = 2,
	rc_INVALID = 0xFFFF,
};

enum {
	constant_class_size_shift = 0x08,
	constant_class_size_mask = 0x00FF,
	constant_class_cast_mask = 0xFF00,
};

enum enum_constant_class {
	rc_1x1 = (0 << constant_class_size_shift) + sizeof( float ),
	rc_1x2 = (0 << constant_class_size_shift) + 2 * sizeof( float ),
	rc_1x3 = (0 << constant_class_size_shift) + 3 * sizeof( float ),
	rc_1x4 = (0 << constant_class_size_shift) + 4 * sizeof( float ),
	rc_2x4 = (1 << constant_class_size_shift) + 8 * sizeof( float ),
	rc_3x4 = (1 << constant_class_size_shift) + 12 * sizeof( float ),
	rc_4x4 = (1 << constant_class_size_shift) + 16 * sizeof( float ),
	rc_1x4a = (2 << constant_class_size_shift) + 4 * sizeof( float ),
	rc_3x4a = (2 << constant_class_size_shift) + 12 * sizeof( float ),
	rc_4x4a = (2 << constant_class_size_shift) + 16 * sizeof( float ),
};

enum {
	cb_buffer_index_mask = 0xF,
	cb_buffer_type_mask = 0x30,
	cb_buffer_pixel_shader = 0x10,
	cb_buffer_vertex_shader = 0x20,
	cb_buffer_geometry_shader = 0x30,
	cb_buffer_max_count = 15,
};

template < typename T >
struct constant_type_traits {
	enum {
		value = false,
		type = static_cast<u32>( -1 ),
		class_id = static_cast<u32>( -1 ),
		size = static_cast<u32>( -1 ),
		type_id = static_cast<u32>( -1 ),
	};
};

#define VOSTOK_DECLARE_CONSTANT_TYPE_TRAITS(type_name, constant_type, constant_class, constant_id) \
	template <> struct constant_type_traits< type_name > { \
		enum { \
			value = true, \
			type = constant_type, \
			class_id = constant_class, \
			size = sizeof( type_name ), \
			type_id = constant_id, \
		}; \
	}

VOSTOK_DECLARE_CONSTANT_TYPE_TRAITS( float, rc_float, rc_1x1, 0 );
VOSTOK_DECLARE_CONSTANT_TYPE_TRAITS( math::float2, rc_float, rc_1x2, 1 );
VOSTOK_DECLARE_CONSTANT_TYPE_TRAITS( math::float3, rc_float, rc_1x3, 2 );
VOSTOK_DECLARE_CONSTANT_TYPE_TRAITS( math::float4, rc_float, rc_1x4, 3 );
VOSTOK_DECLARE_CONSTANT_TYPE_TRAITS( math::float4x4, rc_float, rc_4x4, 4 );
VOSTOK_DECLARE_CONSTANT_TYPE_TRAITS( int, rc_int, rc_1x1, 5 );
VOSTOK_DECLARE_CONSTANT_TYPE_TRAITS( math::int2, rc_int, rc_1x2, 6 );
VOSTOK_DECLARE_CONSTANT_TYPE_TRAITS( u32, rc_int, rc_1x1, 7 );
VOSTOK_DECLARE_CONSTANT_TYPE_TRAITS( math::uint2, rc_int, rc_1x2, 8 );

#undef VOSTOK_DECLARE_CONSTANT_TYPE_TRAITS

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_SHADER_CONSTANT_DEFINES_H_INCLUDED
