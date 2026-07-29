////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* INCLUDES */
class vostok::render::tracer_model_instance;

namespace vostok {
namespace render {

class std_allocator< tracer_model_instance_ptr > {
public:
	inline										std_allocator<vostok::resources::resource_ptr<vostok::render::tracer_model_instance,vostok::resources::unmanaged_intrusive_base> >( ) { /* no source */ }

	inline	tracer_model_instance_ptr*			address		( tracer_model_instance_ptr& arg_0 ) const { /* no source */ }
	inline	tracer_model_instance_ptr const*	address		( tracer_model_instance_ptr const& arg_0 ) const { /* no source */ }

	inline	tracer_model_instance_ptr*			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	tracer_model_instance_ptr*			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	tracer_model_instance_ptr*			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	tracer_model_instance_ptr*			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*								__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void								deallocate	( tracer_model_instance_ptr* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void								deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void								construct	( tracer_model_instance_ptr* arg_0, tracer_model_instance_ptr const& arg_1 ) { /* no source */ }

	inline	void								destroy		( tracer_model_instance_ptr* arg_0 ) { /* no source */ }

	inline	u32									max_size	( ) const { /* no source */ }
}; // class std_allocator< tracer_model_instance_ptr >

STATIC_SIZE_ASSERT(std_allocator< tracer_model_instance_ptr >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
