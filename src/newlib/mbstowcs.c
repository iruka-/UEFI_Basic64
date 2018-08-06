/*
FUNCTION
<<mbstowcs>>---minimal multibyte string to wide char converter

INDEX
	mbstowcs

ANSI_SYNOPSIS
	#include <stdlib.h>
	int mbstowcs(wchar_t *<[pwc]>, const char *<[s]>, size_t <[n]>);

TRAD_SYNOPSIS
	#include <stdlib.h>
	int mbstowcs(<[pwc]>, <[s]>, <[n]>)
	wchar_t *<[pwc]>;
	const char *<[s]>;
	size_t <[n]>;

DESCRIPTION
When _MB_CAPABLE is not defined, this is a minimal ANSI-conforming 
implementation of <<mbstowcs>>.  In this case, the
only ``multi-byte character sequences'' recognized are single bytes,
and they are ``converted'' to wide-char versions simply by byte
extension.

When _MB_CAPABLE is defined, this routine calls <<_mbstowcs_r>> to perform
the conversion, passing a state variable to allow state dependent
decoding.  The result is based on the locale setting which may
be restricted to a defined set of locales.

RETURNS
This implementation of <<mbstowcs>> returns <<0>> if
<[s]> is <<NULL>> or is the empty string; 
it returns <<-1>> if _MB_CAPABLE and one of the
multi-byte characters is invalid or incomplete;
otherwise it returns the minimum of: <<n>> or the
number of multi-byte characters in <<s>> plus 1 (to
compensate for the nul character).
If the return value is -1, the state of the <<pwc>> string is
indeterminate.  If the input has a length of 0, the output
string will be modified to contain a wchar_t nul terminator.

PORTABILITY
<<mbstowcs>> is required in the ANSI C standard.  However, the precise
effects vary with the locale.

<<mbstowcs>> requires no supporting OS subroutines.
*/

int mbstowcs(CHAR16 *pwcs, const char *s, int n){
  int count = 0;
  if (n != 0) {
    do {
      if ((*pwcs++ = (CHAR16) *s++) == 0)
	break;
      count++;
    } while (--n != 0);
  }
  return count;
}
