#include"matrix.h"
namespace claris{

//----------------operation-between-matrix------------->>>>>
    matrix& matrix::operator=(const matrix& mat){
        this->data = mat.data;
        this->size_out = mat.size_out;
        this->size_in = mat.size_in;
        return *this;
    }

    matrix matrix::operator-() const{
        auto temp = *this;
        return temp*(-1);
    }

    matrix matrix::operator*(double coef) const{
        auto temp = *this;
        for(int i=0;i<size_out;++i){
            for(int j=0;j<size_in;++j)
                temp.data[i][j]*=coef;
        }
        return temp;
    }

    matrix matrix::operator-(matrix& mat) const{
        return (*this + (mat.operator-()));
    }

    matrix matrix::operator+(const matrix& mat) const{
        if(this->size_out!=mat.size_out||this->size_in!=mat.size_in)errMsg("operation + between two different matrix, invalid operation, program stop");
        auto temp = *this;
        for(int i=0;i<size_out;++i){
            for(int j=0;j<size_in;++j)
                temp.data[i][j] += mat.data[i][j];
        }
        return temp;
    }

    matrix  matrix::operator*(const matrix& mat) const{
        //check mxn * axb n==a?
        if(this->size_in!=mat.size_out)errMsg("Operation * between two matrix failed!");
        auto temp = matrix(this->size_out,mat.size_in);
        for(int col=0;col<mat.size_in;++col){
            std::vector<double> late_vector(mat.size_out,0);
            for(int index=0;index<mat.size_out;++index)
                late_vector[index] = mat.data[index][col];
            for(int row=0;row<temp.size_out;++row){
                temp.data[row][col] = vectorMul(late_vector,this->data[row]);
            }
        }
        return temp;
    }

    matrix operator*(double coef, const matrix& mat){
        return mat * coef;
    }

    matrix& matrix::operator*=(double coef){
        for(int i=0;i<size_out;++i){
            for(int j=0;j<size_in;++j)
                this->data[i][j]*=coef;
        }
        return *this;
    }


//----------------operation-between-matrix-------------<<<<<<

    matrix matrix::getSlimEchelon() const{
        if(empty())return matrix();
        auto temp = this->getEchelon();
        while(1){
            if(zeroVector(temp.data.back())&&(!temp.data.empty()))temp.data.pop_back();
            else break;
        }
        temp.size();
#ifdef DEBUG
        std::cout<<"Slim echelon:"<<std::endl;
        std::cout<<temp<<std::endl;
#endif
        return temp;
    }

    int matrix::rank() const{
        return this->getSlimEchelon().size();
    }

    std::vector<std::vector<double>> matrix::getRoot() const{
        auto temp = this->getSlimEchelon();
        if(temp.data.back().back()!=0){
            auto vec = temp.data.back();
            vec.pop_back();
            if(zeroVector(vec))return {};
        }

        std::set<int> freeVar;
        for(int i=0;i<size_in-1;++i)freeVar.insert(i);
        int index = 0;
        for(int row=0;row<temp.size_out;++row){
            for(int col=index;col<temp.size_in-1;++col){
                if(temp.data[row][col]!=0){
                    index = col+1;
                    freeVar.erase(freeVar.find(col));
                    break;
                }
            }
        }
        MATRIX res;
        auto specificity = temp.getByCol(size_in);
        specificity.resize(temp.size_in-1);
        res.push_back(specificity);
        for(auto v:freeVar){
            auto vec = temp.getByCol(v+1);
            vec.resize(temp.size_in-2);
            vec.insert(vec.begin()+v,-1);
            res.push_back(vec);
        }
        return res;
    }

//--------ensure this matrix not empty-----------
    void matrix::swap_row(int lhv,int rhv){
        if(lhv==rhv)return;
        std::swap(this->data[lhv],this->data[rhv]);
        return;
    }

    void matrix::add2(int dest_row,double k,int row){
        for(int i=0;i<size_in;++i)
            this->data[dest_row][i] += k * this->data[row][i];
    }

    void matrix::mul2(int dest,double k){
        for(int i=0;i<size_in;++i)
            this->data[dest][i] *= k;
    }
//--------ensure this matrix not empty-----------

    bool matrix::zeroVector(const std::vector<double> &vec)const{
        for(auto v:vec)if(v!=0)return false;
        return true;
    }

    void matrix::toEchelon(){
        if(empty())return;
        //for a single vector
        if(this->size_out==1){
            for(auto v:this->data[0]){
                if(v!=0){
                    *this *= 1/v;
                    break;
                }
            }
            return;
        }

        int rows = size_out;
        int cols = size_in;
        int tails = cols - rows;
        while(tails>0){
            this->data.push_back(data.back());
            --tails;
        }
        size();
        tails = cols - rows;
        int step_in = rows>cols?rows:cols;

        for(int col=0;col<cols;++col)
            for(int row=col;row<step_in;++row){
                if(this->data[row][col]==0)continue;
                int start = row;
                for(++row;row<step_in;++row){
                    if(this->data[row][col]==0)continue;
                    double k = -this->data[row][col]/this->data[start][col];
                    add2(row,k,start);
#ifdef DEBUG
        std::cout<<*this<<std::endl;
#endif
                }
                swap_row(start,col);
                for(int index=0;index<col;++index){
                    if(this->data[index][col]==0)continue;
                    double k = -this->data[index][col]/this->data[col][col];
                    add2(index,k,col);
#ifdef DEBUG
        std::cout<<*this<<std::endl;
#endif
                }
                //data[col][col] must be no zero number
                mul2(col,1/this->data[col][col]);
            }
        //delete the tails
        while(tails>0){
            for(auto it = this->data.rbegin();it!=this->data.rend();++it){
                if(zeroVector(*it)){
                    this->data.erase(it.base()-1);
                    --tails;
                    break;
                }
            }
        }
        size();
    }


    // Function to calculate the determinant of a square matrix
    double matrix::laplace(const std::vector<std::vector<double>> &val)const {
        int n = val.size();
        double det = 0.0;
        if (n == 1) {
            return val[0][0];
        }
        else if (n == 2) {
            return (val[0][0] * val[1][1]) - (val[0][1] * val[1][0]);
        }
        else {
            for (int k = 0; k < n; k++) {
                std::vector<std::vector<double>> subval(n - 1, std::vector<double>(n - 1));

                for (int i = 1; i < n; i++) {
                    int j = 0;
                    for (int l = 0; l < n; l++) {
                        if (l != k) {
                            subval[i - 1][j] = val[i][l];
                            j++;
                        }
                    }
                }
                det += std::pow(-1.0, k) * val[0][k] * laplace(subval);
            }
        }
        return det;
    }


    void matrix::transform(){
        MATRIX tmp(size_in,std::vector<double>(size_out));
        for(int i=0;i<size_out;++i){
            for(int j=0;j<size_in;++j){
                tmp[j][i] = data[i][j];
            }
        }
        double sizeTmp = size_in;
        size_in = size_out;
        size_out = sizeTmp;
        data = tmp;
    }

    //assign from bad vector,the shape depends on the first vector
    void matrix::assign(const MATRIX& mat){
        size_out = mat.size();
        if(size_out==0){
            size_in = 0;
            return;
        }
        size_in = mat[0].size();
        this->data = std::vector<std::vector<double>>(size_out,std::vector<double>(size_in,0));
        for(int i=0;i<size_out;++i){
            int size = mat[i].size();
            int index_min = (size_in>size)?size:size_in;
            for(int j=0;j<index_min;++j){
                data[i][j] = mat[i][j];
            }
        }
        return;
    }

    matrix::matrix(std::vector<std::vector<double>> val,int type){
        this->assign(val);
        if(type==COL)transform();
    }


    std::ostream& operator<<(std::ostream& os,const matrix& ma)
    {
        int index = 0;
        int gap = 5;
        int last = ma.size_out - 1;
        int in_last = ma.size_in - 1;
        std::cout<<std::setprecision(gap)<<std::noshowpoint;
        for(;index<ma.size_out;++index){
            //head
            if(index==0){
                std::cout<<"/| ";
            }else{
                if(index==last)std::cout<<"\\| ";
                else std::cout<<"|| ";
            }
            //body
            for(int i=0;i<ma.size_in;++i){
                double val = 0;
                if(ma.data[index][i]!=0)val = ma.data[index][i];
                std::cout<<std::left;
                std::cout<<std::setw(gap)<<val<<" |";
                if(i!=in_last)std::cout<<" ";
            }
            //tail
            if(index==0){
                std::cout<<"\\";
            }else{
                if(index==last)std::cout<<"/";
                else std::cout<<"|";
            }
            std::cout<<std::endl;
        }
        return os;
    }

}
