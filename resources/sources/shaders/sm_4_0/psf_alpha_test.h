#ifndef	FUN_ALPHA_TEST_H
#define FUN_ALPHA_TEST_H

// Passes using material_parameters clip against alpha_ref_parameter instead.
void	psf_alpha_test( float alpha)
{
	clip( alpha - 0.25);
}

#endif // FUN_ALPHA_TEST_H
