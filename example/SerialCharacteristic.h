//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <bluez-dbus-cpp/GattCharacteristicBuilder.h>
#include <bluez-dbus-cpp/Client.h>
#include <bluez-dbus-cpp/Device1.h>

#include <iostream>
#include <queue>
#include <set>

#include "bluez-dbus-cpp/GenericCharacteristic.h"
#include "bluez-dbus-cpp/Util.h"

namespace org::bluez {
    class SerialClient : public Client {
    public:
        SerialClient(const sdbus::ObjectPath &path, const uint16_t usable_mtu,
                     std::vector<uint8_t> &&initialValue) : Client{
                                                                path, usable_mtu
                                                            },
                                                            value_{initialValue} {
        }

        void setData(std::vector<uint8_t> &&value) {
            this->value_ = std::move(value);
        }

        void setData(const std::vector<uint8_t> &value) {
            this->value_.assign(value.begin(), value.end());
        }

        std::vector<uint8_t> &&getData() {
            return std::move(value_);
        }

    protected:
        std::vector<uint8_t> value_;
    };

    class SerialCharacteristic final :
            public GattCharacteristicBuilder<GenericCharacteristic> {
    public:
        SerialCharacteristic(const std::shared_ptr<GattService1> &service, std::shared_ptr<IConnection> connection,
                             const std::string &uuid)
            : GattCharacteristicBuilder{service, uuid, false, false, true, true},
              connection_{std::move(connection)} {
            flags_ = {"notify", "write-without-response"};
        }

        static SerialCharacteristic &create(const std::shared_ptr<GattService1> &service,
                                            const std::shared_ptr<IConnection> &connection,
                                            const std::string &uuid) {
            const auto self = new SerialCharacteristic(service, connection, uuid);
            return *self;
        }

    protected:
        std::vector<uint8_t> ReadValue(const std::map<std::string, sdbus::Variant> &options) override {
            std::cerr << "Serial RX: " << std::string(value_.begin(), value_.end()) << std::endl;
            throw sdbus::Error("org.bluez.Error.NotSupported",
                               "'read' not supported on 'SerialCharacteristic', use 'notify' instead");
        }

        void WriteValue(const std::vector<uint8_t> &value,
                        const std::map<std::string, sdbus::Variant> &options) override {
            std::cout << "Serial TX: " << std::string(value.begin(), value.end()) << std::endl;
            const auto client = getClient(options);
            if (client == lastClient_) {
                if (lastDevice_
                    && !value.empty()
                    && value[0] == 'd') {
                    std::cout << "Disconnecting from client" << std::endl;
                    lastDevice_->Disconnect();
                }
            }
            client->setData(value);
            Notify(client);
        }

        void StartNotify(const std::map<std::string, sdbus::Variant> &options) override {
            // insert client into our list
            const auto client = getClient(options);
            notifying_.insert(client);
            std::cout << "SerialCharacteristic::StartNotify '" << client->getPath() << "'" << std::endl;
        }

        void StopNotify(const std::map<std::string, sdbus::Variant> &options) override {
            // remove client from our list
            if (!options.empty()) {
                //When disconnecting from device, StopNotify is called with an empty options map
                const auto client = getClient(options);
                const auto iter = notifying_.find(client);
                notifying_.erase(iter);
                std::cout << "SerialCharacteristic::StopNotify '" << client->getPath() << "'" << std::endl;
            }
        }

        void Notify(const std::shared_ptr<SerialClient> &client) {
            // in this example we always append a 'k' message to test the multi-packet aspect
            // that is part of the 'DirectedValue' BlueZ patch rev2 feature
            directedQueue_.insert(std::make_pair(client->getPath(),
                                                 std::vector<std::vector<uint8_t> >{
                                                     client->getData(), std::vector<uint8_t>{'k'}
                                                 }));

            if (notifying_.find(client) != notifying_.end()) {
                emitPropertyChangedSignal("DirectedValue");
            }
        }

        std::map<sdbus::ObjectPath, std::vector<std::vector<uint8_t> > > DirectedValue() override {
            std::cout << "DirectedValue()" << std::endl;
            return std::move(directedQueue_);
        }

    protected:
        std::shared_ptr<SerialClient> getClient(const std::map<std::string, sdbus::Variant> &options) {
            auto [real_mtu, usable_mtu] = Util::getMTUFromOptions(options);

            auto path = Util::getObjectPathFromOptions(options);
            auto iter = clients_.find(path);
            if (iter == clients_.end()) {
                std::cout << "SerialCharacteristic::getClient - creating client '" << path << "'" << std::endl;
                auto client = std::make_shared<SerialClient>(path, usable_mtu, std::vector<uint8_t>());
                iter = clients_.insert(std::make_pair(path, client)).first;
                lastClient_ = iter->second;
                lastDevice_ = std::make_shared<Device1>(*connection_, "org.bluez", path);
            }
            return iter->second;
        }

        void removeClient(const std::map<std::string, sdbus::Variant> &options) {
            const auto path = Util::getObjectPathFromOptions(options);
            if (const auto iter = clients_.find(path); iter != clients_.end()) {
                std::cout << "SerialCharacteristic::removeClient '" << path << "'" << std::endl;
                clients_.erase(iter);
            }
        }

        std::map<sdbus::ObjectPath, std::vector<std::vector<uint8_t> > > directedQueue_;
        std::map<sdbus::ObjectPath, std::shared_ptr<SerialClient> > clients_;
        std::set<std::shared_ptr<SerialClient> > notifying_;
        std::shared_ptr<SerialClient> lastClient_;
        std::shared_ptr<Device1> lastDevice_;
        std::shared_ptr<IConnection> connection_;
    };
}
