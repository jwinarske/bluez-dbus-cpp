//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include "GattCharacteristicBuilder.h"


namespace org::bluez {
    class GenericCharacteristic final :
            public GattCharacteristicBuilder<GenericCharacteristic> {
    public:
        GenericCharacteristic(const std::shared_ptr<GattService1> &service, const std::string &uuid)
            : GattCharacteristicBuilder{service, uuid, false, false, true} {
        }

        static GenericCharacteristic &create(const std::shared_ptr<GattService1> &service, const std::string &uuid) {
            const auto self = new GenericCharacteristic{service, uuid};
            return *self;
        }
    };
}
