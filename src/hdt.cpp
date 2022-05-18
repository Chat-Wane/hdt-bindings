// from https://github.com/RDFLib/rdflib-hdt/blob/master/src/hdt.cpp
#include <pybind11/pybind11.h>

// #include "docstrings.hpp"
#include "hdt_document.hpp"
#include "lazy_id_iterator.hpp"



namespace py = pybind11;

// (TODO) name the module with package name
PYBIND11_MODULE(hdt_bindings, m) {
  py::class_<LazyIDIterator>(m, "LazyIDIterator", "LazyIDIterator doc")
    .def("next", &LazyIDIterator::next, "next doc")
         // [](LazyIDIterator& self) {
         //   self.next();
         //   return make_tuple(self.subjectID(), self.predicateID(), self.objectID());
         // }, "next doc")
    .def_property_readonly("get_offset"  , &LazyIDIterator::getOffset  , "offset doc")
    .def_property_readonly("cardinality" , &LazyIDIterator::cardinality, "cardinality doc")
    .def_property_readonly("subject_id"  , &LazyIDIterator::subjectID  , "subject id doc")
    .def_property_readonly("predicate_id", &LazyIDIterator::predicateID, "predicate id doc")
    .def_property_readonly("object_id"   , &LazyIDIterator::objectID   , "object id doc")
    // vvvv   https://github.com/pybind/pybind11/issues/2469
    // long story short: no shared pointer for strings, instead, python copies the string
    // which implies a performance loss…
    .def("subject",   [](LazyIDIterator& self) { return *self.subject(); },   "subject doc")
    .def("predicate", [](LazyIDIterator& self) { return *self.predicate(); }, "predicate doc")
    .def("object",    [](LazyIDIterator& self) { return *self.object(); },    "object doc");

  py::class_<HDTDocument>(m, "HDTDocument", "HDTDocument doc")
    .def(py::init([](string file, bool map, bool indexed) {
      return unique_ptr<HDTDocument>(new HDTDocument(file, map, indexed));
    }))
    .def("search_ids",
         [](HDTDocument& self, int subject, int predicate, int object, int offset) {
           return new LazyIDIterator(self.searchIDs(subject, predicate, object, offset));
         }, "search_ids doc",
         py::arg("subject"), py::arg("predicate"), py::arg("object"),
         py::arg("offset") = 0)
    .def("search_triples",
         [](HDTDocument& self, string subject, string predicate, string object, int offset) {
           return new LazyIDIterator(self.searchTriples(subject, predicate, object, offset));
         }, "search_triples doc",
         py::arg("subject"), py::arg("predicate"), py::arg("object"),
         py::arg("offset") = 0)
    .def("get_subject_id"  , &HDTDocument::getSubjectID  , py::arg("subject"))
    .def("get_predicate_id", &HDTDocument::getPredicateID, py::arg("predicate"))
    .def("get_object_id"   , &HDTDocument::getObjectID   , py::arg("object"));
}
