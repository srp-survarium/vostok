////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::decal_instance *,vostok::render::std_allocator<vostok::render::decal_instance *> >
	stlp_std::vector<vostok::render::decal_instance * >;
class vostok::render::decal_instance *;

/* FORWARD REFS */
class vostok::render::decal_instance;
class vostok::render::decal_instance*;

namespace vostok {
namespace render {

class vector< decal_instance* > : public std::vector< decal_instance* > {
public:
	inline								vector<vostok::render::decal_instance *>( ) { /* no source */ }
	inline								vector<vostok::render::decal_instance *>( u32 arg_0, decal_instance* const& arg_1 ) { /* no source */ }
	inline	explicit					vector<vostok::render::decal_instance *>( u32 arg_0 ) { /* no source */ }

	inline	decal_instance*&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	decal_instance* const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32							size		( ) const { /* no source */ }

	inline								~vector<vostok::render::decal_instance *>( ) { /* no source */ }
}; // class vector< decal_instance* >

STATIC_SIZE_ASSERT(vector< decal_instance* >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
