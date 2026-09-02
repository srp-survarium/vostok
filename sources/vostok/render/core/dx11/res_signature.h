// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_DX11_RES_SIGNATURE_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_RES_SIGNATURE_H_INCLUDED
#include <vostok/intrusive_ptr.h>
#include <vostok/render/core/resource_intrusive_base.h>
#include <vostok/render/core/render_include.h>

namespace vostok {
namespace render {

class resource_manager;

class res_signature : public resource_intrusive_base {
	friend class resource_intrusive_base;
	friend class resource_manager;

	explicit res_signature( ID3D10Blob* signature ) :
		m_signature( signature ),
		m_is_registered( false )
	{
		if ( m_signature )
			m_signature->AddRef( );
	}

	~res_signature( );
	void destroy_impl( ) const;

public:
	ID3D10Blob* hw_signature( ) const { return m_signature; }
	bool equal( ID3D10Blob const* other ) { return other == m_signature; }
	bool is_registered( ) const { return m_is_registered; }
	void mark_registered( ) { m_is_registered = true; }

private:
	ID3D10Blob*	m_signature;
	bool		m_is_registered;
};

typedef intrusive_ptr<
	res_signature,
	res_signature,
	threading::single_threading_policy
> input_layout_element_signature_ptr;

typedef intrusive_ptr<
	res_signature const,
	res_signature const,
	threading::single_threading_policy
> signature_const_ptr;

STATIC_SIZE_ASSERT( res_signature, 0xC );

inline bool operator<( res_signature const& left, res_signature const& right )
{
	return left.hw_signature( ) < right.hw_signature( );
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_RES_SIGNATURE_H_INCLUDED
