// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_DX11_SHADER_CONSTANT_BUFFER_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_SHADER_CONSTANT_BUFFER_H_INCLUDED
#include <vostok/fixed_string.h>
#include <vostok/intrusive_ptr.h>
#include <vostok/render/core/render_include.h>
#include <vostok/render/core/resource_intrusive_base.h>
#include <vostok/render/core/shader_constant.h>
#include <vostok/render/core/shader_constant_defines.h>
#include <vostok/render/core/shader_constant_slot.h>
#include <vostok/render/core/shader_defines.h>

namespace vostok {
namespace render {

class resource_manager;

class shader_constant_buffer : public resource_intrusive_base {
protected:
	friend class resource_manager;

	shader_constant_buffer(
		fixed_string<64> const& name,
		enum_shader_type destination,
		D3D_CBUFFER_TYPE type,
		u32 size
	);
	~shader_constant_buffer( );

public:
	void destroy_impl( ) const;

	void set( shader_constant const& constant );
	void zero( shader_constant const& constant );

	template < typename T >
	void set_typed( shader_constant_slot const& slot, T const& value );

	template < typename T >
	void set_typed( shader_constant_slot const& slot, T const* values, u32 array_size );

	bool similiar( shader_constant_buffer const& other ) const
	{
		return m_name == other.m_name && m_type == other.m_type
			&& m_dest == other.m_dest && m_buffer_size == other.m_buffer_size;
	}

	void update( );
	ID3D11Buffer* hardware_buffer( ) const;

private:
	void set( shader_constant_slot const& slot, void* pointer, u32 size );
	void set( shader_constant_slot const& slot, void* pointer, u32 size, u32 array_size );
	void zero( shader_constant_slot const& slot, u32 size );
	void set_memory( u32 const offset, pcstr source, u32 const size );
	void zero_memory( u32 const offset, u32 const size );
	void* access( u32 offset );

public:
	fixed_string<64> const& name( ) const;
	enum_shader_type& dest( );
	enum_shader_type const& dest( ) const;
	D3D_CBUFFER_TYPE type( ) const;
	u32 size( ) const;
	bool is_registered( ) const { return m_is_registered; }
	void mark_registered( )
	{
		R_ASSERT( !m_is_registered );
		m_is_registered = true;
	}

	enum { line_size = 1 };

private:
	fixed_string<64> m_name;
	D3D_CBUFFER_TYPE m_type;
	enum_shader_type m_dest;
	void* m_buffer_data;
	u32 m_buffer_size;
	ID3D11Buffer* m_hardware_buffer;
	bool m_changed;
	bool m_is_registered;
};

typedef intrusive_ptr<
	shader_constant_buffer,
	resource_intrusive_base,
	threading::single_threading_policy
> shader_constant_buffer_ptr;

typedef intrusive_ptr<
	shader_constant_buffer const,
	resource_intrusive_base const,
	threading::single_threading_policy
> shader_constant_buffer_const_ptr;

} // namespace render
} // namespace vostok

#include <vostok/render/core/dx11/shader_constant_buffer_inline.h>

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_SHADER_CONSTANT_BUFFER_H_INCLUDED
