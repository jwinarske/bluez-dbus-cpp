//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include "adaptor/PropertiesExt_adaptor.h"
#include "adaptor/GattDescriptor1_adaptor.h"

#include <sdbus-c++/sdbus-c++.h>

namespace org::bluez {
    // Forward declarations
    class GattCharacteristic1;

    using namespace sdbus;

    class GattDescriptor1 :
            public AdaptorInterfaces<GattDescriptor1_adaptor, PropertiesExt_adaptor>,
            public std::enable_shared_from_this<GattDescriptor1> {
    public:
        GattDescriptor1(const std::shared_ptr<GattCharacteristic1> &service, const std::string &name, std::string uuid);

        ~GattDescriptor1() override;

        const std::string &getPath() const {
            return path_;
        }

        std::vector<uint8_t> ReadValue(const std::map<std::string, Variant> &options) override {
            throw Error("org.bluez.Error.NotSupported", "Method 'ReadValue' default handler");
        }

        void WriteValue(const std::vector<uint8_t> &value,
                        const std::map<std::string, Variant> &options) override {
            throw Error("org.bluez.Error.NotSupported", "Method 'WriteValue' default handler");
        }

        /**
     * @brief
     *
     * @return std::string
     */
        std::string UUID() override {
            return uuid_;
        }

        ObjectPath Characteristic() override {
            // TODO implement
            return {};
        }

        std::vector<uint8_t> Value() override {
            // TODO implement
            return {};
        }

        std::vector<std::string> Flags() override {
            // TODO implement
            return {};
        }

    protected:
        void emitPropertyChangedSignal(const std::string &property) {
            PropertiesExt_adaptor::emitPropertyChangedSignal(GattDescriptor1_adaptor::INTERFACE_NAME, property);
        }

        std::map<std::string, Variant> dict_;
        std::string path_;
        std::string uuid_;
        std::shared_ptr<GattCharacteristic1> characteristic_;
    };
}
