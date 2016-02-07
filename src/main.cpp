#include <iostream>


#include <string.h>
#include <stdio.h>
#include "CPU/CPU.hpp"
#include "Disasm/Disasm.hpp"

uint8_t* loadFile(const std::string& fileName, unsigned int* fileSize) {
    FILE* f = fopen(fileName.c_str(), "rb");
    if(f == NULL) {
        printf("Unable to open");
        return 0;
    }
    fseek(f, 0, SEEK_END);
    *fileSize = ftell(f); 
    rewind(f);

    uint8_t* buff = (uint8_t*) malloc(1024 * 1024);
    fread(buff, *fileSize, 1, f);
    return buff;

}


int main() {
   
/*    unsigned int fileSize;
    ubyte* bytes = loadFile("bin/test.bin", &fileSize);
//    CPU cpu;
//    Memory* mem = cpu.getMemory(); 
//    memcpy( &(*mem)[0], bytes, fileSize);

//    cpu.runTest();

*/

    Disasm dis;
    dis.bindMem(bytes);

    dis.disasmCount(0x0000, 10);

    free(bytes);


}
