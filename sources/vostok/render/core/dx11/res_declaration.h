#ifndef VOSTOK_RENDER_CORE_DX11_RES_DECLARATION_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_RES_DECLARATION_H_INCLUDED

#include <boost/noncopyable.hpp>
#include <vostok/intrusive_ptr.h>
#include <vostok/render/core/resource_intrusive_base.h>

struct D3D11_INPUT_ELEMENT_DESC;

namespace vostok {
namespace render {

class res_declaration :
	public resource_intrusive_base,
	private boost::noncopyable
{
public:
	res_declaration( D3D11_INPUT_ELEMENT_DESC const* declaration, u32 count );
	~res_declaration( ) { }

	void destroy_impl( ) const { }

private:
	unsigned char m_carcass[0x1C];
};

STATIC_SIZE_ASSERT( res_declaration, 0x20 );

typedef intrusive_ptr<
	res_declaration,
	resource_intrusive_base,
	threading::single_threading_policy
> res_declaration_ptr;

typedef res_declaration_ptr ref_declaration;

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_RES_DECLARATION_H_INCLUDED
