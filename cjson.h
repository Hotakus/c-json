/*
  Copyright (c) 2009-2017 Dave Gamble and cJSON contributors

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef CJSON__H
#define CJSON__H

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(__WINDOWS__) && (defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32))
#define __WINDOWS__
#endif

#ifdef __WINDOWS__

/* When compiling for windows, we specify a specific calling convention to avoid issues where we are being called from a project with a different default calling convention.  For windows you have 3 define options:

CJSON_HIDE_SYMBOLS - Define this in the case where you don't want to ever dllexport symbols
CJSON_EXPORT_SYMBOLS - Define this on library build when you want to dllexport symbols (default)
CJSON_IMPORT_SYMBOLS - Define this if you want to dllimport symbol

For *nix builds that support visibility attribute, you can define similar behavior by

setting default visibility to hidden by adding
-fvisibility=hidden (for gcc)
or
-xldscope=hidden (for sun cc)
to CFLAGS

then using the CJSON_API_VISIBILITY flag to "export" the same symbols the way CJSON_EXPORT_SYMBOLS does

*/

#define CJSON_CDECL __cdecl
#define CJSON_STDCALL __stdcall

/* export symbols by default, this is necessary for copy pasting the C and header file */
#if !defined(CJSON_HIDE_SYMBOLS) && !defined(CJSON_IMPORT_SYMBOLS) && !defined(CJSON_EXPORT_SYMBOLS)
#define CJSON_EXPORT_SYMBOLS
#endif

#if defined(CJSON_HIDE_SYMBOLS)
#define CJSON_PUBLIC(type)   type CJSON_STDCALL
#elif defined(CJSON_EXPORT_SYMBOLS)
#define CJSON_PUBLIC(type)   __declspec(dllexport) type CJSON_STDCALL
#elif defined(CJSON_IMPORT_SYMBOLS)
#define CJSON_PUBLIC(type)   __declspec(dllimport) type CJSON_STDCALL
#endif
#else /* !__WINDOWS__ */
#define CJSON_CDECL
#define CJSON_STDCALL

#if (defined(__GNUC__) || defined(__SUNPRO_CC) || defined (__SUNPRO_C)) && defined(CJSON_API_VISIBILITY)
#define CJSON_PUBLIC(type)   __attribute__((visibility("default"))) type
#else
#define CJSON_PUBLIC(type) type
#endif
#endif

/* project version */
#define CJSON_VERSION_MAJOR 1
#define CJSON_VERSION_MINOR 7
#define CJSON_VERSION_PATCH 16

#include <stddef.h>

/* cJSON Types: */
#define CJSON_INVALID   (0)
#define CJSON_FALSE     (1 << 0)
#define CJSON_TRUE      (1 << 1)
#define CJSON_NULL      (1 << 2)
#define CJSON_NUMBER    (1 << 3)
#define CJSON_STRING    (1 << 4)
#define CJSON_ARRAY     (1 << 5)
#define CJSON_OBJECT    (1 << 6)
#define CJSON_RAW       (1 << 7) /* raw json */

#define CJSON_IS_REFERENCE      256
#define CJSON_STRING_IS_CONST   512

/* The cJSON structure: */
typedef struct cjson_t
{
    /* next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem */
    struct cjson_t *next;
    struct cjson_t *prev;
    /* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */
    struct cjson_t *child;

    /* The type of the item, as above. */
    int type;

    /* The item's string, if type==cJSON_String  and type == cJSON_Raw */
    char *valuestring;
    /* writing to valueint is DEPRECATED, use cJSON_SetNumberValue instead */
    int valueint;
    /* The item's number, if type==cJSON_Number */
    double valuedouble;

    /* The item's name string, if this item is the child of, or is in the list of subitems of an object. */
    char *string;
} cjson_t;

typedef struct cjson_hooks_t
{
      /* malloc/free are CDECL on Windows regardless of the default calling convention of the compiler, so ensure the hooks allow passing those functions directly. */
      void *(CJSON_CDECL *malloc_fn)(size_t sz);
      void (CJSON_CDECL *free_fn)(void *ptr);
} cjson_hooks_t;

typedef int cjson_bool_t;

/* Limits how deeply nested arrays/objects can be before cJSON rejects to parse them.
 * This is to prevent stack overflows. */
#ifndef CJSON_NESTING_LIMIT
#define CJSON_NESTING_LIMIT 1000
#endif

/* returns the version of cJSON as a string */
CJSON_PUBLIC(const char*) cjson_version(void);

/* Supply malloc, realloc and free functions to cJSON */
CJSON_PUBLIC(void) cjson_init_hooks(cjson_hooks_t* hooks);

/* Memory Management: the caller is always responsible to free the results from all variants of cJSON_Parse (with cJSON_Delete) and cJSON_Print (with stdlib free, cJSON_Hooks.free_fn, or cJSON_free as appropriate). The exception is cJSON_PrintPreallocated, where the caller has full responsibility of the buffer. */
/* Supply a block of JSON, and this returns a cJSON object you can interrogate. */
CJSON_PUBLIC(cjson_t *) cjson_parse(const char *value);
CJSON_PUBLIC(cjson_t *) cjson_parse_with_length(const char *value, size_t buffer_length);
/* ParseWithOpts allows you to require (and check) that the JSON is null terminated, and to retrieve the pointer to the final byte parsed. */
/* If you supply a ptr in return_parse_end and parsing fails, then return_parse_end will contain a pointer to the error so will match cJSON_GetErrorPtr(). */
CJSON_PUBLIC(cjson_t *) cjson_parse_with_opts(const char *value, const char **return_parse_end, cjson_bool_t require_null_terminated);
CJSON_PUBLIC(cjson_t *) cjson_parse_with_length_opts(const char *value, size_t buffer_length, const char **return_parse_end, cjson_bool_t require_null_terminated);

/* Render a cJSON entity to text for transfer/storage. */
CJSON_PUBLIC(char *) cjson_print(const cjson_t *item);
/* Render a cJSON entity to text for transfer/storage without any formatting. */
CJSON_PUBLIC(char *) cjson_print_unformatted(const cjson_t *item);
/* Render a cJSON entity to text using a buffered strategy. prebuffer is a guess at the final size. guessing well reduces reallocation. fmt=0 gives unformatted, =1 gives formatted */
CJSON_PUBLIC(char *) cjson_print_buffered(const cjson_t *item, int prebuffer, cjson_bool_t fmt);
/* Render a cJSON entity to text using a buffer already allocated in memory with given length. Returns 1 on success and 0 on failure. */
/* NOTE: cJSON is not always 100% accurate in estimating how much memory it will use, so to be safe allocate 5 bytes more than you actually need */
CJSON_PUBLIC(cjson_bool_t) cjson_print_preallocated(cjson_t *item, char *buffer, const int length, const cjson_bool_t format);
/* Delete a cJSON entity and all subentities. */
CJSON_PUBLIC(void) cjson_delete(cjson_t *item);

/* Returns the number of items in an array (or object). */
CJSON_PUBLIC(int) cjson_get_array_size(const cjson_t *array);
/* Retrieve item number "index" from array "array". Returns NULL if unsuccessful. */
CJSON_PUBLIC(cjson_t *) cjson_get_array_item(const cjson_t *array, int index);
/* Get item "string" from object. Case insensitive. */
CJSON_PUBLIC(cjson_t *) cjson_get_object_item(const cjson_t * const object, const char * const string);
CJSON_PUBLIC(cjson_t *) cjson_get_object_item_case_sensitive(const cjson_t * const object, const char * const string);
CJSON_PUBLIC(cjson_bool_t) cjson_has_object_item(const cjson_t *object, const char *string);
/* For analysing failed parses. This returns a pointer to the parse error. You'll probably need to look a few chars back to make sense of it. Defined when cJSON_Parse() returns 0. 0 when cJSON_Parse() succeeds. */
CJSON_PUBLIC(const char *) cjson_get_error_ptr(void);

/* Check item type and return its value */
CJSON_PUBLIC(char *) cjson_get_string_value(const cjson_t * const item);
CJSON_PUBLIC(double) cjson_get_number_value(const cjson_t * const item);

/* These functions check the type of an item */
CJSON_PUBLIC(cjson_bool_t) cjson_is_invalid(const cjson_t * const item);
CJSON_PUBLIC(cjson_bool_t) cjson_is_false(const cjson_t * const item);
CJSON_PUBLIC(cjson_bool_t) cjson_is_true(const cjson_t * const item);
CJSON_PUBLIC(cjson_bool_t) cjson_is_bool(const cjson_t * const item);
CJSON_PUBLIC(cjson_bool_t) cjson_is_null(const cjson_t * const item);
CJSON_PUBLIC(cjson_bool_t) cjson_is_number(const cjson_t * const item);
CJSON_PUBLIC(cjson_bool_t) cjson_is_string(const cjson_t * const item);
CJSON_PUBLIC(cjson_bool_t) cjson_is_array(const cjson_t * const item);
CJSON_PUBLIC(cjson_bool_t) cjson_is_object(const cjson_t * const item);
CJSON_PUBLIC(cjson_bool_t) cjson_is_raw(const cjson_t * const item);

/* These calls create a cJSON item of the appropriate type. */
CJSON_PUBLIC(cjson_t *) cjson_create_null(void);
CJSON_PUBLIC(cjson_t *) cjson_create_true(void);
CJSON_PUBLIC(cjson_t *) cjson_create_false(void);
CJSON_PUBLIC(cjson_t *) cjson_create_bool(cjson_bool_t boolean);
CJSON_PUBLIC(cjson_t *) cjson_create_number(double num);
CJSON_PUBLIC(cjson_t *) cjson_create_string(const char *string);
/* raw json */
CJSON_PUBLIC(cjson_t *) cjson_create_raw(const char *raw);
CJSON_PUBLIC(cjson_t *) cjson_create_array(void);
CJSON_PUBLIC(cjson_t *) cjson_create_object(void);

/* Create a string where valuestring references a string so
 * it will not be freed by cJSON_Delete */
CJSON_PUBLIC(cjson_t *) cjson_create_string_reference(const char *string);
/* Create an object/array that only references it's elements so
 * they will not be freed by cJSON_Delete */
CJSON_PUBLIC(cjson_t *) cjson_create_object_reference(const cjson_t *child);
CJSON_PUBLIC(cjson_t *) cjson_create_array_reference(const cjson_t *child);

/* These utilities create an Array of count items.
 * The parameter count cannot be greater than the number of elements in the number array, otherwise array access will be out of bounds.*/
CJSON_PUBLIC(cjson_t *) cjson_create_int_array(const int *numbers, int count);
CJSON_PUBLIC(cjson_t *) cjson_create_float_array(const float *numbers, int count);
CJSON_PUBLIC(cjson_t *) cjson_create_double_array(const double *numbers, int count);
CJSON_PUBLIC(cjson_t *) cjson_create_string_array(const char *const *strings, int count);

/* Append item to the specified array/object. */
CJSON_PUBLIC(cjson_bool_t) cjson_add_item_to_array(cjson_t *array, cjson_t *item);
CJSON_PUBLIC(cjson_bool_t) cjson_add_item_to_object(cjson_t *object, const char *string, cjson_t *item);
/* Use this when string is definitely const (i.e. a literal, or as good as), and will definitely survive the cJSON object.
 * WARNING: When this function was used, make sure to always check that (item->type & cJSON_StringIsConst) is zero before
 * writing to `item->string` */
CJSON_PUBLIC(cjson_bool_t) cjson_add_item_to_object_cs(cjson_t *object, const char *string, cjson_t *item);
/* Append reference to item to the specified array/object. Use this when you want to add an existing cJSON to a new cJSON, but don't want to corrupt your existing cJSON. */
CJSON_PUBLIC(cjson_bool_t) cjson_add_item_reference_to_array(cjson_t *array, cjson_t *item);
CJSON_PUBLIC(cjson_bool_t) cjson_add_item_reference_to_object(cjson_t *object, const char *string, cjson_t *item);

/* Remove/Detach items from Arrays/Objects. */
CJSON_PUBLIC(cjson_t *) cjson_detach_item_via_pointer(cjson_t *parent, cjson_t * const item);
CJSON_PUBLIC(cjson_t *) cjson_detach_item_from_array(cjson_t *array, int which);
CJSON_PUBLIC(void) cjson_delete_item_from_array(cjson_t *array, int which);
CJSON_PUBLIC(cjson_t *) cjson_detach_item_from_object(cjson_t *object, const char *string);
CJSON_PUBLIC(cjson_t *) cjson_detach_item_from_object_case_sensitive(cjson_t *object, const char *string);
CJSON_PUBLIC(void) cjson_delete_item_from_object(cjson_t *object, const char *string);
CJSON_PUBLIC(void) cjson_delete_item_from_object_case_sensitive(cjson_t *object, const char *string);

/* Update array items. */
CJSON_PUBLIC(cjson_bool_t) cjson_insert_item_in_array(cjson_t *array, int which, cjson_t *newitem); /* Shifts pre-existing items to the right. */
CJSON_PUBLIC(cjson_bool_t) cjson_replace_item_via_pointer(cjson_t * const parent, cjson_t * const item, cjson_t * replacement);
CJSON_PUBLIC(cjson_bool_t) cjson_replace_item_in_array(cjson_t *array, int which, cjson_t *newitem);
CJSON_PUBLIC(cjson_bool_t) cjson_replace_item_in_object(cjson_t *object, const char *string, cjson_t *newitem);
CJSON_PUBLIC(cjson_bool_t) cjson_replace_item_in_object_case_sensitive(cjson_t *object, const char *string, cjson_t *newitem);

/* Duplicate a cJSON item */
CJSON_PUBLIC(cjson_t *) cjson_duplicate(const cjson_t *item, cjson_bool_t recurse);
/* Duplicate will create a new, identical cJSON item to the one you pass, in new memory that will
 * need to be released. With recurse!=0, it will duplicate any children connected to the item.
 * The item->next and ->prev pointers are always zero on return from Duplicate. */
/* Recursively compare two cJSON items for equality. If either a or b is NULL or invalid, they will be considered unequal.
 * case_sensitive determines if object keys are treated case sensitive (1) or case insensitive (0) */
CJSON_PUBLIC(cjson_bool_t) cjson_compare(const cjson_t * const a, const cjson_t * const b, const cjson_bool_t case_sensitive);

/* Minify a strings, remove blank characters(such as ' ', '\t', '\r', '\n') from strings.
 * The input pointer json cannot point to a read-only address area, such as a string constant,
 * but should point to a readable and writable address area. */
CJSON_PUBLIC(void) cjson_minify(char *json);

/* Helper functions for creating and adding items to an object at the same time.
 * They return the added item or NULL on failure. */
CJSON_PUBLIC(cjson_t*) cjson_add_null_to_object(cjson_t * const object, const char * const name);
CJSON_PUBLIC(cjson_t*) cjson_add_true_to_object(cjson_t * const object, const char * const name);
CJSON_PUBLIC(cjson_t*) cjson_add_false_to_object(cjson_t * const object, const char * const name);
CJSON_PUBLIC(cjson_t*) cjson_add_bool_to_object(cjson_t * const object, const char * const name, const cjson_bool_t boolean);
CJSON_PUBLIC(cjson_t*) cjson_add_number_to_object(cjson_t * const object, const char * const name, const double number);
CJSON_PUBLIC(cjson_t*) cjson_add_string_to_object(cjson_t * const object, const char * const name, const char * const string);
CJSON_PUBLIC(cjson_t*) cjson_add_raw_to_object(cjson_t * const object, const char * const name, const char * const raw);
CJSON_PUBLIC(cjson_t*) cjson_add_object_to_object(cjson_t * const object, const char * const name);
CJSON_PUBLIC(cjson_t*) cjson_add_array_to_object(cjson_t * const object, const char * const name);

/* When assigning an integer value, it needs to be propagated to valuedouble too. */
#define CJSON_SET_INT_VALUE(object, number) ((object) ? (object)->valueint = (object)->valuedouble = (number) : (number))
/* helper for the cJSON_SetNumberValue macro */
CJSON_PUBLIC(double) cjson_set_number_helper(cjson_t *object, double number);
#define CJSON_SET_NUMBER_VALUE(object, number) ((object != NULL) ? cjson_set_number_helper(object, (double)number) : (number))
/* Change the valuestring of a cJSON_String object, only takes effect when type of object is cJSON_String */
CJSON_PUBLIC(char*) cjson_set_value_string(cjson_t *object, const char *valuestring);

/* If the object is not a boolean type this does nothing and returns cJSON_Invalid else it returns the new type*/
#define CJSON_SET_BOOL_VALUE(object, boolValue) ( \
    (object != NULL && ((object)->type & (CJSON_FALSE|CJSON_TRUE))) ? \
    (object)->type=((object)->type &(~(CJSON_FALSE|CJSON_TRUE)))|((boolValue)?CJSON_TRUE:CJSON_FALSE) : \
    CJSON_INVALID\
)

/* Macro for iterating over an array or object */
#define CJSON_ARRAY_FOREACH(element, array) for(element = (array != NULL) ? (array)->child : NULL; element != NULL; element = element->next)

/* malloc/free objects using the malloc/free functions that have been set with cJSON_InitHooks */
CJSON_PUBLIC(void *) cjson_malloc(size_t size);
CJSON_PUBLIC(void) cjson_free(void *object);

#ifdef __cplusplus
}
#endif

#endif
