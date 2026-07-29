////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::culling::aab_rect,vostok::render::std_allocator<vostok::render::culling::aab_rect> >
	stlp_std::vector<vostok::render::culling::aab_rect >;
class vostok::render::culling::aab_rect;

namespace vostok {
namespace render {

class vector< culling::aab_rect > : public std::vector< culling::aab_rect > {
public:
	inline								vector<vostok::render::culling::aab_rect>( ) { /* no source */ }
	inline								vector<vostok::render::culling::aab_rect>( u32 arg_0, culling::aab_rect const& arg_1 ) { /* no source */ }
	inline	explicit					vector<vostok::render::culling::aab_rect>( u32 arg_0 ) { /* no source */ }

	inline	culling::aab_rect&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	culling::aab_rect const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32							size		( ) const { /* no source */ }

	inline								~vector<vostok::render::culling::aab_rect>( ) { /* no source */ }
}; // class vector< culling::aab_rect >

STATIC_SIZE_ASSERT(vector< culling::aab_rect >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
