pub use crate::ffi::HDTDocument;
use autocxx::prelude::*;

include_cpp! {
    #include "hdt_document.hpp" // your header file name
    #include "lazy_id_iterator.hpp"
    safety!(unsafe_ffi) // see details of unsafety policies described in include_cpp
        generate!("HDTDocument") // add this line for each function or type you wish to generate
        generate!("LazyIDIterator")
}

// vvv So HDTDocument can be shared through arc<mutex>
unsafe impl Send for HDTDocument {}

// (TODO) IDIterator exported as a proper iterator with trait actually implemented

#[cfg(test)]
mod tests {
    use super::HDTDocument;
    use autocxx::{c_int, c_uint, WithinUniquePtr};

    #[test]
    #[cfg(feature = "test_watdiv")]
    fn simple_hdt_document_opening() {
        // (TODO) configurable path
        let path = "../../../sage-benchmark/datasets/watdiv/watdiv10M.hdt";
        let hdt = HDTDocument::new(path, true, true).within_unique_ptr();
        let any = c_int::from(0);
        let mut it = hdt
            .as_ref()
            .unwrap()
            .searchIDs(any, any, any, c_uint::from(0))
            .within_unique_ptr();
        assert_eq!(it.as_ref().unwrap().cardinality(), 10916457);
        let mut nexted = it.as_mut().unwrap().next();
        assert!(nexted);
        let mut nexted = it.as_mut().unwrap().next();
        assert!(nexted);
    }
}
