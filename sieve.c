/*
 * sieve.c
 * 
 * Copyright 2014 aman <aman@aman-Aspire-4750>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define N 1000000000L

typedef struct prime_struct {
	long *primes;
	long c;
} Primes;

Primes find_primes(Primes ps, long n)
{
	ps.primes = malloc(sizeof(long) * n);
	ps.c = 0;
	
	char *sieve = malloc(n+1);
	memset(sieve, 1, n+1);
	
	long p, i;
	for(p = 2; p <= n; p++) {
		if(sieve[p]) {
			for(i = p*p; i <= n; i+=p) {
				sieve[i] = 0;
			}
		}
	}
	
	for(i = 3; i <= n; i++) {
		if(sieve[i]) {
			ps.primes[(ps.c)++] = i;
		}
	}
	
	free(sieve);
	
	memcpy(ps.primes, ps.primes, ps.c);
	return ps;
}

long segment(long lo, long hi, long delta)
{
	long qInit(long p)
	{
		if((((lo + p + 1) / -2) % p) == 0) return 0;
		else return (((lo + p + 1) / -2) % p) + p;
	}
				
	long qReset(long p, long q)
	{
		if(((q - delta) % p) >= 0) return ((q - delta) % p);
		else return ((q - delta) % p) + p;
	}
		
	long c = 0;
	char *a = malloc(delta);
	memset(a, 1, delta);
	
	Primes ps;
	ps = find_primes(ps, (long) sqrt(hi));
	c = ps.c + 1;
	long *qs = malloc(sizeof(long) * ps.c);
	long i;
	for(i = 0; i < ps.c; i++) {
		qs[i] = qInit(ps.primes[i]);
	}
		
	long j, t;
	while(lo < hi) {
						
		for(i = 0; i < delta; i++) {
			a[i] = 1;
		}
		
		for(j = 0; j < ps.c; j++) {
			for(i = qs[j]; i < delta; i+=ps.primes[j]) {
				a[i] = 0;
			}
		}
		
		for(i = 0; i < ps.c; i++) {
			qs[i] = qReset(ps.primes[i], qs[i]);
		}
		
		for(i = 0, t = lo+1; (i < delta) && (t < hi); i++, t+=2) {
			if(a[i]) {
				c++;
			}
		}
		
		lo += (2 * delta);
	}
	
	free(a);
	free(ps.primes);
	free(qs);
	
	return c;
}

int main(int argc, char **argv)
{
	printf("%ld\n", segment((long) sqrt(N), N, (long) sqrt(N)));
	return 0;
}

