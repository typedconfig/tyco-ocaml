## Tyco OCaml Binding

OCaml wrapper over `libtyco_c` that exposes `Tyco.load_file` / `Tyco.load_string`, returning `Yojson.Basic.t`.

### Prerequisites

1. Build the shared C library once:
   ```bash
   cd ../tyco-c
   cmake -S . -B build
   cmake --build build
   ```
2. Install dependencies:
   ```bash
   sudo dnf install -y ocaml ocaml-dune ocaml-findlib ocaml-yojson
   ```

### Build & Test

```bash
cd tyco-ocaml
export TYCO_LIB=$(realpath ../tyco-c/build/libtyco_c.so)
export TYCO_LIB_DIR=$(dirname "$TYCO_LIB")
dune test
```

The test executable iterates every file in `../tyco-test-suite/inputs` and ensures the resulting JSON matches the canonical `expected/*.json`.


## Quick Start

This package includes a ready-to-use example Tyco file at:

      example.tyco

([View on GitHub](https://github.com/typedconfig/tyco-ocaml/blob/main/example.tyco))

You can load and parse this file using the OCaml Tyco API. Example usage:

```ocaml
open Tyco

let () =
   let context = Tyco.load_file "example.tyco" in
   let globals = Tyco.get_globals context in
   let environment = Hashtbl.find globals "environment" in
   let debug = Hashtbl.find globals "debug" in
   let timeout = Hashtbl.find globals "timeout" in
   Printf.printf "env=%s debug=%b timeout=%d\n" environment debug timeout
   (* ... access objects, etc ... *)
```

See the [example.tyco](https://github.com/typedconfig/tyco-ocaml/blob/main/example.tyco) file for the full configuration example.
