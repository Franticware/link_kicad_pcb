#ifndef CSV_H
#define CSV_H

#include <vector>
#include <string>

class Csv
{
public:
    int read(const char* fname, char separator = ',');

    std::vector<std::vector<std::string> > data;
};

#endif // CSV_H
