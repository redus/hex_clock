#pragma once

typedef enum {CA, CZ, DA, DE, EN, ES, FI, FIL, FR, HR, HU, ID, IT, LT, LV, 
				  NL, NO, PO, PT, RO, SK, SL, SV, TR, VI}
			Locale;

// REQ: day_number counts from Sunday (0 == Sun, 1 == Mon, etc.)
// EFF: return weekday according to the locale language
const char* get_weekday(int day_number, const Locale locale);

// EFF: char* to Locale converter
Locale set_locale(const char* locale);