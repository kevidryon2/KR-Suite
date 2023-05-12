#include "commands.h"
#include <string.h>

#define MAX_TOKENS 16

int parse(char *cmd) {
  
  //remove newline
  for (int i=0; i<strlen(cmd); i++) {
    cmd[i] = (cmd[i] == '\n') ? 0 : cmd[i];
  }
  
  //separate command into tokens
  char *tokens[MAX_TOKENS];
  int ntok;
  tokens[0] = strtok(cmd, " ");
  for (int i=1; i<MAX_TOKENS; i++)
    if (!(tokens[i] = strtok(NULL, " "))) {
      ntok = i;
      break;
    }
  
  //check if empty command
  if (!tokens[0]) {
    return 0;
  }
  
  //test first token against all commands
  for (int i=0; i<NUM_COMMANDS; i++) {
    if (!strcmp(tokens[0], commands[i].cmdname)) {
      commands[i].handler(ntok-1, tokens);
      break;
    }
    if (i == NUM_COMMANDS-1) {
      return -1;
    }
  }
  
  
  return 1;
}
