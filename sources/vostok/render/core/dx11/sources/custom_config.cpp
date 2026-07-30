#include "pch.h"
#include <vostok/configs_binary_config.h>
#include <vostok/render/core/custom_config.h>
#include <vostok/render/core/effect_options_descriptor.h>

namespace vostok {
namespace render {

custom_config::custom_config( )
	: own_buffer( false ), call_destructors( true )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55a710]
}

void custom_config::destroy( custom_config* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55a910]
}

u32 custom_config::get_need_buffer_size(
	effect_options_descriptor const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x55a100]
	return 0;
}

namespace {

template < typename value_type >
u16 convert_type( u16 )
{
	return 0;
}

template < >
u16 convert_type<custom_config_value>( u16 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x559d00]
	return 0;
}

template < >
u16 convert_type<effect_options_descriptor>( u16 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x559cf0]
	return 0;
}

template < >
u16 convert_type<configs::binary_config_value>( u16 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x559d60]
	return 0;
}

template < typename source_type, typename destination_type >
void copy_destroyer( source_type const&, destination_type& )
{
}

template < >
void copy_destroyer<configs::binary_config_value, custom_config_value>(
	configs::binary_config_value const&,
	custom_config_value&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x559c70]
}

template < >
void copy_destroyer<custom_config_value, custom_config_value>(
	custom_config_value const&,
	custom_config_value&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x559ce0]
}

template < >
void copy_destroyer<effect_options_descriptor, custom_config_value>(
	effect_options_descriptor const&,
	custom_config_value&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x559cd0]
}

template < typename source_type, typename destination_type >
void construct(
	source_type const&,
	destination_type&,
	mutable_buffer&
)
{
}

template < >
void construct<effect_options_descriptor, custom_config_value>(
	effect_options_descriptor const&,
	custom_config_value&,
	mutable_buffer&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x55a560]
}

template < typename value_type >
void sort_by_crc( value_type& )
{
}

template < >
void sort_by_crc<custom_config_value>( custom_config_value& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x559d10]
}

template < typename value_type >
struct config_crc_predicate {
	static bool compare( value_type const&, value_type const& )
	{
		return false;
	}
};

template < >
bool config_crc_predicate<configs::binary_config_value>::compare(
	configs::binary_config_value const&,
	configs::binary_config_value const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x559ca0]
	return false;
}

void* align4( void* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x559cc0]
	return 0;
}

template < typename value_type >
void fill_data( value_type const&, mutable_buffer& )
{
}

template < >
void fill_data<custom_config_value>(
	custom_config_value const&,
	mutable_buffer&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x55a000]
}

template < typename value_type >
void fill_data_crc_buffer( value_type const&, mutable_buffer& )
{
}

template < >
void fill_data_crc_buffer<custom_config_value>(
	custom_config_value const&,
	mutable_buffer&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x559e10]
}

template < typename value_type >
u32 get_data_crc_buffer_size( value_type const& )
{
	return 0;
}

template < >
u32 get_data_crc_buffer_size<custom_config_value>(
	custom_config_value const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x559eb0]
	return 0;
}

template < typename value_type >
u32 calc_data_crc( value_type const& )
{
	return 0;
}

template < >
u32 calc_data_crc<custom_config_value>( custom_config_value const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55a4f0]
	return 0;
}

template < typename value_type >
u32 get_num_config_fields_impl( value_type const& )
{
	return 0;
}

template < >
u32 get_num_config_fields_impl<configs::binary_config_value>(
	configs::binary_config_value const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x55a140]
	return 0;
}

template < typename value_type >
u32 get_num_config_fields( value_type const& )
{
	return 0;
}

template < >
u32 get_num_config_fields<configs::binary_config_value>(
	configs::binary_config_value const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x55a240]
	return 0;
}

template < typename value_type >
u32 get_config_data_memory_usage( value_type const&, u32& )
{
	return 0;
}

template < >
u32 get_config_data_memory_usage<configs::binary_config_value>(
	configs::binary_config_value const&,
	u32&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x55a190]
	return 0;
}

template < typename value_type >
u32 calc_config_memory_usage_impl( value_type const& )
{
	return 0;
}

template < >
u32 calc_config_memory_usage_impl<configs::binary_config_value>(
	configs::binary_config_value const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x55a6a0]
	return 0;
}

template < >
u32 calc_config_memory_usage_impl<effect_options_descriptor>(
	effect_options_descriptor const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x55a6d0]
	return 0;
}

template < typename value_type >
custom_config* create_custom_config_impl(
	value_type const&,
	mutable_buffer&,
	u32&,
	bool
)
{
	return 0;
}

template < >
custom_config* create_custom_config_impl<effect_options_descriptor>(
	effect_options_descriptor const&,
	mutable_buffer&,
	u32&,
	bool
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x55a840]
	return 0;
}

template < typename value_type >
custom_config_ptr create_custom_config_impl(
	value_type const&,
	u32&,
	bool
)
{
	return custom_config_ptr( );
}

template < >
custom_config_ptr create_custom_config_impl<effect_options_descriptor>(
	effect_options_descriptor const&,
	u32&,
	bool
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x55aa60]
	return custom_config_ptr( );
}

} // namespace

custom_config_ptr create_custom_config(
	effect_options_descriptor const&,
	u32&,
	bool
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x55ab30]
	return custom_config_ptr( );
}

custom_config_ptr create_custom_config(
	custom_config_value const&,
	u32&,
	bool
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x55ab10]
	return custom_config_ptr( );
}

custom_config_ptr create_custom_config(
	configs::binary_config_value const&,
	u32&,
	bool
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x55aaf0]
	return custom_config_ptr( );
}

custom_config_ptr create_custom_config(
	effect_options_descriptor const&,
	mutable_buffer&,
	u32&,
	bool
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x55a950]
	return custom_config_ptr( );
}

u32 calc_config_memory_usage( effect_options_descriptor const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55a8d0]
	return 0;
}

} // namespace render
} // namespace vostok
