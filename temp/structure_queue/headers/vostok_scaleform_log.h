////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_SCALEFORM_LOG_H_INCLUDED
#define VOSTOK_SCALEFORM_LOG_H_INCLUDED

/* INCLUDES */
class Scaleform::Log;

namespace survarium {

class vostok_scaleform_log : public Scaleform::Log {
public:
	virtual	void	LogMessageVarg			( Scaleform::LogMessageId arg_0, pcstr arg_1, char* arg_2 ) override { /* no source */ }

	inline			vostok_scaleform_log	( ) { /* no source */ }
	virtual			~vostok_scaleform_log	( ) { /* no source */ }
}; // class vostok_scaleform_log

STATIC_SIZE_ASSERT(vostok_scaleform_log, 0x8);

} // namespace survarium

#endif // #ifndef VOSTOK_SCALEFORM_LOG_H_INCLUDED
