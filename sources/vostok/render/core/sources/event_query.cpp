#include "pch.h"
#include <vostok/render/core/dx11/device.h>
#include <vostok/render/core/dx11/sources/com_utils.h>
#include <vostok/render/core/sources/event_query.h>

namespace vostok {
namespace render {

event_query::event_query( )
	: m_query( 0 )
{
	init_query( );
}

event_query::~event_query( )
{
	release_query( );
}

void event_query::init_query( )
{
	D3D11_QUERY_DESC query_desc;
	query_desc.MiscFlags = 0;
	query_desc.Query = D3D11_QUERY_EVENT;
	device::ref( ).d3d_device( )->CreateQuery( &query_desc, &m_query );
}

void event_query::release_query( )
{
	if ( m_query )
		m_query->Release( );

	m_query = 0;
}

void event_query::issue( )
{
	device::ref( ).d3d_context( )->End( m_query );
}

void event_query::wait( )
{
	BOOL result = FALSE;
	while ( device::ref( ).get_query_data( m_query, &result, sizeof( result ), true ) && !result )
	{
	}
}

} // namespace render
} // namespace vostok
