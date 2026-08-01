#include "pch.h"
#include <vostok/render/core/shader_constant_buffer.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/device.h>
#include <vostok/render/core/resource_manager.h>
#include "com_utils.h"

namespace vostok {
namespace render {

void shader_constant_buffer::update()
{
	// FUNCTION BODY[0x738f40]
	if ( m_changed)
	{
		if ( !backend::ref().disabled_shader_constansts_set)
			device::ref().d3d_context()->UpdateSubresource( m_hardware_buffer, 0, NULL, m_buffer_data, 0, 0);

		m_changed = false;
	}
}

shader_constant_buffer::~shader_constant_buffer()
{
	// FUNCTION BODY[0x738f80]
	//	Flush();
	safe_release	( m_hardware_buffer);
	FREE			( m_buffer_data);
}

void shader_constant_buffer::destroy_impl() const
{
	// FUNCTION BODY[0x738fc0]
	resource_manager::ref().release( this);
}

shader_constant_buffer::shader_constant_buffer( fixed_string<64> const & name, enum_shader_type dest, D3D_CBUFFER_TYPE type, u32 size):
m_name			( name),
m_type			( type),
m_dest			( dest),
m_buffer_size	( size),
m_changed		( true),
m_is_registered	( false )
{
	// FUNCTION BODY[0x738fd0]
	D3D_BUFFER_DESC	desc;
	desc.ByteWidth		= m_buffer_size;
	desc.Usage			= D3D_USAGE_DYNAMIC;
	desc.BindFlags		= D3D_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D_CPU_ACCESS_WRITE;
	desc.MiscFlags		= 0;

	HRESULT res = device::ref().d3d_device()->CreateBuffer( &desc, 0, &m_hardware_buffer);
	CHECK_RESULT(res);
	ASSERT( m_hardware_buffer);

	m_buffer_data = ALLOC( u8, m_buffer_size);
	ASSERT( m_buffer_data);

	memset( m_buffer_data, 0, m_buffer_size);
}

} // namespace render
} // namespace vostok
