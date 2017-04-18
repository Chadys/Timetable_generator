//
// Created by julie on 19/04/17.
//

#ifndef TIMETABLE_GENERATOR_NRPA_H
#define TIMETABLE_GENERATOR_NRPA_H


#include "Generator.h"
#include "Timetable.h"

class NRPA {
public:
    NRPA(Generator &gen_);
    Timetable generate();

private:
    Generator gen;
    std::unordered_map<Possibility, double> rollout_policy;
};


#endif //TIMETABLE_GENERATOR_NRPA_H
