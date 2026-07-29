////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_TEMPORAL_PROJECTION_MATRIX_MODIFIER_H_INCLUDED
#define RENDER_TEMPORAL_PROJECTION_MATRIX_MODIFIER_H_INCLUDED

/* INCLUDES */
class vostok::core::noncopyable;
class vostok::render::renderer_context;
class vostok::render::renderer_context* const;

namespace vostok {
namespace render {

struct temporal_projection_matrix_modifier : public core::noncopyable {
					temporal_projection_matrix_modifier	(
						renderer_context* const		context,
						const u32					window_size_x,
						const u32					window_size_y,
						const bool					need_modify
					);

			void	push_jittering						( );
			void	pop_jittering						( );

	inline			~temporal_projection_matrix_modifier( ) { /* no source */ }

	/* 0x0000 */	/* core::noncopyable */
private:
	/* 0x0000 */	renderer_context* const		m_renderer_context;
	/* 0x0004 */	const u32					m_window_size_x;
	/* 0x0008 */	const u32					m_window_size_y;
	/* 0x000c */	const bool					m_need_modify;
	/* 0x000d */	bool						m_jittered;
}; // struct temporal_projection_matrix_modifier

STATIC_SIZE_ASSERT(temporal_projection_matrix_modifier, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_TEMPORAL_PROJECTION_MATRIX_MODIFIER_H_INCLUDED
