//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include "GattCharacteristic1.h"


namespace org::bluez {
    // Forward declarations
    class GattService1;

    using namespace sdbus;


    template<typename _Derived>
    class GattCharacteristicBuilder :
            public GattCharacteristic1 {
    protected:
        GattCharacteristicBuilder(const std::shared_ptr<GattService1> &service,
                                  const std::string &uuid,
                                  const bool hasAcquireWrite = false,
                                  const bool hasAcquireNotify = false,
                                  const bool hasValue = false,
                                  const bool valueIsDirected = false)
            : GattCharacteristic1{service, uuid, hasAcquireWrite, hasAcquireNotify, hasValue, valueIsDirected} {
        }

    public:
        _Derived &withValue(const std::string &value) {
            addValue(value);
            return static_cast<_Derived &>(*this);
        }

        _Derived &withValue(const std::vector<uint8_t> &value) {
            addValue(value);
            return static_cast<_Derived &>(*this);
        }

        _Derived &withFlag(const std::string &flag) {
            addFlag(flag);
            return static_cast<_Derived &>(*this);
        }

        std::shared_ptr<_Derived> finalize() {
            auto self = std::shared_ptr<GattCharacteristicBuilder>(this);
            registerWithService(self);
            return std::static_pointer_cast<_Derived>(self);
        }
    };
}
