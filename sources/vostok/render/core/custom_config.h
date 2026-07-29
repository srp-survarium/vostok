#ifndef VOSTOK_RENDER_CORE_CUSTOM_CONFIG_H_INCLUDED
#define VOSTOK_RENDER_CORE_CUSTOM_CONFIG_H_INCLUDED

#include <vostok/resources_unmanaged_resource.h>

#include <vostok/render/core/custom_config_value.h>

namespace vostok {

namespace configs {

class binary_config_value;

} // namespace configs

namespace render {

struct effect_options_descriptor;

struct custom_config : public resources::unmanaged_intrusive_base {
	custom_config( );

	void destroy( custom_config* instance );

	bool operator==( configs::binary_config_value const& ) const
	{
		return false;
	}

	bool operator==( custom_config_value const& ) const
	{
		return false;
	}

	custom_config_value const& root( ) const
	{
		return m_root;
	}

	operator custom_config_value const&( ) const
	{
		return m_root;
	}

	static u32 get_need_buffer_size( effect_options_descriptor const& value );

	bool				own_buffer;
	bool				call_destructors;
	custom_config_value	m_root;
};

STATIC_SIZE_ASSERT( custom_config, 0x20 );

typedef intrusive_ptr<
	custom_config,
	custom_config,
	threading::simple_lock
> custom_config_ptr;

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_CUSTOM_CONFIG_H_INCLUDED
