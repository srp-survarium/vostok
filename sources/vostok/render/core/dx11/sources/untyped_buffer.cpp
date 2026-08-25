#include "pch.h"
#include <vostok/render/core/untyped_buffer.h>
#include "com_utils.h"
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/device.h>

namespace vostok {
namespace render {

untyped_buffer::untyped_buffer	( u32 const size, pcvoid const data, enum_buffer_type const type, bool const is_dynamic, bool const staging ):
	m_size					( size ),
	m_type					( type )
{
	D3D_BUFFER_DESC			desc;
	desc.ByteWidth			= size;
	desc.Usage				= is_dynamic ? D3D_USAGE_DYNAMIC : D3D_USAGE_DEFAULT;
	desc.BindFlags			= (type == enum_buffer_type_index) ? D3D_BIND_INDEX_BUFFER : D3D_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags		= is_dynamic ? D3D_CPU_ACCESS_WRITE : 0;
	desc.MiscFlags			= 0;

	if ( staging )
	{
		desc.Usage			= D3D_USAGE_STAGING;
		desc.BindFlags		= 0;
		desc.CPUAccessFlags	= D3D_CPU_ACCESS_READ;
	}

	D3D_SUBRESOURCE_DATA init_data;
	init_data.pSysMem		= data;
	init_data.SysMemPitch	= 0;
	init_data.SysMemSlicePitch = 0;

	D3D_SUBRESOURCE_DATA* ptr_init_data = data == NULL ? NULL : &init_data;

	HRESULT const result	= device::ref().d3d_device()->CreateBuffer( &desc, ptr_init_data, &m_hardware_buffer);
	CHECK_RESULT			( result );
	ASSERT					( m_hardware_buffer );
}

untyped_buffer::~untyped_buffer	( )
{
	safe_release			( m_hardware_buffer );
}

void untyped_buffer::destroy_impl() const
{
	resource_manager::ref().release( this );
}

} // namespace render
} // namespace vostok
