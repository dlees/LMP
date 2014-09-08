#ifndef COMPARITOR_H
#define COMPARITOR_H

#include "error.h"

/* You should always use get_comparitor to use these types
 *
 * This is used as the implementation of a Bridge for DataPointComparers
 */
template <typename T>
class Comparitor {
public:
    virtual bool compare(const T &lhs, const T &rhs) = 0;
};

template <typename T>
class Less_Than : public Comparitor<T> {
public:
    bool compare(const T &lhs, const T &rhs) {return lhs < rhs;}
};

template <typename T>
class Greater_Than : public Comparitor<T> {
public:
    bool compare(const T &lhs, const T &rhs) {return lhs > rhs;}
};


/* Possible types:
 *   "less than"
 */
template <typename T>
static Comparitor<T> *get_comparitor(const std::string &type) {
    Comparitor<T> *to_return = 0;
    if (type == "less than") {
        static Less_Than<T> *less_than = new Less_Than<T>;
        to_return = less_than;
    }
    else if (type == "greater than") {
        static Greater_Than<T> *greater_than = new Greater_Than<T>;
        to_return = greater_than;
    }
    else {
        throw Error(QString("No Comparitor of type:") + QString::fromStdString(type)) ;
    }

    return to_return;
}
#endif // COMPARITOR_H
