//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <sdbus-c++/sdbus-c++.h>

namespace org::bluez {
    using namespace sdbus;

    class LEAdvertisingManager1 :
            public std::enable_shared_from_this<LEAdvertisingManager1> {
        std::unique_ptr<IProxy> proxy_;
        static constexpr auto INTERFACE_NAME = "org.bluez.LEAdvertisingManager1";

    public:
        LEAdvertisingManager1(IConnection &connection, std::string destination, std::string objectPath)
            : proxy_{createProxy(connection, std::move(destination), std::move(objectPath))} {
            proxy_->finishRegistration();
        }

        LEAdvertisingManager1(const std::shared_ptr<IConnection> &connection, std::string destination,
                              std::string objectPath)
            : proxy_{createProxy(*connection, std::move(destination), std::move(objectPath))} {
            proxy_->finishRegistration();
        }

        ~LEAdvertisingManager1() {
            proxy_->unregister();
        }

        void RegisterAdvertisement(const ObjectPath &advertisementPath,
                                   const std::map<std::string, Variant> &options = {}) const {
            proxy_->callMethod("RegisterAdvertisement").onInterface(INTERFACE_NAME).withArguments(
                advertisementPath, options);
        }

        AsyncMethodInvoker RegisterAdvertisementAsync(const ObjectPath &advertisementPath,
                                                      const std::map<std::string, Variant> &options = {}) const {
            return proxy_->callMethodAsync("RegisterAdvertisement").onInterface(INTERFACE_NAME).withArguments(
                advertisementPath, options);
        }

        void UnregisterAdvertisement(const ObjectPath &advertisementPath) const {
            proxy_->callMethod("UnregisterAdvertisement").onInterface(INTERFACE_NAME).withArguments(advertisementPath);
        }

        uint8_t ActiveInstances() const {
            return proxy_->getProperty("ActiveInstances").onInterface(INTERFACE_NAME);
        }

        uint8_t SupportedInstances() const {
            return proxy_->getProperty("SupportedInstances").onInterface(INTERFACE_NAME);
        }

        std::vector<std::string> SupportedIncludes() const {
            return proxy_->getProperty("SupportedIncludes").onInterface(INTERFACE_NAME);
        }
    };
}
