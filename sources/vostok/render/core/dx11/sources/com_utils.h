#ifndef VOSTOK_RENDER_CORE_DX11_SOURCES_COM_UTILS_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_SOURCES_COM_UTILS_H_INCLUDED

template < typename com_type >
inline void safe_release( com_type*& object )
{
	if ( !object )
		return;

	object->Release( );
	object = 0;
}

template < typename com_type >
inline void log_ref_count( pcstr object_name, com_type* object )
{
	ASSERT( object );

	object->AddRef( );
	ULONG const reference_count = object->Release( );
	LOG_INFO( "%-20s:refCount: %d", object_name, reference_count );

	// FUNCTION BODY[0x124450] for IDXGIAdapter
	// FUNCTION BODY[0x12b980] for ID3D11Texture2D
	// FUNCTION BODY[0x12bb60] for ID3D11DepthStencilView
}

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_SOURCES_COM_UTILS_H_INCLUDED
