#include "bns.h"
#include <errno.h>
#include <string.h>

LoadedScript *loadScript(char *data, int len) {
	
	//If not enough bytes are contained in the file error out
	if (len < sizeof(NSHeader))
		goto notscript;
	
	//Allocate memory
	NSHeader h;
	LoadedScript *s = malloc(sizeof(s));
	
	//Read header
	memcpy(&h, data, sizeof(h));
	
	//Check header magic
	if (strncmp(h.magic, "nets", 4))
		goto notscript;
	
	//Check header version
	if (NETC_VERSION_MAJOR != h.ver_major || NETC_VERSION_MINOR != h.ver_minor) {
		printf("(warning: bad file version)");
	}
	
	char *p = h.paths;
	
	//Load paths
	for (int i=0; p<h.paths+64; i++) {
		s->paths[i] = malloc(strlen(p));
		if (strlen(p)) {
			memcpy(s->paths[i], p, strlen(p)+1);
		}
		p += strlen(p)+1;
	};
	
	memcpy(s->script, data+sizeof(h), len-sizeof(h));
	
	return s;
	
notscript:
	errno=EINVAL;
	return NULL;
}
