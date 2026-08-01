#ifndef VOSTOK_RENDER_CORE_DX11_DESTROY_DATA_HELPER_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_DESTROY_DATA_HELPER_H_INCLUDED

namespace vostok {
namespace render {

struct destroy_data_helper_base {
	virtual void destroy( pcvoid data ) = 0;
};

template < typename T >
struct destroy_data_helper : public destroy_data_helper_base {
	virtual void destroy( pcvoid data )
	{
		// Observed enum_vertex_input_type const instantiation [0x22c50].
		static_cast<T*>( data )->~T( );
	}
};

STATIC_SIZE_ASSERT( destroy_data_helper_base, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_DESTROY_DATA_HELPER_H_INCLUDED
