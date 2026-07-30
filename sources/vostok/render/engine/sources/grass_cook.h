#ifndef VOSTOK_RENDER_ENGINE_GRASS_COOK_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_GRASS_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>
#include "grass_data.h"

namespace vostok {
namespace render {

class grass_cook : public resources::translate_query_cook {
public:
	grass_cook( );
	virtual ~grass_cook( ) { }

	virtual void translate_query( resources::query_result_for_cook& parent );
	virtual void delete_resource( resources::resource_base* resource );

private:
	void on_layers_loaded( resources::queries_result& data, grass_cook_data* cook_data );
};

STATIC_SIZE_ASSERT( grass_cook, 0x20 );

} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_ENGINE_GRASS_COOK_H_INCLUDED
