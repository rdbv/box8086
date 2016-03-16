#include "Memory.hpp"


ubyte& Memory::operator[] (std::size_t index) {
    
    assert(index < DEFAULT_MEMORY_SIZE); 
    return _memory[index]; 

}

const ubyte& Memory::operator[] (std::size_t index) const {

    assert(index < DEFAULT_MEMORY_SIZE);
    return _memory[index];

}

