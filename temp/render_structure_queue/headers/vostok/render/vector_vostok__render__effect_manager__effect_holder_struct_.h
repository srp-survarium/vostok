////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::effect_manager::effect_holder_struct,vostok::render::std_allocator<vostok::render::effect_manager::effect_holder_struct> >
	stlp_std::vector<vostok::render::effect_manager::effect_holder_struct >;
class vostok::render::effect_manager::effect_holder_struct;

namespace vostok {
namespace render {

class vector< effect_manager::effect_holder_struct > : public std::vector< effect_manager::effect_holder_struct > {
public:
	inline				vector<vostok::render::effect_manager::effect_holder_struct>( ) { /* no source */ }
	inline				vector<vostok::render::effect_manager::effect_holder_struct>( u32 arg_0, effect_manager::effect_holder_struct const& arg_1 ) { /* no source */ }
	inline	explicit	vector<vostok::render::effect_manager::effect_holder_struct>( u32 arg_0 ) { /* no source */ }

	inline	effect_manager::effect_holder_struct&	operator[]	( u32 arg_0 ) { /* no source */ }
	inline	effect_manager::effect_holder_struct const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32			size		( ) const { /* no source */ }

	inline				~vector<vostok::render::effect_manager::effect_holder_struct>( ) { /* no source */ }
}; // class vector< effect_manager::effect_holder_struct >

STATIC_SIZE_ASSERT(vector< effect_manager::effect_holder_struct >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
