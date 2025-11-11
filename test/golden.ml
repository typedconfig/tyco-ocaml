let suite_root =
  let root = Option.value ~default:"." (Sys.getenv_opt "DUNE_SOURCEROOT") in
  Filename.concat root "../tyco-test-suite"

let () =
  let inputs_dir = suite_root ^ "/inputs" in
  let files =
    Sys.readdir inputs_dir |> Array.to_list |> List.filter (Filename.check_suffix @@ ".tyco") |> List.sort String.compare
  in
  let failed = ref false in
  List.iter
    (fun file ->
      let expected = suite_root ^ "/expected/" ^ Filename.remove_extension file ^ ".json" in
      if Sys.file_exists expected then
        let actual = String.trim (Tyco.load_file (Filename.concat inputs_dir file)) in
        let expected_json =
          let ic = open_in expected in
          Fun.protect ~finally:(fun () -> close_in ic) (fun () ->
              really_input_string ic (in_channel_length ic))
        in
        if String.trim expected_json <> actual then (
          Printf.eprintf "Mismatch: %s\n%!" file;
          failed := true))
    files;
  if !failed then exit 1
