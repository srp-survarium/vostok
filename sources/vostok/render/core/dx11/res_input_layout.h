#ifndef VOSTOK_RENDER_CORE_DX11_RES_INPUT_LAYOUT_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_RES_INPUT_LAYOUT_H_INCLUDED

#include <vostok/intrusive_ptr.h>
#include <vostok/render/core/resource_intrusive_base.h>
#include <vostok/render/core/render_include.h>
#include <vostok/render/core/dx11/res_signature.h>

namespace vostok {
namespace render {

class res_declaration;

class res_input_layout : public resource_intrusive_base {
	friend bool operator<( res_input_layout const& left, res_input_layout const& right );

public:
	res_input_layout( res_declaration const* declaration, res_signature const* signature );
	~res_input_layout( );
	void destroy_impl( ) const;

	bool equal(
		res_declaration const* declaration,
		res_signature const* signature
	) const
	{
		return m_declaration == declaration && m_signature == signature;
	}
	bool is_registered( ) const { return m_is_registered; }
	void mark_registered( ) { m_is_registered = true; }
	ID3D11InputLayout* hardware_layout( ) { return m_hw_input_layout; }

private:
	ID3D11InputLayout* m_hw_input_layout;
	res_declaration const* m_declaration;
	signature_const_ptr m_signature;
	bool m_is_registered;
};

typedef intrusive_ptr<
	res_input_layout,
	resource_intrusive_base,
	threading::single_threading_policy
> res_input_layout_ptr;

STATIC_SIZE_ASSERT( res_input_layout, 0x14 );

inline bool operator<( res_input_layout const& left, res_input_layout const& right )
{
	if ( left.m_declaration < right.m_declaration )
		return true;
	if ( left.m_declaration > right.m_declaration )
		return false;
	return left.m_signature < right.m_signature;
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_RES_INPUT_LAYOUT_H_INCLUDED
