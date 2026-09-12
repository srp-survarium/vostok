// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef BREATH_HOLDING_STATES_H_INCLUDED
#define BREATH_HOLDING_STATES_H_INCLUDED

#include <vostok/ai/fsm_state.h>

namespace survarium {

class breath_state : public ai::fsm_state , private core::noncopyable {
public:
	virtual	void		set_breath_holding_params	( breath_holding_params const* params );
	inline	float		get_multiplier				( ) { return m_multiplier; }

	virtual	void		tick						( const float arg_0 ) = 0;

	virtual	void		initialize					( ) override;
	virtual	void		execute						( ) override { /* no source */ }
	virtual	void		finalize					( ) override { /* no source */ }

protected:
			explicit	breath_state				( float& breath_holding_reserve );

	/* 0x0000 */	/* ai::fsm_state */
	/* 0x0019 */	/* core::noncopyable */
	/* 0x001c */	float&							m_breath_holding_reserve;
	/* 0x0020 */	breath_holding_params const*	m_params;
	/* 0x0024 */	float							m_multiplier;
}; // class breath_state

STATIC_SIZE_ASSERT(breath_state, 0x28);


class breath_state_holding : public breath_state {
public:
	typedef breath_state super;
	inline	explicit	breath_state_holding		( float& breath_holding_reserve ) : breath_state( breath_holding_reserve ) { }

private:
	// folds with breath_state_normal::is_ready_for_transition (same `return true` body)
	virtual	bool		is_ready_for_transition		( ) const override { return true; }
	virtual	void		set_breath_holding_params	( breath_holding_params const* params ) override;
	virtual	void		tick						( const float dt ) override;
}; // class breath_state_holding

STATIC_SIZE_ASSERT(breath_state_holding, 0x28);

class breath_state_normal : public breath_state {
public:
	typedef breath_state super;
	inline	explicit	breath_state_normal		( float& breath_holding_reserve ) : breath_state( breath_holding_reserve ) { m_multiplier = 1.0f; }

private:
	virtual	bool		is_ready_for_transition	( ) const override;
	virtual	void		tick					( const float dt ) override;
}; // class breath_state_normal

STATIC_SIZE_ASSERT(breath_state_normal, 0x28);

class breath_state_shortbreathing : public breath_state {
public:
	typedef breath_state super;
	inline	explicit	breath_state_shortbreathing	( float& breath_holding_reserve ) : breath_state( breath_holding_reserve ), m_restoring_speed( 1.0f ) { }

private:
	virtual	void		set_breath_holding_params	( breath_holding_params const* params ) override;
	virtual	void		tick						( const float dt ) override;
	virtual	bool		is_ready_for_transition		( ) const override;

	/* 0x0000 */	/* breath_state */
	/* 0x0028 */	float		m_restoring_speed;
}; // class breath_state_shortbreathing

STATIC_SIZE_ASSERT(breath_state_shortbreathing, 0x2C);


} // namespace survarium

#endif // #ifndef BREATH_HOLDING_STATES_H_INCLUDED
