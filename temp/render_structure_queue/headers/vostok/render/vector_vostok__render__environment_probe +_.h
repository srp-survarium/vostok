////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::environment_probe *,vostok::render::std_allocator<vostok::render::environment_probe *> >
	stlp_std::vector<vostok::render::environment_probe * >;
class vostok::render::environment_probe *;

/* FORWARD REFS */
class vostok::render::environment_probe;
class vostok::render::environment_probe*;

namespace vostok {
namespace render {

class vector< environment_probe* > : public std::vector< environment_probe* > {
public:
	inline									vector<vostok::render::environment_probe *>( ) { /* no source */ }
	inline									vector<vostok::render::environment_probe *>( u32 arg_0, environment_probe* const& arg_1 ) { /* no source */ }
	inline	explicit						vector<vostok::render::environment_probe *>( u32 arg_0 ) { /* no source */ }

	inline	environment_probe*&				operator[]	( u32 arg_0 ) { /* no source */ }
	inline	environment_probe* const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32								size		( ) const { /* no source */ }

	inline									~vector<vostok::render::environment_probe *>( ) { /* no source */ }
}; // class vector< environment_probe* >

STATIC_SIZE_ASSERT(vector< environment_probe* >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
