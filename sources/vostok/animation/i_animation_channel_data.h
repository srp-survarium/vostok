////////////////////////////////////////////////////////////////////////////
//	Created		: 19.01.2010
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef I_ANIMATION_CHANNEL_DATA_H_INCLUDED
#define I_ANIMATION_CHANNEL_DATA_H_INCLUDED

#include <vostok/animation/i_bi_spline_data.h>

namespace vostok {
namespace animation {

class i_animation_channel_data {

public:
	typedef i_bi_spline_data<float>			i_data_channel_type;

public:
	virtual void							save					( vostok::configs::lua_config_value config  )const =0;
	virtual void							load					( vostok::configs::lua_config_value const& config  ) =0;

public:
	virtual const i_data_channel_type&		bi_spline				()const			=0;
	virtual i_data_channel_type&			bi_spline				()				=0;

protected:
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( i_animation_channel_data )
}; // class i_animation_channel_data

} // namespace animation 
} // namespace vostok 

#endif // #ifndef I_ANIMATION_CHANNEL_DATA_H_INCLUDED