////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SET_H_INCLUDED
#define RENDER_SET_H_INCLUDED

/* INCLUDES */
class stlp_std::set<vostok::render::shader_constant_table *,vostok::render::resource_manager::constant_table_predicate,vostok::render::std_allocator<vostok::render::shader_constant_table *> >;

/* FORWARD REFS */
class vostok::render::resource_manager::constant_table_predicate;

namespace vostok {
namespace render {

class set< shader_constant_table*, resource_manager::constant_table_predicate > : public std::set< shader_constant_table*, resource_manager::constant_table_predicate, std_allocator< shader_constant_table* > > {
public:
	inline				set<vostok::render::shader_constant_table *,vostok::render::resource_manager::constant_table_predicate>( ) { /* no source */ }
	inline	explicit	set<vostok::render::shader_constant_table *,vostok::render::resource_manager::constant_table_predicate>( resource_manager::constant_table_predicate const& arg_0 ) { /* no source */ }
	inline				~set<vostok::render::shader_constant_table *,vostok::render::resource_manager::constant_table_predicate>( ) { /* no source */ }
}; // class set< shader_constant_table*, resource_manager::constant_table_predicate >

STATIC_SIZE_ASSERT(set< shader_constant_table*, resource_manager::constant_table_predicate >, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SET_H_INCLUDED
