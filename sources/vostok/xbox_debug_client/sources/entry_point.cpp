////////////////////////////////////////////////////////////////////////////
//	Created		: 15.01.2010
//	Author		: Alexander Maniluk
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/core/simple_engine.h>
#include <vostok/core/core.h>
#include "debug_client.h"

vostok::core::simple_engine				core_engine;
vostok::memory::doug_lea_allocator_type	g_allocator;

void error_handler(pcstr const error_descr = "unknown error")
{
	printf						("\nERROR: %s\n", error_descr);
}

static vostok::command_line::key		s_debug_output	("debug_output", "d", "main", "allows printing debug output to stdout");
static vostok::command_line::key		s_image_name	("image_name", "i", "main", "image name to run on xbox");
static vostok::command_line::key		s_work_dir		("work_dir", "", "main", "work dir for image to run on xbox");
static vostok::command_line::key		s_xbox_name		("xbox_name", "x", "main", "xbox machine name");
static vostok::command_line::key		s_testing_mode	("run_tests", "t", "main", "sets --run_tests_and_exit command line parameter to image and waits for results");
static vostok::command_line::key		s_cmd_params	("cmd_params", "c", "main", "sets command line parameter to image");
static vostok::command_line::key		s_wait_params	("wait_time", "w", "main", "waits a number of seconds");


int main(int argc, char ** argv)
{
	VOSTOK_UNREFERENCED_PARAMETERS			( argc, argv );

	using namespace vostok;
	vostok::core::preinitialize				(&core_engine, 
											 core::no_log,
											 GetCommandLine(), 
											 command_line::contains_application_true,
											 "xbox_debug_client",
											 __DATE__);
	g_allocator.user_current_thread_id		();
	g_allocator.do_register					(64*1024*1024, "g_allocator");
	vostok::memory::allocate_region			();
	vostok::core::initialize					("xbox_debug_client", core::perform_debug_initialization);

	vostok::fixed_string512	xbox_name_store;
	pcstr xbox_name							= NULL;
	if (s_xbox_name.is_set_as_string(&xbox_name_store))
	{
		xbox_name = xbox_name_store.c_str();
	}
	debug_client							dclient(xbox_name, s_debug_output);
	if (dclient.failed())
	{
		error_handler(dclient.error_descr());
		vostok::core::finalize();
		return EXIT_FAILURE;
	}

	
	vostok::fixed_string512	image_name;
	if (!s_image_name.is_set_as_string(&image_name))
	{
		printf("ERROR: image name not set. Please set image name with key --%s",
			s_image_name.full_name());
		vostok::core::finalize();
		return EXIT_FAILURE;
	}

	vostok::fixed_string512	work_dir;
	if (!s_work_dir.is_set_as_string(&work_dir))
	{
		printf("ERROR: work dir is not set. Please set work dir with key --%s",
			s_work_dir.full_name());
		vostok::core::finalize();
		return EXIT_FAILURE;
	}	
	
	vostok::fixed_string512	cmd_line;
	if (!s_cmd_params.is_set_as_string(&cmd_line))
	{
		cmd_line = "";
	}

	if (s_testing_mode)
	{
		if (!s_cmd_params.is_set())
		{
			printf("ERROR: command line parameters not set. Please set it using key --%s",
				s_cmd_params.full_name());
			vostok::core::finalize();
			return EXIT_FAILURE;
		}
	}
	
	if (!dclient.run_image(image_name.c_str(), work_dir.c_str(), cmd_line.c_str()))
	{
		error_handler			(dclient.error_descr());
		vostok::core::finalize	();
		return EXIT_FAILURE;
	}

	u32		wait_number = 5; //5 seconds by default
	float	wait_float	= 0.f;
	if (s_wait_params.is_set_as_number(&wait_float))
	{
		wait_number = static_cast<u32>(wait_float);
	}
	
	if (!s_testing_mode)
	{
		printf("\nwaiting for results...");
		dclient.wait_title_finish(wait_number);
		vostok::core::finalize();

		if (dclient.is_title_crashed())
		{
			error_handler("title has crashed :(");
			return EXIT_FAILURE;
		} else if (!dclient.is_title_finished())
		{
			error_handler("wait time out :(");
			return EXIT_FAILURE;
		}
		printf("\nSUCCESS: image has run successfully !");
		return EXIT_SUCCESS;
	}
	
	printf("\ntesting mode: waiting for hello...");
	if (!dclient.wait_hello(wait_number))
	{
		if (dclient.is_title_crashed())
		{
			error_handler("title has crashed :(");
		} else
		{
			error_handler("wait timeout :(");
		}
		vostok::core::finalize	();
		return EXIT_FAILURE;
	}

	int ret_code = dclient.wait_tests_result(wait_number);
	if (ret_code == 0)
	{
		printf("SUCCESS: test has completed successfully !");
		vostok::core::finalize();
		return EXIT_SUCCESS;
	}
	printf("\nERROR: test has FAILED: %d", ret_code);
	vostok::core::finalize();
	return ret_code;
}