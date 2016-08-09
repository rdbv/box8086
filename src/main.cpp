#include <iostream>
#include <iomanip>

#include <string.h>
#include <stdio.h>

#include "CPU/CPU.hpp"
#include "Disasm/Disasm.hpp"

uint8_t* loadFile(const std::string& fileName, unsigned int* fileSize) {
    FILE* f = fopen(fileName.c_str(), "rb");
    if(f == NULL) {
        printf("Unable to open");
        return nullptr;
    }
    fseek(f, 0, SEEK_END);
    *fileSize = ftell(f); 
    rewind(f);

    uint8_t* buff = (uint8_t*) malloc(1024 * 1024);
    fread(buff, *fileSize, 1, f);
    return buff;

}

int main() {
    unsigned int fileSize;
    ubyte* bytes = loadFile("bin/readme.exe", &fileSize);
    //ubyte* bytes = loadFile("bin/test.bin", &fileSize);

    Disasm dis;
    dis.bindMemory(bytes);

    auto instr = dis.disasm(0x0000, 0x30);
    for(const auto& i : instr) {
        printf("%4x:\t%s\n", i.position, i.instr.c_str());  
    }

    free(bytes);
}

/*
#include "CPU/Overrides.hpp" 
int main() {
    // 
    // 0x26 es
    // 0x2e cs
    //
    uint8_t prx[] {
        0x26, 0x26,  0x2e, 0xf0,
    };

    unsigned int c = 0;
    Overrides ovr;

    while(c < sizeof(prx)) {
        ovr.clear();
        ovr.setOverrides(c, prx);
        printf("ovc:%d\n", ovr.getOverrideCount());
        c++;
    }
}
*/
