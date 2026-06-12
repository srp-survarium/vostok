////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef EMPTY_HANDS_COOK_H_INCLUDED
#define EMPTY_HANDS_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::translate_query_cook;

/* FORWARD REFS */
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::resources::resource_base;

namespace survarium {

class empty_hands_cook : public resources::translate_query_cook , public boost::noncopyable {
public:
					empty_hands_cook				( );

	virtual	void	translate_query					( resources::query_result_for_cook& parent ) override;

	virtual	void	delete_resource					( resources::resource_base* resource ) override;

			void	on_empty_hands_config_loaded	( resources::queries_result& data );
			void	on_empty_hands_animations_loaded( resources::queries_result& data );

	virtual			~empty_hands_cook				( ) { /* no source */ }
}; // class empty_hands_cook

STATIC_SIZE_ASSERT(empty_hands_cook, 0x20);

} // namespace survarium

#endif // #ifndef EMPTY_HANDS_COOK_H_INCLUDED
