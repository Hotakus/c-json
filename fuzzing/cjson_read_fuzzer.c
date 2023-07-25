#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "../cjson.h"

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size); /* required by C89 */

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    cjson_t *json;
    size_t offset = 4;
    unsigned char *copied;
    char *printed_json = NULL;
    int minify, require_termination, formatted, buffered;


    if(size <= offset) return 0;
    if(data[size-1] != '\0') return 0;
    if(data[0] != '1' && data[0] != '0') return 0;
    if(data[1] != '1' && data[1] != '0') return 0;
    if(data[2] != '1' && data[2] != '0') return 0;
    if(data[3] != '1' && data[3] != '0') return 0;

    minify              = data[0] == '1' ? 1 : 0;
    require_termination = data[1] == '1' ? 1 : 0;
    formatted           = data[2] == '1' ? 1 : 0;
    buffered            = data[3] == '1' ? 1 : 0;

    json = cjson_parse_with_opts((const char *)data + offset, NULL, require_termination);

    if(json == NULL) return 0;

    if(buffered)
    {
        printed_json = cjson_print_buffered(json, 1, formatted);
    }
    else
    {
        /* unbuffered printing */
        if(formatted)
        {
            printed_json = cjson_print(json);
        }
        else
        {
            printed_json = cjson_print_unformatted(json);
        }
    }

    if(printed_json != NULL) free(printed_json);

    if(minify)
    {
        copied = (unsigned char*)malloc(size);
        if(copied == NULL) return 0;

        memcpy(copied, data, size);

        cjson_minify((char *)copied + offset);

        free(copied);
    }

    cjson_delete(json);

    return 0;
}

#ifdef __cplusplus
}
#endif

