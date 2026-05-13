#include <iostream>
#include "refinement.h"
#include "parameters.h"
#include "apta.h"
#include "input/inputdata.h"
#include "input/inputdatalocator.h"
#include "state_merger.h"

using namespace std;

refinement::refinement(){
    score = 0.0;
    red = nullptr;
    red_trace = nullptr;
    size = 0;
    refs = 0;

    time = 0;
}

merge_refinement::merge_refinement(state_merger* m, double s, apta_node* l, apta_node* r){
    red = l;
    red_trace = m->get_trace_from_state(l);
    red_trace->inc_refs();
    blue = r;
    blue_trace = m->get_trace_from_state(r);
    blue_trace->inc_refs();
    score = s;
    size = r->get_size();
    refs = 1;
    time = m->get_num_merges();
    if(CURRENT_CONFIG.RANDOMIZE_SCORES > 0.0) score = score - (score * CURRENT_CONFIG.RANDOMIZE_SCORES * random_double());
}

void merge_refinement::initialize(state_merger* m, double s, apta_node* l, apta_node* r){
    red = l;
    red_trace = m->get_trace_from_state(l);
    red_trace->inc_refs();
    blue = r;
    blue_trace = m->get_trace_from_state(r);
    blue_trace->inc_refs();
    score = s;
    size = r->get_size();
    refs = 1;
    time = m->get_num_merges();
    if(CURRENT_CONFIG.RANDOMIZE_SCORES > 0.0) score = score - (score * CURRENT_CONFIG.RANDOMIZE_SCORES * random_double());
}

split_refinement::split_refinement(state_merger* m, double s, apta_node* r, tail* t, int a){
    split_point = inputdata_locator::get()->access_tail(t);
    red = r;
    red_trace = m->get_trace_from_state(r);
    red_trace->inc_refs();
    score = s;
    attribute = a;
    size = r->get_size();
    refs = 1;
    time = m->get_num_merges();
    if(CURRENT_CONFIG.RANDOMIZE_SCORES > 0.0) score = score - (score * CURRENT_CONFIG.RANDOMIZE_SCORES * random_double());
}

void split_refinement::initialize(state_merger* m, double s, apta_node* r, tail* t, int a){
    split_point = inputdata_locator::get()->access_tail(t);
    red = r;
    red_trace = m->get_trace_from_state(r);
    red_trace->inc_refs();
    score = s;
    attribute = a;
    size = r->get_size();
    refs = 1;
    time = m->get_num_merges();
    if(CURRENT_CONFIG.RANDOMIZE_SCORES > 0.0) score = score - (score * CURRENT_CONFIG.RANDOMIZE_SCORES * random_double());
}

extend_refinement::extend_refinement(state_merger* m, apta_node* r){
    red = r;
    red_trace = m->get_trace_from_state(r);
    red_trace->inc_refs();
    score = CURRENT_CONFIG.EXTEND_SCORE;
    size = r->get_size();
    refs = 1;
    time = m->get_num_merges();
    if(CURRENT_CONFIG.RANDOMIZE_SCORES > 0.0) score = score - (score * CURRENT_CONFIG.RANDOMIZE_SCORES * random_double());
}

void extend_refinement::initialize(state_merger* m, apta_node* r){
    red = r;
    red_trace= m->get_trace_from_state(r);
    red_trace->inc_refs();
    score = 0.0;
    size = r->get_size();
    refs = 1;
    time = m->get_num_merges();
    if(CURRENT_CONFIG.RANDOMIZE_SCORES > 0.0) score = score - (score * CURRENT_CONFIG.RANDOMIZE_SCORES * random_double());
}

inline std::string refinement::to_string() const{
    return "score " + std::to_string(score);
};

inline void refinement::print_json(iostream& output) const{
    output << "\t\t[\n";
    output << "score " << score << endl;
    output << "\t\t]\n";
};

inline std::string refinement::to_short_string() const{
    return std::to_string(score);
};

inline void refinement::doref(state_merger* m){
};
	
inline void refinement::undo(state_merger* m){
};

inline bool refinement::testref(state_merger* m){
    return true;
};

inline void refinement::increfs(){
    ++refs;
};

inline void refinement::erase(){
};

inline std::string merge_refinement::to_string() const{
    if(STORE_ACCESS_STRINGS)
        return "merge( " + std::to_string(score) + " " + red_trace->to_string() + " " + blue_trace->to_string() + " )";
    else
        return "merge( " + std::to_string(score) + " " + std::to_string(red->get_number()) + " " + std::to_string(blue->get_number()) + " )";
};
	
inline std::string merge_refinement::to_short_string() const{
    return "m" + std::to_string(score);
};

inline void merge_refinement::print_json(iostream& output) const{
    output << "\t\t{\n";
    output << "\t\t\t\"type\" : \"merge\", " << endl;
    output << "\t\t\t\"red\" : " << red->get_number() << "," << endl;
    output << "\t\t\t\"blue\" : " << blue->get_number() << "," << endl;
    output << "\t\t\t\"score\" : " << score << endl;
    output << "\t\t}\n";
};

inline void merge_refinement::doref(state_merger* m){
    apta_node* left = red;
    apta_node* right = blue;
    if(STORE_ACCESS_STRINGS){
        left = m->get_state_from_trace(red_trace);
        right = m->get_state_from_trace(blue_trace);
    }
    if(!left->is_red()){
        /** this is a blueblue merge */
        m->extend(left);
        right->set_red(true);
    }
    m->perform_merge(left, right);
};
	
inline void merge_refinement::undo(state_merger* m){
    apta_node* left = red;
    apta_node* right = blue;
    if(STORE_ACCESS_STRINGS){
        left = m->get_state_from_trace(red_trace);
        right = m->get_state_from_trace(blue_trace);
    }
    m->undo_perform_merge(left, right);
    if(right->is_red()){
        /** this was a blueblue merge */
        right->set_red(false);
        m->undo_extend(left);
    }
};

inline bool merge_refinement::testref(state_merger* m){
    apta_node* left = red;
    apta_node* right = blue;
    if(STORE_ACCESS_STRINGS){
        left = m->get_state_from_trace(red_trace);
        right = m->get_state_from_trace(blue_trace);
    }
    if(left == right) return false;
    if((!left->is_red() && !left->get_source()->find()->is_red()) || right->is_red() || !right->get_source()->find()->is_red()) return false;
    if(left->rep() != 0 || right->rep() != 0) return false;
    refinement* ref = m->test_merge(left, right);
    if(ref != 0){
        score = ref->score;
        return true;
    }
    return false;
};

inline void merge_refinement::erase(){
    refs -= 1;
    if(refs == 0) mem_store::delete_merge_refinement(this);
};

inline std::string split_refinement::to_string() const{
    if(STORE_ACCESS_STRINGS)
        return "split( " + std::to_string(score) + " q:" + red_trace->to_string() + " s:" + split_point->to_string() + " a:" + std::to_string(attribute) + " )";
    else
        return "split( " + std::to_string(score) + " q:" + std::to_string(red->get_number()) + " s:" + split_point->to_string() + " a:" + std::to_string(attribute) + " )";
};
	
inline std::string split_refinement::to_short_string() const{
    return "s" + std::to_string(score);
};

inline void split_refinement::print_json(iostream& output) const{
    output << "\t\t{\n";
    output << "\t\t\t\"type\" : \"split\", " << endl;
    output << "\t\t\t\"red\" : " << red->get_number() << "," << endl;
    output << "\t\t\t\"point\" : " << split_point->to_string() << "," << endl;
    output << "\t\t\t\"attribute\" : " << attribute << "," << endl;
    output << "\t\t\t\"score\" : " << score << endl;
    output << "\t\t}\n";
};

inline void split_refinement::doref(state_merger* m){
    apta_node* right = red;
    if(STORE_ACCESS_STRINGS){
        right = m->get_state_from_trace(red_trace);
    }
    m->perform_split(right, split_point, attribute);
};
	
inline void split_refinement::undo(state_merger* m){
    apta_node* right = red;
    if(STORE_ACCESS_STRINGS){
        right = m->get_state_from_trace(red_trace);
    }
    m->undo_perform_split(right, split_point, attribute);
};

inline bool split_refinement::testref(state_merger* m){
    apta_node* right = red;
    if(STORE_ACCESS_STRINGS){
        right = m->get_state_from_trace(red_trace);
    }
    if(!right->is_red()) return false;
    if(right->guard(split_point) == 0) return false;
    if(right->guard(split_point)->get_target() == 0) return false;
    if(right->guard(split_point)->get_target()->rep() != 0) return false;
    if(right->guard(split_point)->get_target()->is_red()) return false;
    refinement* ref = m->test_split(right, split_point, attribute);
    if(ref != 0){
        score = ref->score;
        return true;
    }
    return false;
};

inline void split_refinement::erase(){
    refs -= 1;
    if(refs == 0) mem_store::delete_split_refinement(this);
};

inline std::string extend_refinement::to_string() const{
    if(STORE_ACCESS_STRINGS)
        return "extend( " + std::to_string(score) + " " + red_trace->to_string() + " )";
    else
        return "extend( " + std::to_string(score) + " " + std::to_string(red->get_number()) + " )";
};
	
inline std::string extend_refinement::to_short_string() const{
    return "x" + std::to_string(size);
};

inline void extend_refinement::print_json(iostream& output) const{
    output << "\t\t{\n";
    output << "\t\t\t\"type\" : \"extend\", " << endl;
    output << "\t\t\t\"red\" : " << red->get_number() << "," << endl;
    output << "\t\t\t\"score\" : " << score << endl;
    output << "\t\t}\n";
};

inline void extend_refinement::doref(state_merger* m){
    apta_node* right = red;
    if(STORE_ACCESS_STRINGS){
        right = m->get_state_from_trace(red_trace);
    }
    m->extend(right);
};
	
inline void extend_refinement::undo(state_merger* m){
    apta_node* right = red;
    if(STORE_ACCESS_STRINGS){
        right = m->get_state_from_trace(red_trace);
    }
    m->undo_extend(right);
};

inline bool extend_refinement::testref(state_merger* m){
    apta_node* right = red;
    if(STORE_ACCESS_STRINGS){
        right = m->get_state_from_trace(red_trace);
    }
    if(right->is_red() || !right->get_source()->find()->is_red()) return false;
    if(right->rep() != 0) return false;
    return true;
};

inline void extend_refinement::erase(){
    refs -= 1;
    if(refs == 0) mem_store::delete_extend_refinement(this);
};

void refinement::print_refinement_list_json(iostream& output, refinement_list* list){
    output << "[\n";

    refinement_list::iterator it = list->begin();
    while(it != list->end()){
        (*it)->print_json(output);
        it++;
        if(it != list->end())
            output << ",\n";
    }

    output << "]\n";
};

int refinement::type() const {
    return 0;
}
int split_refinement::type() const{
    return 1;
}
int merge_refinement::type() const{
    return 2;
}
int extend_refinement::type() const{
    return 3;
}

inline int refinement::get_time(){
    return time;
}

