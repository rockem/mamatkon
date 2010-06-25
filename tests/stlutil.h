#ifndef STLUTIL_H
#define STLUTIL_H
#include <vector>
#include <boost/shared_ptr.hpp>

namespace MaTest {

using ::std::vector;
using ::boost::shared_ptr;

template <class T>
        bool listContains(vector<shared_ptr<T> > list, shared_ptr<T> object) {
    typename vector<shared_ptr<T> >::iterator i;
    for(i = list.begin(); i != list.end(); ++i) {
        if(*i->get() == *object) {
            return true;
        }
    }
    return false;
}

}
#endif // STLUTIL_H
