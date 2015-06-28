#include <pebble.h>
#include "translate.h"

typedef enum {CA, CZ, DA, DE, EN, ES, FI, FIL, FR, HR, HU, ID, IT, LT, LV, 
	NL, NO, PO, PT, RO, SK, SL, SV, TR, VI}
	Locale;

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
static Locale saved_locale;
static bool is_locale_set = false;
/*
const char* WEEKDAY_KO[7] = {"일", "월", "화", "수", "목", "금", "토"};
const char* WEEKDAY_JP[7] = {"日", "月", "火", "水", "木", "金", "土"};
// both for traditional and simplified (equal characters)
const char* WEEKDAY_ZH[7] = {"星期天", "星期一", "星期二", "星期三", "星期四", "星期五",
	"星期六"}; 
*/

void set_locale(const char* locale){
	if (strcmp(locale, "EN") == 0){
		saved_locale = EN;
	} else if (strcmp(locale, "ES") == 0){
		saved_locale = ES;
	} else if (strcmp(locale, "DE") == 0){
		saved_locale = DE;
	} else if (strcmp(locale, "FR") == 0){
		saved_locale = FR;
	} else if (strcmp(locale, "CA") == 0){
		saved_locale = CA;
	} else if (strcmp(locale, "CZ") == 0){
		saved_locale = CZ;
	} else if (strcmp(locale, "DA") == 0){
		saved_locale = DA;
	} else if (strcmp(locale, "FI") == 0){
		saved_locale = FI;
	} else if (strcmp(locale, "FIL") == 0){
		saved_locale = FIL;
	} else if (strcmp(locale, "FR") == 0){
		saved_locale = FR;
	} else if (strcmp(locale, "HR") == 0){
		saved_locale = HR;
	} else if (strcmp(locale, "HU") == 0){
		saved_locale = HU;
	} else if (strcmp(locale, "ID") == 0){
		saved_locale = ID;
	} else if (strcmp(locale, "IT") == 0){
		saved_locale = IT;
	} else if (strcmp(locale, "LT") == 0){
		saved_locale = LT;
	} else if (strcmp(locale, "LV") == 0){
		saved_locale = LV;
	} else if (strcmp(locale, "NL") == 0){
		saved_locale = NL;
	} else if (strcmp(locale, "NO") == 0){
		saved_locale = NO;
	} else if (strcmp(locale, "PO") == 0){
		saved_locale = PO;
	} else if (strcmp(locale, "PT") == 0){
		saved_locale = PT;
	} else if (strcmp(locale, "RO") == 0){
		saved_locale = RO;
	} else if (strcmp(locale, "SK") == 0){
		saved_locale = SK;
	} else if (strcmp(locale, "SL") == 0){
		saved_locale = SL;
	} else if (strcmp(locale, "SV") == 0){
		saved_locale = SV;
	} else if (strcmp(locale, "TR") == 0){
		saved_locale = TR;
	} else if (strcmp(locale, "VI") == 0){
		saved_locale = VI;
	} else {
		saved_locale = EN;
	}
	is_locale_set = true;
}
const char* get_weekday(int day_number, const char* locale){
	if (!is_locale_set){
		set_locale(locale);
		is_locale_set = true;
	}
	return WEEKDAY[saved_locale][day_number];
}