#include <hirolib.h>
#include "verbs.h"

typedef enum {
  COMMAND_WARRANTY = 0,
  COMMAND_COPYING,
  COMMAND_VERB,
  COMMAND_URL,
  COMMAND_SEND,
  COMMAND_HEADER,
  COMMAND_CLEAR,
  COMMAND_EXIT,
  NUM_COMMANDS,
} CommandNumber;

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
extern int cy;

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

void nexit() {
  MoveCursorHome();
  ClearScreen();
  exit(0);
}

void clear() {
  ClearScreen();
  cy = 0;
}

const struct {
  char *cmdname;
  void (*handler)(int,char**);
} commands[NUM_COMMANDS] = {
    {"warranty", warranty},
    {"copying", notimpl},
    {"setverb", sverb},
    {"seturl", notimpl},
    {"sendrequest", notimpl},
    {"setheader", notimpl},
    {"clear", clear},
    {"exit", nexit}
};
