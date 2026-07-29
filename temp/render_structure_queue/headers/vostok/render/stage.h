////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_H_INCLUDED
#define RENDER_STAGE_H_INCLUDED

/* INCLUDES */
class vostok::render::renderer;
class vostok::render::renderer_context;

namespace vostok {
namespace render {

class stage : public boost::noncopyable {
public:
						stage					( renderer* in_renderer, renderer_context* in_context );
	virtual				~stage					( ) { /* no source */ }

	virtual	void		execute					( );

	virtual	void		execute_disabled		( ) { /* no source */ }

	virtual	void		debug_render			( ) { /* no source */ }

	virtual	pcstr		stage_name				( ) const { /* no source */ }

			void		set_enabled				( bool enable );
	virtual	bool		is_enabled				( ) const;

	inline	void		try_disable				( ) { /* no source */ }
	inline	void		try_enable				( ) { /* no source */ }

	inline	void		u_compute_texgen_screen	( float4x4& arg_0 ) { /* no source */ }
	inline	void		u_compute_texgen_jitter	( float4x4& arg_0 ) { /* no source */ }

	static	inline	void		rm_near					( ) { /* no source */ }
	static	inline	void		rm_normal				( ) { /* no source */ }
	static	inline	void		rm_far					( ) { /* no source */ }

	/* 0x0004 */	/* boost::noncopyable */
protected:
	/* 0x0004 */	renderer_context*		m_context;
	/* 0x0008 */	renderer*				m_renderer;
	/* 0x000c */	bool					m_enabled;
	/* 0x000d */	bool					m_prev_enabled;
}; // class stage

STATIC_SIZE_ASSERT(stage, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_H_INCLUDED
