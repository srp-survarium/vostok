////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BREATH_STATE_H_INCLUDED
#define BREATH_STATE_H_INCLUDED

#include <vostok/ai/fsm_state.h>
#include <vostok/detail_noncopyable.h>

namespace survarium {

struct breath_holding_params;

class breath_state : public ai::fsm_state, private core::noncopyable {
public:
	virtual	void		set_breath_holding_params	( breath_holding_params const* params ) { /* no source */ }

	inline	float		get_multiplier				( ) { return m_multiplier; }

	virtual	void		tick						( float dt ) = 0;

	virtual	void		initialize					( ) { /* no source */ }
	virtual	void		execute						( ) { /* no source */ }
	virtual	void		finalize					( ) { /* no source */ }

	inline	explicit	breath_state				( float& breath_holding_reserve ) : m_breath_holding_reserve( breath_holding_reserve ) { }
	virtual				~breath_state				( ) { /* no source */ }

private:
	/* 0x0000 */	/* ai::fsm_state */
	/* 0x0019 */	/* core::noncopyable */
	/* 0x001c */	float&							m_breath_holding_reserve;
	/* 0x0020 */	breath_holding_params const*	m_params;
	/* 0x0024 */	float							m_multiplier;
}; // class breath_state

STATIC_SIZE_ASSERT(breath_state, 0x28);

} // namespace survarium

#endif // #ifndef BREATH_STATE_H_INCLUDED
