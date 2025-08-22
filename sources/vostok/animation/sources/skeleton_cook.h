////////////////////////////////////////////////////////////////////////////
//	Created		: 18.07.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SKELETON_COOK_H_INCLUDED
#define SKELETON_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace animation {

class skeleton_cook :
	public resources::translate_query_cook,
	private boost::noncopyable
{
public:
					skeleton_cook			( ) ;
			void	on_sub_resources_loaded	( resources::queries_result & result );
	virtual void	translate_query			( resources::query_result_for_cook& parent );
	virtual void	delete_resource			( resources::resource_base* res );

private:
	typedef resources::translate_query_cook	super;
}; // class skeleton_cook

} // namespace animation
} // namespace vostok

#endif // #ifndef SKELETON_COOK_H_INCLUDED