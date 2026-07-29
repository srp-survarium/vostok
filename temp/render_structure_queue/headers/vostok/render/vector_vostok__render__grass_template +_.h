////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::grass_template *,vostok::render::std_allocator<vostok::render::grass_template *> >
	stlp_std::vector<vostok::render::grass_template * >;
class vostok::render::grass_template *;

/* FORWARD REFS */
class vostok::render::grass_template;
class vostok::render::grass_template*;

namespace vostok {
namespace render {

class vector< grass_template* > : public std::vector< grass_template* > {
public:
	inline								vector<vostok::render::grass_template *>( ) { /* no source */ }
	inline								vector<vostok::render::grass_template *>( u32 arg_0, grass_template* const& arg_1 ) { /* no source */ }
	inline	explicit					vector<vostok::render::grass_template *>( u32 arg_0 ) { /* no source */ }

	inline	grass_template*&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	grass_template* const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32							size		( ) const { /* no source */ }

	inline								~vector<vostok::render::grass_template *>( ) { /* no source */ }
}; // class vector< grass_template* >

STATIC_SIZE_ASSERT(vector< grass_template* >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
