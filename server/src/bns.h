#include <stdlib.h>
#include <stdio.h>
#pragma once

typedef struct {
  char *paths[16];
  char script[BUFSIZ];
} LoadedScript;

typedef struct {
	char magic[4]; //Always 'nets'
	unsigned char ver_major;
	unsigned char ver_minor;
	char paths[64]; //An array of zero-terminated path strings
} NSHeader;

typedef enum {
	INST_NOP 			= 0x30,
	INST_RETURN 		= 0xd6,
	INST_STRING_PACKET	= 0x1e //Followed by a zero-terminated string
} NSInstID;

typedef struct {
	unsigned char inst_id;
	char data[3];
} NSInstruction;

#define NETC_VERSION_MAJOR 0x00
#define NETC_VERSION_MINOR 0x03
