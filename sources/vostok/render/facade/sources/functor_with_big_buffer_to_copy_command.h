#ifndef VOSTOK_RENDER_FACADE_SOURCES_FUNCTOR_WITH_BIG_BUFFER_TO_COPY_COMMAND_H_INCLUDED
#define VOSTOK_RENDER_FACADE_SOURCES_FUNCTOR_WITH_BIG_BUFFER_TO_COPY_COMMAND_H_INCLUDED

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <vostok/render/facade/base_command.h>

namespace vostok {
namespace render {

template < typename T >
class functor_with_big_buffer_to_copy_command :
	public base_command,
	private boost::noncopyable
{
public:
	typedef boost::function< void ( T const& ) > on_execute_type;
	typedef boost::function< void ( base_command& ) > on_defer_execution_type;

	inline functor_with_big_buffer_to_copy_command(
		on_execute_type const& on_execute,
		T const& data,
		on_defer_execution_type const& on_defer_execution = on_defer_execution_type( )
	)
		: base_command( !on_defer_execution.empty( ) ),
		  m_on_execute( on_execute ),
		  m_on_defer_execution( on_defer_execution ),
		  m_data( data )
	{
		// FUNCTION BODY[0x15e90] for game::renderer::draw_scene_params
		// FUNCTION BODY[0x19c70] for decal_properties
		R_ASSERT			( on_execute );
	}

	virtual void execute( )
	{
		// FUNCTION BODY[0x1a2f0]
		m_on_execute		( m_data );
	}

	virtual void defer_execution( )
	{
		// FUNCTION BODY[0x1a5f0]
		m_on_defer_execution( *this );
	}

	virtual ~functor_with_big_buffer_to_copy_command( ) { /* no source */ }

private:
	on_execute_type m_on_execute;
	on_defer_execution_type m_on_defer_execution;
	T const m_data;
};

// Observed retail specialization sizes:
//   fs_new::virtual_path_string                 0x1b0
//   math::float4x4                             0x0d8
//   ambient_volume_properties                  0x0e0
//   cloud_key_parameters                       0x0e0
//   cloud_parameters                           0x0b0
//   decal_properties                           0x100
//   environment_probe_properties               0x210
//   game::renderer::draw_scene_params           0x0b8
//   sky_ambient_occlusion_properties            0x1c8
//   trample_desc                                0x0b0
//   volume_fog_parameters                      0x110

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_SOURCES_FUNCTOR_WITH_BIG_BUFFER_TO_COPY_COMMAND_H_INCLUDED
