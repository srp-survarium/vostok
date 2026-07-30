#ifndef VOSTOK_RENDER_CORE_DX11_TEXTURE_COOK_WRAPPER_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_TEXTURE_COOK_WRAPPER_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace vostok {

namespace resources {
class queries_result;
} // namespace resources

namespace render {

struct texture_cook_wrapper : public resources::translate_query_cook {
	texture_cook_wrapper( );

	virtual void translate_query( resources::query_result_for_cook& query );
	virtual void delete_resource( resources::resource_base* resource );

private:
	void query_converted_texture( resources::query_result_for_cook* parent );
	void on_texture_loaded( resources::queries_result& result );
	void make_source_path(
		fs_new::virtual_path_string*,
		resources::query_result_for_cook&
	)
	{
		// STATE[STUB]
	}
};

STATIC_SIZE_ASSERT( texture_cook_wrapper, 0x20 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_TEXTURE_COOK_WRAPPER_H_INCLUDED
