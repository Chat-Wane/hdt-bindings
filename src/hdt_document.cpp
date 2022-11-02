#include "hdt_document.hpp"

using namespace hdt;



HDTDocument::HDTDocument(string path, bool map, bool indexed) {
  if (!map and !indexed) {
    hdt = HDTManager::loadHDT(path.c_str());
  } else if (!map and indexed) {
    hdt = HDTManager::loadIndexedHDT(path.c_str());
  } else if (map and indexed) {
    hdt = HDTManager::mapIndexedHDT(path.c_str());
  } else { // map and !indexed
    hdt = HDTManager::mapHDT(path.c_str());
  }
}

HDTDocument::~HDTDocument(){
  delete hdt;
}



template <typename T>
inline void applyOffset(T *it, unsigned int offset, unsigned int cardinality) {
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



// looks like:
// https://github.com/rdfhdt/hdt-cpp/blob/cc90812896b5c4f55a835557a230d6d5b54d3ac0/libhdt/src/hdt/BasicHDT.cpp#L164
// but returns our own iterator with skipped
// searchTriples convenient for debug but should never be used because it looks for IDS first
LazyIDIterator HDTDocument::searchTriples(string subject, string predicate, string object,
                                      unsigned int offset) const {
  
  TripleString ts(subject.c_str(), predicate.c_str(), object.c_str());
  TripleID tid;
  hdt->getDictionary()->tripleStringtoTripleID(ts, tid);

  IteratorTripleID *it;
  if((!subject.empty()   and tid.getSubject()==0) ||
     (!predicate.empty() and tid.getPredicate()==0) ||
     (!object.empty()    and tid.getObject()==0)) {
    it = new IteratorTripleID();
  } else {
    it = hdt->getTriples()->search(tid);
    applyOffset<IteratorTripleID>(it, offset, it->estimatedNumResults());
  }

  return LazyIDIterator(hdt->getDictionary(), it, offset);
}



LazyIDIterator HDTDocument::searchIDs(int subject, int predicate, int object,
                                  unsigned int offset) const {
  IteratorTripleID *it;
  if (subject < 0 || predicate < 0 || object < 0) { // no identifier found for one of those
    it = new IteratorTripleID();
  } else {
    TripleID tid = TripleID(subject, predicate, object);
    it = hdt->getTriples()->search(tid);
    applyOffset<IteratorTripleID>(it, offset, it->estimatedNumResults());
  }

  return LazyIDIterator(hdt->getDictionary(), it, offset);
}



int HDTDocument::getSubjectID(string subject) const {
  // https://github.com/rdfhdt/hdt-cpp/blob/cc90812896b5c4f55a835557a230d6d5b54d3ac0/libhdt/include/Dictionary.hpp#L68
  // doc (see above) says throws when not found; but actually returns 0
  size_t result = hdt->getDictionary()->stringToId(subject, SUBJECT);
  if (result == 0 && !subject.empty()) { return -1; }
  return result;
}

int HDTDocument::getPredicateID(string predicate) const {
  size_t result = hdt->getDictionary()->stringToId(predicate, PREDICATE);
  if (result == 0 && !predicate.empty()) { return -1; }
  return result;
}

int HDTDocument::getObjectID(string object) const {
  size_t result = hdt->getDictionary()->stringToId(object, OBJECT);
  if (result == 0 && !object.empty()) { return -1; }
  return result;
}

string HDTDocument::getSubject(int subject) const {
  return hdt->getDictionary()->idToString(subject, SUBJECT);
}

string HDTDocument::getPredicate(int predicate) const {
  return hdt->getDictionary()->idToString(predicate, PREDICATE);
}

string HDTDocument::getObject(int object) const {
  return hdt->getDictionary()->idToString(object, OBJECT);
}
