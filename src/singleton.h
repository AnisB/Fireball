#ifndef SINGLETON
#define SINGLETON

// Include système 
#include <iostream>

// Classe de singleton
template <typename T>
  class Singleton
  {
  protected:
    Singleton () { }
    ~Singleton () { }

  public:
  // Interface publique
    static T *Instance_Ptr ()
    {
      if (FInstance == NULL)
      {
        FInstance = new T;
      }

      return (static_cast<T*> (FInstance));
    }
    static T &Instance ()
    {
      if (NULL == FInstance)
      {
        FInstance = new T;
      }

      return *(static_cast<T*> (FInstance));
    }

    static void Destroy ()
    {
      if (NULL != FInstance)
      {
        delete FInstance;
        FInstance = NULL;
      }
    }

  private:
  // Unique instance
    static T *FInstance;
  };

template <typename T>
  T *Singleton<T>::FInstance = NULL;

#endif //SINGLETON