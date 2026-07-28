////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "flash_renderer.h"
#include "flash_movie.h"
#include "flash_text_manager.h"
#include "scaleform_render_command_queue.h"

#include "GFx.h"
#include "GFx_Renderer_D3D1x.h"

namespace survarium {

// claude@NOTE: the flash_renderer trio is reachability-pinned by the scaleform anchor
// (anchor_scaleform.cpp) but parked on render-adjacent D3D1x SDK reconstruction
// (matched last per the render-module-last policy): the ctor (9 stmts) builds a
// Scaleform::Render::D3D1x::HAL + Renderer2D and a D3D1x::HALInitParams (device/
// context/0x100 halConfigFlags) then InitHAL via vtable+0x128; on_reset_device (5 stmts)
// re-runs the same HALInitParams/InitHAL path or HAL::RestoreAfterReset; present (16 stmts)
// drives the per-movie Display + the command-queue. All depend on the D3D1x HAL SDK glue.

// STATE[STUB]
void flash_renderer::present( flash_movie** movies, u32 movies_count, flash_text_manager* text_manager )
{
	// FUNCTION BODY[0x7019d0]
	VOSTOK_UNREFERENCED_PARAMETERS	( movies, movies_count, text_manager );
}

// STATE[STUB]
void flash_renderer::on_reset_device( u32 width, u32 height, ID3D11Device* device, ID3D11DeviceContext* context )
{
	// FUNCTION BODY[0x701b80]
	VOSTOK_UNREFERENCED_PARAMETERS	( width, height, device, context );
}

// STATE[STUB]
flash_renderer::flash_renderer(
		scaleform_render_command_queue*	queue,
		ID3D11Device*					device,
		ID3D11DeviceContext*			context
	)
{
	// FUNCTION BODY[0x701c10]
	VOSTOK_UNREFERENCED_PARAMETERS	( queue, device, context );
}

} // namespace survarium
