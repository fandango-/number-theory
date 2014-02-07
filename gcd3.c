/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2009 William Hart
    Copyright (C) 2014 Abhinav Baid

******************************************************************************/

#include <gmp.h>
#include "flint/flint.h"
#include "flint/ulong_extras.h"
#include <time.h>

#if ((GMP_LIMB_BITS == 64 && defined (__amd64__)) || (GMP_LIMB_BITS == 32 && (defined (__i386__) \
   || defined (__i486__) || defined(__amd64__)))) 
mp_limb_t
n_gcd2(mp_limb_t x, mp_limb_t y)
{
	if(x == 0) return y;
	if(y == 0) return x;
	register unsigned s0, s1;
	count_trailing_zeros(s0, x);
	count_trailing_zeros(s1, y);
	register const unsigned f = FLINT_MIN(s0, s1);
  x >>= s0;
  y >>= s1;

  while(x!=y)
  {
    if(x<y)
    {
      y-=x;
      count_trailing_zeros(s1, y);
      y >>= s1;
    }
    else
    {
      x-=y;
      count_trailing_zeros(s0, x);
      x >>= s0;
    }
  }
  return x<<f;
}
#endif
int main(void)
{
	clock_t begin, end;
	double time1 = 0, time2 = 0;
	int i, result;
	FLINT_TEST_INIT(state);
   
   
	
   flint_printf("gcd....");
   fflush(stdout);
   
   

   for (i = 0; i < 100000; i++) 
   {
      mp_limb_t a, b, c, bits1, bits2, bits3;
      
      bits1 = n_randint(state, FLINT_BITS-1) + 1;
      bits2 = n_randint(state, bits1) + 1;
      bits3 = n_randint(state, FLINT_BITS - bits1) + 1;

      do
      {
         a = n_randtest_bits(state, bits1);
         b = n_randtest_bits(state, bits2);
      } while ((n_gcd2(a, b) != UWORD(1)));

      c = n_randtest_bits(state, bits3);
      begin = clock();
      result = (n_gcd_full(a*c, b*c) == c);
      end = clock();
      time1 += (double)(end - begin) / CLOCKS_PER_SEC;
	begin = clock();	
      result &= (n_gcd2(a*c, b*c) == c);
      end = clock();
      time2 += (double)(end - begin) / CLOCKS_PER_SEC;
      if (!result)
      {
         flint_printf("FAIL:\n");
         flint_printf("a = %wu, b = %wu, c = %wu\n", a, b, c); 
         abort();
      }
   }

   
   
   flint_printf("PASS\n");
   
   FLINT_TEST_CLEANUP(state);
   printf("time for n_gcd_full: %.40f\n", time1);
   printf("time for n_gcd2: %.40f\n", time2);
	return 0;
}
