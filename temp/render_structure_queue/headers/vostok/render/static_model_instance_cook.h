////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STATIC_MODEL_INSTANCE_COOK_H_INCLUDED
#define RENDER_STATIC_MODEL_INSTANCE_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::translate_query_cook;

/* FORWARD REFS */
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::resources::resource_base;

namespace vostok {
namespace render {

struct static_model_instance_cook : public resources::translate_query_cook {
					static_model_instance_cook	( );

	virtual	void	translate_query				( resources::query_result_for_cook& parent ) override;

	virtual	void	delete_resource				( resources::resource_base* resource ) override;

private:
			void	on_subresources_loaded		( resources::queries_result& data, resources::query_result_for_cook* parent_query );

public:
	virtual			~static_model_instance_cook	( ) { /* no source */ }
}; // struct static_model_instance_cook

STATIC_SIZE_ASSERT(static_model_instance_cook, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STATIC_MODEL_INSTANCE_COOK_H_INCLUDED
