#include "errors.h"


int get_error_line() {
	register int line = 1;
	register char* start = prog_start;
	for (line; start != program; start++)
		if (*start == '\r') line++;
	return line;
}