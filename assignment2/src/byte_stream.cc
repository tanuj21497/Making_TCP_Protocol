#include "byte_stream.hh"

#include <algorithm>
#include<deque>
#include<string.h>
// You will need to add private members to the class declaration in `byte_stream.hh`

/* Replace all the dummy definitions inside the methods in this file. */


using namespace std;

ByteStream::ByteStream(const size_t capa)
{
  // deque<char> d;
  n =capa;
  t_read =0;
  t_write =0;
  ending=false;
  

}

size_t ByteStream::write(const string &data) {
  if(_error){
    return 0;
  }
  if(input_ended()){
    return 0;
  }
  int i=0;
  for(char a: data){
    if(n>d.size()){

      d.push_back(a);
      i++;
    }
  }

  t_write+=i;
  
  return i;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
  if(_error){
    return "";
  }
  string s="";
  int l = min(buffer_size(),len);
  
  int i=0;
  while(l--){
    s+= d.at(i);
    i++;
  }
  return s;
  // return {};
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
  if(_error){
    return;
  }
  int l = min(buffer_size(),len);
  
  while(l--){
    t_read++;
    d.pop_front();
  }
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
  if(_error){
    return "";
  }
  string s="";
  int l = min(buffer_size(),len);
  while(l--){

    

    s+= d.at(0);
    d.pop_front();
  }
  t_read+=s.size();

  return s;


  // return {};
}

void ByteStream::end_input() { ending=true;}

bool ByteStream::input_ended() const {
  if(ending){
    return true;
  }
   return false;}

size_t ByteStream::buffer_size() const {return d.size(); }

bool ByteStream::buffer_empty() const {
  if (buffer_size())
  {
    /* code */
    return false;
  }
  
  return true; }

bool ByteStream::eof() const { 
  if(buffer_empty() && input_ended()){
    return true;
  }
  return false; }

size_t ByteStream::bytes_written() const { return t_write; }

size_t ByteStream::bytes_read() const { return t_read;  }

size_t ByteStream::remaining_capacity() const { return n-d.size(); }
