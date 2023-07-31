#ifndef CLARIS_MATRIX
#define CLARIS_MATRIX

#include<iostream>
#include<cmath>
#include<vector>
#include<iomanip>
#include<algorithm>
#include<set>

namespace claris{

    template<typename T>class matrix;

    template<typename T>
    std::ostream& operator<<(std::ostream& os,const claris::matrix<T>& ma);

    template<typename T>
    claris::matrix<T> operator*(T coef, const claris::matrix<T>&);

    template<typename T>
    class matrix{
    public:
        using MATRIX = std::vector<std::vector<T>>;
        enum {ROW,COL};
        matrix(){
            this->size_in=0;
            this->size_out=0;
        }
        matrix(int row,int col,double val=0){
            this->size_out = row;
            this->size_in = col;
            this->data = MATRIX(size_out,std::vector<T>(size_in,val));
        }
        matrix(std::vector<std::vector<T>>,int type=ROW);
        ~matrix()=default;

        matrix& operator=(const matrix&);
        matrix& operator*=(T);
        matrix operator-()const;
        matrix operator-(matrix&)const;
        matrix operator+(const matrix&)const;
        matrix operator*(const matrix&)const;
        matrix operator*(T)const;

        //row and col begin from 1
        std::vector<T> get_by_row(int row)const{return this->data[row-1];}
        std::vector<T> get_by_col(int col)const{
            auto res = std::vector<T>(size_out,0);
            for(int i=0;i<size_out;++i){
                res[i] = this->data[i][col-1];
            }
            return res;
        }

        bool empty() const{return this->data.empty();}
        matrix<T> get_echelon() const{
            auto temp = *this;
            temp.to_echelon();
            return temp;
        }
        std::vector<std::vector<T>> get_inhomogeneous_solution() const;
        void transform();
        void to_echelon();
        int rank() const;
        int size(){
            this->size_out = data.size();
            return size_out;
        }

    private:
        void swap_row(int,int);
        void add2(int dest,T k,int row);
        void mul2(int dest,T k);
        matrix<T> get_slim_echelon() const;

    private:
        bool zero_vector(const std::vector<T> &vec)const;
        T laplace(const std::vector<std::vector<T>> &val)const;
        void assign(const MATRIX&);
        T vector_mul(const std::vector<T> &lhv, const std::vector<T> &rhv)const{
            T res = 0;
            int size = lhv.size()>rhv.size()?rhv.size():lhv.size();
            for(int i=0;i<size;++i){
                res += lhv[i]*rhv[i];
            }
            return res;
        }
        void err_msg(const std::string& msg) const{
            std::cout<<msg<<std::endl;
            exit(-1);
        }
    private:
        MATRIX data;
        size_t size_out;
        size_t size_in;

    template<typename X>
        friend std::ostream& operator<<(std::ostream& os,const claris::matrix<X>& ma);
    template<typename X>
        friend claris::matrix<T> operator*(X coef, const claris::matrix<X>&);
    };


}


namespace claris{

//----------------operation-between-matrix------------->>>>>
    template<typename T>
    matrix<T>& matrix<T>::operator=(const matrix<T>& mat){
        this->data = mat.data;
        this->size_out = mat.size_out;
        this->size_in = mat.size_in;
        return *this;
    }

    template<typename T>
    matrix<T> matrix<T>::operator-() const{
        auto temp = *this;
        return temp*(-1);
    }

    template<typename T>
    matrix<T> matrix<T>::operator*(T coef) const{
        auto temp = *this;
        for(int i=0;i<size_out;++i){
            for(int j=0;j<size_in;++j)
                temp.data[i][j]*=coef;
        }
        return temp;
    }

    template<typename T>
    matrix<T>& matrix<T>::operator*=(T coef){
        for(int i=0;i<size_out;++i){
            for(int j=0;j<size_in;++j)
                this->data[i][j]*=coef;
        }
        return *this;
    }

    template<typename T>
    matrix<T> matrix<T>::operator-(matrix<T>& mat) const{
        return (*this + (mat.operator-()));
    }

    template<typename T>
    matrix<T> matrix<T>::operator+(const matrix<T>& mat) const{
        if(this->size_out!=mat.size_out||this->size_in!=mat.size_in)this->err_msg("operation + between two different matrix<T>, invalid operation, program stop");
        auto temp = *this;
        for(int i=0;i<size_out;++i){
            for(int j=0;j<size_in;++j)
                temp.data[i][j] += mat.data[i][j];
        }
        return temp;
    }

    template<typename T>
    matrix<T>  matrix<T>::operator*(const matrix<T>& mat) const{
        //check mxn * axb n==a?
        if(this->size_in!=mat.size_out)this->err_msg("operation * between two matrix<T> failed!");
        auto temp = matrix<T>(this->size_out,mat.size_in);
        for(int col=0;col<mat.size_in;++col){
            std::vector<T> late_vector(mat.size_out,0);
            for(int index=0;index<mat.size_out;++index)
                late_vector[index] = mat.data[index][col];
            for(int row=0;row<temp.size_out;++row){
                temp.data[row][col] = vector_mul(late_vector,this->data[row]);
            }
        }
        return temp;
    }

//----------------operation-between-matrix-------------<<<<<<

    template<typename T>
    matrix<T> matrix<T>::get_slim_echelon() const{
        if(empty())return matrix<T>();
        auto temp = this->get_echelon();
        while(1){
            if(zero_vector(temp.data.back())&&(!temp.data.empty()))temp.data.pop_back();
            else break;
        }
        temp.size();
#ifdef debug
        std::cout<<"slim echelon:"<<std::endl;
        std::cout<<temp<<std::endl;
#endif
        return temp;
    }

    template<typename T>
    int matrix<T>::rank() const{
        return this->get_slim_echelon().size();
    }

    template<typename T>
    std::vector<std::vector<T>> matrix<T>::get_inhomogeneous_solution() const{
        auto temp = this->get_slim_echelon();
        if(temp.data.back().back()!=0){
            auto vec = temp.data.back();
            vec.pop_back();
            if(zero_vector(vec))return {};
        }

        std::set<int> freevar;
        for(int i=0;i<size_in-1;++i)freevar.insert(i);
        int index = 0;
        for(int row=0;row<temp.size_out;++row){
            for(int col=index;col<temp.size_in-1;++col){
                if(temp.data[row][col]!=0){
                    index = col+1;
                    freevar.erase(freevar.find(col));
                    break;
                }
            }
        }
        MATRIX res;
        auto specificity = temp.get_by_col(size_in);
        specificity.resize(temp.size_in-1);
        res.push_back(specificity);
        for(auto v:freevar){
            auto vec = temp.get_by_col(v+1);
            vec.resize(temp.size_in-2);
            vec.insert(vec.begin()+v,-1);
            res.push_back(vec);
        }
        return res;
    }

//--------ensure this matrix<T> not empty-----------
    template<typename T>
    void matrix<T>::swap_row(int lhv,int rhv){
        if(lhv==rhv)return;
        std::swap(this->data[lhv],this->data[rhv]);
        return;
    }

    template<typename T>
    void matrix<T>::add2(int dest_row,T k,int row){
        for(int i=0;i<size_in;++i)
            this->data[dest_row][i] += k * this->data[row][i];
    }

    template<typename T>
    void matrix<T>::mul2(int dest,T k){
        for(int i=0;i<size_in;++i)
            this->data[dest][i] *= k;
    }
//--------ensure this matrix<T> not empty-----------

    template<typename T>
    bool matrix<T>::zero_vector(const std::vector<T> &vec)const{
        for(auto v:vec)if(v!=0)return false;
        return true;
    }

    template<typename T>
    void matrix<T>::to_echelon(){
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
                    T k = -this->data[row][col]/this->data[start][col];
                    add2(row,k,start);
#ifdef debug
        std::cout<<*this<<std::endl;
#endif
                }
                swap_row(start,col);
                for(int index=0;index<col;++index){
                    if(this->data[index][col]==0)continue;
                    T k = -this->data[index][col]/this->data[col][col];
                    add2(index,k,col);
#ifdef debug
        std::cout<<*this<<std::endl;
#endif
                }
                //data[col][col] must be no zero number
                mul2(col,1/this->data[col][col]);
            }
        //delete the tails
        while(tails>0){
            for(auto it = this->data.rbegin();it!=this->data.rend();++it){
                if(zero_vector(*it)){
                    this->data.erase(it.base()-1);
                    --tails;
                    break;
                }
            }
        }
        size();
    }


    // function to calculate the determinant of a square matrix
    template<typename T>
    T matrix<T>::laplace(const std::vector<std::vector<T>> &val)const {
        int n = val.size();
        T det = 0.0;
        if (n == 1) {
            return val[0][0];
        }
        else if (n == 2) {
            return (val[0][0] * val[1][1]) - (val[0][1] * val[1][0]);
        }
        else {
            for (int k = 0; k < n; k++) {
                std::vector<std::vector<T>> subval(n - 1, std::vector<T>(n - 1));

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


    template<typename T>
    void matrix<T>::transform(){
        MATRIX tmp(size_in,std::vector<T>(size_out));
        for(int i=0;i<size_out;++i){
            for(int j=0;j<size_in;++j){
                tmp[j][i] = data[i][j];
            }
        }
        int sizetmp = size_in;
        size_in = size_out;
        size_out = sizetmp;
        data = tmp;
    }

    //assign from bad vector,the shape depends on the first vector
    template<typename T>
    void matrix<T>::assign(const MATRIX &mat){
        size_out = mat.size();
        if(size_out==0){
            size_in = 0;
            return;
        }
        size_in = mat[0].size();
        this->data = std::vector<std::vector<T>>(size_out,std::vector<T>(size_in,0));
        for(int i=0;i<size_out;++i){
            int size = mat[i].size();
            int index_min = (size_in>size)?size:size_in;
            for(int j=0;j<index_min;++j){
                data[i][j] = mat[i][j];
            }
        }
        return;
    }

    template<typename T>
    matrix<T>::matrix(std::vector<std::vector<T>> val,int type){
        this->assign(val);
        if(type==COL)transform();
    }


    template<typename T>
    std::ostream& operator<<(std::ostream& os,const claris::matrix<T>& ma)
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
                T val = 0;
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

    template<typename T>
    claris::matrix<T> operator*(T coef, const claris::matrix<T>& mat){
        return mat * coef;
    }

}


#endif

