// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_SHADER_CONSTANT_HOST_H_INCLUDED
#define VOSTOK_RENDER_CORE_SHADER_CONSTANT_HOST_H_INCLUDED
// claude@NOTE: the available developer sources contain declarations but no bodies for these overloads.
#include <vostok/math_float2.h>
#include <vostok/math_float3.h>
#include <vostok/math_float4.h>
#include <vostok/math_float4x4.h>
#include <vostok/math_int2.h>
#include <vostok/shared_string.h>
#include <vostok/render/core/shader_constant_defines.h>
#include <vostok/render/core/shader_constant_slot.h>
#include <vostok/render/core/shader_constant_source.h>

namespace vostok {
namespace render {

class shader_constant_host {
public:
	shader_constant_host( shared_string const& name, enum_constant_type type );

	void set( float ) { /* STATE[STUB] */ }
	void set( math::float2 const& ) { /* STATE[STUB] */ }
	void set( math::float3 const& ) { /* STATE[STUB] */ }
	void set( math::float4 const& ) { /* STATE[STUB] */ }
	void set( math::float4x4 const& ) { /* STATE[STUB] */ }
	void set( s32 ) { /* STATE[STUB] */ }
	void set( math::int2 const& ) { /* STATE[STUB] */ }

	void reset_update_markers( )
	{
		m_update_markers[0] = 0;
		m_update_markers[1] = 0;
		m_update_markers[2] = 0;
	}

	shader_constant_source const& source( ) const { return m_source; }
	shader_constant_slot const& shader_slot( u32 index ) const
	{
		R_ASSERT_CMP( index, <, enum_shader_types_count );
		return m_shader_slots[index];
	}

	shader_constant_slot& shader_slot( u32 index )
	{
		R_ASSERT_CMP( index, <, enum_shader_types_count );
		return m_shader_slots[index];
	}

	shared_string const& name( ) const { return m_name; }
	enum_constant_type type( ) const { return m_type; }
	shader_constant_source m_source;
	shader_constant_slot m_shader_slots[enum_shader_types_count];
	shared_string m_name;
	u32 m_update_markers[enum_shader_types_count];
	enum_constant_type m_type;
};

} // namespace render
} // namespace vostok

#include <vostok/render/core/shader_constant_host_inline.h>

#endif // #ifndef VOSTOK_RENDER_CORE_SHADER_CONSTANT_HOST_H_INCLUDED
