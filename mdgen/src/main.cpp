#include"read_only.h"

class mdcontent{
    public:
        mdcontent(const std::string &name){
            file_name = name;
            work();
        }
    private:
        std::string file_name;
        std::vector<std::string> lines
        void handleInput();
        void handleOutput();
        void work(){
            claris::read_only temp(file_name);
            this->lines = temp.getlines();

        }
    friend class InputBase;
    friend class OutputBase;
};

class InputBase{
    public:
        InputBase(mdcontent& md){this->ptr = &md;}
        InputBase(mdcontent* md){this->ptr = md;}
        const std::vector<std::string>& getlines(){return ptr->lines;}
        void operator()(const std::vector <std::string>& val)=0;
        static std::vector<std::string> res;
    private:
        mdcontent *ptr=nullptr;
}

class OutputBase{
    public:
        OutputBase(mdcontent& md){this->ptr = &md;}
        OutputBase(mdcontent* md){this->ptr = md;}
        const std::vector<std::string>& getlines(){return ptr->lines;}
        void operator()(const std::vector <std::string>& val)=0;
        static std::vector<std::string> res;
    private:
        mdcontent *ptr=nullptr;
}
