//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <sdbus-c++/sdbus-c++.h>


namespace org::bluez {
    using namespace sdbus;

    class GattManager1 :
            public std::enable_shared_from_this<GattManager1> {
        std::unique_ptr<IProxy> proxy_;
        static constexpr auto INTERFACE_NAME = "org.bluez.GattManager1";

    public:
        GattManager1(IConnection &connection, std::string destination, std::string objectPath)
            : proxy_{createProxy(connection, std::move(destination), std::move(objectPath))} {
            proxy_->finishRegistration();
        }

        GattManager1(const std::shared_ptr<IConnection> &connection, std::string destination,
                     std::string objectPath)
            : proxy_{createProxy(*connection, std::move(destination), std::move(objectPath))} {
            proxy_->finishRegistration();
        }

        ~GattManager1() {
            proxy_->unregister();
        }

        void RegisterApplication(const ObjectPath &application,
                                 const std::map<std::string, Variant> &options) const {
            proxy_->callMethod("RegisterApplication").onInterface(INTERFACE_NAME).withArguments(application, options);
        }

        AsyncMethodInvoker RegisterApplicationAsync(const ObjectPath &application,
                                                    const std::map<std::string, Variant> &options) const {
            return proxy_->callMethodAsync("RegisterApplication").onInterface(INTERFACE_NAME).withArguments(
                application, options);
        }

        void UnregisterApplication(const ObjectPath &application) const {
            proxy_->callMethod("UnregisterApplication").onInterface(INTERFACE_NAME).withArguments(application);
        }
    };
}
