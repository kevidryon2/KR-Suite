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
