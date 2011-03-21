#ifndef __STL_ADDS_H
#define __STL_ADDS_H

#include<list>
#include<string>
#include <vector>

#define FOREACH(colltype, coll, it) for( colltype::iterator it = (coll).begin(); it!= (coll).end(); it++)
#define FOREACHC(colltype, coll, it) for( colltype::const_iterator it = (coll).begin(); it!= (coll).end() ; it++ )


/// contains some additions to the standard template library
namespace std {

  /// absolute function for all types
  template<typename T>
  inline T abs(T v)
  { return ((v>0)?v:-v); }

  /// += operators for list (list concat)
  template <class T, class A>
    list<T,A>& operator += (list<T,A>& l1, const list<T,A>& l2) { 
    l1.insert(l1.end(), l2.begin(), l2.end()); 
    return l1;
  }

  /// += operators for list (append)
  template <class T, class A>
    list<T,A>& operator += (list<T,A>& l1, const T& v) { 
    l1.push_back(v); 
    return l1;
  }

  /// + operators for lists (list concat)
  template <class T, class A>
    list<T,A> operator + (const list<T,A>& l1, const list<T,A>& l2) { 
    list<T,A> rv(l1.begin(),l1.end());
    rv += l2;
    return rv;
  }

  string itos(int i);

  /**
   * Determines if a collection contains an element and at which position
   * it was found. -- O(n)
   * @param coll the collection to search through
   * @param elem the element to find
   * @return the iterator pointing to the element if found, otherwise coll.end()
   */
  template<typename collT, typename elemT> typename collT::iterator posInColl(collT coll, elemT elem) {
      FOREACH(typename collT, coll, it)
        if (*it==elem)
          return it;
      return coll.end();
  }

}

#endif
