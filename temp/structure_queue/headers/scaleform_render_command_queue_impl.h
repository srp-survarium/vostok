////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef SCALEFORM_RENDER_COMMAND_QUEUE_IMPL_H_INCLUDED
#define SCALEFORM_RENDER_COMMAND_QUEUE_IMPL_H_INCLUDED

/* INCLUDES */
class Scaleform::Render::HAL;
class Scaleform::Render::Renderer2D;
class Scaleform::Render::TextureManager;
class Scaleform::Render::ThreadCommandQueue;
class survarium::scaleform_game_engine;

namespace survarium {

class scaleform_render_command_queue_impl : public Scaleform::Render::ThreadCommandQueue , public boost::noncopyable {
public:
	inline	explicit	scaleform_render_command_queue_impl	( scaleform_game_engine& arg_0 ) { /* no source */ }

	virtual	void		PushThreadCommand					( Scaleform::Render::ThreadCommand* arg_0 ) override { /* no source */ }

	virtual	void		GetRenderInterfaces					( Scaleform::Render::Interfaces* arg_0 ) override { /* no source */ }

	virtual				~scaleform_render_command_queue_impl( ) { /* no source */ }

private:
	/* 0x0000 */	/* Scaleform::Render::ThreadCommandQueue */
	/* 0x0004 */	/* boost::noncopyable */
	/* 0x0004 */	Scaleform::Render::HAL*					pHAL;
	/* 0x0008 */	Scaleform::Render::Renderer2D*			pR2D;
	/* 0x000c */	Scaleform::Render::TextureManager*		pHALTextureMgr;
	/* 0x0010 */	void*									pRenderThreadId;
	/* 0x0014 */	scaleform_game_engine&					engine;
}; // class scaleform_render_command_queue_impl

STATIC_SIZE_ASSERT(scaleform_render_command_queue_impl, 0x18);

} // namespace survarium

#endif // #ifndef SCALEFORM_RENDER_COMMAND_QUEUE_IMPL_H_INCLUDED
