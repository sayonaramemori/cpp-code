#include"read_matrix.h"

namespace claris{
    
    void read_matrix::getVec(){
        const auto &lines = getlines();
        for(const auto &v:lines){
            temp.push_back(string2number(v));
        }
    }

}

