#include "kicad_pcb.h"
#include "csv.h"
#include "proc_csv.h"

#include <cstdio>
#include <algorithm>

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        printf("Usage example: link_kicad_pcb input.kicad_pcb links.csv output.kicad_pcb\n");
        return 0;
    }
    const char* input_kicad_pcb_file = argv[1];
    const char* links_csv_file = argv[2];
    const char* output_kicad_pcb_file = argv[3];
    KicadPcb kp;
    kp.read(input_kicad_pcb_file);
    Csv csv;
    csv.read(links_csv_file);
    std::map<std::string, std::map<int, std::string> > linkMap = proc_csv(csv);
    kp.addLinks(linkMap);
    kp.write(output_kicad_pcb_file);
    return 0;
}
