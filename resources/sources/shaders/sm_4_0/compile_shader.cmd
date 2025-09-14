@"../shader_compiler.exe" -max_resources_size=0 "%1" "defines.def"  -dx10 -shader_model="4.0" -ext="ps,vs" -size  -instruction_number -size -time -show_define_names
@pause
