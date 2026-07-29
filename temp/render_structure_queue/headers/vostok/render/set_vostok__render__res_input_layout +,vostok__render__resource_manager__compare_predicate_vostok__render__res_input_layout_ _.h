////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SET_H_INCLUDED
#define RENDER_SET_H_INCLUDED

/* INCLUDES */
class stlp_std::set<vostok::render::res_input_layout *,vostok::render::resource_manager::compare_predicate<vostok::render::res_input_layout>,vostok::render::std_allocator<vostok::render::res_input_layout *> >;

/* FORWARD REFS */
class vostok::render::resource_manager::compare_predicate<vostok::render::res_input_layout>;

namespace vostok {
namespace render {

class set< res_input_layout*, resource_manager::compare_predicate< res_input_layout > > : public std::set< res_input_layout*, resource_manager::compare_predicate< res_input_layout >, std_allocator< res_input_layout* > > {
public:
	inline				set<vostok::render::res_input_layout *,vostok::render::resource_manager::compare_predicate<vostok::render::res_input_layout> >( ) { /* no source */ }
	inline	explicit	set<vostok::render::res_input_layout *,vostok::render::resource_manager::compare_predicate<vostok::render::res_input_layout> >( resource_manager::compare_predicate< res_input_layout > const& arg_0 ) { /* no source */ }
	inline				~set<vostok::render::res_input_layout *,vostok::render::resource_manager::compare_predicate<vostok::render::res_input_layout> >( ) { /* no source */ }
}; // class set< res_input_layout*, resource_manager::compare_predicate< res_input_layout > >

STATIC_SIZE_ASSERT(set< res_input_layout*, resource_manager::compare_predicate< res_input_layout > >, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SET_H_INCLUDED
