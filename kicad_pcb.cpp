#include "kicad_pcb.h"

#include <cstdio>
#include <algorithm>
#include <set>
#include <cstring>

inline bool whiteString(const std::string& s, const char* whiteSpaces = " \r\n\t")
{
    for (const auto& c : s)
    {
        const char* whiteSpacesEnd = whiteSpaces + strlen(whiteSpaces);
        if (std::find(whiteSpaces, whiteSpacesEnd, c) == whiteSpacesEnd)
        {
            return false; // non-white char found, therefore the string is non-white
        }
    }
    return true;
}

int KicadPcb::read(const char* fname)
{
    const int ok = 0;
    const int err = 1;
    FILE* fin = fopen(fname, "r");
    if (!fin)
    {
        return err;
    }
    int ch = 0;
    int level = 0;
    bool inQuotes = false;
    std::string strAccum;
    while ((ch = fgetc(fin)) != EOF)
    {
        if (!inQuotes && (ch == '(' || ch == ')'))
        {
            if (!whiteString(strAccum))
            {
                KicadPcbItem item;
                item.l = level;
                item.s.swap(strAccum);
                items.push_back(item);
            }
            strAccum.clear();
            if (ch == '(')
            {
                ++level;
            }
            else
            {
                --level;
            }
        }
        else
        {
            strAccum += ch;
        }
        if (ch == '"')
        {
            inQuotes = !inQuotes;
        }
    }
    fclose(fin);
    return ok;
}

int KicadPcb::write(const char* fname) const
{
    // this creates a mess - open and resave it in KiCad PCB editor
    const int ok = 0;
    const int err = 1;
    FILE* fout = fopen(fname, "w");
    if (!fout)
    {
        return err;
    }
    int level = 0;
    for (const auto& item : items)
    {
        if (item.l > level)
        {
            int levelDiff = item.l - level;
            for (int i = 0; i < levelDiff; ++i)
            {
                fprintf(fout, "(\n");
            }
        }
        else if (item.l < level)
        {
            int levelDiff = level - item.l;
            for (int i = 0; i < levelDiff; ++i)
            {
                fprintf(fout, ")\n");
            }
            fprintf(fout, ")\n(\n");
        }
        else
        {
            fprintf(fout, ")\n(");
        }
        fprintf(fout, "%s", item.s.c_str());
        level = item.l;
    }
    for (int i = 0; i < level; ++i)
    {
        fprintf(fout, ")\n");
    }
    fclose(fout);
    return ok;
}

inline std::string trim(const std::string & source, const char* trimChars = " \n\r\t\"")
{
    std::string s(source);
    s.erase(0,s.find_first_not_of(trimChars));
    s.erase(s.find_last_not_of(trimChars)+1);
    return s;
}

inline bool startsWith(const std::string& l, const std::string& s)
{
    return l.rfind(s, 0) == 0;
}

inline std::string makeNet(int num, const std::string& name)
{
    char buff[1024] = {0};
    snprintf(buff, 1023, "net %d \"Net-%s\"", num, name.c_str());
    return std::string(buff);
}

inline std::string makeNet(const std::vector<std::string> netVec, const std::string& name)
{
    auto it = std::find(netVec.begin(), netVec.end(), name);
    if (it == netVec.end())
    {
        return "net 0 \"\"";
    }
    int index = it - netVec.begin();
    return makeNet(index+1, name);
}

void KicadPcb::addLinks(const std::map<std::string, std::map<int, std::string> >& linkMap)
{
    std::set<std::string> netSet;
    for (const auto& m : linkMap)
    {
        for (const auto& n : m.second)
        {
            netSet.insert(n.second);
        }
    }
    std::vector<std::string> netVec;
    std::copy(netSet.begin(), netSet.end(), std::back_inserter(netVec));
    std::vector<KicadPcbItem> newItems;
    std::string currentIcName;
    const std::map<int, std::string>* prd = nullptr;
    for (const KicadPcbItem& item : items)
    {
        newItems.push_back(item);
        const std::string textRef = "fp_text reference";
        if (startsWith(item.s, textRef))
        {
            currentIcName = trim(item.s.substr(textRef.size()));

            auto it = linkMap.find(currentIcName);
            if (it != linkMap.end())
            {
                prd = &(it->second);
            }
            else
            {
                prd = nullptr;
            }
        }
        else if (startsWith(item.s, "net 0 ") || startsWith(item.s, "net \"0\" "))
        {
            int si = 1;
            for (const auto& s : netVec)
            {
                KicadPcbItem newItem;
                newItem.s = makeNet(si++, s);
                newItem.l = item.l;
                newItems.push_back(newItem);
            }
        }
        else if (startsWith(item.s, "pad "))
        {
            std::string padStr = item.s.substr(4);
            std::replace(padStr.begin(), padStr.end(), '"', ' ');
            int padI = 0;
            sscanf(padStr.c_str(), "%d", &padI);
            if (padI && prd)
            {
                auto it = prd->find(padI);
                if (it != prd->end())
                {
                    KicadPcbItem newItem;
                    newItem.s = makeNet(netVec, it->second);
                    newItem.l = item.l + 1;
                    newItems.push_back(newItem);
                }
            }
        }
    }
    items.swap(newItems);
}
