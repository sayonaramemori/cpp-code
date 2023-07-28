#include"string_tools.h"
#include"matrix.h"
#include"read_only.h"
namespace claris{
    class read_matrix : public read_only
    {
        public:
            read_matrix(const std::string &file,const std::string &cm=""):read_only(file,cm)
            {
                getVec();
            }
            matrix getMatrix(){
                return matrix(temp);
            }
        private:
            std::vector<std::vector<double>> temp;
            void getVec();
    };
}
