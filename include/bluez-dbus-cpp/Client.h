//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <sdbus-c++/sdbus-c++.h>

#include <utility>
#include "Socket.h"


namespace org::bluez {
    /**
     * @brief Hold information about a client connection
     */
    class Client {
    public:
        /**
         * @brief Construct a new Client object
         *
         * @param path the path on DBus as reported by BlueZ
         * @param usable_mtu
         */
        Client(ObjectPath path, const uint16_t usable_mtu) : path_{std::move(path)},
                                                                    mtu_{usable_mtu} {
        }

        /**
         * @brief Get the DBus path of this client
         *
         * @return const sdbus::ObjectPath&
         */
        [[nodiscard]] const ObjectPath &getPath() const {
            return path_;
        }

        /**
         * @brief Get the usable MTU for this client
         *
         * @return uint16_t the mtu
         */
        [[nodiscard]] uint16_t getMtu() const {
            return mtu_;
        }

        /**
         * @brief Add a Socket with Tag for this client
         *
         * @param socketTag a Tag identifying this socket
         * @param socket the Socket to add
         */
        void addSocket(int socketTag, const std::shared_ptr<Socket> &socket) {
            sockets_.insert(std::make_pair(socketTag, socket));
        }

        /**
         * @brief Get the Socket by Tag
         *
         * @param socketTag the Tag for the Socket to retrieve
         * @return std::shared_ptr<Socket> the Socket if available or nullptr if not
         */
        std::shared_ptr<Socket> getSocket(const int socketTag) {
            std::shared_ptr<Socket> socket{nullptr};
            if (const auto iter = sockets_.find(socketTag); iter != sockets_.end()) {
                socket = iter->second;
            }
            return socket;
        }

        /**
         * @brief Remove a Socket by Tag
         *
         * @param socketTag the Tag that identifies the Socket to remove
         */
        void removeSocket(const int socketTag) {
            if (const auto iter = sockets_.find(socketTag); iter != sockets_.end()) {
                sockets_.erase(iter);
            }
        }

    protected:
        ObjectPath path_;
        uint16_t mtu_;
        std::map<int, std::shared_ptr<Socket> > sockets_;
    };
}
