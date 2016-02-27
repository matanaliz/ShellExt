#include "logg.h"

int main()
{
	logg::d << "Debug output" << std::endl;

	// Output callstack to debug output
	logg::d << logg::callstack();

	return 0;
}