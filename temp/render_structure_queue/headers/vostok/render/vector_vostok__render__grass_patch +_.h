////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::grass_patch *,vostok::render::std_allocator<vostok::render::grass_patch *> >
	stlp_std::vector<vostok::render::grass_patch * >;
class vostok::render::grass_patch *;

/* FORWARD REFS */
class vostok::render::grass_patch;
class vostok::render::grass_patch*;

namespace vostok {
namespace render {

class vector< grass_patch* > : public std::vector< grass_patch* > {
public:
	inline							vector<vostok::render::grass_patch *>( ) { /* no source */ }
	inline							vector<vostok::render::grass_patch *>( u32 arg_0, grass_patch* const& arg_1 ) { /* no source */ }
	inline	explicit				vector<vostok::render::grass_patch *>( u32 arg_0 ) { /* no source */ }

	inline	grass_patch*&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	grass_patch* const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32						size		( ) const { /* no source */ }

	inline							~vector<vostok::render::grass_patch *>( ) { /* no source */ }
}; // class vector< grass_patch* >

STATIC_SIZE_ASSERT(vector< grass_patch* >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
