#include "pch.h"
#include <vostok/render/core/custom_config_value.h>
#include <boost/crc.hpp>
#include <vostok/render/core/destroy_data_helper.h>

namespace vostok {
namespace render {

bool custom_config_value::value_exists( pcstr key ) const
{
	// FUNCTION BODY[0x73a440]
	R_ASSERT					(type == vostok::configs::t_table_named || type == vostok::configs::t_table_indexed);
	const_iterator it			= begin();
	const_iterator it_e			= end();

	boost::crc_32_type			processor;
	processor.process_block		(key, key + strings::length(key));
	u32 const crc				= processor.checksum();

	const_iterator const result	= std::lower_bound(it, it_e, crc);
	if ( result == it_e)
		return					false;

	if ( result->id_crc != crc )
		return					false;

	return						strings::equal( key, result->id );
}

void custom_config_value::call_data_destructor( ) const
{
	// FUNCTION BODY[0x73a320]
	if (destroyer)
	{
		if (count<=sizeof(u32))
			((destroy_data_helper_base*)&destroyer)->destroy(&data);
		else
			((destroy_data_helper_base*)&destroyer)->destroy(data);
	}

	if (type!=vostok::configs::t_table_named && type!=vostok::configs::t_table_indexed)
		return;

	for (const_iterator it=begin(); it!=end(); ++it)
	{
		it->call_data_destructor();
	}
}

custom_config_value const& custom_config_value::operator[]( pcstr key ) const
{
	// FUNCTION BODY[0x73a380]
	R_ASSERT					(type == vostok::configs::t_table_named);
	const_iterator it			= begin();
	const_iterator it_e			= end();

	boost::crc_32_type			processor;
	processor.process_block		(key, key + strings::length(key));
	u32 const crc				= processor.checksum();

	const_iterator const result	= std::lower_bound(it, it_e, crc);

	R_ASSERT					(result!=it_e, "item not found [%s]", key);
	R_ASSERT					(result->id_crc==crc, "item not found [%s]", id);

	do {
		if ( strings::equal( key, result->id ) )
			return				*result;
	} while ( result->id_crc == crc );

	UNREACHABLE_CODE			( return *result );
}

} // namespace render
} // namespace vostok
