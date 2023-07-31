#include"read_matrix.h"
#include"matrix.h"
#include"string_tools.h"

using std::cout;
using std::endl;

template class claris::matrix<double>;
template class claris::read_matrix<double>;

int main(int argc,char* argv[]){
    if(argc!=2)abort();
    claris::read_matrix<double> test(argv[1]);
    cout<<test<<endl;
    auto res = test.getMatrix();
    cout<<res<<endl;
    cout<<res.get_echelon()<<endl;
    return 0;
}
