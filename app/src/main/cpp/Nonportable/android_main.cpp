#include "Entry.h"

void android_main(android_app* application) {
	Flappy::Platform::Context context {application};
	
	Flappy::entry(context);
}
