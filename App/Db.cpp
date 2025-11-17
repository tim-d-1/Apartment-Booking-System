#include "Db.h"
#include "CsvFileStorage.h"

Db& Db::GetInstance()
{
    static Db instance(std::make_unique<CsvFileStorage>());
    return instance;
}
