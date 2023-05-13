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


typedef enum {
  FORMAT_NONE = 0,
  FORMAT_JSON,
  FORMAT_XML,
  FORMAT_YAML,
  FORMAT_EDN,
  FORMAT_TXT,
  FORMAT_BIN,
  FORMAT_OTHER,
  NUM_FORMATS
} BodyFormat;

char *fmtnames[NUM_FORMATS] = {
  "NONE", "JSON", "XML", "YAML", "EDN", "TXT", "BIN", "OTHER"
};

char *fmtnames_lower[NUM_FORMATS] = {
  "none", "json", "xml", "yaml", "edn", "txt", "bin", "other"
};
