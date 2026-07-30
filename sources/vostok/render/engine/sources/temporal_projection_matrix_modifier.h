#ifndef VOSTOK_RENDER_ENGINE_TEMPORAL_PROJECTION_MATRIX_MODIFIER_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_TEMPORAL_PROJECTION_MATRIX_MODIFIER_H_INCLUDED

#include <vostok/detail_noncopyable.h>

namespace vostok {
namespace render {

class renderer_context;

struct temporal_projection_matrix_modifier : public core::noncopyable {
	temporal_projection_matrix_modifier(
		renderer_context* const context,
		u32 const window_size_x,
		u32 const window_size_y,
		bool const need_modify
	);

	void push_jittering( );
	void pop_jittering( );

	~temporal_projection_matrix_modifier( ) { }

private:
	renderer_context* const	m_renderer_context;
	u32 const				m_window_size_x;
	u32 const				m_window_size_y;
	bool const				m_need_modify;
	bool					m_jittered;
};

STATIC_SIZE_ASSERT( temporal_projection_matrix_modifier, 0x10 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_TEMPORAL_PROJECTION_MATRIX_MODIFIER_H_INCLUDED
