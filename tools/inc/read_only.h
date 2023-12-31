#ifndef READ_ONLY
#define READ_ONLY
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<utility>


namespace claris{

    class comment{
    public:
        virtual std::string get_start()=0;
        virtual std::string get_end(){return std::string();}
        comment()=default;
        virtual ~comment(){}
    };

    class line_comment: public comment{
    public:
        line_comment(const std::string& mark):comment(){this->comment_mark = mark;}
        virtual std::string get_start() override {return comment_mark;}
    private:
        std::string comment_mark;
    };

    class block_comment: public comment{
    public:
        block_comment(const std::string& pre,const std::string& lat):comment(){
            this->comment_mark.first = pre;
            this->comment_mark.second = lat;
        }
        virtual std::string get_start() override {return comment_mark.first;}
        std::string get_end() override {return comment_mark.second;}
    private:
        std::pair<std::string,std::string> comment_mark;
    };


    class read_only{
    public:
        read_only()=delete;
        read_only(const read_only&)=delete;
        read_only(const std::string& file_name,const std::string& comment_mark="");
        bool reset(const std::string& file_name,const std::string& comment_mark="");
        virtual void trim();
        std::vector<std::string> get_trim_lines(){
            auto temp = single_lines;
            trim();
            auto res = this->single_lines;
            this->single_lines = temp;
            return res;
        }
        const std::vector<std::string>& getlines(){return this->single_lines;}
        bool add_comment_mark(const std::string& pre, const std::string& lat="");
        void rm_comment_mark(const std::string& pre);
        void rm_comment_marks(){
            while(!comment_marks.empty()){
                delete comment_marks.back();
                comment_marks.pop_back();
            }
        }
        void show_comment_marks();
        bool get_state(){return this->state;}
        std::string get_file_name(){return this->file_name;}
        virtual ~read_only(){this->rm_comment_marks();}
    protected:
        void set_state(bool val){this->state = val;}
        void set_file_name(const std::string& name){this->file_name = name;}
        std::vector<comment*> comment_marks;
        std::vector<std::string> single_lines;

    private:
        std::string file_name;
        bool state=false;
        bool open_file(const std::string &);
        friend std::ostream& operator<<(std::ostream& os,const read_only& ro);
    };

    std::ostream& operator<<(std::ostream& os,const read_only& ro);

}

#endif

/*
 * constructor:
 *          read_only(const string& file_name);
 *          read_only(const string& file_name, const string& single_comment_mark);
 * interface:
 *          add_comment_mark(const string&);
 *          add_comment_mark(const string&,const string&);     //for couple comment mark
 *          rm_comment_mark(const string&);
 *          rm_comment_marks();
 *
 */



