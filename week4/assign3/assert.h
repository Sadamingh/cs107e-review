#ifndef ASSERT_H
#define ASSERT_H

#define assert(x) if(!(x)) { abort(); }
#define invalid_code_path assert(!"invalid code path")

#endif