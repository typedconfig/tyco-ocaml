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
  let timezone = json |> member "timezone" |> to_string in
  Printf.printf "timezone=%s\n" timezone;
  (match member "Application" json with
  | `List (primary :: _) ->
      let service = primary |> member "service" |> to_string in
      let command = primary |> member "command" |> to_string in
      Printf.printf "primary service -> %s (%s)\n" service command
  | _ -> ());
  (match member "Host" json with
  | `List (_ :: backup :: _) ->
      let hostname = backup |> member "hostname" |> to_string in
      let cores = backup |> member "cores" |> to_int in
      Printf.printf "host %s cores=%d\n" hostname cores
  | _ -> ())
```

### Example Tyco File

```
tyco/example.tyco
```

```tyco
str timezone: UTC  # this is a global config setting

Application:       # schema defined first, followed by instance creation
  str service:
  str profile:
  str command: start_app {service}.{profile} -p {port.number}
  Host host:
  Port port: Port(http_web)  # reference to Port instance defined below
  - service: webserver, profile: primary, host: Host(prod-01-us)
  - service: webserver, profile: backup,  host: Host(prod-02-us)
  - service: database,  profile: mysql,   host: Host(prod-02-us), port: Port(http_mysql)

Host:
 *str hostname:  # star character (*) used as reference primary key
  int cores:
  bool hyperthreaded: true
  str os: Debian
  - prod-01-us, cores: 64, hyperthreaded: false
  - prod-02-us, cores: 32, os: Fedora

Port:
 *str name:
  int number:
  - http_web,   80  # can skip field keys when obvious
  - http_mysql, 3306
```
