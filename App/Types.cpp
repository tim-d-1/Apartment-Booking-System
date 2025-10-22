#include "Db.h"
#include "Types.h"

template<>
TypeInfo<User>& getTypeInfo<User>() {
    auto& db = Db::GetInstance();
    static TypeInfo<User> info{
        db.GetUsers(),
        "Data/Users.csv",
        db.GetLastUserId()
    };
    return info;
}

template<>
TypeInfo<Admin>& getTypeInfo<Admin>() {
    auto& db = Db::GetInstance();
    static TypeInfo<Admin> info{
        db.GetAdmins(),
        "Data/Admins.csv",
        db.GetLastAdminId()
    };
    return info;
}

template<>
TypeInfo<Apartment>& getTypeInfo<Apartment>() {
    auto& db = Db::GetInstance();
    static TypeInfo<Apartment> info{
        db.GetApartments(),
        "Data/Apartments.csv",
        db.GetLastApartmentId()
    };
    return info;
}