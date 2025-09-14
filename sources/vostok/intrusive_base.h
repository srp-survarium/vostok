////////////////////////////////////////////////////////////////////////////
//	Created 	: 10.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_INTRUSIVE_BASE_H_INCLUDED
#define VOSTOK_INTRUSIVE_BASE_H_INCLUDED

#include <vostok/threading_simple_lock.h>

namespace vostok {

	class intrusive_base {
	public:
		inline			intrusive_base	( );

	template < typename T >
	inline	void	destroy			( T* object ) const;

	inline	u32		reference_count	( ) const { return m_reference_count; }

private:
	friend class threading::simple_lock;
	friend class threading::single_threading_policy;
	u32		m_reference_count;
}; // class intrusive_base

class editor_intrusive_base {
public:
	inline			editor_intrusive_base	( );

	template < typename T >
	inline	void	destroy			( T* object ) const;

	inline	u32		reference_count	( ) const { return m_reference_count; }

private:
	friend class threading::simple_lock;
	friend class threading::single_threading_policy;
	u32		m_reference_count;
}; // class editor_intrusive_base

class intrusive_interlocked_base {
public:
	inline intrusive_interlocked_base( );

	template < typename T >
	inline	void	destroy			( T* object ) const;

	inline	u32		reference_count	( ) const { return m_reference_count; }

private:
	friend class threading::single_threading_policy;
	threading::atomic32_type	m_reference_count;
}; // class intrusive_interlocked_base

} // namespace vostok

#include <vostok/intrusive_base_inline.h>

#endif // #ifndef VOSTOK_INTRUSIVE_BASE_H_INCLUDED