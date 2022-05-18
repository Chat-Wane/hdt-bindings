use glob::glob;

fn main() -> miette::Result<()> {
    // println!("cargo:rerun-if-changed=build.rs"); // already happens
    println!("cargo:rerun-if-changed=src/");
    println!("cargo:rerun-if-changed=include/");

    let source_paths = [
        "src/",
        "hdt-cpp/libcds/src/static/bitsequence/",
        "hdt-cpp/libcds/src/static/coders/",
        "hdt-cpp/libcds/src/static/mapper/",
        "hdt-cpp/libcds/src/static/sequence/",
        "hdt-cpp/libcds/src/static/permutation/",
        "hdt-cpp/libcds/src/utils/",
        "hdt-cpp/libhdt/src/bitsequence/",
        "hdt-cpp/libhdt/src/dictionary/",
        "hdt-cpp/libhdt/src/hdt/",
        "hdt-cpp/libhdt/src/header/",
        "hdt-cpp/libhdt/src/huffman/",
        "hdt-cpp/libhdt/src/libdcs/",
        "hdt-cpp/libhdt/src/libdcs/fmindex/",
        "hdt-cpp/libhdt/src/rdf/",
        "hdt-cpp/libhdt/src/sequence/",
        "hdt-cpp/libhdt/src/triples/",
        "hdt-cpp/libhdt/src/util/",
        "hdt-cpp/libhdt/src/sparql/",
    ];

    let mut sources = Vec::new(); // looking for cpp file to compile
    for &path in source_paths.iter() {
        let pattern = format!("{}*.cpp", path);
        // println!("cargo:warning={}", pattern); // allow to print stuff during build
        for file in glob(pattern.as_str()).unwrap() {
            // println!("cargo:warning={}", file.unwrap().into_os_string().into_string().unwrap());
            sources.push(file.unwrap().into_os_string().into_string().unwrap());
        }
    }

    // exclude hdt.cpp that generate python bindings
    sources = sources
        .into_iter()
        .filter(|element| !element.ends_with("hdt.cpp"))
        .collect();

    let include_paths = [
        "include/",
        "hdt-cpp/libhdt/include/",
        "hdt-cpp/libhdt/src/dictionary/",
        "hdt-cpp/libhdt/src/sparql/",
        "hdt-cpp/libcds/include/",
        "hdt-cpp/libcds/src/static/bitsequence",
        "hdt-cpp/libcds/src/static/coders",
        "hdt-cpp/libcds/src/static/mapper",
        "hdt-cpp/libcds/src/static/permutation",
        "hdt-cpp/libcds/src/static/sequence",
        "hdt-cpp/libcds/src/utils",
    ];

    let bindings = std::path::PathBuf::from("src");
    let headers = std::path::PathBuf::from("include");
    let libhdt = std::path::PathBuf::from("hdt-cpp/libhdt/include");

    let mut b =
        autocxx_build::Builder::new("src/lib.rs", &[&bindings, &headers, &libhdt]).build()?;
    b.flag_if_supported("-std=c++14")
        .flag_if_supported("-w") // disable the numerous warnings
        .files(sources)
        .includes(include_paths)
        .compile("another-sage-engine");

    Ok(())
}
