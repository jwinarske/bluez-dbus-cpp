//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#include "bluez-dbus-cpp/Util.h"
#include <algorithm>

namespace org::bluez {
    std::tuple<uint16_t /* real */, uint16_t /* usable */> Util::getMTUFromOptions(
        const std::map<std::string, sdbus::Variant> &options) {
        if (const auto deviceIter = options.find("mtu"); deviceIter != options.end()) {
            auto realMtu = deviceIter->second.get<uint16_t>();
            uint16_t usableMtu;
            if (constexpr uint16_t ATT_MTU = 251; realMtu > ATT_MTU) {
                usableMtu = realMtu - realMtu / ATT_MTU * 10
                        /* return usable MTU, subtract 10 byte overhead per ATT packet */;
            } else {
                usableMtu = realMtu - 3;
            }
            return std::make_tuple(realMtu, usableMtu);
        }
        return std::make_tuple(static_cast<uint16_t>(0), static_cast<uint16_t>(0));
    }

    sdbus::ObjectPath Util::getObjectPathFromOptions(const std::map<std::string, sdbus::Variant> &options) {
        if (const auto deviceIter = options.find("device"); deviceIter != options.end()) {
            return deviceIter->second.get<sdbus::ObjectPath>();
        }
        return "";
    }

    std::string Util::getDeviceMAC(const sdbus::ObjectPath &path) {
        std::string mac;

        if (const size_t pLen = path.size(); pLen > 17 &&
                                             path.at(pLen - 18) == '_') {
            mac = path.substr(pLen - 17);
            std::replace(mac.begin(), mac.end(), '_', ':');
        }

        return mac;
    }

    std::string Util::optionsListToString(const std::map<std::string, sdbus::Variant> &options) {
        std::string res;

        for (auto iter{options.cbegin()}; iter != options.cend(); ++iter) {
            if (iter->first == "device") {
                res.append("device=").append(getDeviceMAC(iter->second.get<sdbus::ObjectPath>())).
                        append(", ");
            } else if (iter->first == "mtu") {
                res.append("mtu=").append(std::to_string(iter->second.get<uint16_t>())).append(", ");
            } else if (iter->first == "link") {
                res.append("link=").append(iter->second.get<std::string>()).append(", ");
            } else {
                res.append("'").append(iter->first).append("',");
            }
        }

        return res;
    }
}
