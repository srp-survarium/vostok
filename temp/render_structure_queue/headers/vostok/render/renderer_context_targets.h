////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RENDERER_CONTEXT_TARGETS_H_INCLUDED
#define RENDER_RENDERER_CONTEXT_TARGETS_H_INCLUDED

/* INCLUDES */
class const vostok::math::uint2;
class vostok::math::uint2;
struct vostok::render::render_target_instance;
class vostok::render::enum_render_target_index;
class vostok::render::enum_rt_usage;

namespace vostok {
namespace render {

class renderer_context_targets {
public:
			explicit		renderer_context_targets	( math::uint2 size );
							~renderer_context_targets	( );

			void			new_rt						(
								enum_render_target_index	index,
								DXGI_FORMAT					in_format,
								const math::uint2			arg_2 /* math::uint2 in_size */,
								enum_rt_usage				usage,
								bool						enabled
							);
	inline	void			new_ds						( enum_render_target_index arg_0, DXGI_FORMAT arg_1, const math::uint2 arg_2 ) { /* no source */ }
			void			new_lt						(
								enum_render_target_index	index,
								DXGI_FORMAT					in_format,
								const math::uint2			arg_2 /* math::uint2 in_size */
							);

	inline	math::uint2		size						( ) const { /* no source */ }

			void			resize						( math::uint2 size, bool force_resize );

	inline	u32				memory_usage				( ) const { /* no source */ }

	inline	u32				get_id						( ) const { /* no source */ }

private:
			void			create_targets				( math::uint2 size, bool force_resize );

public:
	/* 0x0000 */	render_target_instance		m_family[70];
	/* 0x2bc0 */	math::uint2					m_size;
private:
	/* 0x2bc8 */	s32							m_id;
	/* 0x2bcc */	u32							m_memory_usage;
}; // class renderer_context_targets

STATIC_SIZE_ASSERT(renderer_context_targets, 0x2BD0);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RENDERER_CONTEXT_TARGETS_H_INCLUDED
