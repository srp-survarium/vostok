////////////////////////////////////////////////////////////////////////////
//	Created		: 25.03.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "ogg_encoded_sound_interface.h"

namespace vostok {
namespace sound {

ogg_encoded_sound_interface::ogg_encoded_sound_interface		(resources::managed_resource_ptr raw_file)
{
	m_raw_file.resource			= raw_file;
	m_raw_file.pointer			= 0;

	ov_callbacks ovc			= {ogg_utils::ov_read_func, ogg_utils::ov_seek_func, ogg_utils::ov_close_func, ogg_utils::ov_tell_func};
 	ov_open_callbacks			(&m_raw_file, &m_ovf, NULL, 0, ovc);

	vorbis_info* ovi			= ov_info(&m_ovf, -1);

	ASSERT						(ovi, "Invalid source info");

	memory::zero				(&m_wfx, sizeof(m_wfx));

	m_wfx.nSamplesPerSec		= (ovi->rate);
	m_wfx.wFormatTag			= WAVE_FORMAT_PCM;
	m_wfx.nChannels				= u16(ovi->channels);
	m_wfx.wBitsPerSample		= 16;

	m_wfx.nBlockAlign			= (m_wfx.nChannels * m_wfx.wBitsPerSample) / 8;
	m_wfx.nAvgBytesPerSec		= m_wfx.nSamplesPerSec * m_wfx.nBlockAlign;

	m_bytes_per_sample			= 16 >> 3;
	m_length_in_pcm				= ov_pcm_total(&m_ovf, -1);
	m_samples_per_sec			= (ovi->rate);
	m_length_in_msec			= (m_length_in_pcm * 1000) / m_samples_per_sec;
	m_channels_num				= u8(ovi->channels);
}

ogg_encoded_sound_interface::~ogg_encoded_sound_interface		( )
{
	ov_clear	(&m_ovf);
}

u32	ogg_encoded_sound_interface::decompress(	pbyte dest,
												u32 pcm_pointer,
												u32& pcm_pointer_after_decompress,
												u32 bytes_needed )
{
	u32 res							= ogg_utils::decompress(&m_ovf, dest, pcm_pointer, bytes_needed);
	pcm_pointer_after_decompress	= pcm_pointer;
	return							res;
}
} // namespace sound
} // namespace vostok
