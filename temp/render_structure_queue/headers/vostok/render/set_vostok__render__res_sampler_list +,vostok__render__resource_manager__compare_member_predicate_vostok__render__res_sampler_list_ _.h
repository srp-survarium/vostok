////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SET_H_INCLUDED
#define RENDER_SET_H_INCLUDED

/* INCLUDES */
class stlp_std::set<vostok::render::res_sampler_list *,vostok::render::resource_manager::compare_member_predicate<vostok::render::res_sampler_list>,vostok::render::std_allocator<vostok::render::res_sampler_list *> >;

/* FORWARD REFS */
class vostok::render::resource_manager::compare_member_predicate<vostok::render::res_sampler_list>;

namespace vostok {
namespace render {

class set< res_sampler_list*, resource_manager::compare_member_predicate< res_sampler_list > > : public std::set< res_sampler_list*, resource_manager::compare_member_predicate< res_sampler_list >, std_allocator< res_sampler_list* > > {
public:
	inline				set<vostok::render::res_sampler_list *,vostok::render::resource_manager::compare_member_predicate<vostok::render::res_sampler_list> >( ) { /* no source */ }
	inline	explicit	set<vostok::render::res_sampler_list *,vostok::render::resource_manager::compare_member_predicate<vostok::render::res_sampler_list> >( resource_manager::compare_member_predicate< res_sampler_list > const& arg_0 ) { /* no source */ }
	inline				~set<vostok::render::res_sampler_list *,vostok::render::resource_manager::compare_member_predicate<vostok::render::res_sampler_list> >( ) { /* no source */ }
}; // class set< res_sampler_list*, resource_manager::compare_member_predicate< res_sampler_list > >

STATIC_SIZE_ASSERT(set< res_sampler_list*, resource_manager::compare_member_predicate< res_sampler_list > >, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SET_H_INCLUDED
