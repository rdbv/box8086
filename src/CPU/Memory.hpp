#ifndef MEMORY_HPP
#define MEMORY_HPP

#include "Defs.hpp"

#include <type_traits>

class Memory {

public:
    Memory(){}
   
    
    ubyte& operator[](std::size_t);
    const ubyte& operator[](std::size_t) const;  


    template<typename T, int First, int Second>
    inline T getRawData(int Offset) { 
          
        const bool isByte = std::is_same<T, ubyte>() || std::is_same<T, sbyte>();
        const bool isWord = std::is_same<T, uword>() || std::is_same<T, sword>(); 
        static_assert(isByte ^ isWord, "Bad size! (Or isByte and isWord equal true, so WTF?)");   

        if(isByte)
            return _memory[Offset + First]; 
        else 
            return _memory[Offset + First] << 8 | _memory[Offset + Second];

    } 

private:
    ubyte _memory[DEFAULT_MEMORY_SIZE];

};


#endif
