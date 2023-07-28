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
        if(v->get_end().empty())
            std::cout<<v->get_start()<<std::endl;
        else
            std::cout<<v->get_start()<<" & "<<v->get_end()<<std::endl;
    }
    std::cout<<"<<<<<COMMENTS<<<<<"<<std::endl;
}

//the new commenter is unique.
bool claris::read_only::add_comment_mark(const std::string& pre, const std::string& lat){
    if(pre.empty())return false;
    if(lat.empty()){
        for(auto &v:this->comment_marks){
            if(v->get_start()==pre)return false;
        }
        comment_marks.push_back(new line_comment(pre));
    }else{
        //if(pre==lat)return false;
        for(auto &v:this->comment_marks){
            if(v->get_start()==pre||v->get_start()==lat||v->get_end()==pre||v->get_end()==lat)return false;
        }
        comment_marks.push_back(new block_comment(pre,lat));
    }
    return true;
}

//By searching the same string, the commenter is unique.
/*Behavior:
 *  pass a single parameter:
 *      Only delete the line commenter matched.
 *  pass two parameteres:
 *      Only delete the block commenter matched.
*/
void claris::read_only::rm_comment_mark(const std::string& pre){
    if(pre.empty())return;
    for(int i=0;i<comment_marks.size();++i){
        if(comment_marks[i]->get_start()==pre){
            delete comment_marks[i];
            comment_marks.erase(comment_marks.begin()+i);
            break;
        }
    }
}

//delete empty line and comment line
void claris::read_only::trim(){
    bool end_flag = false;
    bool add_flag = true;
    std::vector<std::string> res;
    std::string search_str;
    for(auto &v:single_lines){
        if(v.empty())continue;
        if(end_flag){
            if(v.find(search_str)!=std::string::npos)end_flag = false;
            continue;
        }
        for(auto &p:comment_marks){
            search_str = p->get_start();
            if(v.find(search_str)!=std::string::npos){
                add_flag = false;
                if(!p->get_end().empty()){
                    end_flag = true;
                    search_str = p->get_end();
                }
                break;
            }
        }
        if(add_flag)res.push_back(v);
        add_flag = true;
    }
    this->single_lines = res;
}

