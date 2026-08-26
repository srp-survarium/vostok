////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RIFLE_SCOPE_COOK_H_INCLUDED
#define RIFLE_SCOPE_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace survarium {

class rifle_scope_cook : public resources::translate_query_cook , private boost::noncopyable {
public:
					rifle_scope_cook		( );

private:
	virtual	void	translate_query			( resources::query_result_for_cook& parent ) override;

	virtual	void	delete_resource			( resources::resource_base* resource ) override
	{
		VOSTOK_DELETE_IMPL( g_allocator, resource );
	}

			void	on_config_loaded		( resources::queries_result& data );
			void	on_subresources_loaded	( resources::queries_result& results, configs::binary_config_ptr const& config );

public:
	typedef	resources::translate_query_cook	super;
}; // class rifle_scope_cook

STATIC_SIZE_ASSERT(rifle_scope_cook, 0x20);

} // namespace survarium

#endif // #ifndef RIFLE_SCOPE_COOK_H_INCLUDED
