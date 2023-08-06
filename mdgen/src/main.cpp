#include"read_only.h"
#include<string>
#include<iostream>
#define DEBUG

#ifdef DEBUG
void print(const std::string &msg, const std::vector<std::string> &val){
    std::cout<<msg<<std::endl;
    for(const auto &v:val){
        std::cout<<v<<std::endl;
    }
    return;
}
#endif

//only one instantiation
class mdcontent{
    friend class OperationBase;
    public:
        mdcontent(const std::string &name){
            if(--one_instantiation<0){
                std::cerr<<"Two instantiation forbiden, program stop"<<std::endl;
                exit(-1);
            }
            file_name = name;
            output_name += file_name;
            pre_work();
        }
        ~mdcontent(){if(this->read_ptr!=nullptr)delete read_ptr;}
//        void work(std::vector<std::pair<ContentBase*,std::string>>,std::vector<std::pair<OutputBase*,std::string>>){}
        //call after instantiation
        static std::vector<std::string> get_trim_lines(){if(read_ptr)return read_ptr->get_trim_lines();else return {};}
        static std::vector<std::string> get_lines(){if(read_ptr)return read_ptr->getlines();else return {};}
        static std::string get_file_name(){return file_name;}
        static std::string get_content_name(){return content_name;}
        static std::string get_up_mark(){return up_mark;}
        static std::string get_output_name(){return output_name;}
    private:
        static int one_instantiation;
        static claris::read_only *read_ptr;
        static std::string file_name;
        static std::string output_name;
        static std::string content_name;
        static std::string up_mark;
    private:
        //void handleInput(std::vector<ContentBase*>);
        //void handleOutput(std::vector<OutputBase*>);
        void pre_work(){
            read_ptr = new claris::read_only(file_name);
            read_ptr->add_comment_mark("```","```");
            return;
        }
};

int mdcontent::one_instantiation = 1;
claris::read_only* mdcontent::read_ptr=nullptr;
std::string mdcontent::file_name="";
std::string mdcontent::content_name="Contents";
std::string mdcontent::up_mark="^";
std::string mdcontent::output_name="copyVersion_";

class OperationBase{
    public:
        virtual ~OperationBase(){}
        virtual std::vector<std::string> operator()(const std::vector<std::string> &val = {})=0;
    protected:
        static std::vector<std::string> get_trim_lines(){return mdcontent::get_trim_lines();}
        static std::vector<std::string> get_lines(){return mdcontent::get_lines();}
};

class ContentBase:public OperationBase{
    friend class OutputBase;
    public:
        virtual ~ContentBase(){}
        virtual std::vector<std::string> operator()(const std::vector<std::string> &val = {})=0;
    protected:
        std::string get_single_content(const std::string &headline);

};
std::string ContentBase::get_single_content(const std::string &headline){
    auto it = headline.find_first_of(" ");
    if(it==std::string::npos)return std::string();
    std::string description = headline.substr(it+1);
    std::string anchor_name = description;
    std::string res;
    res += std::string("[");
    res += description;
    res += std::string("]");
    res += std::string("(#");
    res += anchor_name;
    res += std::string(")");
    return res;
}


//Input:NULL or all trimed lines, for range search
//Output:All headlines in a vector
class SearchHeadline:public ContentBase{
    public:
        virtual std::vector<std::string> operator()(const std::vector<std::string> &trim_lines={}){
            std::vector<std::string> res;
            if(trim_lines.empty())
                res = get_headline(get_trim_lines());
            else
                res = get_headline(trim_lines);
#ifdef DEBUG
            print("All searching headlines:",res);
#endif
            return res;
        }
    private:
        std::vector<std::string> get_headline(const std::vector<std::string> &val){
            std::vector<std::string> res;
            for(const auto &v:val){
                if(v.empty())continue;
                if(v[0]=='#')res.push_back(v);
            }
            return res;
        }
};


//Input:All headlines in a vector
//Output:A string int index of headline in all lines -- the first element in the result.
class SearchHeadlineIndex:public ContentBase{
    public:
        virtual std::vector<std::string> operator()(const std::vector<std::string> &head_lines={}){
            auto res = get_index(get_lines(),head_lines);
#ifdef DEBUG
            print("All headlines index:",{res});
#endif
            return {res};
        }
        static std::vector<int> stoi(const std::string &str){
            std::vector<int> res;
            int begin = 0;
            for(int i=0;i<str.size();++i){
                if(str[i]==' '){
                    continue;
                }else{
                    begin = i;
                    ++i;
                    while(str[i]!=' '&&i<str.size())++i;
                    res.push_back(std::stoi(str.substr(begin,i-begin)));
                }
            }
            return res;
        }
    private:
        std::string get_index(const std::vector<std::string> &all_lines,const std::vector<std::string> &head_lines){
            if(head_lines.empty())return "";
            std::string res;
            int index = 0;
            for(int i=0;i<all_lines.size();++i){
                if(head_lines[index]==all_lines[i]){
                    res += std::to_string(i) + " ";
                    ++index;
                }
            }
            return res;
        }
};

//Input:All headlines
//Output:Rough contents
class SimplyGenerateContent:public ContentBase{
    public:
        virtual std::vector<std::string> operator()(const std::vector<std::string> &head_lines={}){
            std::vector<std::string> res;
            for(const auto &v:head_lines){
                auto temp = this->get_single_content(v);
                res.push_back(temp);
            }
#ifdef DEBUG
//            print("Rough Contents:",res);
#endif
            return res;
        }
};

class BodyBase : public OperationBase{
    friend class OutputBase;
    public:
        virtual ~BodyBase(){}
        virtual std::vector<std::string> operator()(const std::vector<std::string> &val={}){return get_lines();}
};

class OutputBase:public OperationBase{
    public:
        virtual std::vector<std::string> operator()(const std::vector<std::string> &val={})=0;
};

//Input:Null
//Output:Null
//
std::vector<std::string> Converge(const std::vector<std::string> &content={},const std::vector<std::string> &body={}){
    std::vector<std::string> res;
    res.push_back("### " + mdcontent::get_content_name());
    res.push_back("----");
    res.insert(res.end(),content.begin(),content.end());
    res.push_back("----");
    res.insert(res.end(),body.begin(),body.end());
    return res;
}

//Input:the last res
//Output:Null
class WriteTo:public OutputBase{
    public:
        virtual std::vector<std::string> operator()(const std::vector<std::string> &val={}){
            this->write(mdcontent::get_output_name(),val);
            return{};
        }
    private:
        void write(const std::string &file_name, const std::vector<std::string> &val={}){
            std::ofstream ofs(file_name);
            if(ofs.is_open()){
                std::string line;
                for(const auto &v:val){
                    line = v + "\n";
                    ofs<<line;
                }
                std::cout<<"Successfully write to "<<file_name<<"!"<<std::endl;
            }else 
                std::cout<<"write failed"<<std::endl;
        }
};


int main(int argc,char *argv[]){
    if(argc==1){
        std::cout<<"please offer a file"<<std::endl;
        exit(-1);
    }
    mdcontent test(argv[1]);
    auto headlines = SearchHeadline()();
    auto content = SimplyGenerateContent()(headlines);
    auto body = BodyBase()();
    auto res = Converge(content,body);
    print("res:",res);
    WriteTo()();
    return 0;
}
