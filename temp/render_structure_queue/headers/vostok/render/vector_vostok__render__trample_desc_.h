////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::trample_desc,vostok::render::std_allocator<vostok::render::trample_desc> >
	stlp_std::vector<vostok::render::trample_desc >;
class vostok::render::trample_desc;

namespace vostok {
namespace render {

class vector< trample_desc > : public std::vector< trample_desc > {
public:
	inline							vector<vostok::render::trample_desc>( ) { /* no source */ }
	inline							vector<vostok::render::trample_desc>( u32 arg_0, trample_desc const& arg_1 ) { /* no source */ }
	inline	explicit				vector<vostok::render::trample_desc>( u32 arg_0 ) { /* no source */ }

	inline	trample_desc&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	trample_desc const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32						size		( ) const { /* no source */ }

	inline							~vector<vostok::render::trample_desc>( ) { /* no source */ }
}; // class vector< trample_desc >

STATIC_SIZE_ASSERT(vector< trample_desc >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
