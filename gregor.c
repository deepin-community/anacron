/*
    Anacron - run commands periodically
    Copyright (C) 1998  Itai Tzur <itzur@actcom.co.il>
    Copyright (C) 1999  Sean 'Shaleh' Perry <shaleh@debian.org>
 
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
 
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
 
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 
    The GNU General Public License can also be found in the file
    `COPYING' that comes with the Anacron source distribution.
*/


#include <limits.h>
#include "gregor.h"

const static int
days_in_month[] = {
    31,  /* Jan */
    28,  /* Feb (non-leap) */
    31,  /* Mar */
    30,  /* Apr */
    31,  /* May */
    30,  /* Jun */
    31,  /* Jul */
    31,  /* Aug */
    30,  /* Sep */
    31,  /* Oct */
    30,  /* Nov */
    31  /* Dec */
};

static int leap(int year);

int
day_num(int year, int month, int day)
/* Return the "day number" of the date year-month-day according to the
 * "proleptic Gregorian calendar".
 * If the given date is invalid, return -1.
 *
 * Here, "day number" is defined as the number of days since December 31,
 * 1 B.C. (Gregorian).  (January 1, 1 A.D. is day number 1 etc...)
 *
 * The Gregorian calendar was instituted by Pope Gregory XIII in 1582,
 * and has gradually spread to become the international standard calendar.
 * The proleptic Gregorian calendar is formed by projecting the date system
 * of the Gregorian calendar to dates before its adoption.
 *
 * For more details, see:
 * http://astro.nmsu.edu/~lhuber/leaphist.html
 * http://www.magnet.ch/serendipity/hermetic/cal_stud/cal_art.htm
 * and your local library.
 */
{
    int dn;
    int i;
    const int isleap; /* save three calls to leap() */

    /* Some validity checks */

    /* we don't deal with B.C. years here */
    if (year < 1) return - 1;
    /* conservative overflow estimate */
    if (year > (INT_MAX / 366)) return - 1;
    if (month > 12 || month < 1) return - 1;
    if (day < 1) return - 1;
  
    isleap = leap(year);
  
    if (month != 2) {
	if(day > days_in_month[month - 1]) return - 1;
    }
    else if ((isleap && day > 29) || (!isleap && day > 28))
	return - 1;

    /* First calculate the day number of December 31 last year */

    /* save us from doing (year - 1) over and over */
    i = year - 1;
    /* 365 days in a "regular" year + number of leap days */
    dn = (i * 365) + ((i / 4) - (i / 100) + (i / 400));

    /* Now, day number of the last day of the previous month */

    for (i = month - 1; i > 0; --i)
	dn += days_in_month[i - 1];
    /* Add 29 February ? */
    if (month > 2 && isleap) ++dn;

    /* How many days into month are we */

    dn += day;

    return dn;
}

static int
leap(int year)
/* Is this a leap year ? */
{
    /* every year exactly divisible by 4 is "leap" */
    /* unless it is exactly divisible by 100 */
    /* but not by 400 */
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}
