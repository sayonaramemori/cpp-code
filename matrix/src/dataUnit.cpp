#include<vector>
#include<iostream>

/**************
 * General: 
 *      val()
 *      calBy(vector)
 *      setOrigin()
 *      getOrigin()
 *
 * ************
 * Beta:
 *
 * ************
 */
class adtData
{
public:
    adtData()=default;
    adtData(double init){this->value = init;}
    virtual void calBy(std::vector<double>){};
    double val(){return this->value;}
    void setVal(double val){this->value = val;}
private:
    double value=0;
};


class Beta : public adtData
{
public:
    Beta()=default;
    Beta(double init):adtData(init){}
    virtual void calBy(std::vector<double>) override;
    void setOrigin(double val){origin = val;}
    double getOrigin(){return origin;}
private:
    static double origin;
};

double Beta::origin = 0;

void Beta::calBy(std::vector<double> para)
{
    double ans = getOrigin() * para[0] / para[1];
    setVal(ans);
    return;
}

class Qn : public adtData
{
public:
    Qn()=default;
    Qn(double init):adtData(init){}
    virtual void calBy(std::vector<double>) override;
    void setOrigin(double val){origin = val;}
    double getOrigin(){return origin;}
private:
    static double origin;
};

double Qn::origin = 0;

void Qn::calBy(std::vector<double> para)
{
    double ans = getOrigin() * para[0];
    setVal(ans);
    return;
}

class En : public adtData
{
public:
    En()=default;
    En(double init):adtData(init){}
    virtual void calBy(std::vector<double>) override;
    void setOrigin(double val){origin = val;}
    double getOrigin(){return origin;}
private:
    static double origin;
};

double En::origin = 0;

void En::calBy(std::vector<double> para)
{
    double ans = getOrigin() * para[0];
    setVal(ans);
    return;
}


class Yn : public adtData
{
public:
    Yn()=default;
    Yn(double init):adtData(init){}
    void calBy(std::vector<double> val){if(!val.empty())setVal(val[0]);}
    void setOrigin(double val){origin = val;}
    double getOrigin(){return origin;}
private:
    static double origin;
};

double Yn::origin = 1;


class Cn : public adtData
{
public:
    Cn()=default;
    Cn(double init):adtData(init){}
    virtual void calBy(std::vector<double>) override;
};

void Cn::calBy(std::vector<double> para)
{
    double bottom = 1 + para[0]/para[1];
    double ans = 100.0/bottom;
    setVal(ans);
    return;
}


class Wn : public adtData
{
public:
    Wn()=default;
    Wn(double init):adtData(init){}
    virtual void calBy(std::vector<double>) override;
};

void Wn::calBy(std::vector<double> para)
{
    double ans = para[0] * para[1];
    setVal(ans);
    return;
}


class dataPoint
{
public:
    Yn y;
    En e;
    Beta b;
}


int main(){
    Qn Q1(1);
    Q1.setOrigin(1);
    En E1(1);
    E1.setOrigin(2);
    std::cout<<E1.getOrigin()<<std::endl;
    std::cout<<Q1.getOrigin()<<std::endl;
    return 0;
}
