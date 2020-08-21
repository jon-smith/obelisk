#include <obelisk/StdThreadSupportWrappers.h>

#include <iostream>

int main()
{
    obelisk::Mutex m;

    obelisk::UniqueLock<obelisk::Mutex> lk(m);

    std::cout << "locked";

    return 0;
}