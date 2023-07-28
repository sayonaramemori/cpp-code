#include"read_matrix.h"
#include"string_tools.h"

using std::cout;
using std::endl;

int main(int argc,char* argv[]){
    if(argc!=2)abort();
    claris::read_matrix test(argv[1]);
    cout<<test<<endl;
    auto res = test.getMatrix();
    cout<<res<<endl;
    cout<<res.getEchelon()<<endl;
    return 0;
}
