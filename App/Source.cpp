#include "AppService.h"
#include "TypeInfo.h"

int main()
{
    auto& info = getTypeInfo<Apartment>();

    std::cout << info.filename;
    return 0;
}