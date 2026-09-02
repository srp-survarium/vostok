// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.11.2025
////////////////////////////////////////////////////////////////////////////
#ifndef LOGGING_LOGGING_FILTERS_CONSOLE_COMMAND_H_INCLUDED
#define LOGGING_LOGGING_FILTERS_CONSOLE_COMMAND_H_INCLUDED

#include <vostok/logging/api.h>
#include <vostok/console_command.h>

namespace vostok {
namespace logging {

class filter_tree;

class logging_filters_console_command : public console_commands::console_command , private boost::noncopyable {
public:
			logging_filters_console_command	(
						filter_tree&						filter_tree,
						pcstr const							name,
						bool const							serializable,
						console_commands::command_type const	command_type,
						console_commands::execution_filter const	execution_filter
					);

	virtual	void	execute							( pcstr args ) override;

	virtual	void	save_to							( console_commands::save_storage& f, memory::base_allocator* a ) const override;

private:
	/* 0x0000 */	/* console_commands::console_command */
	/* 0x0041 */	/* boost::noncopyable */
	/* 0x0044 */	filter_tree&	m_filter_tree;
}; // class logging_filters_console_command

STATIC_SIZE_ASSERT(logging_filters_console_command, 0x48);

} // namespace logging
} // namespace vostok

#endif // #ifndef LOGGING_LOGGING_FILTERS_CONSOLE_COMMAND_H_INCLUDED
