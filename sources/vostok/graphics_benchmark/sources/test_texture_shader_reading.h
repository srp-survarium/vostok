////////////////////////////////////////////////////////////////////////////
//	Created		: 15.06.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef TEST_TEXTURE_SHADER_READING_H_INCLUDED
#define TEST_TEXTURE_SHADER_READING_H_INCLUDED

#include "benchmark_test.h"
#include "vostok/math_randoms_generator.h"

namespace vostok {
namespace graphics_benchmark {

////////////////////////////////////////////////////////////////////////////
class test_texture_shader_reading: public benchmark_test, public creator_base<test_texture_shader_reading>
{
public:
	virtual bool initialize	( benchmark&, char const*, char const*);
	virtual bool initialize	( benchmark&, char const*, std::map<std::string,std::string>& parameters, std::string&);
	virtual void execute	( benchmark&, u32, u32, gpu_timer&);
private:
	std::string m_ps_code;
	vostok::render_dx10::ref_effect	 m_sh;

	vostok::render_dx10::constant_host*			m_X;
	vostok::render_dx10::constant_host*			m_Y;
	vostok::render_dx10::constant_host*			m_Z;
	vostok::render_dx10::constant_host*			m_W;
	vostok::render_dx10::res_texture*				m_texture;
	vostok::render_dx10::res_texture_list			m_texture_list;
	vostok::render_dx10::constant_host*			m_texture_host;

	vostok::math::random32						m_random;
}; // class test_texture_shader_reading

} // namespace graphics_benchmark
} // namespace vostok

#endif // #ifndef TEST_TEXTURE_SHADER_READING_H_INCLUDED