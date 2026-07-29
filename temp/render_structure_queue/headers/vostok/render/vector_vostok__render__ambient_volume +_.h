////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::ambient_volume *,vostok::render::std_allocator<vostok::render::ambient_volume *> >
	stlp_std::vector<vostok::render::ambient_volume * >;
class vostok::render::ambient_volume *;

/* FORWARD REFS */
class vostok::render::ambient_volume;
class vostok::render::ambient_volume*;

namespace vostok {
namespace render {

class vector< ambient_volume* > : public std::vector< ambient_volume* > {
public:
	inline								vector<vostok::render::ambient_volume *>( ) { /* no source */ }
	inline								vector<vostok::render::ambient_volume *>( u32 arg_0, ambient_volume* const& arg_1 ) { /* no source */ }
	inline	explicit					vector<vostok::render::ambient_volume *>( u32 arg_0 ) { /* no source */ }

	inline	ambient_volume*&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	ambient_volume* const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32							size		( ) const { /* no source */ }

	inline								~vector<vostok::render::ambient_volume *>( ) { /* no source */ }
}; // class vector< ambient_volume* >

STATIC_SIZE_ASSERT(vector< ambient_volume* >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
