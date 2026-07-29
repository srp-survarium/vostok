////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CUSTOM_CONFIG_VALUE_H_INCLUDED
#define RENDER_CUSTOM_CONFIG_VALUE_H_INCLUDED

namespace vostok {
namespace render {

class custom_config_value {
public:
	inline									custom_config_value	( ) { /* no source */ }

	inline	u32								size				( ) const { /* no source */ }

	inline	custom_config_value const*		begin				( ) const { /* no source */ }

	inline	custom_config_value const*		end					( ) const { /* no source */ }

			custom_config_value const&		operator[]			( pcstr key ) const;

	inline	bool							operator<			( u32 arg_0 ) const { /* no source */ }

	inline	pcvoid							get_data_ptr		( ) const { /* no source */ }

			bool							value_exists		( pcstr key ) const;

private:
			void							call_data_destructor( ) const;

public:
	/* 0x0000 */	pcstr		id;
	/* 0x0004 */	pcvoid		data;
	/* 0x0008 */	u32			id_crc;
	/* 0x000c */	u16			type;
	/* 0x000e */	u16			count;
	/* 0x0010 */	pcvoid		destroyer;
}; // class custom_config_value

STATIC_SIZE_ASSERT(custom_config_value, 0x14);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CUSTOM_CONFIG_VALUE_H_INCLUDED
