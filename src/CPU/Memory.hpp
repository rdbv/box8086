#ifndef MEMORY_HPP
#define MEMORY_HPP

#include "Defs.hpp"

#include <type_traits>

class Memory {

public:
    Memory(){}
   
    
    ubyte& operator[](std::size_t);
    const ubyte& operator[](std::size_t) const;  

    template<typename T, unsigned int first, unsigned int second>
    inline T getRawData(int offset) { 
          
        const bool isByte = std::is_same<T, ubyte>() || std::is_same<T, sbyte>();
        const bool isWord = std::is_same<T, uword>() || std::is_same<T, sword>(); 
        static_assert(isByte ^ isWord, "Bad size! (Or isByte and isWord equal true, so WTF?)");   
        
        if(isByte)
            return _memory[offset + first]; 
        else 
            return _memory[offset + first] << 8 | _memory[offset + second];

    } 

private:
    ubyte _memory[DEFAULT_MEMORY_SIZE];

};


#endif
