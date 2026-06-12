////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef GENERATE_SHADERS_WORLD_H_INCLUDED
#define GENERATE_SHADERS_WORLD_H_INCLUDED

/* INCLUDES */
class vostok::render::game::renderer;
struct vostok::engine_user::world;

/* FORWARD REFS */
class vostok::input::world;
class vostok::render::world;
class vostok::ui::world;

namespace survarium {

class generate_shaders_world : public engine_user::world {
public:
			explicit			generate_shaders_world		( render::world& render_world );

	virtual	void				enable						( bool value ) override { /* no source */ }

	virtual	void				tick						( u32 current_frame_id ) override;

	virtual	void				clear_resources				( ) override { /* no source */ }

	virtual	void				load						( pcstr arg_0 ) override { /* no source */ }

	virtual	bool				is_loading					( ) const override { /* no source */ }

	virtual	void				unload						( pcstr __formal, bool __formal ) override { /* no source */ }

	virtual	void				on_application_activate		( ) override { /* no source */ }
	virtual	void				on_application_deactivate	( ) override { /* no source */ }
	virtual	void				on_fullscreen_alttab		( bool arg_0 ) override { /* no source */ }

	virtual	ui::world&			ui_world					( ) override { /* no source */ }
	virtual	input::world&		input_world					( ) override { /* no source */ }

			void				generate_renderer_shaders	( );
			void				generate_materials_shaders	( );

	virtual						~generate_shaders_world		( ) { /* no source */ }

private:
	/* 0x0000 */	/* engine_user::world */
	/* 0x0004 */	render::game::renderer&		m_renderer;
	/* 0x0008 */	bool						m_first_call_reset_renderer;
}; // class generate_shaders_world

STATIC_SIZE_ASSERT(generate_shaders_world, 0xC);

} // namespace survarium

#endif // #ifndef GENERATE_SHADERS_WORLD_H_INCLUDED
