#ifndef VOSTOK_RENDER_CORE_DX11_RES_DECLARATION_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_RES_DECLARATION_H_INCLUDED

#include <boost/noncopyable.hpp>
#include <algorithm>
#include <cstring>
#include <vostok/intrusive_ptr.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/resource_intrusive_base.h>
#include <vostok/render/core/render_include.h>
#include <vostok/render/core/dx11/res_input_layout.h>
#include <vostok/render/core/dx11/res_signature.h>

namespace vostok {
namespace render {

class res_declaration;

struct signature_layout_pair {
	signature_layout_pair(
		res_declaration const* declaration,
		res_signature const* signature
	);

	bool operator<( res_signature const* other ) const { return &*signature < other; }
	bool operator==( res_signature const* other ) const { return &*signature == other; }

	res_input_layout_ptr input_layout;
	signature_const_ptr signature;
};

class res_declaration : public resource_intrusive_base, private boost::noncopyable {
public:
	res_declaration( D3D11_INPUT_ELEMENT_DESC const* declaration, u32 count );
	~res_declaration( );
	void destroy_impl( ) const;

	bool equal( D3D11_INPUT_ELEMENT_DESC const* other, u32 count ) const
	{
		return count == dcl_code.size( )
			&& (!count || std::memcmp( &dcl_code[0], other, count * sizeof( *other ) ) == 0);
	}

	res_input_layout* get( res_signature const* signature )
	{
		// FUNCTION BODY[0x129210]
		vector<signature_layout_pair>::iterator found = std::lower_bound(
			vs_to_layout.begin( ), vs_to_layout.end( ), signature
		);
		if ( found != vs_to_layout.end( ) && *found == signature )
			return &*found->input_layout;
		return &*vs_to_layout.insert(
			found,
			signature_layout_pair( this, signature )
		)->input_layout;
	}

	bool is_registered( ) const { return m_is_registered; }
	void mark_registered( ) { m_is_registered = true; }

	vector<signature_layout_pair> vs_to_layout;
	vector<D3D11_INPUT_ELEMENT_DESC> const dcl_code;
	bool m_is_registered;
};

inline s32 compare( res_declaration const&, res_declaration const& )
{
	// claude@NOTE: legacy body diverged - legacy has only member equal(desc*,count) keyed on a raw desc array, not a two-object s32 compare; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x1246a0]
	return 0;
}

inline bool operator<( res_declaration const& left, res_declaration const& right )
{
	return compare( left, right ) < 0;
}

STATIC_SIZE_ASSERT( signature_layout_pair, 0x8 );
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
