#ifndef PROJECT_EMIRIS_UTILS_H
#define PROJECT_EMIRIS_UTILS_H


//Uncommenting the line below disables ASSERT
//#define ASSERT

#ifndef ASSERT
#ifndef NDEBUG
#define ASSERT(...) assert(__VA_ARGS__)
#else
#define
#endif // NDEBUG
#endif // STATIC_ASSERT

#endif //PROJECT_EMIRIS_UTILS_H