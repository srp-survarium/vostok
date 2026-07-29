////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::material_effects_entry,vostok::render::std_allocator<vostok::render::material_effects_entry> >
	stlp_std::vector<vostok::render::material_effects_entry >;
class vostok::render::material_effects_entry;

namespace vostok {
namespace render {

class vector< material_effects_entry > : public std::vector< material_effects_entry > {
public:
	inline										vector<vostok::render::material_effects_entry>( ) { /* no source */ }
	inline										vector<vostok::render::material_effects_entry>( u32 arg_0, material_effects_entry const& arg_1 ) { /* no source */ }
	inline	explicit							vector<vostok::render::material_effects_entry>( u32 arg_0 ) { /* no source */ }

	inline	material_effects_entry&				operator[]	( u32 arg_0 ) { /* no source */ }
	inline	material_effects_entry const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32									size		( ) const { /* no source */ }

	inline										~vector<vostok::render::material_effects_entry>( ) { /* no source */ }
}; // class vector< material_effects_entry >

STATIC_SIZE_ASSERT(vector< material_effects_entry >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
