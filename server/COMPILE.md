# Compilation
To compile KR Server, you just have to:
1. Install GCC and Make
2. Run `make` to compile everything

## Compilation flags
There are a number that can be stored in the CFLAGS variable
that will affect compilation:

- `-DDISABLE_CACHE`: Disable caching.
- `-DNO_REDIRECT_ROOT`: Disable redirecting / to /index.html.
- `-DREDIRECT_ROOT_PHP`: Redirecting / to /index.php.
