////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_PRIORITY_LESS_H_INCLUDED
#define RENDER_PRIORITY_LESS_H_INCLUDED

/* INCLUDES */
typedef stlp_std::binary_function<vostok::resources::resource_ptr<survarium::flash_movie_resource,vostok::resources::unmanaged_intrusive_base> const &,vostok::resources::resource_ptr<survarium::flash_movie_resource,vostok::resources::unmanaged_intrusive_base> const &,bool>
	stlp_std::binary_function<survarium::flash_movie_resource_ptr const &,vostok::resources::resource_ptr<survarium::flash_movie_resource,vostok::resources::unmanaged_intrusive_base> const &,bool>;
class survarium::flash_movie_resource;

namespace vostok {
namespace render {

struct priority_less : public std::binary_function< survarium::flash_movie_resource_ptr const&, resources::resource_ptr< survarium::flash_movie_resource, resources::unmanaged_intrusive_base > const&, bool > {
	inline	bool	operator()		(
						survarium::flash_movie_resource_ptr const&	arg_0,
						survarium::flash_movie_resource_ptr const&	arg_1
					) const { /* no source */ }

	inline			~priority_less	( ) { /* no source */ }
}; // struct priority_less

STATIC_SIZE_ASSERT(priority_less, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_PRIORITY_LESS_H_INCLUDED
