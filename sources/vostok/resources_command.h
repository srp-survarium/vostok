////////////////////////////////////////////////////////////////////////////
//	Created		: 20.01.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RESOURCES_COMMAND_H_INCLUDED
#define VOSTOK_RESOURCES_COMMAND_H_INCLUDED

namespace vostok {
namespace resources {

class VOSTOK_CORE_API resource_command
{
public:
	resource_command (type_enum type) : m_type(type), m_next(NULL), m_prev(NULL)	{}

	query_result *					cast_query_result	();
	fs_task *						cast_fs_task		();

	resource_command *				m_next;
	resource_command *				m_prev;
private:
	enum	type_enum				{ type_query, type_fs_task };
	type_enum						m_type;
};

typedef	intrusive_double_linked_list<	resource_command, 
										resource_command, 
										& resource_command::m_prev, 
										& resource_command::m_next		>	command_list;

} // namespace resources
} // namespace vostok

#endif // #ifndef VOSTOK_RESOURCES_COMMAND_H_INCLUDED