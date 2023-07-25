# c-json

该 fork 将 [cJSON](https://github.com/DaveGamble/cJSON) 项目使用下划线风格（snake_case）进行重构，  
若你喜欢下划线命名风格，那你可以直接使用这个 fork 项目，使用 git 拉取：
```cmdline
git clone --depth=1 https://github.com/Hotakus/c-json.git
```
该项目适用人群为：  
喜欢下划线命名风格，并且项目为新建的项目（因为该项目是大量的 API 改动）。  
功能与 [cJSON](https://github.com/DaveGamble/cJSON) 完全相同。

---

## 如何使用 (How to use)
很简单，用法和 [cJSON](https://github.com/DaveGamble/cJSON) 完全相同，但你需要知道该项目改动了什么。  
以下是更改说明。

### 类型
该项目将类型（type）重构为 `xxx_t` 的形式：  
例如，将类型 `cJSON` 重构为 `cjson_t`。  
又如，将类型 `cJSON_Hooks` 重构为 `cjson_hooks_t`  
以此类推。

### 函数
该项目将函数名小写化，并在单词间增加 `_`。  
例如，将函数 `cJSON_CreateObject()` 重构为 `cjson_create_object()`。  
又如，将函数 `cJSON_Delete()` 重构为 `cjson_delete()`。  
以此类推。

### 预定义（#define）
该项目将 预定义 内容重构为纯大写、下划线形式。  
例如，将定义 `cJSON_Object` 重构为 `CJSON_OBJECT`。  
又如，将定义 `cJSON_StringISConst` 重构为 `CJSON_STRING_IS_CONST`。  
以此类推。

## 例程
以下是一段读取 JSON 数据并解析的例程：
```c
#include <stdio.h>
#include "cjson.h"

const char *json_data = "{"
                        "   \"name\" : \"Hotakus\", "
                        "   \"age\" : 114514, "
                        "   \"gender\" : \"helicopter\""
                        "}"; 

int main() {
    // 解析 json_data, 创建 cjson 根对象
    cjson_t *json_body = cjson_parse(json_data);
    if(json_body == NULL) {
        printf("Error: cannot parse JSON data\n");
        return -1;
    }
    
    // 根据 key 获取 value
    cjson_t *name = cjson_get_object_item(json_body, "name");
    cjson_t *age = cjson_get_object_item(json_body, "age");
    cjson_t *gender = cjson_get_object_item(json_body, "gender");
    
    // 输出解析结果
    printf("name  : %s\n", cjson_get_string_value(name));
    printf("age   : %f\n", cjson_get_number_value(age));
    // printf("age   : %d\n", age->valueint);
    printf("gender: %s\n", cjson_get_string_value(gender));
    
    // 用完释放
    cjson_delete(json_body);
    
    return 0;
}
```
输出：
```cmdline
name  : Hotakus
age   : 114514.000000
gender: helicopter
```
---
