////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CUSTOM_CONFIG_H_INCLUDED
#define RENDER_CUSTOM_CONFIG_H_INCLUDED

/* INCLUDES */
class vostok::render::custom_config_value;
class vostok::resources::unmanaged_intrusive_base;

/* FORWARD REFS */
class vostok::configs::binary_config_value;
class vostok::render::effect_options_descriptor;

namespace vostok {
namespace render {

struct custom_config : public resources::unmanaged_intrusive_base {
											custom_config	( );

			void							destroy			( custom_config* in_this );

	inline	bool							operator==		( configs::binary_config_value const& arg_0 ) { /* no source */ }
	inline	bool							operator==		( custom_config_value const& arg_0 ) { /* no source */ }

	inline	custom_config_value const&		root			( ) { /* no source */ }

	inline	custom_config_value const&		operator const class vostok::render::custom_config_value &( ) { /* no source */ }

	static	u32								get_need_buffer_size( effect_options_descriptor const& v );

	/* 0x0000 */	/* resources::unmanaged_intrusive_base */
	/* 0x0008 */	bool					own_buffer;
	/* 0x0009 */	bool					call_destructors;
	/* 0x000c */	custom_config_value		m_root;
}; // struct custom_config

STATIC_SIZE_ASSERT(custom_config, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CUSTOM_CONFIG_H_INCLUDED
