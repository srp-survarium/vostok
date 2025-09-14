////////////////////////////////////////////////////////////////////////////
//	Created		: 15.04.2010
//	Author		: Sergey Pryshchepa
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////
#ifndef GAME_DIALOG_COOKER_H_INCLUDED
#define GAME_DIALOG_COOKER_H_INCLUDED

#pragma managed( push, off )

#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace dialog_editor {
	class dialog;
	class game_dialog_cooker : public resources::translate_query_cook
	{
	public:
						game_dialog_cooker		();
		virtual void	translate_query		(vostok::resources::query_result_for_cook& parent);
		virtual void	delete_resource		(resources::resource_base* res);

	private:
				void	on_loaded			(resources::queries_result& result);
	}; // class game_dialog_cooker
} // namespace dialog_editor 
} // namespace vostok 

#pragma managed( pop )

#endif // #ifndef GAME_DIALOG_COOKER_H_INCLUDED