////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_TRACER_MODEL_INSTANCE_COOK_H_INCLUDED
#define RENDER_TRACER_MODEL_INSTANCE_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::translate_query_cook;

/* FORWARD REFS */
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::resources::resource_base;

namespace vostok {
namespace render {

class tracer_model_instance_cook : public resources::translate_query_cook {
public:
					tracer_model_instance_cook	( );

	virtual	void	translate_query				( resources::query_result_for_cook& parent ) override;

	virtual	void	delete_resource				( resources::resource_base* resource ) override;

			void	on_model_ready				( resources::queries_result& data );

	virtual			~tracer_model_instance_cook	( ) { /* no source */ }
}; // class tracer_model_instance_cook

STATIC_SIZE_ASSERT(tracer_model_instance_cook, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_TRACER_MODEL_INSTANCE_COOK_H_INCLUDED
