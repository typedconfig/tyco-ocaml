#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/fail.h>

typedef enum {
    TYCO_OK = 0,
} tyco_status;

typedef struct tyco_json_result {
    tyco_status status;
    char *json;
    char *error;
} tyco_json_result;

tyco_json_result tyco_parse_file_json(const char *path);
tyco_json_result tyco_parse_string_json(const char *source, const char *source_name);
void tyco_json_result_free(tyco_json_result *result);

static value tyco_json_result_to_string(tyco_json_result *result) {
    CAMLparam0();
    if (result->status != TYCO_OK || result->json == NULL) {
        const char *message = result->error ? result->error : "Tyco parse error";
        tyco_json_result_free(result);
        caml_failwith(message);
    }
    value str = caml_copy_string(result->json);
    tyco_json_result_free(result);
    CAMLreturn(str);
}

CAMLprim value caml_tyco_load_file(value v_path) {
    CAMLparam1(v_path);
    tyco_json_result result = tyco_parse_file_json(String_val(v_path));
    CAMLreturn(tyco_json_result_to_string(&result));
}

CAMLprim value caml_tyco_load_string(value v_content, value v_name) {
    CAMLparam2(v_content, v_name);
    tyco_json_result result =
        tyco_parse_string_json(String_val(v_content), String_val(v_name));
    CAMLreturn(tyco_json_result_to_string(&result));
}
