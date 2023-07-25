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

#include "unity/examples/unity_config.h"
#include "unity/src/unity.h"
#include "common.h"

static cjson_t item[1];

static void assert_is_object(cjson_t *object_item)
{
    TEST_ASSERT_NOT_NULL_MESSAGE(object_item, "Item is NULL.");

    assert_not_in_list(object_item);
    assert_has_type(object_item, CJSON_OBJECT);
    assert_has_no_reference(object_item);
    assert_has_no_const_string(object_item);
    assert_has_no_valuestring(object_item);
    assert_has_no_string(object_item);
}

static void assert_is_child(cjson_t *child_item, const char *name, int type)
{
    TEST_ASSERT_NOT_NULL_MESSAGE(child_item, "Child item is NULL.");
    TEST_ASSERT_NOT_NULL_MESSAGE(child_item->string, "Child item doesn't have a name.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE(name, child_item->string, "Child item has the wrong name.");
    TEST_ASSERT_BITS(0xFF, type, child_item->type);
}

static void assert_not_object(const char *json)
{
    parse_buffer parsebuffer = { 0, 0, 0, 0, { 0, 0, 0 } };
    parsebuffer.content = (const unsigned char*)json;
    parsebuffer.length = strlen(json) + sizeof("");
    parsebuffer.hooks = global_hooks;

    TEST_ASSERT_FALSE(parse_object(item, &parsebuffer));
    assert_is_invalid(item);
    reset(item);
}

static void assert_parse_object(const char *json)
{
    parse_buffer parsebuffer = { 0, 0, 0, 0, { 0, 0, 0 } };
    parsebuffer.content = (const unsigned char*)json;
    parsebuffer.length = strlen(json) + sizeof("");
    parsebuffer.hooks = global_hooks;

    TEST_ASSERT_TRUE(parse_object(item, &parsebuffer));
    assert_is_object(item);
}

static void parse_object_should_parse_empty_objects(void)
{
    assert_parse_object("{}");
    assert_has_no_child(item);
    reset(item);

    assert_parse_object("{\n\t}");
    assert_has_no_child(item);
    reset(item);
}

static void parse_object_should_parse_objects_with_one_element(void)
{

    assert_parse_object("{\"one\":1}");
    assert_is_child(item->child, "one", CJSON_NUMBER);
    reset(item);

    assert_parse_object("{\"hello\":\"world!\"}");
    assert_is_child(item->child, "hello", CJSON_STRING);
    reset(item);

    assert_parse_object("{\"array\":[]}");
    assert_is_child(item->child, "array", CJSON_ARRAY);
    reset(item);

    assert_parse_object("{\"null\":null}");
    assert_is_child(item->child, "null", CJSON_NULL);
    reset(item);
}

static void parse_object_should_parse_objects_with_multiple_elements(void)
{
    assert_parse_object("{\"one\":1\t,\t\"two\"\n:2, \"three\":3}");
    assert_is_child(item->child, "one", CJSON_NUMBER);
    assert_is_child(item->child->next, "two", CJSON_NUMBER);
    assert_is_child(item->child->next->next, "three", CJSON_NUMBER);
    reset(item);

    {
        size_t i = 0;
        cjson_t *node = NULL;
        int expected_types[7] =
        {
                CJSON_NUMBER,
                CJSON_NULL,
                CJSON_TRUE,
                CJSON_FALSE,
                CJSON_ARRAY,
                CJSON_STRING,
                CJSON_OBJECT
        };
        const char *expected_names[7] =
        {
            "one",
            "NULL",
            "TRUE",
            "FALSE",
            "array",
            "world",
            "object"
        };
        assert_parse_object("{\"one\":1, \"NULL\":null, \"TRUE\":true, \"FALSE\":false, \"array\":[], \"world\":\"hello\", \"object\":{}}");

        node = item->child;
        for (
                i = 0;
                (i < (sizeof(expected_types)/sizeof(int)))
                && (node != NULL);
                (void)i++, node = node->next)
        {
            assert_is_child(node, expected_names[i], expected_types[i]);
        }
        TEST_ASSERT_EQUAL_INT(i, 7);
        reset(item);
    }
}

static void parse_object_should_not_parse_non_objects(void)
{
    assert_not_object("");
    assert_not_object("{");
    assert_not_object("}");
    assert_not_object("[\"hello\",{}]");
    assert_not_object("42");
    assert_not_object("3.14");
    assert_not_object("\"{}hello world!\n\"");
}

int CJSON_CDECL main(void)
{
    /* initialize cJSON item */
    memset(item, 0, sizeof(cjson_t));

    UNITY_BEGIN();
    RUN_TEST(parse_object_should_parse_empty_objects);
    RUN_TEST(parse_object_should_not_parse_non_objects);
    RUN_TEST(parse_object_should_parse_objects_with_multiple_elements);
    RUN_TEST(parse_object_should_parse_objects_with_one_element);
    return UNITY_END();
}
