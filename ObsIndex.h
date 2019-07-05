/* 
 * File:   ObsIndex.h
 * Author: essam
 *
 * Created on June 18, 2015, 2:52 PM
 */

#ifndef OBSINDEX_H
#define	OBSINDEX_H

#include <vector>
#include <string>
#include <unordered_map>
#include <vector>

class ObsIndex {
public:
    ObsIndex();
    ObsIndex(const ObsIndex& orig);
    virtual ~ObsIndex();
    /**
     * 
     * @param states
     * @param emit_p
     * @return unordered map with all observations and associated emitted states.
     */
    void
    Initialise(std::unordered_map<std::string, std::vector<std::string>> &oi,
            std::vector<std::string> &states, // array of observations
            std::unordered_map<std::string, std::unordered_map<std::string, double>> &emit_p);
private:

};

#endif	/* OBSINDEX_H */

