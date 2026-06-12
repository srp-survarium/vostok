////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef CIRCULAR_BUFFER_H_INCLUDED
#define CIRCULAR_BUFFER_H_INCLUDED

/* INCLUDES */
class vostok::memory::base_allocator;
struct survarium::client_player_history_item;

namespace survarium {

class circular_buffer< client_player_history_item > : public boost::noncopyable {
public:
	inline											circular_buffer<survarium::client_player_history_item>( memory::base_allocator& arg_0, const u32 arg_1 ) { /* no source */ }
	inline											~circular_buffer<survarium::client_player_history_item>( ) { /* no source */ }

	inline	void									resize		( const u32 arg_0 ) { /* no source */ }

	inline	u32										size		( ) const { /* no source */ }

	inline	client_player_history_item&				new_item	( ) { /* no source */ }

	inline	client_player_history_item&				insert		( const u32 arg_0 ) { /* no source */ }

	inline	void									pop_tail	( ) { /* no source */ }

	inline	client_player_history_item&				oldest		( ) { /* no source */ }
	inline	client_player_history_item const&		oldest		( ) const { /* no source */ }

	inline	client_player_history_item&				newest		( ) { /* no source */ }
	inline	client_player_history_item const&		newest		( ) const { /* no source */ }

	inline	bool									empty		( ) const { /* no source */ }

	inline	bool									full		( ) const { /* no source */ }

	inline	u32										next		( u32 arg_0 ) const { /* no source */ }

	inline	u32										previous	( u32 arg_0 ) const { /* no source */ }

	inline	client_player_history_item&				operator[]	( const u32 arg_0 ) { /* no source */ }
	inline	client_player_history_item const&		operator[]	( const u32 arg_0 ) const { /* no source */ }

	inline	u32										head		( ) const { /* no source */ }

	inline	u32										tail		( ) const { /* no source */ }

	inline	u32										max_count	( ) const { /* no source */ }

	inline	u32										index		( client_player_history_item const& arg_0 ) const { /* no source */ }

	inline	bool									is_valid	( client_player_history_item const& arg_0 ) const { /* no source */ }

private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	client_player_history_item*		m_history;
	/* 0x0004 */	memory::base_allocator&			m_allocator;
	/* 0x0008 */	u32								m_max_count;
	/* 0x000c */	u32								m_head;
	/* 0x0010 */	u32								m_tail;
}; // class circular_buffer< client_player_history_item >

STATIC_SIZE_ASSERT(circular_buffer< client_player_history_item >, 0x14);

} // namespace survarium

#endif // #ifndef CIRCULAR_BUFFER_H_INCLUDED
