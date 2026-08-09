#ifndef VOSTOK_RENDER_ENGINE_MODEL_COOKER_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_MODEL_COOKER_H_INCLUDED

#include <vostok/animation/skeleton.h>
#include <vostok/render/facade/model.h>
#include <vostok/resources_cook_classes.h>

namespace vostok {

namespace render {

struct static_model_instance_cook : public resources::translate_query_cook {
	static_model_instance_cook( );

	virtual void translate_query( resources::query_result_for_cook& parent );
	virtual void delete_resource( resources::resource_base* resource );

private:
	void on_subresources_loaded(
		resources::queries_result&			data,
		resources::query_result_for_cook*	parent_query
	);

public:
	virtual ~static_model_instance_cook( ) { }
};

STATIC_SIZE_ASSERT( static_model_instance_cook, 0x20 );

struct skeleton_model_instance_cook_data {
	explicit skeleton_model_instance_cook_data(
		resources::query_result_for_cook* in_parent_query
	) :
		render_model_ready	( false ),
		skeleton_ready		( false ),
		parent_query		( in_parent_query )
	{
	}

	bool								render_model_ready;
	bool								skeleton_ready;
	resources::query_result_for_cook*	parent_query;
	render_model_instance_ptr			render_model;
	animation::skeleton_ptr				skeleton;
};

STATIC_SIZE_ASSERT( skeleton_model_instance_cook_data, 0x10 );

struct skeleton_model_instance_cook : public resources::translate_query_cook {
	skeleton_model_instance_cook( );

	virtual void translate_query( resources::query_result_for_cook& parent );
	virtual void delete_resource( resources::resource_base* resource );

private:
	void on_render_model_loaded(
		resources::queries_result&			result,
		skeleton_model_instance_cook_data*	cook_data
	);

	void on_skeleton_config_loaded(
		resources::queries_result&			result,
		skeleton_model_instance_cook_data*	cook_data
	);

	void on_skeleton_loaded(
		resources::queries_result&			result,
		skeleton_model_instance_cook_data*	cook_data
	);

	void on_all_subresources_ready( skeleton_model_instance_cook_data* cook_data );

public:
	virtual ~skeleton_model_instance_cook( ) { }
};

STATIC_SIZE_ASSERT( skeleton_model_instance_cook, 0x20 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_MODEL_COOKER_H_INCLUDED
