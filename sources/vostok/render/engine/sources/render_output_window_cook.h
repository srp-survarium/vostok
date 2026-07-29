#ifndef VOSTOK_RENDER_ENGINE_RENDER_OUTPUT_WINDOW_COOK_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDER_OUTPUT_WINDOW_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace render {

class render_output_window_cook : public resources::translate_query_cook {
public:
	render_output_window_cook( );
	virtual ~render_output_window_cook( ) { }

	virtual void translate_query( resources::query_result_for_cook& parent ) override;
	virtual void delete_resource( resources::resource_base* resource ) override;
};

STATIC_SIZE_ASSERT( render_output_window_cook, 0x20 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDER_OUTPUT_WINDOW_COOK_H_INCLUDED
