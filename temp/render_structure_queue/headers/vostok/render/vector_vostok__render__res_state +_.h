////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::res_state *,vostok::render::std_allocator<vostok::render::res_state *> >
	stlp_std::vector<vostok::render::res_state * >;
class vostok::render::res_state *;

/* FORWARD REFS */
class vostok::render::res_state;
class vostok::render::res_state*;

namespace vostok {
namespace render {

class vector< res_state* > : public std::vector< res_state* > {
public:
	inline							vector<vostok::render::res_state *>	( ) { /* no source */ }
	inline							vector<vostok::render::res_state *>	( u32 arg_0, res_state* const& arg_1 ) { /* no source */ }
	inline	explicit				vector<vostok::render::res_state *>	( u32 arg_0 ) { /* no source */ }

	inline	res_state*&				operator[]							( u32 arg_0 ) { /* no source */ }
	inline	res_state* const&		operator[]							( u32 arg_0 ) const { /* no source */ }

	inline	u32						size								( ) const { /* no source */ }

	inline							~vector<vostok::render::res_state *>( ) { /* no source */ }
}; // class vector< res_state* >

STATIC_SIZE_ASSERT(vector< res_state* >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
