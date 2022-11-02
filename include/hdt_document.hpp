#pragma once

#include <HDTManager.hpp>
#include <HDT.hpp>
#include <HDTEnums.hpp>
#include <Iterator.hpp> // IteratorTripleID
#include <SingleTriple.hpp> // TripleID
#include "lazy_id_iterator.hpp"



using namespace hdt;
// It is meant to look like the implementation for
// python bindings: https://github.com/Callidon/pyHDT/blob/master/src/hdt_document.cpp
// so if documentation is missing here, look for this document instead ^^^^
class HDTDocument {
  HDT* hdt; // The opened hdt file
  
public:  
  HDTDocument(string path, bool map, bool indexed); // no default possible in bindings
  ~HDTDocument();

  LazyIDIterator searchTriples(string subject, string predicate, string object,
                               unsigned int offset) const;
  
  LazyIDIterator searchIDs(int subject, int predicate, int object,
                           unsigned int offset) const;
  
  int getSubjectID  (string subject)   const;
  int getPredicateID(string predicate) const;
  int getObjectID   (string object)    const;
  
  string getSubject  (int subject)   const;
  string getPredicate(int predicate) const;
  string getObject   (int object)    const;
};

