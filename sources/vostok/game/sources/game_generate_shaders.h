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

	// STATE[STUB]
	virtual	void			enable						( bool value ) override
	{
		// FUNCTION BODY[0x22c50]
		// <0x22c50>|0x000|      :'28'	{
		// ******
	}

	virtual	void			tick						( u32 current_frame_id ) override;

	// STATE[STUB]
	virtual	void			clear_resources				( ) override
	{
		// FUNCTION BODY[0x4f210]
		// <0x4f210>|0x000|      :'30'	{
		// ******
	}

	// target mangling has the one-param shape; our legacy engine_user.h world
	// interface declares load( pcstr, pcstr = NULL ), so no `override` here
	// (same family as the input::handler quirk; revisit at TU enablement)
	virtual	void			load						( pcstr arg_0 ) { /* no source */ }

	// not in our legacy engine_user.h world interface (see load above)
	virtual	bool			is_loading					( ) const { /* no source */ return false; }

	// STATE[STUB]
	// PDB: both params __formal, genuinely unused (left unnamed - two __formal collide)
	virtual	void			unload						( pcstr, bool ) override
	{
		// FUNCTION BODY[0x38090]
		// <0x38090>|0x000|      :'33'	{
		// ******
	}

	virtual	void			on_application_activate		( ) override { /* no source */ }
	virtual	void			on_application_deactivate	( ) override { /* no source */ }

	// not in our legacy engine_user.h world interface (see load above)
	virtual	void			on_fullscreen_alttab		( bool arg_0 ) { /* no source */ }

	// STATE[STUB]
	virtual	ui::world&		ui_world					( ) override
	{
		return *( ui::world* )NULL;	// buildability return

		// FUNCTION BODY[0x427c0]
		// <0x427c0>|0x000|      :'37'	{
		// ******
	}

	virtual	input::world&	input_world					( ) override { /* no source */ return *( input::world* )NULL; }

			void			generate_renderer_shaders	( );
			void			generate_materials_shaders	( );

	virtual					~generate_shaders_world		( ) { /* no source */ }

private:
	/* 0x0000 */	/* engine_user::world */
	/* 0x0004 */	render::game::renderer&		m_renderer;
	/* 0x0008 */	bool						m_first_call_reset_renderer;
}; // class generate_shaders_world

STATIC_SIZE_ASSERT(generate_shaders_world, 0xC);

} // namespace survarium

#endif // #ifndef GAME_GENERATE_SHADERS_H_INCLUDED
