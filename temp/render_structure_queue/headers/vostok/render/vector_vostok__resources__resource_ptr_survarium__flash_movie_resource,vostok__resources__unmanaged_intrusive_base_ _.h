////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::resources::resource_ptr<survarium::flash_movie_resource,vostok::resources::unmanaged_intrusive_base>,vostok::render::std_allocator<vostok::resources::resource_ptr<survarium::flash_movie_resource,vostok::resources::unmanaged_intrusive_base> > >
	stlp_std::vector<survarium::flash_movie_resource_ptr >;
class survarium::flash_movie_resource;

namespace vostok {
namespace render {

class vector< survarium::flash_movie_resource_ptr > : public std::vector< survarium::flash_movie_resource_ptr > {
public:
	inline				vector<vostok::resources::resource_ptr<survarium::flash_movie_resource,vostok::resources::unmanaged_intrusive_base> >( ) { /* no source */ }
	inline				vector<vostok::resources::resource_ptr<survarium::flash_movie_resource,vostok::resources::unmanaged_intrusive_base> >( u32 arg_0, survarium::flash_movie_resource_ptr const& arg_1 ) { /* no source */ }
	inline	explicit	vector<vostok::resources::resource_ptr<survarium::flash_movie_resource,vostok::resources::unmanaged_intrusive_base> >( u32 arg_0 ) { /* no source */ }

	inline	survarium::flash_movie_resource_ptr&	operator[]	( u32 arg_0 ) { /* no source */ }
	inline	survarium::flash_movie_resource_ptr const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32			size		( ) const { /* no source */ }

	inline				~vector<vostok::resources::resource_ptr<survarium::flash_movie_resource,vostok::resources::unmanaged_intrusive_base> >( ) { /* no source */ }
}; // class vector< survarium::flash_movie_resource_ptr >

STATIC_SIZE_ASSERT(vector< survarium::flash_movie_resource_ptr >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
