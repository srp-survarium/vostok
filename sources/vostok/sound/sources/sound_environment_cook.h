// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SOUND_SOUND_ENVIRONMENT_COOK_H_INCLUDED
#define SOUND_SOUND_ENVIRONMENT_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace resources {
class queries_result;
} // namespace resources

namespace sound {

class sound_environment_cook :
	public resources::translate_query_cook,
	public boost::noncopyable
{
	typedef resources::translate_query_cook super;

public:
					sound_environment_cook			( );

	virtual void	translate_query					( resources::query_result_for_cook& parent );
	virtual void	delete_resource					( resources::resource_base* resource );

private:
			void	on_model_config_loaded			( resources::queries_result& data );
			void	on_environment_options_loaded	( resources::queries_result& data, float4x4* transform );
}; // class sound_environment_cook

STATIC_SIZE_ASSERT(sound_environment_cook, 0x20);

} // namespace sound
} // namespace vostok

#endif // #ifndef SOUND_SOUND_ENVIRONMENT_COOK_H_INCLUDED
