// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DAMAGE_MODEL_COOK_H_INCLUDED
#define DAMAGE_MODEL_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>
#include <vostok/console_command.h>

namespace survarium {

class damage_model_cook : public resources::translate_query_cook {
public:
					damage_model_cook			( );

	virtual	void	translate_query				( resources::query_result_for_cook& parent ) override;
	virtual	void	delete_resource				( resources::resource_base* resource ) override;

public:
	static	pcstr	get_current_hit_type		( );

private:
			void	on_hit_params_received		( resources::queries_result& data );

public:
	static fixed_vector< fixed_string<24>, 12 >					m_hit_types_strings;

private:
	static console_commands::cc_token*							m_hit_types_commands;
	static fixed_vector< console_commands::command_token, 12 >	m_hit_types;

}; // class damage_model_cook

STATIC_SIZE_ASSERT(damage_model_cook, 0x20);

} // namespace survarium

#endif // #ifndef DAMAGE_MODEL_COOK_H_INCLUDED
