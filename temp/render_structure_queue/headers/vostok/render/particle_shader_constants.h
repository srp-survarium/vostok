////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_PARTICLE_SHADER_CONSTANTS_H_INCLUDED
#define RENDER_PARTICLE_SHADER_CONSTANTS_H_INCLUDED

/* INCLUDES */
class const vostok::math::float3;
class vostok::quasi_singleton<vostok::render::particle_shader_constants>;
class vostok::render::shader_constant_host;
class vostok::particle::enum_particle_locked_axis;
class vostok::particle::enum_particle_screen_alignment;

namespace vostok {
namespace render {

class particle_shader_constants : public quasi_singleton< particle_shader_constants > {
public:
					particle_shader_constants	( );

			void	set_time					( float time );

			void	set							(
						const float3							arg_0 /* float3 up_vector */,
						const float3							arg_1 /* float3 right_vector */,
						const float3							arg_2 /* float3 view_location */,
						particle::enum_particle_locked_axis		locked_axis,
						particle::enum_particle_screen_alignment	screen_alignment
					);

	inline			~particle_shader_constants	( ) { /* no source */ }

	/* 0x0000 */	/* quasi_singleton< particle_shader_constants > */
private:
	/* 0x0000 */	shader_constant_host*		m_right_view_vector;
	/* 0x0004 */	shader_constant_host*		m_up_view_vector;
	/* 0x0008 */	shader_constant_host*		m_use_align_by_dir;
	/* 0x000c */	shader_constant_host*		m_view_location;
	/* 0x0010 */	shader_constant_host*		m_current_time;
	/* 0x0014 */	shader_constant_host*		m_use_fixed_axis;
	/* 0x0018 */	shader_constant_host*		m_rotation_fixed_axis;
	/* 0x001c */	shader_constant_host*		m_locked_no_ratate_axis_index;
}; // class particle_shader_constants

STATIC_SIZE_ASSERT(particle_shader_constants, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_PARTICLE_SHADER_CONSTANTS_H_INCLUDED
