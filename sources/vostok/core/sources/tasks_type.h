////////////////////////////////////////////////////////////////////////////
//	Created		: 06.05.2010
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef TASKS_TYPE_H
#define TASKS_TYPE_H

#include <vostok/intrusive_list.h>
#include <vostok/intrusive_queue.h>
#include "tasks_allocator.h"
#include <vostok/tasks_system.h>

namespace vostok {
namespace tasks {

class task_type
{
public:
							task_type			();
							task_type			(pcstr description, task_type_flags_enum const flags);

	u64						min_tasks_ordinal	() const { return m_min_task_ordinal; }
	task_type_flags_enum	flags				() const { return (task_type_flags_enum)m_flags; }
	typedef intrusive_queue<task, task, & task::m_next_task_in_type_queue>		tasks_type_queue;

private:
	tasks_type_queue							m_tasks;
//	VOSTOK_CACHE_LINE_PAD;
	threading::atomic64_type					m_min_task_ordinal;

public: // really private
	task_type *									m_next_task_type;

private:
	pcstr										m_description;
	char										m_flags;

	friend					class				task_manager;
};

typedef			intrusive_list<task_type, task_type *, & task_type::m_next_task_type, threading::mutex_tasks_unaware>		task_type_list_container;

} // namespace tasks
} // namespace vostok

#endif // #ifndef TASKS_TYPE_H
