#ifndef OVERRIDES_HPP
#define OVERRIDES_HPP

#include "Defs.hpp"

struct Overrides {
    
    ubyte regOverride  = 0, regOverrideCount  = 0;
    ubyte repOverride  = 0, repOverrideCount  = 0;
    ubyte lockOverride = 0, lockOverrideCount = 0;    

    /* Return overrides count for instruction */

    unsigned int getOverrideCount() {
        unsigned int counter=0;

        if(regOverrideCount > 1 || repOverrideCount > 1 || lockOverrideCount > 1) 
            return 0;

        if(regOverride  != 0) counter++;
        if(repOverride  != 0) counter++;
        if(lockOverride != 0) counter++;
        return counter; 
    }

    /* Set overrides from memory beginning at address counter */

    void setOverrides(unsigned int counter, uint8_t* memory) {    
        assert(memory != nullptr);

        enum regOvrType { REGISTER_OVERRIDE, REP_OVERRIDE, LOCK_OVERRIDE, NOT_OVERRIDE };

            /* Detect override type */
            auto isOverrideOpcode = [this, &counter, memory]() {
                if(memory[counter] == CS_OVR || memory[counter] == DS_OVR ||
                   memory[counter] == ES_OVR || memory[counter] == SS_OVR) 
                        return REGISTER_OVERRIDE;
                
                if(memory[counter] == REPZ_OVR || memory[counter] == REPNZ_OVR)
                        return REP_OVERRIDE;
               
                if(memory[counter] == LOCK_OVR)
                        return LOCK_OVERRIDE;
               
                return NOT_OVERRIDE;
            };
        
        regOvrType ovr;
        
        while( (ovr = isOverrideOpcode()) != NOT_OVERRIDE ) {        
            if(ovr == REGISTER_OVERRIDE) 
                regOverride  = memory[counter], regOverrideCount++;
            
            if(ovr == REP_OVERRIDE)      
                repOverride  = memory[counter], repOverrideCount++;
            
            if(ovr == LOCK_OVERRIDE)     
                lockOverride = memory[counter], lockOverrideCount++;
//
//            printf("Wiem. reg:%d rep:%d lock:%d\n", regOverrideCount, 
//                                                    repOverrideCount, 
//                                                    lockOverrideCount); 
//
            counter++;
        }

    }    

    /* Just clear */

    inline void clear() {
        regOverride = repOverride = lockOverride = 0; 
        regOverrideCount = 0, repOverrideCount = 0, lockOverrideCount = 0;
    }

}; 

#endif
