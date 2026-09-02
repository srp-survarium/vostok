// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SCALEFORM_RENDER_COMMAND_QUEUE_IMPL_H_INCLUDED
#define SCALEFORM_RENDER_COMMAND_QUEUE_IMPL_H_INCLUDED

#include "Render/Render_ThreadCommandQueue.h"	// Scaleform::Render::ThreadCommandQueue (base)

namespace Scaleform {
namespace Render {
	class HAL;
	class Renderer2D;
	class TextureManager;
	// The shipped wrapper adds GetRenderInterfaces beyond this SDK base.
	struct Interfaces;
} // namespace Render
} // namespace Scaleform

namespace survarium {

class scaleform_game_engine;
class flash_factory;
struct flash_renderer;

class scaleform_render_command_queue_impl : public Scaleform::Render::ThreadCommandQueue , private boost::noncopyable {
public:
	inline	explicit	scaleform_render_command_queue_impl	( scaleform_game_engine& arg_0 )
		:	engine	( arg_0 )
	{ }

	virtual	void		PushThreadCommand					( Scaleform::Render::ThreadCommand* arg_0 ) override;

	// no `override`: the vendored SDK's ThreadCommandQueue lacks this virtual (see above)
	virtual	void		GetRenderInterfaces					( Scaleform::Render::Interfaces* arg_0 );


public:
	friend class flash_factory;
	friend struct flash_renderer;

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
