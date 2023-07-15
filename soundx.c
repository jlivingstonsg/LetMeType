/*@F*************************< soundex()     >*******************************
*                                                                           *
*                      produces soundex value of input                      *
*                      ---------------------------------                    *
*                                                                           *
*   Produces the soundex algorithm based on Vol. 3 of Knuth's Algorithms.   *
*   The input string is converted to a string of the form XYYY, where:      *
*      X = the first letter                                                 *
*      YYY = the Soundex values of the next several non-vowels. Doubled     *
*            values of Y are ignored.                                       *
*   This implementation with return the Soundex code as defined by Knuth.   *
*                                                                           *
*   Following test values are documented on pg 392:                         *
*                                                                           *
*   Euler, E460, Gauss, G200, Hilbert, H416, Knuth, K530, Lloyd, L300       *
*   Lukasiewicz L222                                                        *
*                                                                           *
*   char *soundex(char *name)                                               *
*                                                                           *
*   Parameters:                                                             *
*       name            pointer to string to convert                        *
*                                                                           *
*   Returns:                                                                *
*       pointer to string with Soundex. Original string is not modified     *
*       pointer to null string if name is non-alpha                         *
*  Note: static area is used for return, will be reused on subsequent call  *
*        to the routine. Copy to buffer if it needs to be saved.            *
*                                                                           *
*************************************************************************@F*/

/* $Id: soundx.c,v 1.1.1.1 2002/05/12 14:40:56 cc Exp $ */


#include <ctype.h>
#include <assert.h>
#include "soundx.h"

static char code_table[] =
    {'0','1','2','3','0','1','2','0','0','2','2','4','5','5','0','1',
      '2','6','2','3','0','1','0','2','0','2'};
#define GROUPS	7
/* Grouping of characters:
   0: a e h i o u w y
   1: b f p v
   2: c g j k q s x z
   3: d t
   4: l
   5: m n
   6: r
*/

enum SoundEx String2SoundEx(const char *name)
{
    register char cval; /* current soundex value */
    register char pval; /* previous soundex value */
    unsigned int i= 0;  /* index through name */
    unsigned int j= 0;  /* index through buf*/
	char buf[5];
    char ch;
    *buf = '\0';                     /* setup null string error return */
    while (name[i] && !isalpha(name[i]))    /* get to the first alpha */
        i++;

    if (name[i])
        buf[j] = toupper(name[i]);        	/* make it uppercase */
    else                                    /* end of string no alpha */
        return 0;                     		/* return error      */

    pval = code_table[*buf - 'A'];

    if (name[++i]) 
    	do
    	{
        	ch = toupper(name[i]);
        	if (isalpha(ch)) 
        	{
            	cval = code_table[ch - 'A'];
            	if (cval != '0')				/* drop vowels h w y */
                	if (cval != pval)			/* don't use adjecent codes */
                    	buf[++j] = cval;
				pval = cval;   /* keep track of adjecent character values */
        	}
    	} while (name[++i] && j < 3);

    while (j < 3)
        buf[++j] = '0';
    return (buf[0]-'A')*GROUPS*GROUPS*GROUPS + (buf[1]-'0')*GROUPS*GROUPS +
	  (buf[2]-'0')*GROUPS + buf[3]-'0' +1;
}

char *SoundEx2String(enum SoundEx sx)
{
  static char s[5];
  unsigned x = sx;
  assert(x > 0);

  x--;
  s[0] = x / (GROUPS*GROUPS*GROUPS) + 'A';
  s[1] = x / (GROUPS*GROUPS) - x / (GROUPS*GROUPS*GROUPS) * GROUPS + '0';
  s[2] = x / GROUPS - x / (GROUPS*GROUPS) * GROUPS + '0';
  s[3] = x - x / GROUPS *GROUPS + '0';
  s[4] = 0;
  return s;
}

#ifdef MAIN
#include <stdio.h>
main()
{
    char inbuf[80];

    *inbuf = '\0';
    do 
    {
        printf("\nName to convert: ");
        gets(inbuf);
        printf("Soundex Result = %s\n", SoundEx2String(String2SoundEx(inbuf)));
    } while (*inbuf);
}
#endif
