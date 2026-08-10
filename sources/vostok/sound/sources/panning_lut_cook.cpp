////////////////////////////////////////////////////////////////////////////
//	Created		: 26.05.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "panning_lut_cook.h"
#include "panning_lut.h"

namespace vostok {
namespace sound {

panning_lut_cook::panning_lut_cook 		( ) :
	super								( resources::sound_panning_lut_class, reuse_true, use_resource_manager_thread_id )
{}

panning_lut_cook::~panning_lut_cook 	( )
{}


void panning_lut_cook::translate_query	( resources::query_result_for_cook&	parent )
{
	resources::user_data_variant* user_data		= parent.user_data( );
	u8 channels_num								= 2;
	if ( user_data )
	{
		user_data->try_get( channels_num );
	}

	panning_lut* lut				= UNMANAGED_NEW ( lut, panning_lut ) ( channels_num );

	if ( !lut )
	{
		parent.set_out_of_memory	( resources::unmanaged_memory, sizeof( panning_lut ) );
		parent.finish_query			( result_out_of_memory );
		return;
	}

	parent.set_unmanaged_resource	( lut, resources::nocache_memory, sizeof( panning_lut ) );
	parent.finish_query				( result_success );
}

void panning_lut_cook::delete_resource	( resources::resource_base* resource )
{
	UNMANAGED_DELETE				( resource );
}

} // namespace sound
} // namespace vostok
