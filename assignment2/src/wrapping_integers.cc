#include "wrapping_integers.hh"

#include <iostream>
#include <utility>

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) { return isn + uint32_t(n); }

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t get_base(){
    uint64_t n = (UINT32_MAX);
    n+=1;
    return n;
}
size_t get_case(uint64_t base_checkpoint, uint64_t modulus_checkpoint, uint64_t n_mod){
    if(n_mod > modulus_checkpoint && base_checkpoint >= get_base() && (get_base() - n_mod + modulus_checkpoint) <= (n_mod - modulus_checkpoint)) return 1;
    if(n_mod > modulus_checkpoint && !(base_checkpoint >= get_base() && (get_base() - n_mod + modulus_checkpoint) <= (n_mod - modulus_checkpoint))) return 2;
    if(((modulus_checkpoint - n_mod) <= (n_mod + get_base() - modulus_checkpoint)) && n_mod<=modulus_checkpoint) return 2;
    return 10;


}


uint64_t get_n(pair<WrappingInt32, WrappingInt32> p){
    uint64_t n = static_cast<uint64_t>(p.first.raw_value()- p.second.raw_value());
    return n; 
}


uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    uint64_t n_mod = get_n({n, isn});
    uint64_t modulus_checkpoint = 0;
    uint64_t base_checkpoint = 0;
    if(checkpoint>=get_base() && checkpoint%get_base()==0) {
        modulus_checkpoint= get_base();
        base_checkpoint = checkpoint -get_base();
    }
    else{
        modulus_checkpoint = checkpoint%get_base();
        base_checkpoint = checkpoint - (checkpoint%get_base());
    }

    size_t cases= get_case(base_checkpoint, modulus_checkpoint, n_mod);

    uint64_t val = (base_checkpoint+n_mod);
    switch (cases)
    {
    case 1:
        return val-get_base();
        break;
    case 2:
        return val;
        break;

    
    default:
        return val+get_base();
    }

 return 0;
}
