////////////////////////////////////////////////////////////////////////////
//	Created		: 06.11.2009
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

namespace vostok {
namespace rtp {

template< class space_params, class world_space_params, class action  >
abstract_action<space_params, world_space_params, action>::abstract_action( vostok::rtp::action_base<action_type> &ab )
:m_id(u32(-1)),
m_action_base(ab)
{
	m_id = m_action_base.add(static_cast<action_type*>(this));
}

template< class space_params, class world_space_params, class action  >
bool	abstract_action<space_params, world_space_params, action>::is_my_base	( const  action_base<action_type> &ab )const	
{
	return &m_action_base == &ab && m_action_base.get( id() ) == this;
}

#ifndef MASTER_GOLD
template< class space_params, class world_space_params, class action  >
inline void	abstract_action<space_params, world_space_params, action>::save				( vostok::configs::lua_config_value cfg )const
{
	cfg["id"] = m_id;
}
#endif

template< class space_params, class world_space_params, class action  >
inline void	abstract_action<space_params, world_space_params, action>::load				( const vostok::configs::binary_config_value &cfg )
{
	m_id = cfg["id"];
}

} // namespace rtp
} // namespace vostok