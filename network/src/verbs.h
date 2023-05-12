#pragma once

typedef enum {
  HTTP_GET = 0,
  HTTP_POST,
  HTTP_PUT,
  HTTP_PATCH,
  HTTP_DELETE,
  HTTP_OPTIONS,
  HTTP_HEAD,
  NUM_VERBS
} HTTPVerb;

char *verbnames[NUM_VERBS] = {
  "GET", "POST", "PUT", "PATCH", "DELETE", "OPTIONS", "HEAD"
};

char *verbnames_lower[NUM_VERBS] = {
  "get", "post", "put", "patch", "delete", "options", "head"
};

char *verbnames_abbrev[NUM_VERBS] = {
  "get", "post", "put", "patch", "del", "opt", "head"
};
