////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SKELETON_RENDER_MODEL_H_INCLUDED
#define RENDER_SKELETON_RENDER_MODEL_H_INCLUDED

/* INCLUDES */
class vostok::render::render_model;
class vostok::render::shader_constant_host;
class vostok::render::vector<vostok::math::float4x4>;

/* FORWARD REFS */
class vostok::memory::reader;

namespace vostok {
namespace render {

class skeleton_render_model : public render_model {
public:
					skeleton_render_model	( );

			void	load_bones				( memory::reader& bones_chunk );

			void	update					( vector< float4x4 > const& bones );

			void	get_bind_pose			( float4x4* matrices, u32 count ) const;

	virtual			~skeleton_render_model	( ) { /* no source */ }

	/* 0x0000 */	/* render_model */
	/* 0x0138 */	shader_constant_host*		m_bones_matrices_shader_constant;
	/* 0x013c */	shader_constant_host*		m_prev_bones_matrices_shader_constant;
	/* 0x0140 */	vector< float4x4 >			m_inverted_bones_matrices_in_bind_pose;
}; // class skeleton_render_model

STATIC_SIZE_ASSERT(skeleton_render_model, 0x150);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SKELETON_RENDER_MODEL_H_INCLUDED
