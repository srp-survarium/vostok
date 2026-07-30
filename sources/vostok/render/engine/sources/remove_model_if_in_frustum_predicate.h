#ifndef VOSTOK_RENDER_ENGINE_REMOVE_MODEL_IF_IN_FRUSTUM_PREDICATE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_REMOVE_MODEL_IF_IN_FRUSTUM_PREDICATE_H_INCLUDED

namespace vostok {

namespace math {

class frustum;

} // namespace math

namespace render {

struct lpv_render_surface;
struct render_surface_instance;

struct remove_model_if_in_frustum_predicate {
	explicit remove_model_if_in_frustum_predicate( math::frustum& frustum ) :
		m_frustum( &frustum )
	{
	}

	bool operator()( render_surface_instance* in_model );
	bool operator()( lpv_render_surface const& surface );

	math::frustum* m_frustum;
};

STATIC_SIZE_ASSERT( remove_model_if_in_frustum_predicate, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_REMOVE_MODEL_IF_IN_FRUSTUM_PREDICATE_H_INCLUDED
