#ifndef PROC_CSV_H
#define PROC_CSV_H

#include <map>
#include <string>
#include "csv.h"

std::map<std::string, std::map<int, std::string> > proc_csv(const Csv& csv);

#endif // PROC_CSV_H
