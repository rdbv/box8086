#ifndef MODRM_HPP
#define MODRM_HPP

#include <bitset>
#include "Defs.hpp"

struct ModRM {
    
    std::bitset<2> mode;         
    std::bitset<3> reg;          
    std::bitset<3> rm;         
    
    inline void clear() {
        mode = 0;
        reg = rm = 0;
    }

    inline void decode(ubyte _b) {
        mode = _b >> 6;
        reg = _b >> 3;
        rm = _b;
    }
   
    int getModInstrSize(int overrideCount) {

        switch(mode.to_ulong()) {
        
            case REGISTER_MODE:
                return 2 + overrideCount;
            
            case NO_DISPLACEMENT:
                if(rm == UWORD)
                    return 4 + overrideCount;
                else
                    return 2 + overrideCount;
            
            case BYTE_DISPLACEMENT:
                return 3 + overrideCount;
            
            case WORD_DISPLACEMENT:
                return 4 + overrideCount;
            default:
                assert(false);
        }

    }

}; 


#endif
