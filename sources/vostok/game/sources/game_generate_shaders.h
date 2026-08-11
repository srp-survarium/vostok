////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_GENERATE_SHADERS_H_INCLUDED
#define GAME_GENERATE_SHADERS_H_INCLUDED

#include <vostok/engine/engine_user.h>

namespace vostok {
namespace render {
	class world;
	namespace game {
		class renderer;
	} // namespace game
} // namespace render
} // namespace vostok

namespace survarium {

// void* generate_shaders_world::`scalar deleting destructor'( u32 ) // FUNCTION BODY[0x1234a0]: <0xb2740>|0x000|      :'32'	{

// one of game_entry_point.cpp's pending statics (s_generate_shaders_world) -
// that refresh belongs to the TU-enablement batch
class generate_shaders_world : public engine_user::world {
public:
			explicit		generate_shaders_world		( render::world& render_world );

	virtual	void			enable						( bool value ) override
	{
	}

	virtual	void			tick						( u32 current_frame_id ) override;

	virtual	void			clear_resources				( ) override
	{
	}

	virtual	void			load						( pcstr arg_0 ) override { /* no source */ }

	virtual	bool			is_loading					( ) const { /* no source */ return false; }

	virtual	void			unload						( pcstr, bool ) override
	{
	}

	virtual	void			on_application_activate		( ) override { /* no source */ }
	virtual	void			on_application_deactivate	( ) override { /* no source */ }

	// not in our legacy engine_user.h world interface (see load above)
	virtual	void			on_fullscreen_alttab		( bool arg_0 ) { /* no source */ }

	virtual	ui::world&		ui_world					( ) override
	{
		return *( ui::world* )NULL;
	}

	virtual	input::world&	input_world					( ) override { /* no source */ return *( input::world* )NULL; }

	virtual					~generate_shaders_world		( ) { /* no source */ }

private:
			void			generate_renderer_shaders	( );
			void			generate_materials_shaders	( );
	/* 0x0000 */	/* engine_user::world */
	/* 0x0004 */	render::game::renderer&		m_renderer;
	/* 0x0008 */	bool						m_first_call_reset_renderer;
}; // class generate_shaders_world

STATIC_SIZE_ASSERT(generate_shaders_world, 0xC);

} // namespace survarium

#endif // #ifndef GAME_GENERATE_SHADERS_H_INCLUDED
