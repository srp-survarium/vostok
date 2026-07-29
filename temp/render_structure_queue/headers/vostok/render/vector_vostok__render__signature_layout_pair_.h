////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::signature_layout_pair,vostok::render::std_allocator<vostok::render::signature_layout_pair> >
	stlp_std::vector<vostok::render::signature_layout_pair >;
class vostok::render::signature_layout_pair;

namespace vostok {
namespace render {

class vector< signature_layout_pair > : public std::vector< signature_layout_pair > {
public:
	inline									vector<vostok::render::signature_layout_pair>( ) { /* no source */ }
	inline									vector<vostok::render::signature_layout_pair>( u32 arg_0, signature_layout_pair const& arg_1 ) { /* no source */ }
	inline	explicit						vector<vostok::render::signature_layout_pair>( u32 arg_0 ) { /* no source */ }

	inline	signature_layout_pair&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	signature_layout_pair const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32								size		( ) const { /* no source */ }

	inline									~vector<vostok::render::signature_layout_pair>( ) { /* no source */ }
}; // class vector< signature_layout_pair >

STATIC_SIZE_ASSERT(vector< signature_layout_pair >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
