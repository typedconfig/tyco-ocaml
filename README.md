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

### Usage

Add the library via `dune` and use:

```ocaml
let json = Tyco.load_file "../tyco-test-suite/inputs/simple1.tyco" in
print_endline json
```
