#include <pebble.h>
#include "translate.h"

static const char* WEEKDAY[][7] = 
	{{"DG", "DL", "DT", "DC", "DJ", "DV", "DS"},
	{"NE", "PO", "ÚT", "ST", "ÇT", "PÁ", "SO"},
	{"SØN", "MAN", "TIR", "ONS", "TOR", "FRE", "LØR"},
	{"SO", "MO", "DI", "MI", "DO", "FR", "SA"},
	{"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"},
	{"DOM", "LUN", "MAR", "MIÉ", "JUE", "VIE", "SÁB"},
	{"SU", "MA", "TI", "KE", "TO", "PE", "LA"},
	{"LIN", "LUN", "MAR", "MIY", "HUW", "BIY", "SAB"},
	{"DIM", "LUN", "MAR", "MER", "JEU", "VEN", "SAM"},
	{"NED", "PON", "UTO", "SRI", "ÇET", "PET", "SUB"},
	{"V", "H", "K", "SZE", "CS", "P", "SZO"},
	{"MIN", "SEN", "SEL", "RAB", "KAM", "JUM", "SAB"},
	{"DOM", "LUN", "MAR", "MER", "GIO", "VEN", "SAB"},
	{"日", "月", "火", "水", "木", "金", "土"},
	{"일", "월", "화", "수", "목", "금", "토"},
	{"SK", "PR", "AN", "TR", "KT", "PN", "ST"},
	{"SV", "PR", "OT", "TR", "CE", "PK", "SE"},
	{"ZO", "MA", "DI", "WO", "DO", "VR", "ZA"},
	{"SØ", "MA", "TI", "ON", "TO", "FR", "LØ"},
	{"NIEDZ", "PON", "WT", "SR", "CZW", "PT", "SOB"},
	{"DOM", "SEG", "TER", "QUA", "QUI", "SEX", "SÁB"},
	{"DUM", "LUN", "MAR", "MIE", "JOI", "VAN", "SAM"},
	{"NE", "PO", "UT", "ST", "SHT", "PI", "SO"},
	{"NED", "PON", "TOR", "SRE", "CET", "PET", "SOB"},
	{"SÖN", "MÅN", "TIS", "ONS", "TOR", "FRE", "LÖR"},
	{"PAZ", "PZT", "SAL", "ÇAR", "PER", "CUM", "CMT"},
	{"CN", "TH2", "TH3", "TH4", "TH5", "TH6", "TH7"},
	{"星期天", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"}};

static const char* LOCALE_LIST[] = 
	{"CA", "CZ", "DA", "DE", "EN", "ES", "FI", "FIL", "FR", "HR", "HU",
	 "ID", "IT", "JP", "KR", "LT", "LV", "NL", "NO", "PO", "PT", "RO",
	 "SK", "SL", "SV", "TR", "VI", "ZH"};
const int LOCALE_SIZE = 28;

Locale set_locale(const char* locale){
	for (int i = 0; i < LOCALE_SIZE; ++i){
		if (strcmp(locale, LOCALE_LIST[i]) == 0){
			return i;
		}
	}
	return EN;
}

const char* get_weekday(int day_number, const Locale locale){
	return WEEKDAY[locale][day_number];
}