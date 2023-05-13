#include <hirolib.h>
#include "verbs.h"
#include <string.h>
#include <stdlib.h>

#pragma once
#define min(a,b) (a>b?b:a)
#define max(a,b) (a>b?a:b)

void echo(char *s);

void warranty(int argc, char **argv) {
  echo("THERE IS NO WARRANTY FOR THE PROGRAM, \
TO THE EXTENT PERMITTED BY APPLICABLE LAW. \
EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR \
OTHER PARTIES PROVIDE THE PROGRAM “AS IS” WITHOUT WARRANTY OF ANY KIND, \
EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, \
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. \
THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU. \
SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, \
REPAIR OR CORRECTION.");
}

void notimpl(int argc, char **argv) {
  echo("This command hasn't been implemented yet. If you see this command and you're not a KRSuite developer, please wait for a new update.");
}

extern HTTPVerb verb;
BodyFormat bformat = FORMAT_NONE;
extern int cy;
char body[BUFSIZ];
char buffer3[BUFSIZ];

struct {
  unsigned used : 1;
  char *header, *value;
} headers[256];


void argparses(int argc, char **argv, int start, char *buffer) {
  buffer[0] = 0;
  for (int i=start; i<=argc; i++) {
    strcat(buffer, argv[i]);
    if (i < argc) strcat(buffer, " ");
  }
}

void sverb(int argc, char **argv) {
  if (argc<1) {
    echo("Usage: setverb <verb>");
    echo("Verbs: GET, POST, PUT, PATCH, DELETE, OPTIONS, HEAD");
    return;
  }
  for (int i=0; i<NUM_VERBS; i++) {
    if (!strcmp(argv[1], verbnames[i]) || 
        !strcmp(argv[1], verbnames_lower[i]) || 
        !strcmp(argv[1], verbnames_abbrev[i])) {
      verb = i;
      echo("Set verb to %d succesfully");
      break;
    }
    if (i == NUM_VERBS-1) {
      echo("The verb doesn't exist!");
    }
  }
  
}

void sbody(int argc, char **argv) {
  if (argc<2) {
    echo("Usage: setbody <format> <data/file>");
    echo("Formats: NONE, JSON, XML, YAML, EDN, TXT, BIN, OTHER");
    echo("Use file when specifying binary data.");
    return;
  }
  
  for (int i=0; i<NUM_FORMATS; i++) {
    if (!strcmp(argv[1], fmtnames[i]) || 
        !strcmp(argv[1], fmtnames_lower[i])) {
      bformat = i;
      break;
    }
    if (i == NUM_VERBS-1) {
      echo("The format doesn't exist!");
      return;
    }
  }
  
  argparses(argc, argv, 2, body);
                                    
  echo("Body set correctly");
}

void sendreq(int argc, char **argv) {
  
}

void addheader(int argc, char **argv) {
  if (argc<2) {
    echo("Usage: addheader <header> <value>");
    return;
  }
  for (int i=0; i<256; i++) {
    if (!headers[i].used) {
      headers[i].header = malloc(BUFSIZ);
      strcpy(headers[i].header, argv[1]);
      
      argparses(argc, argv, 2, buffer3);
      headers[i].value = malloc(BUFSIZ);
      strcpy(headers[i].value, buffer3);
      
      headers[i].used = 1;
      sprintf(buffer3, "Set header %d to %s: %s", i, headers[i].header, headers[i].value);
      echo(buffer3);
      break;
    }
  }
}

void eheaders(int argc, char **argv) {
  int start = (argc<1) ? 0 : min(atoi(argv[1]), 255);
  int end = (argc<1) ? 16 : max(min(atoi(argv[1]), 256), start);
  for (int i=0; i<=end; i++) {
    if (headers[i].used) {
      buffer3[0] = 0;
      sprintf(buffer3, "Header %d is %s: \"%s\"", i, headers[i].header, headers[i].value);
      echo(buffer3);
    }
  }
}

void delheader(int argc, char **argv) {
  if (argc<1) {
    echo("Usage: rmheader <headernum>");
    return;
  }
  headers[atoi(argv[1])].used = 0;
}

void nexit() {
  MoveCursorHome();
  ClearScreen();
  exit(0);
}

void clear() {
  ClearScreen();
  cy = 0;
}

#define NUM_COMMANDS 10

const struct {
  char *cmdname;
  void (*handler)(int,char**);
} commands[NUM_COMMANDS] = {
    {"warranty", warranty},
    {"copying", notimpl},
  
    {"setverb", sverb},
    {"setbody", sbody},
  
    {"addheader", addheader},
    {"rmheader", notimpl},
    {"echoheaders", eheaders},
  
    {"sendrequest", sendreq},
  
    {"clear", clear},
    {"exit", nexit}
};
