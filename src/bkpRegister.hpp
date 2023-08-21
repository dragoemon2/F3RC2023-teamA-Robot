#pragma once
#include <functional>

class BKPRegister{
    
    public:
        BKPRegister();
        void open(unsigned int reg);
        void set(unsigned int begin, unsigned int size, unsigned int value);
        int get(unsigned int begin, unsigned int size);
        void save();
        
    private:
        unsigned int buffer = 0;
        unsigned int register_num = 0;
        
};