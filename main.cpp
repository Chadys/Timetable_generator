#include <iostream>
#include <map>
#include <functional>
#include "DataProvider.h"
#include "NRPA.h"
#include "Timetable.h"

int main(int argc, char* argv[]) {
    if (argc != 4){
        std::cout << "Usage: Timetable_Generator [Filename for classes infos]" << ' ' <<
                "[Filename for teachers infos]" << ' ' << "[Filename for classrooms infos]" << std::endl;
        return 1;
    }

    DataProvider generator(argv[3], argv[1], argv[2]);
    NRPA solver(generator);
    vector<Timetable> timetables = solver.generate();
    return 0;
}