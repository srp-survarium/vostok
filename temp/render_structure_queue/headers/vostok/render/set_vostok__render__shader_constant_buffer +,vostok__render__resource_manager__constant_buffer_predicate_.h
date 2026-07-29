////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SET_H_INCLUDED
#define RENDER_SET_H_INCLUDED

/* INCLUDES */
class stlp_std::set<vostok::render::shader_constant_buffer *,vostok::render::resource_manager::constant_buffer_predicate,vostok::render::std_allocator<vostok::render::shader_constant_buffer *> >;

/* FORWARD REFS */
class vostok::render::resource_manager::constant_buffer_predicate;

namespace vostok {
namespace render {

class set< shader_constant_buffer*, resource_manager::constant_buffer_predicate > : public std::set< shader_constant_buffer*, resource_manager::constant_buffer_predicate, std_allocator< shader_constant_buffer* > > {
public:
	inline				set<vostok::render::shader_constant_buffer *,vostok::render::resource_manager::constant_buffer_predicate>( ) { /* no source */ }
	inline	explicit	set<vostok::render::shader_constant_buffer *,vostok::render::resource_manager::constant_buffer_predicate>( resource_manager::constant_buffer_predicate const& arg_0 ) { /* no source */ }
	inline				~set<vostok::render::shader_constant_buffer *,vostok::render::resource_manager::constant_buffer_predicate>( ) { /* no source */ }
}; // class set< shader_constant_buffer*, resource_manager::constant_buffer_predicate >

STATIC_SIZE_ASSERT(set< shader_constant_buffer*, resource_manager::constant_buffer_predicate >, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SET_H_INCLUDED
