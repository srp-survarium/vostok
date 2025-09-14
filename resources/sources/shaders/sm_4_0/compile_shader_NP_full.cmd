"%1/../shader_compiler.exe" -no_memory_usage_stats "%2" "%1/defines_full.def"  -dx10 -shader_model="4.0" -ext="ps,vs" -size -constants -input -output  -instruction_number -size -time -show_define_names -use_declarated_defines -show_not_affect_defines 
| F:/stalker/resources/shaders/log_duplicator.exe F:/stalker/resources/shaders/shader_compiler.log

pause