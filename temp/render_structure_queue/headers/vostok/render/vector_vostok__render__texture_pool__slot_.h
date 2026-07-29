////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::texture_pool::slot,vostok::render::std_allocator<vostok::render::texture_pool::slot> >
	stlp_std::vector<vostok::render::texture_pool::slot >;
class vostok::render::texture_pool::slot;

namespace vostok {
namespace render {

class vector< texture_pool::slot > : public std::vector< texture_pool::slot > {
public:
	inline									vector<vostok::render::texture_pool::slot>( ) { /* no source */ }
	inline									vector<vostok::render::texture_pool::slot>( u32 arg_0, texture_pool::slot const& arg_1 ) { /* no source */ }
	inline	explicit						vector<vostok::render::texture_pool::slot>( u32 arg_0 ) { /* no source */ }

	inline	texture_pool::slot&				operator[]	( u32 arg_0 ) { /* no source */ }
	inline	texture_pool::slot const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32								size		( ) const { /* no source */ }

	inline									~vector<vostok::render::texture_pool::slot>( ) { /* no source */ }
}; // class vector< texture_pool::slot >

STATIC_SIZE_ASSERT(vector< texture_pool::slot >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
