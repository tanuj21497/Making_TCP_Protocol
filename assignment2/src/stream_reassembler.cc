#include "stream_reassembler.hh"

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}
#include <bits/stdc++.h>
#include <iostream>
#include <map>
#include <numeric>
#include <limits>
using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    :_output(capacity),  eof_index(numeric_limits<size_t>::max())
{
    wait_index =0;
    cap = capacity;
}


//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    size_t st =0;
    size_t n =data.size();
    
    if (eof) {
        eof_index = index + n;
    }

    if(wait_index>index) {

        st = wait_index-index;
    }

    bool b1 = assm_map.empty();
    bool more_cap = (n<=st);
    if(wait_index>=eof_index &&b1 && more_cap) _output.end_input();
    if(more_cap) return;

    if(wait_index-st==index){
        string write_data = data.substr(st);
        
        size_t num= min(min(write_data.size(), cap + _output.bytes_read() - index), cap - _output.buffer_size() - unassembled_bytes());
        string w_data = write_data.substr(0, num);
        wait_index = wait_index + _output.write(write_data);

        writeAndUpdate();
        if(assm_map.empty() && wait_index==eof_index) _output.end_input();
        return;

    }
    else if(index>wait_index-st){
        string write_data = data.substr(st);
        insertion_c(write_data, index);



        if(assm_map.empty() && wait_index==eof_index) _output.end_input();
        return;   
    }

    if(assm_map.empty() && wait_index==eof_index) _output.end_input();







}

void StreamReassembler::writeAndUpdate(){
    update();
    while(assm_map.find(wait_index)!=assm_map.end()){
        auto it = assm_map.find(wait_index);
        int w = wait_index;
        wait_index = wait_index+_output.write(it->second);
        assm_map.erase(assm_map.find(w));
        update();
    }


}

void StreamReassembler::update(){
    auto it = assm_map.begin();

    while (it != assm_map.end()) {
        
        string data = it->second;
        if(it->first >= wait_index) {
            ++it;
            continue;
        }  
        if(it->first < wait_index) {
            auto iter = assm_map.erase(it);
            size_t n = data.size();
            if (it->first + n> wait_index) {
                data = data.substr(wait_index - it->first);
                insertion_c(data, wait_index);
            }
            it = iter;
            continue;
        } 
    }
        
}

size_t StreamReassembler::get_c(size_t st_insertion, size_t end_insertion,size_t end_particular,size_t st_particular){
    if(st_insertion <= end_particular && st_particular <= end_insertion && st_insertion <= st_particular && end_insertion >= end_particular) return 1;
    if(st_insertion <= end_particular && st_particular <= end_insertion && st_particular <= st_insertion && end_particular >= end_insertion) return 2;
    if(st_insertion <= end_particular && st_particular <= end_insertion && st_insertion <= st_particular) return 3;
    if(st_insertion <= end_particular && st_particular <= end_insertion) return 4;
    return 5;





}
void StreamReassembler::insertion_c(const string &data, uint64_t index) {
    string data_insertion = data;
  
    bool b = data_insertion.empty();
    auto it = assm_map.begin(); 
    size_t st_insertion = index;
    size_t end_insertion = index + data.size();
    end_insertion-=1;
    while(it != assm_map.end()) {
        string data_particular = it->second;
        size_t st_particular = it->first;
        size_t end_particular = it->first + (it->second).size() ;
        end_particular-=1;

        size_t y = get_c(st_insertion, end_insertion,end_particular,st_particular);
        string sr ="";
        switch (y)
        {
        case 1:
            /* code */
            it = assm_map.erase(it);
            break;
        case 2:
            data_insertion.clear();
            ++it;
            break;
        case 3:
            sr= data_particular.substr(end_insertion + 1 - st_particular, end_particular - end_insertion);
            data_insertion= data_insertion+sr;
            it = assm_map.erase(it);
            break;
        case 4:
            index = st_particular;
            data_insertion.insert(0, data_particular.substr(0, st_insertion - st_particular));
            it = assm_map.erase(it);
            break;
        
        default:
            it++;
            break;
        }


    }
    size_t mini1 = min(data_insertion.size(), cap + _output.bytes_read() - index);
    mini1 = min(mini1, cap - _output.buffer_size() - unassembled_bytes());


    if (!data_insertion.empty()) {
        assm_map.insert({index,data_insertion.substr(0, mini1)});

    }
}

size_t StreamReassembler::unassembled_bytes() const { 
    size_t totalSize = accumulate(
        assm_map.begin(),
        assm_map.end(),
        0,
        [](size_t accumulator, const std::pair<int, std::string>& pair) {
            return accumulator + pair.second.length();
        }
    );
    return totalSize;
    
}


bool StreamReassembler::empty() const { 
    if(assm_map.size()==0) return true;
    return false; 
}

size_t StreamReassembler::ack_index() const { 

    return wait_index; }