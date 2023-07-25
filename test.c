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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cjson.h"

/* Used by some code below as an example datatype. */
struct record
{
    const char *precision;
    double lat;
    double lon;
    const char *address;
    const char *city;
    const char *state;
    const char *zip;
    const char *country;
};


/* Create a bunch of objects as demonstration. */
static int print_preallocated(cjson_t *root)
{
    /* declarations */
    char *out = NULL;
    char *buf = NULL;
    char *buf_fail = NULL;
    size_t len = 0;
    size_t len_fail = 0;

    /* formatted print */
    out = cjson_print(root);

    /* create buffer to succeed */
    /* the extra 5 bytes are because of inaccuracies when reserving memory */
    len = strlen(out) + 5;
    buf = (char*)malloc(len);
    if (buf == NULL)
    {
        printf("Failed to allocate memory.\n");
        exit(1);
    }

    /* create buffer to fail */
    len_fail = strlen(out);
    buf_fail = (char*)malloc(len_fail);
    if (buf_fail == NULL)
    {
        printf("Failed to allocate memory.\n");
        exit(1);
    }

    /* Print to buffer */
    if (!cjson_print_preallocated(root, buf, (int)len, 1)) {
        printf("cjson_print_preallocated failed!\n");
        if (strcmp(out, buf) != 0) {
            printf("cjson_print_preallocated not the same as cjson_print!\n");
            printf("cjson_print result:\n%s\n", out);
            printf("cjson_print_preallocated result:\n%s\n", buf);
        }
        free(out);
        free(buf_fail);
        free(buf);
        return -1;
    }

    /* success */
    printf("%s\n", buf);

    /* force it to fail */
    if (cjson_print_preallocated(root, buf_fail, (int)len_fail, 1)) {
        printf("cjson_print_preallocated failed to show error with insufficient memory!\n");
        printf("cjson_print result:\n%s\n", out);
        printf("cjson_print_preallocated result:\n%s\n", buf_fail);
        free(out);
        free(buf_fail);
        free(buf);
        return -1;
    }

    free(out);
    free(buf_fail);
    free(buf);
    return 0;
}

/* Create a bunch of objects as demonstration. */
static void create_objects(void)
{
    /* declare a few. */
    cjson_t *root = NULL;
    cjson_t *fmt = NULL;
    cjson_t *img = NULL;
    cjson_t *thm = NULL;
    cjson_t *fld = NULL;
    int i = 0;

    /* Our "days of the week" array: */
    const char *strings[7] =
    {
        "Sunday",
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday"
    };
    /* Our matrix: */
    int numbers[3][3] =
    {
        {0, -1, 0},
        {1, 0, 0},
        {0 ,0, 1}
    };
    /* Our "gallery" item: */
    int ids[4] = { 116, 943, 234, 38793 };
    /* Our array of "records": */
    struct record fields[2] =
    {
        {
            "zip",
            37.7668,
            -1.223959e+2,
            "",
            "SAN FRANCISCO",
            "CA",
            "94107",
            "US"
        },
        {
            "zip",
            37.371991,
            -1.22026e+2,
            "",
            "SUNNYVALE",
            "CA",
            "94085",
            "US"
        }
    };
    volatile double zero = 0.0;

    /* Here we construct some JSON standards, from the JSON site. */

    /* Our "Video" datatype: */
    root = cjson_create_object();
    cjson_add_item_to_object(root, "name", cjson_create_string("Jack (\"Bee\") Nimble"));
    cjson_add_item_to_object(root, "format", fmt = cjson_create_object());
    cjson_add_string_to_object(fmt, "type", "rect");
    cjson_add_number_to_object(fmt, "width", 1920);
    cjson_add_number_to_object(fmt, "height", 1080);
    cjson_add_false_to_object(fmt, "interlace");
    cjson_add_number_to_object(fmt, "frame rate", 24);

    /* Print to text */
    if (print_preallocated(root) != 0) {
        cjson_delete(root);
        exit(EXIT_FAILURE);
    }
    cjson_delete(root);

    /* Our "days of the week" array: */
    root = cjson_create_string_array(strings, 7);

    if (print_preallocated(root) != 0) {
        cjson_delete(root);
        exit(EXIT_FAILURE);
    }
    cjson_delete(root);

    /* Our matrix: */
    root = cjson_create_array();
    for (i = 0; i < 3; i++)
    {
        cjson_add_item_to_array(root, cjson_create_int_array(numbers[i], 3));
    }

    /* cJSON_ReplaceItemInArray(root, 1, cJSON_CreateString("Replacement")); */

    if (print_preallocated(root) != 0) {
        cjson_delete(root);
        exit(EXIT_FAILURE);
    }
    cjson_delete(root);

    /* Our "gallery" item: */
    root = cjson_create_object();
    cjson_add_item_to_object(root, "Image", img = cjson_create_object());
    cjson_add_number_to_object(img, "Width", 800);
    cjson_add_number_to_object(img, "Height", 600);
    cjson_add_string_to_object(img, "Title", "View from 15th Floor");
    cjson_add_item_to_object(img, "Thumbnail", thm = cjson_create_object());
    cjson_add_string_to_object(thm, "Url", "http:/*www.example.com/image/481989943");
    cjson_add_number_to_object(thm, "Height", 125);
    cjson_add_string_to_object(thm, "Width", "100");
    cjson_add_item_to_object(img, "IDs", cjson_create_int_array(ids, 4));

    if (print_preallocated(root) != 0) {
        cjson_delete(root);
        exit(EXIT_FAILURE);
    }
    cjson_delete(root);

    /* Our array of "records": */
    root = cjson_create_array();
    for (i = 0; i < 2; i++)
    {
        cjson_add_item_to_array(root, fld = cjson_create_object());
        cjson_add_string_to_object(fld, "precision", fields[i].precision);
        cjson_add_number_to_object(fld, "Latitude", fields[i].lat);
        cjson_add_number_to_object(fld, "Longitude", fields[i].lon);
        cjson_add_string_to_object(fld, "Address", fields[i].address);
        cjson_add_string_to_object(fld, "City", fields[i].city);
        cjson_add_string_to_object(fld, "State", fields[i].state);
        cjson_add_string_to_object(fld, "Zip", fields[i].zip);
        cjson_add_string_to_object(fld, "Country", fields[i].country);
    }

    /* cJSON_ReplaceItemInObject(cJSON_GetArrayItem(root, 1), "City", cJSON_CreateIntArray(ids, 4)); */

    if (print_preallocated(root) != 0) {
        cjson_delete(root);
        exit(EXIT_FAILURE);
    }
    cjson_delete(root);

    root = cjson_create_object();
    cjson_add_number_to_object(root, "number", 1.0 / zero);

    if (print_preallocated(root) != 0) {
        cjson_delete(root);
        exit(EXIT_FAILURE);
    }
    cjson_delete(root);
}

const char *json_data = "{"
                        "   \"name\" : \"Hotakus\", "
                        "   \"age\" : 114514, "
                        "   \"gender\" : \"helicopter\""
                        "}";

int CJSON_CDECL main(void)
{
    /* print the version */
    printf("Version: %s\n", cjson_version());

    /* Now some samplecode for building objects concisely: */
    create_objects();

    return 0;
}
