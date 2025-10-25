#include "AppService.h"
#include "CsvFileStorage.h"
#include "TypeInfo.h"

int main()
{
    auto &info1 = getTypeInfo<Apartment>();
    auto &info2 = getTypeInfo<Admin>();
    auto &info3 = getTypeInfo<User>();

    /*storage->WriteAll(info2.filename, { "1,admin,admin" });

    std::cout << "Written to: " << info2.filename;*/
    return 0;
}
