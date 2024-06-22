#include "tcp_receiver.hh"

#include <algorithm>


using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    const TCPHeader head = seg.header();
    
    string stream_data = seg.payload().copy();
    
    if(_synReceived==false){
        if(head.syn==false) return;

        _synReceived=true;
        _isn = head.seqno;
    }
    

    // ...

    // --- Hint ------
        // convert the seqno into absolute seqno
    uint64_t checkpoint = _reassembler.ack_index();
    uint64_t abs_seqno = unwrap(head.seqno, _isn, checkpoint);
    uint64_t stream_idx = abs_seqno - _synReceived;
    // --- Hint ------  
    if(stream_data.empty()==false){
        if(head.syn) _reassembler.push_substring(stream_data,stream_idx, head.fin) ;
        if(head.seqno!=_isn) _reassembler.push_substring(stream_data,stream_idx, head.fin) ;

    }

    // ... 
    if(head.fin==true) _finReceived = true;
    size_t byt = _reassembler.unassembled_bytes();
    
    if(_finReceived==true){
        
        if(byt==0){
            _reassembler.stream_out().end_input();

        }
    }
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    bool b = _reassembler.stream_out().input_ended();
    uint64_t index = _reassembler.ack_index();

    if (_synReceived && b){
        optional<WrappingInt32> ans = {};
        ans.emplace(wrap(index+2, _isn));
        return ans;
    }
    if(_synReceived){
        optional<WrappingInt32> ans = {};
        ans.emplace(wrap(index+1, _isn));
        return ans;
    }
       

    return {};
}

size_t TCPReceiver::window_size() const { 
    auto a = _reassembler.stream_out();
    size_t y = a.buffer_size();
    return _capacity-y;
}