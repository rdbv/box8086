#include "Memory.hpp"


ubyte& Memory::operator[] (std::size_t Index) {
    
    assert(Index < DEFAULT_MEMORY_SIZE); 
    return _memory[Index]; 

}

const ubyte& Memory::operator[] (std::size_t Index) const {

    assert(Index < DEFAULT_MEMORY_SIZE);
    return _memory[Index];

}

