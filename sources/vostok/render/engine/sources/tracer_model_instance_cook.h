// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_TRACER_MODEL_INSTANCE_COOK_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_TRACER_MODEL_INSTANCE_COOK_H_INCLUDED
#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace render {

class tracer_model_instance_cook : public resources::translate_query_cook {
public:
	tracer_model_instance_cook( );

	virtual void translate_query( resources::query_result_for_cook& parent );
	virtual void delete_resource( resources::resource_base* resource );

	void on_model_ready( resources::queries_result& data );
};

STATIC_SIZE_ASSERT( tracer_model_instance_cook, 0x20 );

} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_ENGINE_TRACER_MODEL_INSTANCE_COOK_H_INCLUDED
