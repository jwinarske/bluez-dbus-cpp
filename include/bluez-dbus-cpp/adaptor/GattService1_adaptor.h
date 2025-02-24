
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef _sdbuscpp_include_bluez_dbus_cpp_adaptor_GattService1_adaptor_h_adaptor_H_
#define _sdbuscpp_include_bluez_dbus_cpp_adaptor_GattService1_adaptor_h_adaptor_H_

#include <sdbus-c++/sdbus-c++.h>
#include <string>


namespace org::bluez {
    class GattService1_adaptor {
    public:
        static constexpr auto INTERFACE_NAME = "org.bluez.GattService1";

    protected:
        explicit GattService1_adaptor(IObject &object)
            : object_(object) {
            object_.registerProperty("UUID").onInterface(INTERFACE_NAME).withGetter([this] { return this->UUID(); });
            object_.registerProperty("Primary").onInterface(INTERFACE_NAME).withGetter([this] {
                return this->Primary();
            });
            object_.registerProperty("Includes").onInterface(INTERFACE_NAME).withGetter([this] {
                return this->Includes();
            });
        }

        ~GattService1_adaptor() = default;

    private:
        virtual std::string UUID() = 0;

        virtual bool Primary() = 0;

        virtual std::vector<ObjectPath> Includes() = 0;

        IObject &object_;
    };
} // namespaces

#endif
