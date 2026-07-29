////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<D3D11_INPUT_ELEMENT_DESC,vostok::render::std_allocator<D3D11_INPUT_ELEMENT_DESC> >
	stlp_std::vector<D3D11_INPUT_ELEMENT_DESC >;
class D3D11_INPUT_ELEMENT_DESC;

namespace vostok {
namespace render {

class vector< D3D11_INPUT_ELEMENT_DESC > : public std::vector< D3D11_INPUT_ELEMENT_DESC > {
public:
	inline										vector<D3D11_INPUT_ELEMENT_DESC>	( ) { /* no source */ }
	inline										vector<D3D11_INPUT_ELEMENT_DESC>	( u32 arg_0, D3D11_INPUT_ELEMENT_DESC const& arg_1 ) { /* no source */ }
	inline	explicit							vector<D3D11_INPUT_ELEMENT_DESC>	( u32 arg_0 ) { /* no source */ }

	inline	D3D11_INPUT_ELEMENT_DESC&			operator[]							( u32 arg_0 ) { /* no source */ }
	inline	D3D11_INPUT_ELEMENT_DESC const&		operator[]							( u32 arg_0 ) const { /* no source */ }

	inline	u32									size								( ) const { /* no source */ }

	inline										~vector<D3D11_INPUT_ELEMENT_DESC>	( ) { /* no source */ }
}; // class vector< D3D11_INPUT_ELEMENT_DESC >

STATIC_SIZE_ASSERT(vector< D3D11_INPUT_ELEMENT_DESC >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
