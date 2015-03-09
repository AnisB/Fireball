#ifndef COMMON
#define COMMON

#define for_each_macro(IT,X) for ( decltype( X.begin() ) IT = X.begin(); IT != X.end(); ++IT )
#define tryget(IT,LIST, ELEM) decltype( LIST.begin()) IT = LIST.find(ELEM);
#define tryget_orthrow(IT,LIST, ELEM) decltype( LIST.begin()) IT = LIST.find(ELEM);

#endif // COMMON