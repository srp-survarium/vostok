////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef FLASH_FACTORY_H_INCLUDED
#define FLASH_FACTORY_H_INCLUDED

// the legacy header defined flash_movie inline, so render's consumers
// (flash_renderer.cpp) expect the complete type from this include
#include "flash_movie.h"

namespace Scaleform {
namespace GFx {
	class Loader;
} // namespace GFx
} // namespace Scaleform

namespace survarium {

struct flash_text_manager;
class scaleform_game_engine;
class scaleform_render_command_queue;

class flash_factory {
public:
			explicit				flash_factory		( scaleform_game_engine& arg_0 );
									~flash_factory		( );

			flash_movie*			build_movie			( void* arg_0, u32 arg_1, pcstr arg_2 );
			void					destroy_movie		( flash_movie* arg_0 );

			void					tick				( );

			flash_text_manager*		create_text_manager	( );
	inline	void					destroy_text_manager( flash_text_manager* arg_0 ) { }

private:
	/* 0x0000 */	Scaleform::GFx::Loader*				m_gfx_loader;

public:
	/* 0x0004 */	scaleform_render_command_queue*		m_render_thread_queue;
}; // class flash_factory

STATIC_SIZE_ASSERT(flash_factory, 0x8);

} // namespace survarium

#endif // #ifndef FLASH_FACTORY_H_INCLUDED
