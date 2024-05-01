#include "fileinformation.h"

int GetDisplayLang() {
	int sysDefault = GetSystemDefaultLangID();

	switch (sysDefault) {
		// English case
		case 1033:
			return sysDefault;
			break;

		// Portuguese case
		case 1046:
			return sysDefault;
			break;

		default:
			printf(TEXT("Could not find the default system langID.\n"));
			break;
	}

	return 0;
}