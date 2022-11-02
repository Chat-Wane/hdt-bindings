#include "lazy_id_iterator.hpp"

using namespace hdt;



LazyIDIterator::LazyIDIterator(Dictionary* dictionary, IteratorTripleID* iterator, int index) {
  offset = index;
  it     = iterator;
  dict   = dictionary;
  tid    = NULL; // new Triple(-1,-1,-1) would be better, but how free it properly then?
}

LazyIDIterator::~LazyIDIterator() {
  // since https://github.com/rdfhdt/hdt-cpp/blob/332a9cc2d5273e76b9daad366f7d2f80adb6b3fc/libhdt/include/Triples.hpp#L61
  delete it;
  // tid is deleted by it on it->next
  // dict is deleted by hdt_document
}

size_t LazyIDIterator::cardinality() const {
  return it->estimatedNumResults();
}

void LazyIDIterator::reset() {
  it->goToStart();
}

void LazyIDIterator::skip(unsigned int index) {
  it->goToStart();
  size_t cardinality = it->estimatedNumResults();
  offset = index;
  if (offset > 0 && offset >= cardinality) {
    // hdt does not allow to skip past beyond the estimated nb of results,
    // so we may have a few results to skip manually
    unsigned int remainingSteps = offset - cardinality + 1;
    it->skip(cardinality - 1);
    while (it->hasNext() && remainingSteps > 0) {
      it->next();
      remainingSteps--;
    }
  } else if (offset > 0) {
    it->skip(offset);
  }
}

bool LazyIDIterator::next() {
  if (!it->hasNext()) {return false;};

  size_t prevSId = tid != NULL ? tid->getSubject()   : -1;
  size_t prevPId = tid != NULL ? tid->getPredicate() : -1;
  size_t prevOId = tid != NULL ? tid->getObject()    : -1;

  tid = it->next();
  offset += 1;
  
  // To only update those that actually change of index
  if (prevSId != tid->getSubject())   { sChanged = true; }
  if (prevPId != tid->getPredicate()) { pChanged = true; }
  if (prevOId != tid->getObject())    { oChanged = true; }
  
  return true; // (TODO) return offset or -1 if nothing
}



int LazyIDIterator::getOffset() const {
  return offset;
}



int LazyIDIterator::subjectID() const {
  return tid->getSubject();
}

int LazyIDIterator::predicateID() const {
  return tid->getPredicate();
}

int LazyIDIterator::objectID() const {
  return tid->getObject();
}



// Allow picking the proper getter, instead of all three at once.
shared_ptr<string> LazyIDIterator::subject() {
  if (sChanged) {
    ss = shared_ptr<string>(new string(dict->idToString(tid->getSubject(), SUBJECT)));
    sChanged = false;
  }
  return shared_ptr<string>(ss);
}

shared_ptr<string> LazyIDIterator::predicate() {
  if (pChanged) {
    sp = shared_ptr<string>(new string(dict->idToString(tid->getPredicate(), PREDICATE)));
    pChanged = false;
  }
  return shared_ptr<string>(sp);
}

shared_ptr<string> LazyIDIterator::object()  {
  if (oChanged) {
    so = shared_ptr<string>(new string(dict->idToString(tid->getObject(), OBJECT)));
    oChanged = false;
  }
  return shared_ptr<string>(so);
}
