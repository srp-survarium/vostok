#include "pch.h"
#include <vostok/render/core/res_xs.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/res_sampler_list.h>

namespace vostok {
namespace render {

template <typename shader_data>
res_xs<shader_data>::res_xs( xs_descriptor<shader_data> const& binder ) :
	m_is_registered	( false )
{
	m_hardware_shader = binder.hardware_shader();
	m_constants = resource_manager::ref().create_const_table	( binder.data().constants);
	m_textures	= resource_manager::ref().create_texture_list	( binder.data().textures);
	m_samplers	= resource_manager::ref().create_sampler_list	( binder.data().samplers);
}

template <typename shader_data>
void res_xs<shader_data>::destroy_impl( ) const
{
	resource_manager::ref().release( this );
}

template <typename shader_data>
s32 res_xs<shader_data>::compare( xs_descriptor<shader_data> const& descriptor ) const
{
	s32 result = m_hardware_shader < descriptor.hardware_shader( ) ? -1
		: descriptor.hardware_shader( ) < m_hardware_shader ? 1 : 0;
	if ( result )
		return result;
	result = m_constants->compare( descriptor.data( ).constants );
	if ( result )
		return result;

	result = m_textures->compare( descriptor.data( ).textures );
	if ( result )
		return result;

	return m_samplers->compare( descriptor.data( ).samplers );

}

template <>
void res_xs<ps_data>::apply( ) const
{
	backend::ref().set_ps			( &*m_hardware_shader);
	backend::ref().set_ps_constants ( &*m_constants);
	backend::ref().set_ps_textures	( &*m_textures);
	backend::ref().set_ps_samplers	( &*m_samplers);
}

template <>
void res_xs<gs_data>::apply( ) const
{
	if (m_hardware_shader)
	{
		backend::ref().set_gs			( &*m_hardware_shader);
		backend::ref().set_gs_constants ( &*m_constants);
		backend::ref().set_gs_textures	( &*m_textures);
		backend::ref().set_gs_samplers	( &*m_samplers);
	}
	else
	{
		backend::ref().set_gs			( NULL );
		backend::ref().set_gs_constants ( NULL );
		backend::ref().set_gs_textures	( NULL );
		backend::ref().set_gs_samplers	( NULL );
	}
}

template <>
void res_xs<vs_data>::apply( ) const
{
	backend::ref().set_vs			( &*m_hardware_shader);
	backend::ref().set_vs_constants ( &*m_constants);
	backend::ref().set_vs_textures	( &*m_textures);
	backend::ref().set_vs_samplers	( &*m_samplers);
}

// Specialization definitions
template class res_xs<vs_data>;
template class res_xs<gs_data>;
template class res_xs<ps_data>;

} // namespace render
} // namespace vostok
