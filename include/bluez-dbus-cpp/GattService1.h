//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include "adaptor/PropertiesExt_adaptor.h"
#include "adaptor/GattService1_adaptor.h"
#include "GattCharacteristic1.h"

#include <sdbus-c++/sdbus-c++.h>

namespace org::bluez {
    // Forward declarations
    class GattApplication1;

    using namespace sdbus;

    class GattService1 :
            public AdaptorInterfaces<GattService1_adaptor, PropertiesExt_adaptor>,
            public std::enable_shared_from_this<GattService1> {
    public:
        GattService1(const GattService1 &service) = delete;

        GattService1(const std::shared_ptr<GattApplication1> &app, const std::string &serviceName,
                     std::string serviceUUID);

        ~GattService1() override;

        virtual void addCharacteristic(std::shared_ptr<GattCharacteristic1> characteristic);

        virtual void removeCharacteristic(const std::shared_ptr<GattCharacteristic1> &characteristic);

        virtual void addSubService(std::shared_ptr<GattService1> service);

        virtual void removeSubService(const std::shared_ptr<GattService1> &service);

        virtual const std::string &getPath() const;

        virtual std::shared_ptr<IConnection> getConnection() const;

        virtual int nextCharacteristicIndex() const;

        /**
     * @brief
     *
     * @return std::string
     */
        std::string UUID() override {
            return uuid_;
        }

        virtual void UUID(const std::string &uuid) {
            uuid_ = uuid;
        }

        bool Primary() override {
            return primary_;
        }

        virtual void Primary(const bool primary) {
            primary_ = primary;
        }

        std::vector<ObjectPath> Includes() override {
            return includes_;
        }

        virtual void Includes(std::vector<ObjectPath> &&includes) {
            includes_ = std::move(includes);
        }

    protected:
        void emitPropertyChangedSignal(const std::string &property) {
            PropertiesExt_adaptor::emitPropertyChangedSignal(GattService1_adaptor::INTERFACE_NAME, property);
        }

        std::shared_ptr<GattApplication1> app_;
        std::string path_;
        std::string uuid_;
        bool primary_;

        std::vector<std::shared_ptr<GattService1> > services_;
        std::vector<std::shared_ptr<GattCharacteristic1> > characteristics_;
        std::vector<ObjectPath> includes_;
    };
}
