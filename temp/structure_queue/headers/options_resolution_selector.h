////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef OPTIONS_RESOLUTION_SELECTOR_H_INCLUDED
#define OPTIONS_RESOLUTION_SELECTOR_H_INCLUDED

/* INCLUDES */
class vostok::fixed_string<32>;
class survarium::options_item_int;

/* FORWARD REFS */
class survarium::options_tab;

namespace survarium {

class options_resolution_selector : public options_item_int {
public:
			explicit	options_resolution_selector	( options_tab& parent_tab );
	inline				~options_resolution_selector( ) { /* no source */ }

	virtual	void		initialize					( ) override;

	virtual	void		apply						( ) override;

			void		fill_resolutions			( u8 monitor_number );

private:
	/* 0x0000 */	/* options_item_int */
	/* 0x0020 */	fixed_string< 32 >		m_cached_resolutions[512];
}; // class options_resolution_selector

STATIC_SIZE_ASSERT(options_resolution_selector, 0x5820);

} // namespace survarium

#endif // #ifndef OPTIONS_RESOLUTION_SELECTOR_H_INCLUDED
