/*
#include<vector>
#include<iostream>

class adtData{
public:
    adtData()=default;
    virtual ~adtData(){}
    double val(){return this->value;}
    void setVal(double val){this->value = val;}
    virtual void calBy(const std::vector<double> &paras){if(!paras.empty())this->value = paras[0];}
private:
    double value = 0;
};

template< typename T >
class dataPoint:public adtData{
public:
    dataPoint()=default;
    dataPoint(double num){setVal(num);}
    static void setOrigin(double val){origin = val;}
    static double getOrigin(){return origin;}
    virtual void calBy(const std::vector<double> &paras);
    dataPoint& operator=(double val){
        setVal(val);
        return *this;
    }
    operator double(){return val();}
    void info(){typeinfo.info();}
private:
    T typeinfo;
    static double origin;
};

template<typename T>
double dataPoint<T>::origin = 0;


class Qn{
public:
    Qn()=default;
    void info(){
        std::cout<<"Qn"<<std::endl;
    }
};

class En{
public:
    En()=default;
    void info(){
        std::cout<<"En"<<std::endl;
    }
};

class Bn{
public:
    Bn()=default;
    void info(){
        std::cout<<"Bn"<<std::endl;
    }
};

class Wn{
public:
    Wn()=default;
    void info(){
        std::cout<<"Wn"<<std::endl;
    }
};


class Yn{
public:
    Yn()=default;
    void info(){
        std::cout<<"Yn"<<std::endl;
    }
};

template<>
void dataPoint<Qn>::calBy(const std::vector<double> &para){
    double ans = getOrigin() * para[0];
    setVal(ans);
}

template<>
void dataPoint<En>::calBy(const std::vector<double> &para){
    double ans = para[0] * para[1] / getOrigin();
    setVal(ans);
}

template<>
void dataPoint<Bn>::calBy(const std::vector<double> &para){
    double ans = getOrigin() * para[0] / para[1];
    setVal(ans);
}

template<>
void dataPoint<Wn>::calBy(const std::vector<double> &para){
    double ans = para[0] * para[1];
    setVal(ans);
}

class dataFrame{
public:
    dataFrame()=default;
    void setG(double val){Gamma = val;}
    void setQ(double val){Quantity = val;}
    void setC(double val){Condensity = val;}
    void setW(double val){Water = val;}
    void setE(double val){Epsilon = val;}
    void setB(double val){Beta = val;}

    void setGBy(const std::vector<double> &para){Gamma.calBy(para);}
    void setQBy(const std::vector<double> &para){Quantity.calBy(para);}
    void setCBy(const std::vector<double> &para){Condensity.calBy(para);}
    void setWBy(const std::vector<double> &para){Water.calBy(para);}
    void setEBy(const std::vector<double> &para){Epsilon.calBy(para);}
    void setBBy(const std::vector<double> &para){Beta.calBy(para);}

    double getG(double val){return Gamma;}
    double getQ(double val){return Quantity;}
    double getC(double val){return Condensity;}
    double getW(double val){return Water;}
    double getE(double val){return Epsilon;}
    double getB(double val){return Beta;}
private:
    dataPoint<Yn> Gamma;
    dataPoint<Qn> Quantity;
    dataPoint<Cn> Condensity;
    dataPoint<Wn> Water;
    std::vector<dataPoint<En>> Epsilon;
    std::vector<dataPoint<Bn>> Beta;
};

class flow_sheet{
public:
    using std::vector<std::pair<double>> = unit;
    flow_sheet(int roughSize,int cleanSize,int sweepSize){
        
    }
    void setVal(const unit &rough, const unit &clean, const unit &sweep);
    void setGrade(double val);
    void setEpsilon(double val=1);
    void setQuantity(double val);
    void setGamma(double val=1);
    void addOre(const unit &rough, const unit &clean, const unit &sweep);
    void addGrade(double val);
private:

    typedef struct{
    public:
        dataframe left;
        dataframe right;
    }flow_unit;

    std::vector<flow_unit> Rough;
    std::vector<flow_unit> Clean;
    std::vector<flow_unit> Sweep;
    std::vector<flow_unit> ConvergeRough;
    std::vector<flow_unit> ConvergeClean;
    std::vector<flow_unit> ConvergeSweep;
};



*/
