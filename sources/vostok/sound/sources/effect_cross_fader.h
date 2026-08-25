////////////////////////////////////////////////////////////////////////////
//	Created		: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef SOUND_EFFECT_CROSS_FADER_H_INCLUDED
#define SOUND_EFFECT_CROSS_FADER_H_INCLUDED

namespace vostok {
namespace sound {

class sound_environment;
class sound_scene;

struct effect_cross_fader : private noncopyable
{
								effect_cross_fader	(
									sound_scene& scene,
									u32 fade_time_in_ms,
									IXAudio2SubmixVoice* first_submix,
									IXAudio2SubmixVoice* second_submix
								);

			void				tick					( u32 delta_time_in_ms, sound_environment* current_environment );
	inline	IXAudio2SubmixVoice*
								submix					( ) { return m_fade_in_submix; }

private:
	sound_scene&				m_scene;
	u32							m_fade_time;
	float						m_fade_in_value;
	sound_environment*			m_fade_in_environment;
	sound_environment*			m_fade_out_environment;
	IXAudio2SubmixVoice*		m_fade_in_submix;
	IXAudio2SubmixVoice*		m_fade_out_submix;
}; // struct effect_cross_fader

STATIC_SIZE_ASSERT( effect_cross_fader, 0x1C );

} // namespace sound
} // namespace vostok

#endif // SOUND_EFFECT_CROSS_FADER_H_INCLUDED
