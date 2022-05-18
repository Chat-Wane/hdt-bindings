#pragma once

#include <Iterator.hpp> // IteratorTripleID
#include <HDT.hpp>
#include <memory> // make sure that std::shared_ptr exists



using namespace hdt;
// More or less and reimplementation of:
// https://github.com/rdfhdt/hdt-cpp/blob/develop/libhdt/src/hdt/TripleIDStringIterator.hpp
// because we always need indexes, and only optionally results, we want to lazy load results
class LazyIDIterator {
  
  Dictionary *dict; // https://github.com/rdfhdt/hdt-cpp/blob/develop/libhdt/include/Dictionary.hpp
  
  IteratorTripleID *it;
  int offset;
  TripleID *tid;

  bool sChanged;
  bool pChanged;
  bool oChanged;

  // lazy loaded values of subject predicate object
  shared_ptr<string> ss;
  shared_ptr<string> sp;
  shared_ptr<string> so;
  
public:

  LazyIDIterator(Dictionary* dict, IteratorTripleID* it, int offset);
  // LazyIDIterator(const LazyIDIterator &it);
  ~LazyIDIterator();

  int getOffset() const;
  
  size_t cardinality() const;
  int subjectID()   const;
  int predicateID() const;
  int objectID()    const;

  bool next();
  shared_ptr<string> subject();
  shared_ptr<string> predicate();
  shared_ptr<string> object();
  
};
