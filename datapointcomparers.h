#ifndef DATAPOINTCOMPARERS_H
#define DATAPOINTCOMPARERS_H

#include "comparitor.h"

#include <string>

class DataPoint;

/* Uses the bridge pattern to decouple the type of comparitor used
 *   from the type of parameter used
 *
 */

class DataPointComparer {
public:
    virtual bool compare(const DataPoint*, const DataPoint*) = 0;
};

class DataPointComparerSTLAdapter : public std::less<DataPoint*>{
public:
    DataPointComparerSTLAdapter(DataPointComparer *comparer) : comp(comparer) {}

    virtual bool operator()(const DataPoint *lhs, const DataPoint *rhs) const {
        return comp->compare(lhs, rhs);
    }
private:
    DataPointComparer *comp;
};

/* remember to delete the pointer */
DataPointComparer *get_DataPointComparer(const std::string &parameter_name, const std::string &comp_type);

#endif // DATAPOINTCOMPARERS_H
