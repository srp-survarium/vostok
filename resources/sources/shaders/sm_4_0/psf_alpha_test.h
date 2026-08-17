#ifndef	FUN_ALPHA_TEST_H
#define FUN_ALPHA_TEST_H

// Ship threshold, byte-proven by all 34 alpha-test permutations of
// depth_accumulate_batched.ps (add r0.x, r0.w, l(-0.250000)); the recovered
// 0.8 predates ship. Passes that clip against material_parameters use
// alpha_ref_parameter directly instead of this helper.
void	psf_alpha_test( float alpha)
{
	clip( alpha - 0.25);
}

#endif // FUN_ALPHA_TEST_H
