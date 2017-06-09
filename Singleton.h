#ifndef SINGLETON_H
#define SINGLETON_H

class Singleton
{
public:
    static inline Singleton& GetInstance() 
    {
      static Singleton slt;
      return slt;
    }
private:
    Singleton();
    Singleton(const Singleton& other);
    Singleton& operator=(const Singleton& other);
};
#endif