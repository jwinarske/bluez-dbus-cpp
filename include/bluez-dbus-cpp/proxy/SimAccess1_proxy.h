
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef _sdbuscpp_include_bluez_dbus_cpp_proxy_SimAccess1_proxy_h_proxy_H_
#define _sdbuscpp_include_bluez_dbus_cpp_proxy_SimAccess1_proxy_h_proxy_H_

#include <sdbus-c++/sdbus-c++.h>
#include <string>

namespace org::bluez {
    class SimAccess1_proxy {
    public:
        static constexpr auto INTERFACE_NAME = "org.bluez.SimAccess1";

    protected:
        explicit SimAccess1_proxy(sdbus::IProxy &proxy)
            : proxy_(proxy) {
        }

        ~SimAccess1_proxy() = default;

    public:
        void Disconnect() const {
            proxy_.callMethod("Disconnect").onInterface(INTERFACE_NAME);
        }

        [[nodiscard]] bool Connected() const {
            return proxy_.getProperty("Connected").onInterface(INTERFACE_NAME);
        }

    private:
        sdbus::IProxy &proxy_;
    };
}

#endif
