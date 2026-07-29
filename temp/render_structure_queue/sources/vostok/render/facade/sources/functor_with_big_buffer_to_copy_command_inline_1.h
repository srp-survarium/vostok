////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\RENDER\FACADE\SOURCES\FUNCTOR_WITH_BIG_BUFFER_TO_COPY_COMMAND_INLINE_H_INCLUDED
#define VOSTOK\RENDER\FACADE\SOURCES\FUNCTOR_WITH_BIG_BUFFER_TO_COPY_COMMAND_INLINE_H_INCLUDED

namespace vostok {
namespace render {

// STATE[STUB]
inline  functor_with_big_buffer_to_copy_command<vostok::render::decal_properties>::functor_with_big_buffer_to_copy_command<vostok::render::decal_properties>(
	boost::function< void( decal_properties const& ) > const&	on_execute,
	decal_properties const&		data,
	boost::function< void( base_command& ) > const&	on_defer_execution
)
{
	// FUNCTION BODY[0x19c70]: 1
	// <0>
	// ******
}

// STATE[STUB]
inline void functor_with_big_buffer_to_copy_command<vostok::math::float4x4>::execute( )
{
	// FUNCTION BODY[0x1a2f0]: 1
	// <0x1a2f0>|0x000|+0x00f:'30'
	// ******
}

// STATE[STUB]
inline void functor_with_big_buffer_to_copy_command<vostok::math::float4x4>::defer_execution( )
{
	// FUNCTION BODY[0x1a5f0]: 1
	// <0x1a5f0>|0x000|+0x009:'36'
	// ******
}

	// TYPEDEFS
	// typedef
	// 	boost::function< void( float4x4 const& ) >
	// 	on_execute_type;

	// typedef
	// 	boost::function< void( vostok::fs_new::virtual_path_string const& ) >
	// 	on_execute_type;

	// typedef
	// 	boost::function< void( vostok::render::ambient_volume_properties const& ) >
	// 	on_execute_type;

	// typedef
	// 	boost::function< void( vostok::render::cloud_key_parameters const& ) >
	// 	on_execute_type;

	// typedef
	// 	boost::function< void( vostok::render::cloud_parameters const& ) >
	// 	on_execute_type;

	// typedef
	// 	boost::function< void( vostok::render::decal_properties const& ) >
	// 	on_execute_type;

	// typedef
	// 	boost::function< void( vostok::render::environment_probe_properties const& ) >
	// 	on_execute_type;

	// typedef
	// 	boost::function< void( vostok::render::sky_ambient_occlusion_properties const& ) >
	// 	on_execute_type;

	// typedef
	// 	boost::function< void( vostok::render::trample_desc const& ) >
	// 	on_execute_type;

	// typedef
	// 	boost::function< void( vostok::render::volume_fog_parameters const& ) >
	// 	on_execute_type;

	// ******

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK\RENDER\FACADE\SOURCES\FUNCTOR_WITH_BIG_BUFFER_TO_COPY_COMMAND_INLINE_H_INCLUDED
