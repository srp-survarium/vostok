////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef FLASH_FACTORY_H_INCLUDED
#define FLASH_FACTORY_H_INCLUDED

/* INCLUDES */
class Scaleform::GFx::Loader;
class survarium::scaleform_render_command_queue;

/* FORWARD REFS */
class survarium::flash_movie;
class survarium::flash_text_manager;
class survarium::scaleform_game_engine;

namespace survarium {

class flash_factory {
public:
	inline	explicit				flash_factory		( scaleform_game_engine& arg_0 ) { /* no source */ }
	inline							~flash_factory		( ) { /* no source */ }

	inline	flash_movie*			build_movie			( void* arg_0, u32 arg_1, pcstr arg_2 ) { /* no source */ }
	inline	void					destroy_movie		( flash_movie* arg_0 ) { /* no source */ }

	inline	void					tick				( ) { /* no source */ }

	inline	flash_text_manager*		create_text_manager	( ) { /* no source */ }
	inline	void					destroy_text_manager( flash_text_manager* arg_0 ) { /* no source */ }

private:
	/* 0x0000 */	Scaleform::GFx::Loader*				m_gfx_loader;
	/* 0x0004 */	scaleform_render_command_queue*		m_render_thread_queue;
}; // class flash_factory

STATIC_SIZE_ASSERT(flash_factory, 0x8);

} // namespace survarium

#endif // #ifndef FLASH_FACTORY_H_INCLUDED
