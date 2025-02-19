//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include "GattCharacteristicBuilder.h"


namespace org::bluez {
    class ReadOnlyCharacteristic final :
            public GattCharacteristicBuilder<ReadOnlyCharacteristic> {
    public:
        ReadOnlyCharacteristic(const std::shared_ptr<GattService1> &service, const std::string &uuid,
                               const std::string &value)
            : GattCharacteristicBuilder{service, uuid, false, false, true} {
            addFlag("read");
            addValue(value);
        }

        static std::shared_ptr<ReadOnlyCharacteristic> createFinal(const std::shared_ptr<GattService1> &service,
                                                                   const std::string &uuid, const std::string &value) {
            const auto self = new ReadOnlyCharacteristic{service, uuid, value};
            auto shared = std::shared_ptr<ReadOnlyCharacteristic>(self);
            self->registerWithService(shared);
            return shared;
        }
    };
}
