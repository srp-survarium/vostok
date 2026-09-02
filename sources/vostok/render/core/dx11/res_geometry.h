// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_DX11_RES_GEOMETRY_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_RES_GEOMETRY_H_INCLUDED
#include <vostok/intrusive_ptr.h>
#include <vostok/render/core/resource_intrusive_base.h>
#include <vostok/render/core/untyped_buffer.h>
#include <vostok/render/core/dx11/res_declaration.h>

namespace vostok {
namespace render {

class resource_manager;

typedef intrusive_ptr<
	untyped_buffer const,
	resource_intrusive_base const,
	threading::single_threading_policy
> untyped_buffer_const_ptr;

class res_geometry : public resource_intrusive_base {
	friend class resource_intrusive_base;
	friend class resource_manager;

	res_geometry(
		untyped_buffer& vertex_buffer,
		untyped_buffer& index_buffer,
		res_declaration* declaration,
		u32 stride
	) :
		m_vb( &vertex_buffer ),
		m_ib( &index_buffer ),
		m_vb_stride( stride ),
		m_dcl( declaration ),
		m_is_registered( false )
	{
	}

	~res_geometry( );
	void destroy_impl( ) const;

public:
	bool equal( res_geometry const& other ) const { return compare( other ) == 0; }
	s32 compare( res_geometry const& other ) const;
	void apply( );
	bool is_registered( ) const { return m_is_registered; }
	void mark_registered( ) { m_is_registered = true; }
	u32 get_stride( ) { return m_vb_stride; }
	untyped_buffer_const_ptr vertex_buffer( ) { return &*m_vb; }
	untyped_buffer_const_ptr intex_buffer( ) { return &*m_ib; }

	public:
	untyped_buffer_ptr m_vb;
	untyped_buffer_ptr m_ib;
	u32 m_vb_stride;
	res_declaration_ptr m_dcl;

	private:
	bool m_is_registered;
};

typedef intrusive_ptr<
	res_geometry,
	resource_intrusive_base,
	threading::single_threading_policy
> res_geometry_ptr;

STATIC_SIZE_ASSERT( res_geometry, 0x18 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_RES_GEOMETRY_H_INCLUDED
