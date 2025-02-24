//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#include "bluez-dbus-cpp/GattDescriptor1.h"
#include "bluez-dbus-cpp/GattCharacteristic1.h"


namespace org::bluez {
    static std::string makePath(const std::string &characteristicPath, const std::string &name) {
        std::string path{characteristicPath};
        path.append("/").append(name);
        return path;
    }

    GattDescriptor1::GattDescriptor1(const std::shared_ptr<GattCharacteristic1> &service,
                                     const std::string &name, std::string uuid)
        : AdaptorInterfaces{*service->getConnection(), makePath(service->getPath(), name)},
          path_{makePath(service->getPath(), name)},
          uuid_{std::move(uuid)},
          characteristic_{service} {
        registerAdaptor();
        characteristic_->addDescriptor(shared_from_this());
    }

    GattDescriptor1::~GattDescriptor1() {
        characteristic_->removeDescriptor(shared_from_this());
        unregisterAdaptor();
    }
}

