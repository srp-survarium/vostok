////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<_D3DVERTEXELEMENT9,vostok::render::std_allocator<_D3DVERTEXELEMENT9> >
	stlp_std::vector<_D3DVERTEXELEMENT9 >;
class _D3DVERTEXELEMENT9;

namespace vostok {
namespace render {

class vector< _D3DVERTEXELEMENT9 > : public std::vector< _D3DVERTEXELEMENT9 > {
public:
	inline									vector<_D3DVERTEXELEMENT9>	( ) { /* no source */ }
	inline									vector<_D3DVERTEXELEMENT9>	( u32 arg_0, _D3DVERTEXELEMENT9 const& arg_1 ) { /* no source */ }
	inline	explicit						vector<_D3DVERTEXELEMENT9>	( u32 arg_0 ) { /* no source */ }

	inline	_D3DVERTEXELEMENT9&				operator[]					( u32 arg_0 ) { /* no source */ }
	inline	_D3DVERTEXELEMENT9 const&		operator[]					( u32 arg_0 ) const { /* no source */ }

	inline	u32								size						( ) const { /* no source */ }

	inline									~vector<_D3DVERTEXELEMENT9>	( ) { /* no source */ }
}; // class vector< _D3DVERTEXELEMENT9 >

STATIC_SIZE_ASSERT(vector< _D3DVERTEXELEMENT9 >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
