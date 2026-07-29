////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RENDERER_COOK_H_INCLUDED
#define RENDER_RENDERER_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::unmanaged_cook;
class vostok::const_buffer;

/* FORWARD REFS */
class vostok::resources::query_result_for_cook;
class vostok::resources::unmanaged_resource;

namespace vostok {
namespace render {

class renderer_cook : public resources::unmanaged_cook {
public:
								renderer_cook		( );

	virtual	mutable_buffer		allocate_resource	(
									resources::query_result_for_cook&		in_query,
									const_buffer							raw_file_data,
									bool									file_exist
								) override;
	virtual	void				deallocate_resource	( void* buffer ) override;
	virtual	void				create_resource		(
									resources::query_result_for_cook&		in_out_query,
									const_buffer							raw_file_data,
									mutable_buffer							in_out_unmanaged_resource_buffer
								) override;
	virtual	void				destroy_resource	( resources::unmanaged_resource* arg_0 ) override { /* no source */ }

	virtual						~renderer_cook		( ) { /* no source */ }
}; // class renderer_cook

STATIC_SIZE_ASSERT(renderer_cook, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RENDERER_COOK_H_INCLUDED
