static const char CONFIG[] =
"{"
"  \"name\": \"is_number\","
"  \"desc\": \"checks if string is any kind of number\","
"  \"pargs\": ["
"     {"
"       \"name\": \"string\","
"       \"minc\": 1,"
"       \"maxc\": 1,"
"       \"desc\": \"any string to be checked\""
"     }"
"  ],"
"  \"oargs\": [],"
"  \"opts\": []"
"}";

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#if defined(_WIN32)
#else
#include <unistd.h>
#endif

#include <jansson.h>
#include <argtable3.h>

typedef double number_t;

static json_error_t *json_error;
static json_t *ivar;
static size_t ivar_index;
static size_t argcount = 0;

#include "is_number.h"

int main(int argc, char *argv[])
{
  int exitcode = EXIT_SUCCESS;
  /* buffer variables */
  json_t *program = NULL;
  void *argtable[100];
  
  /* ======================================================================== */
  /* fetch program definitions                                                */
  /* ======================================================================== */
  program = json_loads(CONFIG, 0, json_error);

  /* ======================================================================== */
  /* argument parse                                                           */
  /* ======================================================================== */
  /* copy positional args */
  for (size_t i = 1; i < argc; i++)
    if (argv[i][0] == 45 && is_number(argv[i]))
      argv[i][0] = 126; // '-' to '~' avoiding argtable literal behaviour
      
      /* positional arg structs*/
      json_t *pargs = json_object_get(program, "pargs");
  json_array_foreach(pargs, ivar_index, ivar)
  {
    const char* name = json_string_value(json_object_get(ivar, "name"));
    json_int_t minc = json_integer_value(json_object_get(ivar, "minc"));
    json_int_t maxc = json_integer_value(json_object_get(ivar, "maxc"));
    const char* desc = json_string_value(json_object_get(ivar, "desc"));
    argtable[argcount++] = arg_strn(NULL, NULL, name, (int)minc, (int)maxc, desc);
  }
  
  /* optional arg structs*/
  json_t *oargs = json_object_get(program, "oargs");
  json_array_foreach(oargs, ivar_index, ivar)
  {
    const char* sh = json_string_value(json_object_get(ivar, "short"));
    const char* ln = json_string_value(json_object_get(ivar, "long"));
    const char* name = json_string_value(json_object_get(ivar, "name"));
    json_int_t minc = json_integer_value(json_object_get(ivar, "minc"));
    json_int_t maxc = json_integer_value(json_object_get(ivar, "maxc"));
    const char* desc = json_string_value(json_object_get(ivar, "desc"));
    argtable[argcount++] = arg_strn(sh, ln, name, (int)minc, (int)maxc, desc);
  }

  /* options structs*/
  json_t *opts = json_object_get(program, "opts");
  json_array_foreach(opts, ivar_index, ivar)
  {
    const char* sh = json_string_value(json_object_get(ivar, "short"));
    const char* ln = json_string_value(json_object_get(ivar, "long"));
    const char* desc = json_string_value(json_object_get(ivar, "desc"));
    argtable[argcount++] = arg_lit0(sh, ln, desc);
  }
  
  /* commong arg structs */
  struct arg_lit *help = arg_lit0(NULL, "help", "display this help and exit");
  struct arg_lit *version = arg_lit0(NULL, "version", "display version");
  struct arg_lit *verbose = arg_lit0(NULL, "verbose", "print detailed output if any");
  struct arg_end *end = arg_end(20);
  argtable[argcount++] = help;
  argtable[argcount++] = version;
  argtable[argcount++] = verbose;
  argtable[argcount] = end;
  
  int arg_errors = arg_parse(argc, argv, argtable);
  
  /* special case: '--help' takes precedence over error reporting */
  if (help->count > 0)
  {
    printf("%s: %s.\n\n", FUNCTION, json_string_value(json_object_get(program, "desc")));
    printf("Usage: %s", FUNCTION);
    arg_print_syntaxv(stdout, argtable, "\n\n");
    arg_print_glossary(stdout, argtable, "  %-25s %s\n");
    goto EXIT;
  }
  
  /* special case: '--version' takes precedence over error reporting */
  if (version->count > 0)
  {
    printf("%s", VERSION);
    goto EXIT;
  }
  
  /* If the parser returned any errors then display them and exit */
  if (arg_errors > 0)
  {
    /* Display the error details contained in the arg_end struct.*/
    arg_print_errors(stdout, end, FUNCTION);
    printf("Try '%s --help' for more information.\n", FUNCTION);
    exitcode = EXIT_FAILURE;
    goto EXIT;
  }
  
  /* minus number_t back in positional args */
  for (size_t i = 1; i < argc; i++)
  if (argv[i][0] == 126)
  argv[i][0] = 45; // '~' to '-'
  
  /* ======================================================================== */
  /* operation                                                          */
  /* ======================================================================== */
  struct arg_str *arg = (struct arg_str *)argtable[0];
  printf("%d\n", is_number(arg->sval[0]));
  
EXIT:;
  // jansson cleanup
  json_decref(program);
  
  // argtable cleanup
  arg_freetable(argtable, argcount + 1); // +1 for end
  return exitcode;
}
