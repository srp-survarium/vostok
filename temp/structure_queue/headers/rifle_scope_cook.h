////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RIFLE_SCOPE_COOK_H_INCLUDED
#define RIFLE_SCOPE_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::translate_query_cook;
class vostok::configs::binary_config;

/* FORWARD REFS */
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::resources::resource_base;

namespace survarium {

class rifle_scope_cook : public resources::translate_query_cook , public boost::noncopyable {
public:
					rifle_scope_cook		( );

	virtual	void	translate_query			( resources::query_result_for_cook& parent ) override;

	virtual	void	delete_resource			( resources::resource_base* resource ) override { /* no source */ }

			void	on_config_loaded		( resources::queries_result& data );
			void	on_subresources_loaded	( resources::queries_result& results, configs::binary_config_ptr const& config );

	virtual			~rifle_scope_cook		( ) { /* no source */ }
}; // class rifle_scope_cook

STATIC_SIZE_ASSERT(rifle_scope_cook, 0x20);

} // namespace survarium

#endif // #ifndef RIFLE_SCOPE_COOK_H_INCLUDED
