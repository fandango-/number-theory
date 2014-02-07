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

mp_limb_t
z_gcd(mp_limb_signed_t x, mp_limb_signed_t y)
{
    mp_limb_signed_t u3, v3;
    mp_limb_signed_t quot;
    
    x = FLINT_ABS(x);
    y = FLINT_ABS(y);

    u3 = x;
    v3 = y;

   if ((mp_limb_signed_t) (x & y) < WORD(0)) // x and y both have top bit set
    {
        quot = u3 - v3;
        u3 = v3;
        v3 = quot;
    } 

    while ((mp_limb_signed_t) (v3 << 1) < WORD(0)) // second value has second msb set 
    {
        quot = u3 - v3;
        u3   = v3;
        if (quot < v3)             v3 = quot;
        else if (quot < (v3 << 1)) v3 = quot - u3;
        else                       v3 = quot - (u3 << 1);
    } 
    
    while (v3)
    {
			quot = u3;
			u3 = v3;
            v3 = (FLINT_ABS(quot % v3) <= FLINT_ABS(quot % v3 - FLINT_ABS(v3))) ? (quot % v3) : (quot % v3 - FLINT_ABS(v3));
    }

    return FLINT_ABS(u3);
}

int main(int argc, char **argv)
{
	clock_t begin, end;
	double time_in_secs = 0;
	int i, result;
	FLINT_TEST_INIT(state);
   
   
	
   flint_printf("gcd....");
   fflush(stdout);
   
   

   for (i = 0; i < 100000; i++) 
   {
      mp_limb_signed_t a, b, c, bits1, bits2, bits3;
      
      bits1 = n_randint(state, 31-1) + 1;
      bits2 = n_randint(state, bits1) + 1;
      bits3 = n_randint(state, 31 - bits1) + 1;

      do
      {
         a = n_randtest_bits(state, bits1);
         b = n_randtest_bits(state, bits2);
      } while ((z_gcd(a, b) != UWORD(1)) || (b > a));

      c = n_randtest_bits(state, bits3);
	begin = clock();	
      result = (z_gcd(a*c, b*c) == c);
      end = clock();
      time_in_secs += (double)(end - begin) / CLOCKS_PER_SEC;
      if (!result)
      {
         flint_printf("FAIL:\n");
         flint_printf("a = %wu, b = %wu, c = %wu\n", a, b, c); 
         abort();
      }
   }

   
   
   flint_printf("PASS\n");
   
   FLINT_TEST_CLEANUP(state);
   printf("%.40f", time_in_secs);
	return 0;
}
