////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<ID3D11SamplerState *,vostok::render::std_allocator<ID3D11SamplerState *> >
	stlp_std::vector<ID3D11SamplerState * >;
class ID3D11SamplerState *;

namespace vostok {
namespace render {

class vector< ID3D11SamplerState* > : public std::vector< ID3D11SamplerState* > {
public:
	inline									vector<ID3D11SamplerState *>	( ) { /* no source */ }
	inline									vector<ID3D11SamplerState *>	( u32 arg_0, ID3D11SamplerState* const& arg_1 ) { /* no source */ }
	inline	explicit						vector<ID3D11SamplerState *>	( u32 arg_0 ) { /* no source */ }

	inline	ID3D11SamplerState*&			operator[]						( u32 arg_0 ) { /* no source */ }
	inline	ID3D11SamplerState* const&		operator[]						( u32 arg_0 ) const { /* no source */ }

	inline	u32								size							( ) const { /* no source */ }

	inline									~vector<ID3D11SamplerState *>	( ) { /* no source */ }
}; // class vector< ID3D11SamplerState* >

STATIC_SIZE_ASSERT(vector< ID3D11SamplerState* >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
