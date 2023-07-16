#ifndef CLARIS_MATRIX
#define CLARIS_MATRIX

#include<iostream>
#include<cmath>
#include<vector>
#include<iomanip>
#include<algorithm>
#include<set>

namespace claris{

    class matrix{
    public:
        using MATRIX = std::vector<std::vector<double>>;
        enum {ROW,COL};

        matrix(){
            this->size_in=0;
            this->size_out=0;
        }
        matrix(int row,int col,double val=0){
            this->size_out = row;
            this->size_in = col;
            this->data = MATRIX(size_out,std::vector<double>(size_in,val));
        }
        matrix(std::vector<std::vector<double>>,int type=ROW);
        ~matrix()=default;

        matrix& operator=(const matrix&);
        matrix& operator*=(double);
        matrix operator-()const;
        matrix operator-(matrix&)const;
        matrix operator+(const matrix&)const;
        matrix operator*(const matrix&)const;
        matrix operator*(double)const;

        //row and col begin from 1
        std::vector<double> getByRow(int row)const{return this->data[row-1];}
        std::vector<double> getByCol(int col)const{
            auto res = std::vector<double>(size_out,0);
            for(int i=0;i<size_out;++i){
                res[i] = this->data[i][col-1];
            }
            return res;
        }

        bool empty() const{return this->data.empty();}
        matrix getEchelon() const{
            auto temp = *this;
            temp.toEchelon();
            return temp;
        }
        std::vector<std::vector<double>> getRoot() const;
        void transform();
        void toEchelon();
        int rank() const;
        int size(){
            this->size_out = data.size();
            return size_out;
        }

    private:
        void swap_row(int,int);
        void add2(int dest,double k,int row);
        void mul2(int dest,double k);
        matrix getSlimEchelon() const;

    private:
        bool zeroVector(const std::vector<double> &vec)const;
        double laplace(const std::vector<std::vector<double>> &val)const;
        void assign(const MATRIX&);
        double vectorMul(const std::vector<double> &lhv, const std::vector<double> &rhv)const{
            double res = 0;
            int size = lhv.size()>rhv.size()?rhv.size():lhv.size();
            for(int i=0;i<size;++i){
                res += lhv[i]*rhv[i];
            }
            return res;
        }
        void errMsg(const std::string& msg) const{
            std::cout<<msg<<std::endl;
            abort();
        }
    private:
        MATRIX data;
        size_t size_out;
        size_t size_in;

    friend std::ostream& operator<<(std::ostream& os,const matrix& ma);

    };

    std::ostream& operator<<(std::ostream& os,const matrix& ma);
    matrix operator*(double coef, const matrix&);

}

//This is a unsolved problem that put this function to globle scope
//I just make it in this way----put it inside claris, and use using.
using claris::operator<<;
using claris::operator*;

#endif

