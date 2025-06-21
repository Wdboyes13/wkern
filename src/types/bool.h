#pragma once

#ifdef true 
#undef true 
#endif 

#ifdef false 
#undef false 
#endif 

#ifdef bool 
#undef bool 
#endif

typedef enum {
    false = 0,
    true = 1
} bool;