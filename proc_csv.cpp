#include "proc_csv.h"

#include <cstdio>

inline int myStoi(const std::string s, bool* ok = nullptr)
{
    int ret = 0;
    int result = sscanf(s.c_str(), "%d", &ret);
    if (ok)
    {
        *ok = result == 1;
    }
    return ret;
}

std::map<std::string, std::map<int, std::string> > proc_csv(const Csv& csv)
{
    std::map<std::string, std::map<int, std::string> > ret;
    if (csv.data.empty())
        return ret;
    const int componentCount = int(csv.data[0].size())/4 + !!(int(csv.data[0].size())%4);
    for (int ci = 0; ci < componentCount; ++ci)
    {
        std::string icName = csv.data[0][ci * 4];
        std::map<int, std::string> netMap;
        for (size_t j = 1; j < csv.data.size(); ++j)
        {
            size_t ci0 = ci * 4;
            const std::vector<std::string>& d = csv.data[j];
            for (int k = 0; k != 2; ++k)
            {
                if (d.size() > ci0 + 1 + k * 2)
                {
                    std::string strI = d[ci0 + (k ? 3 : 0)];
                    bool ok;
                    int intI = myStoi(strI, &ok);
                    std::string strS = d[ci0+ (k ? 2 : 1)];
                    if (ok)
                    {
                        std::map<int, std::string>::const_iterator it;
                        if ((it = netMap.find(intI)) != netMap.end())
                        {
                            printf("Warning: Overwriting \"%s\" pin %d: \"%s\" -> \"%s\"\n", icName.c_str(), intI, it->second.c_str(), strS.c_str());
                        }
                        netMap[intI] = strS;
                    }
                }
            }
        }
        ret[icName] = netMap;
    }
    return ret;
}
