////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_TEXTURE_STORAGE_H_INCLUDED
#define RENDER_TEXTURE_STORAGE_H_INCLUDED

/* INCLUDES */
class vostok::render::map<unsigned int,vostok::render::texture_pool *,stlp_std::less<unsigned int> >;

/* FORWARD REFS */
class vostok::render::res_texture;

namespace vostok {
namespace render {

class texture_storage {
public:
							texture_storage	( );
							~texture_storage( );

	inline	res_texture*	get				( u32 arg_0, u32 arg_1, DXGI_FORMAT arg_2 ) { /* no source */ }

	inline	void			release			( res_texture const* arg_0 ) { /* no source */ }

private:
	/* 0x0000 */	map< u32, texture_pool*, std::less< u32 > >	m_pools;
	/* 0x0018 */	bool	m_initialized;
}; // class texture_storage

STATIC_SIZE_ASSERT(texture_storage, 0x1C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_TEXTURE_STORAGE_H_INCLUDED
