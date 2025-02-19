//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#ifndef _sdbuscpp_src_bluez_adaptor_ObjectManagerExt_adaptor_h_adaptor_H_
#define _sdbuscpp_src_bluez_adaptor_ObjectManagerExt_adaptor_h_adaptor_H_

#include <sdbus-c++/sdbus-c++.h>
#include <string>


namespace org::bluez {
    class ObjectManagerExt_adaptor : public ObjectManager_adaptor {
    public:
        static constexpr auto INTERFACE_NAME = "org.freedesktop.DBus.ObjectManager";

    protected:
        explicit ObjectManagerExt_adaptor(IObject &object)
            : ObjectManager_adaptor(object) {
            object.registerMethod("GetManagedObjects").onInterface(INTERFACE_NAME).implementedAs([this] {
                return this->GetManagedObjects();
            });
        }

        ~ObjectManagerExt_adaptor() = default;

    private:
        virtual std::map<ObjectPath, std::map<std::string, std::map<std::string, Variant> > >
        GetManagedObjects() = 0;
    };
} // namespaces

#endif
