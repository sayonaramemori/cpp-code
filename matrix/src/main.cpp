#include"matrix.h"

using std::cout;
using std::endl;

int main(int argc,char* argv[]){
    std::vector<std::vector<double>> data={
         {11,2,9,3},
    };
    claris::matrix test(data);
    cout<<9*test<<std::endl;

    cout<<"echelon:"<<endl;
    cout<<test.getEchelon()<<endl;

    cout<<"rank is:"<<test.rank()<<endl;
    auto res = test.getRoot();

    for(auto out:res){
        for(auto in:out){
            cout<<in<<" ";
        }
        cout<<endl;
    }

    return 0;
}
