#include "csv.h"

#include <fstream>
#include <sstream>

int Csv::read(const char* fname, char separator)
{
    const int ok = 0;
    const int err = 1;
    data.clear();
    std::ifstream ifstr(fname);
    if (!ifstr.is_open())
    {
        return err;
    }
    std::string line;
    while (std::getline(ifstr, line))
    {
        std::vector<std::string> dataline;
        std::stringstream input_stringstream(line);
        std::string field;
        while (getline(input_stringstream,field,separator))
        {
            dataline.push_back(field);
        }
        data.push_back(dataline);
    }
    return ok;
}
