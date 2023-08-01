#include"string_tools.h"
#include"matrix.h"
#include"read_only.h"
namespace claris{
    template<typename T>
    class read_matrix : public read_only
    {
        public:
            read_matrix(const std::string &file,const std::string &cm=""):read_only(file,cm){getVec();}
            matrix<T> getMatrix(){return matrix<T>(temp);}
        private:
            std::vector<std::vector<T>> temp;
            void getVec(){
                const auto &lines = getlines();
                std::vector<std::vector<long double>> val;
                for(const auto &v:lines)val.push_back(string2number(v));
                assign(val);
            }
            void assign(const std::vector<std::vector<long double>> &val){
                if(val.empty())return;
                temp.resize(val.size(),std::vector<T>(val[0].size(),0));
                for(int i=0;i<val.size();++i)
                    for(int j=0;j<val[0].size();++j){
                        temp[i][j] = val[i][j];
                    }
                return;
            }
    };
}
