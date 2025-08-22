////////////////////////////////////////////////////////////////////////////
//	Created		: 15.04.2010
//	Author		: Sergey Pryshchepa
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////
#ifndef DIALOG_COOKER_H_INCLUDED
#define DIALOG_COOKER_H_INCLUDED

#pragma managed( push, off )

#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace dialog_editor {

class dialog_cooker : public resources::translate_query_cook
{
public:
					dialog_cooker		();
	virtual void	translate_query		(vostok::resources::query_result_for_cook& parent);
	virtual void	delete_resource		(resources::resource_base* res);

private:
			void	on_loaded			(resources::queries_result& result);
}; // class dialog_cooker

} // namespace dialog_editor 
} // namespace vostok 

#pragma managed( pop )

#endif // #ifndef DIALOG_COOKER_H_INCLUDED
