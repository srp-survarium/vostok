////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::untyped_buffer *,vostok::render::std_allocator<vostok::render::untyped_buffer *> >
	stlp_std::vector<vostok::render::untyped_buffer * >;
class vostok::render::untyped_buffer *;

/* FORWARD REFS */
class vostok::render::untyped_buffer;
class vostok::render::untyped_buffer*;

namespace vostok {
namespace render {

class vector< untyped_buffer* > : public std::vector< untyped_buffer* > {
public:
	inline								vector<vostok::render::untyped_buffer *>( ) { /* no source */ }
	inline								vector<vostok::render::untyped_buffer *>( u32 arg_0, untyped_buffer* const& arg_1 ) { /* no source */ }
	inline	explicit					vector<vostok::render::untyped_buffer *>( u32 arg_0 ) { /* no source */ }

	inline	untyped_buffer*&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	untyped_buffer* const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32							size		( ) const { /* no source */ }

	inline								~vector<vostok::render::untyped_buffer *>( ) { /* no source */ }
}; // class vector< untyped_buffer* >

STATIC_SIZE_ASSERT(vector< untyped_buffer* >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
