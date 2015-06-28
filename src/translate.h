#pragma once

// REQ: day_number counts from Sunday (0 == Sun, 1 == Mon, etc.)
// EFF: return weekday according to the locale language
const char* get_weekday(int day_number, const char* locale);


// EFF: cache locale setting, defaults to EN.
void set_locale(const char* locale);