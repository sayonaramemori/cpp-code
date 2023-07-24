#include"read_only.h"

claris::read_only::read_only(const std::string& file_name,const std::string& comment_mark){
    this->reset(file_name,comment_mark);
}

bool claris::read_only::reset(const std::string& file_name,const std::string& comment_mark){
    this->file_name = file_name;
    if(open_file(file_name)){
        add_comment_mark(comment_mark);
    }
    return this->state;
}

bool claris::read_only::open_file(const std::string &name){
    std::ifstream ifs(name);
    this->single_lines.clear();
    if(this->state = ifs.is_open()){
        std::string temp;
        while(std::getline(ifs,temp))single_lines.push_back(temp);
    }
    return this->state;
}

std::ostream& claris::operator<<(std::ostream& os,const read_only& ro)
{
    for(auto &v:ro.single_lines)
    {
        os<<v<<std::endl;
    }
    return os;
}

void claris::read_only::show_comment_marks(){
    std::cout<<">>>>>COMMENTS>>>>>"<<std::endl;
    for(auto &v:comment_marks){
        if(v->get_type()==comment::LINE)
            std::cout<<v->get_start()<<std::endl;
        else
            std::cout<<v->get_start()<<" & "<<v->get_end()<<std::endl;
    }
    std::cout<<"<<<<<COMMENTS<<<<<"<<std::endl;
}

bool claris::read_only::add_comment_mark(const std::string& pre, const std::string& lat){
    if(pre.empty())return false;
    if(lat.empty()){
        for(auto &v:this->comment_marks){
            if(v->get_start()==pre)return false;
        }
        comment_marks.push_back(new line_comment(pre));
    }else{
        if(pre==lat)return false;
        for(auto &v:this->comment_marks){
            if(v->get_start()==pre||v->get_end()==lat)return false;
        }
        comment_marks.push_back(new block_comment(pre,lat));
    }
    return true;
}

void claris::read_only::rm_comment_mark(const std::string& pre, const std::string& lat){
    if(pre.empty())return;
    if(lat.empty()){
        for(int i=0;i<comment_marks.size();++i){
            if(comment_marks[i]->get_start()==pre){
                delete comment_marks[i];
                comment_marks.erase(comment_marks.begin()+i);
                break;
            }
        }
    }
    else{
        for(int i=0;i<comment_marks.size();++i){
            if(comment_marks[i]->get_start()==pre&&comment_marks[i]->get_end()==lat){
                delete comment_marks[i];
                comment_marks.erase(comment_marks.begin()+i);
                break;
            }
        }
    }
}


