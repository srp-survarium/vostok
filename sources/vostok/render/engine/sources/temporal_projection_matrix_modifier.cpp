#include "pch.h"
#include "temporal_projection_matrix_modifier.h"
#include "renderer_context.h"
#include "scene_view.h"

namespace vostok {
namespace render {

temporal_projection_matrix_modifier::temporal_projection_matrix_modifier(
	renderer_context* const context,
	u32 const window_size_x,
	u32 const window_size_y,
	bool const need_modify
) :
	m_renderer_context( context ),
	m_window_size_x( window_size_x ),
	m_window_size_y( window_size_y ),
	m_need_modify( need_modify ),
	m_jittered( false )
{
}

void temporal_projection_matrix_modifier::push_jittering( )
{
	if ( !m_need_modify )
		return;

	float4x4 p					= m_renderer_context->get_p( );

	float w						= ( float )m_window_size_x;
	float h						= ( float )m_window_size_y;

	float jitter0[2]			= { .25f, -.25f };
	float jitter1[2]			= { -.25f, .25f };

	float* jitter;
	if ( !( m_renderer_context->scene_view( )->get_render_frame_index( ) & 1 ) )
		jitter					= jitter0;
	else
		jitter					= jitter1;

	float near_plane			= m_renderer_context->get_near( );
	float left					= -near_plane * p.e11;
	float right					= near_plane * p.e11;
	float bottom				= -near_plane * p.e00;
	float top					= near_plane * p.e00;

	float dx					= ( right - left ) * ( -1.f / w ) * jitter[0];
	float dy					= ( top - bottom ) * jitter[1] * ( -1.f / h );

	float jittered_top			= dy + top;
	float jittered_bottom		= dy + bottom;
	float jittered_left			= dx + left;
	float jittered_right		= dx + right;

	if ( math::abs( jittered_left - jittered_right ) > math::epsilon_5 )
		p.e20					= ( jittered_left + jittered_right ) / ( jittered_left - jittered_right );

	if ( math::abs( jittered_bottom - jittered_top ) > math::epsilon_5 )
		p.e21					= ( jittered_bottom + jittered_top ) / ( jittered_bottom - jittered_top );

	m_renderer_context->push_set_p( p );

	m_jittered					= true;
}

void temporal_projection_matrix_modifier::pop_jittering( )
{
	if ( !m_need_modify )
		return;

	m_renderer_context->pop_p( );

	m_jittered					= false;
}

} // namespace render
} // namespace vostok
