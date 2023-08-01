#include"read_matrix.h"
#include"matrix.h"
#include"string_tools.h"

using std::cout;
using std::endl;

template class claris::matrix<long double>;
template class claris::read_matrix<long double>;

template <typename T>
void print(const std::string &msg,const T &val){
    std::cout<<msg<<std::endl;
    std::cout<<val<<std::endl;
}

int main(int argc,char* argv[]){
    if(argc!=2)abort();
    claris::read_matrix<long double> test(argv[1]);
    print("input:",test);
    auto res = test.getMatrix();
    print("matrix:",res);
    print("echelon:",res.get_echelon());
    print("inhomogeneous_solution:",res.get_inhomogeneous_solution());
    print("homogeneous_solution:",res.get_homogeneous_solution());
    print("submatrix_row_1:",res.get_submatrix_by_row(std::make_pair(0,1)));
    print("submatrix_col_1&2:",res.get_submatrix_by_col(std::make_pair(0,2)));
    print("submatrix_range:",res.get_submatrix_by_range(std::make_pair(1,3),std::make_pair(1,3)));
    return 0;
}
