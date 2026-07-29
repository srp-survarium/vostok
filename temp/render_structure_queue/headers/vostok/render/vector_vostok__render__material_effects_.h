////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::material_effects,vostok::render::std_allocator<vostok::render::material_effects> >
	stlp_std::vector<vostok::render::material_effects >;
class vostok::render::material_effects;

namespace vostok {
namespace render {

class vector< material_effects > : public std::vector< material_effects > {
public:
	inline								vector<vostok::render::material_effects>( ) { /* no source */ }
	inline								vector<vostok::render::material_effects>( u32 arg_0, material_effects const& arg_1 ) { /* no source */ }
	inline	explicit					vector<vostok::render::material_effects>( u32 arg_0 ) { /* no source */ }

	inline	material_effects&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	material_effects const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32							size		( ) const { /* no source */ }

	inline								~vector<vostok::render::material_effects>( ) { /* no source */ }
}; // class vector< material_effects >

STATIC_SIZE_ASSERT(vector< material_effects >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
