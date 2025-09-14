////////////////////////////////////////////////////////////////////////////
//	Created 	: 10.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_SHARED_STRING_H_INCLUDED
#define VOSTOK_SHARED_STRING_H_INCLUDED

#include <vostok/strings_shared_profile.h>

template class VOSTOK_CORE_API vostok::intrusive_ptr<
	vostok::strings::shared::profile,
	vostok::strings::shared::detail::intrusive_base,
	vostok::threading::simple_lock
>;

namespace vostok {

class VOSTOK_CORE_API shared_string {
public:
	inline				shared_string						( );
			IMPLICIT	shared_string						( pcstr value );
	inline	IMPLICIT	shared_string						( shared_string const& object );

public:
	inline	pcstr		c_str								( ) const;
	inline	bool		operator!							( ) const;
	inline	bool		operator<							( shared_string const& string ) const;
	inline	bool		operator==							( shared_string const& string ) const;
	inline	bool		operator!=							( shared_string const& string ) const;
	inline	u32			length								( ) const;
	inline	u32			size								( ) const;

public:
	typedef pcstr ( shared_string::*unspecified_bool_type )	( ) const;
	inline				operator unspecified_bool_type		( ) const;

private:
	typedef strings::shared::profile::profile_ptr			profile_ptr;

private:
	profile_ptr			m_pointer;
}; // class shared_string

} // namespace vostok

#include <vostok/shared_string_inline.h>

#endif // #ifndef VOSTOK_SHARED_STRING_H_INCLUDED