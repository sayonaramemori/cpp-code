#include"string_tools.h"

namespace claris{

    std::vector<long double> string2number(const std::string& str){
        std::vector<long double> res;
        int begin = 0;
        for(int i=0;i<str.size();++i){
            if(str[i]==' '){
                continue;
            }else{
                begin = i;
                ++i;
                while(str[i]!=' '&&i<str.size())++i;
                res.push_back(std::stold(str.substr(begin,i-begin)));
            }
        }
        return res;
    }

}
