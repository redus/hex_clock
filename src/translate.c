#include <pebble.h>
#include "translate.h"

static const char* WEEKDAY[26][7] = 
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
	{"CN", "TH2", "TH3", "TH4", "TH5", "TH6", "TH7"}};

/*
const char* WEEKDAY_KO[7] = {"일", "월", "화", "수", "목", "금", "토"};
const char* WEEKDAY_JP[7] = {"日", "月", "火", "水", "木", "金", "土"};
// both for traditional and simplified (equal characters)
const char* WEEKDAY_ZH[7] = {"星期天", "星期一", "星期二", "星期三", "星期四", "星期五",
	"星期六"}; 
*/

Locale set_locale(const char* locale){
	if (strcmp(locale, "EN") == 0){
		return EN;
	} else if (strcmp(locale, "ES") == 0){
		return ES;
	} else if (strcmp(locale, "DE") == 0){
		return DE;
	} else if (strcmp(locale, "FR") == 0){
		return FR;
	} else if (strcmp(locale, "CA") == 0){
		return CA;
	} else if (strcmp(locale, "CZ") == 0){
		return CZ;
	} else if (strcmp(locale, "DA") == 0){
		return DA;
	} else if (strcmp(locale, "FI") == 0){
		return FI;
	} else if (strcmp(locale, "FIL") == 0){
		return FIL;
	} else if (strcmp(locale, "FR") == 0){
		return FR;
	} else if (strcmp(locale, "HR") == 0){
		return HR;
	} else if (strcmp(locale, "HU") == 0){
		return HU;
	} else if (strcmp(locale, "ID") == 0){
		return ID;
	} else if (strcmp(locale, "IT") == 0){
		return IT;
	} else if (strcmp(locale, "LT") == 0){
		return LT;
	} else if (strcmp(locale, "LV") == 0){
		return LV;
	} else if (strcmp(locale, "NL") == 0){
		return NL;
	} else if (strcmp(locale, "NO") == 0){
		return NO;
	} else if (strcmp(locale, "PO") == 0){
		return PO;
	} else if (strcmp(locale, "PT") == 0){
		return PT;
	} else if (strcmp(locale, "RO") == 0){
		return RO;
	} else if (strcmp(locale, "SK") == 0){
		return SK;
	} else if (strcmp(locale, "SL") == 0){
		return SL;
	} else if (strcmp(locale, "SV") == 0){
		return SV;
	} else if (strcmp(locale, "TR") == 0){
		return TR;
	} else if (strcmp(locale, "VI") == 0){
		return VI;
	} else {
		return EN;
		// APP_LOG(APP_LOG_LEVEL_DEBUG, "why fall here: %s,", locale);
	}
}
const char* get_weekday(int day_number, const Locale locale){
	return WEEKDAY[locale][day_number];
}