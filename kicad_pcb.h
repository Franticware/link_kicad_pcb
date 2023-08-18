#ifndef KICAD_PCB_H
#define KICAD_PCB_H

#include <vector>
#include <string>
#include <map>

struct KicadPcbItem
{
    int l;
    std::string s;
};

class KicadPcb
{
public:
    int read(const char* fname);
    int write(const char* fname) const;
    void addLinks(const std::map<std::string, std::map<int, std::string> >& linkMap);

    std::vector<KicadPcbItem> items;
};

#endif // KICAD_PCB_H
