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

Each binding ships the canonical sample configuration under `tyco/example.tyco`
([view on GitHub](https://github.com/typedconfig/tyco-ocaml/blob/main/tyco/example.tyco)).
Load it and inspect the globals/instances just like the Python README:

```ocaml
open Yojson.Basic
open Yojson.Basic.Util

let () =
  let json =
    Tyco.load_file "tyco/example.tyco"
    |> Yojson.Basic.from_string
  in
  let environment = json |> member "environment" |> to_string in
  let debug = json |> member "debug" |> to_bool in
  let timeout = json |> member "timeout" |> to_int in
  Printf.printf "env=%s debug=%b timeout=%d\n" environment debug timeout;
  match member "Database" json with
  | `List (primary :: _) ->
      let host = primary |> member "host" |> to_string in
      let port = primary |> member "port" |> to_int in
      Printf.printf "primary database -> %s:%d\n" host port
  | _ -> ()
```

### Example Tyco File

```
tyco/example.tyco
```

```tyco
# Global configuration with type annotations
str environment: production
bool debug: false
int timeout: 30

# Database configuration struct
Database:
 *str name:           # Primary key field (*)
  str host:
  int port:
  str connection_string:
  # Instances
  - primary, localhost,    5432, "postgresql://localhost:5432/myapp"
  - replica, replica-host, 5432, "postgresql://replica-host:5432/myapp"

# Server configuration struct  
Server:
 *str name:           # Primary key for referencing
  int port:
  str host:
  ?str description:   # Nullable field (?) - can be null
  # Server instances
  - web1,    8080, web1.example.com,    description: "Primary web server"
  - api1,    3000, api1.example.com,    description: null
  - worker1, 9000, worker1.example.com, description: "Worker number 1"

# Feature flags array
str[] features: [auth, analytics, caching]
```
