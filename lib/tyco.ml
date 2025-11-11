external load_file : string -> string = "caml_tyco_load_file"
external load_string_raw : string -> string -> string = "caml_tyco_load_string"

let load_string ?(name = "<string>") content = load_string_raw content name
