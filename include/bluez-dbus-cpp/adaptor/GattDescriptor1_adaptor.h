
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef _sdbuscpp_include_bluez_dbus_cpp_adaptor_GattDescriptor1_adaptor_h_adaptor_H_
#define _sdbuscpp_include_bluez_dbus_cpp_adaptor_GattDescriptor1_adaptor_h_adaptor_H_

#include <sdbus-c++/sdbus-c++.h>
#include <string>


namespace org::bluez {
    class GattDescriptor1_adaptor {
    public:
        static constexpr auto INTERFACE_NAME = "org.bluez.GattDescriptor1";

    protected:
        explicit GattDescriptor1_adaptor(IObject &object)
            : object_(object) {
            object_.registerMethod("ReadValue").onInterface(INTERFACE_NAME).implementedAs(
                [this](const std::map<std::string, Variant> &options) { return this->ReadValue(options); });
            object_.registerMethod("WriteValue").onInterface(INTERFACE_NAME).implementedAs(
                [this](const std::vector<uint8_t> &value, const std::map<std::string, Variant> &options) {
                    return this->WriteValue(value, options);
                });
            object_.registerProperty("UUID").onInterface(INTERFACE_NAME).withGetter([this] { return this->UUID(); });
            object_.registerProperty("Characteristic").onInterface(INTERFACE_NAME).withGetter([this] {
                return this->Characteristic();
            });
            object_.registerProperty("Value").onInterface(INTERFACE_NAME).
                    withGetter([this] { return this->Value(); });
            object_.registerProperty("Flags").onInterface(INTERFACE_NAME).
                    withGetter([this] { return this->Flags(); });
        }

        ~GattDescriptor1_adaptor() = default;

    private:
        virtual std::vector<uint8_t> ReadValue(const std::map<std::string, Variant> &options) = 0;

        virtual void WriteValue(const std::vector<uint8_t> &value,
                                const std::map<std::string, Variant> &options) = 0;

        virtual std::string UUID() = 0;

        virtual ObjectPath Characteristic() = 0;

        virtual std::vector<uint8_t> Value() = 0;

        virtual std::vector<std::string> Flags() = 0;

        IObject &object_;
    };
} // namespaces

#endif
