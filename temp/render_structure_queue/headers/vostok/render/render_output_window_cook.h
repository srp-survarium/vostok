////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RENDER_OUTPUT_WINDOW_COOK_H_INCLUDED
#define RENDER_RENDER_OUTPUT_WINDOW_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::translate_query_cook;

/* FORWARD REFS */
class vostok::resources::query_result_for_cook;
class vostok::resources::resource_base;

namespace vostok {
namespace render {

class render_output_window_cook : public resources::translate_query_cook {
public:
					render_output_window_cook	( );

	virtual	void	translate_query				( resources::query_result_for_cook& parent ) override;

	virtual	void	delete_resource				( resources::resource_base* resource ) override;

	virtual			~render_output_window_cook	( ) { /* no source */ }
}; // class render_output_window_cook

STATIC_SIZE_ASSERT(render_output_window_cook, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RENDER_OUTPUT_WINDOW_COOK_H_INCLUDED
