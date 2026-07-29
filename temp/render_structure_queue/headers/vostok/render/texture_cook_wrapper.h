////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_TEXTURE_COOK_WRAPPER_H_INCLUDED
#define RENDER_TEXTURE_COOK_WRAPPER_H_INCLUDED

/* INCLUDES */
class vostok::resources::translate_query_cook;

/* FORWARD REFS */
class vostok::fs_new::virtual_path_string;
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::resources::resource_base;

namespace vostok {
namespace render {

struct texture_cook_wrapper : public resources::translate_query_cook {
					texture_cook_wrapper	( );

	virtual	void	translate_query			( resources::query_result_for_cook& query ) override;

	virtual	void	delete_resource			( resources::resource_base* __formal ) override;

private:
			void	query_converted_texture	( resources::query_result_for_cook* parent );

			void	on_texture_loaded		( resources::queries_result& result );

	inline	void	make_source_path		( fs_new::virtual_path_string* arg_0, resources::query_result_for_cook& arg_1 ) { /* no source */ }

public:
	virtual			~texture_cook_wrapper	( ) { /* no source */ }
}; // struct texture_cook_wrapper

STATIC_SIZE_ASSERT(texture_cook_wrapper, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_TEXTURE_COOK_WRAPPER_H_INCLUDED
