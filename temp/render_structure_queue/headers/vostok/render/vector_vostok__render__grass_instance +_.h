////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::grass_instance *,vostok::render::std_allocator<vostok::render::grass_instance *> >
	stlp_std::vector<vostok::render::grass_instance * >;
class vostok::render::grass_instance *;

/* FORWARD REFS */
class vostok::render::grass_instance;
class vostok::render::grass_instance*;

namespace vostok {
namespace render {

class vector< grass_instance* > : public std::vector< grass_instance* > {
public:
	inline								vector<vostok::render::grass_instance *>( ) { /* no source */ }
	inline								vector<vostok::render::grass_instance *>( u32 arg_0, grass_instance* const& arg_1 ) { /* no source */ }
	inline	explicit					vector<vostok::render::grass_instance *>( u32 arg_0 ) { /* no source */ }

	inline	grass_instance*&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	grass_instance* const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32							size		( ) const { /* no source */ }

	inline								~vector<vostok::render::grass_instance *>( ) { /* no source */ }
}; // class vector< grass_instance* >

STATIC_SIZE_ASSERT(vector< grass_instance* >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
