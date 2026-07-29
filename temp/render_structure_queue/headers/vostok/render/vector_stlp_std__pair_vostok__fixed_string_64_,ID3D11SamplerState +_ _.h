////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<stlp_std::pair<vostok::fixed_string<64>,ID3D11SamplerState *>,vostok::render::std_allocator<stlp_std::pair<vostok::fixed_string<64>,ID3D11SamplerState *> > >
	stlp_std::vector<stlp_std::pair<vostok::fixed_string<64>,ID3D11SamplerState *> >;
class stlp_std::pair<vostok::fixed_string<64>,ID3D11SamplerState *>;

namespace vostok {
namespace render {

class vector< std::pair< fixed_string< 64 >, ID3D11SamplerState* > > : public std::vector< std::pair< fixed_string< 64 >, ID3D11SamplerState* > > {
public:
	inline				vector<stlp_std::pair<vostok::fixed_string<64>,ID3D11SamplerState *> >( ) { /* no source */ }
	inline				vector<stlp_std::pair<vostok::fixed_string<64>,ID3D11SamplerState *> >( u32 arg_0, std::pair< fixed_string< 64 >, ID3D11SamplerState* > const& arg_1 ) { /* no source */ }
	inline	explicit	vector<stlp_std::pair<vostok::fixed_string<64>,ID3D11SamplerState *> >( u32 arg_0 ) { /* no source */ }

	inline	std::pair< fixed_string< 64 >, ID3D11SamplerState* >&	operator[]	( u32 arg_0 ) { /* no source */ }
	inline	std::pair< fixed_string< 64 >, ID3D11SamplerState* > const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32			size		( ) const { /* no source */ }

	inline				~vector<stlp_std::pair<vostok::fixed_string<64>,ID3D11SamplerState *> >( ) { /* no source */ }
}; // class vector< std::pair< fixed_string< 64 >, ID3D11SamplerState* > >

STATIC_SIZE_ASSERT(vector< std::pair< fixed_string< 64 >, ID3D11SamplerState* > >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
