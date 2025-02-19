//
// Copyright Audiofile LLC 2019 - 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//

#include <csignal>
#include <sstream>

#include <bluez-dbus-cpp/bluez.h>
#include <bluez-dbus-cpp/ReadOnlyCharacteristic.h>
#include "SerialCharacteristic.h"

#include <spdlog/spdlog-inl.h>

using namespace org::bluez;

constexpr auto BLUEZ_SERVICE = "org.bluez";
constexpr auto DEVICE0 = "/org/bluez/hci0";


static void sig_callback(const int signum)
{
    exit(signum);
}

[[noreturn]] int main(int /* argc */, char* /* argv */[])
{
    if (signal(SIGINT, sig_callback) == SIG_ERR)
        spdlog::error("error registering signal handler");

    constexpr auto APP_PATH = "/org/bluez/example";
    constexpr auto ADV_PATH = "/org/bluez/example/advertisement1";

    constexpr auto NAME = "ExampleBlueZ";

    std::shared_ptr connection{std::move(createSystemBusConnection())};

    // ---- Adapter Info -----------------------------------------------------------------------------------------------

    {
        const Adapter1 adapter1{*connection, BLUEZ_SERVICE, DEVICE0};

        adapter1.Powered(true);
        adapter1.Discoverable(true);
        adapter1.Pairable(true);
        adapter1.Alias(NAME);

        spdlog::info("Found adapter '{}'", DEVICE0);
        spdlog::info("Name: '{}'", adapter1.Name());
        spdlog::info("Address: {} type: {}", adapter1.Address(), adapter1.AddressType());
        spdlog::info("Powered: {}", adapter1.Powered());
        spdlog::info("Discoverable: {}", adapter1.Discoverable());
        spdlog::info("Pairable: {}", adapter1.Pairable());
    }

    // ---- Services ---------------------------------------------------------------------------------------------------
    const GattManager1 gattMgr{connection, BLUEZ_SERVICE, DEVICE0};
    auto app = std::make_shared<GattApplication1>(connection, APP_PATH);
    const auto srv1 = std::make_shared<GattService1>(app, "deviceinfo", "180A");
    ReadOnlyCharacteristic::createFinal(srv1, "2A24", NAME); // model name
    ReadOnlyCharacteristic::createFinal(srv1, "2A25", "333-12345678-888"); // serial number
    ReadOnlyCharacteristic::createFinal(srv1, "2A26", "1.0.1"); // fw rev
    ReadOnlyCharacteristic::createFinal(srv1, "2A27", "rev A"); // hw rev
    ReadOnlyCharacteristic::createFinal(srv1, "2A28", "5.0"); // sw rev
    ReadOnlyCharacteristic::createFinal(srv1, "2A29", "ACME Inc."); // manufacturer

    const auto srv2 = std::make_shared<GattService1>(app, "serial", "368a3edf-514e-4f70-ba8f-2d0a5a62bc8c");
    SerialCharacteristic::create(srv2, connection, "de0a7b0c-358f-4cef-b778-8fe9abf09d53")
        .finalize();

    auto register_app_callback = [](const Error* error)
    {
        if (error == nullptr)
        {
            spdlog::info("Application registered");
        }
        else
        {
            spdlog::error("Error registering application {} with message {}", error->getName(), error->getMessage());
        }
    };

    gattMgr.RegisterApplicationAsync(app->getPath(), {})
           .uponReplyInvoke(register_app_callback);

    // ---- Advertising ------------------------------------------------------------------------------------------------

    const auto mgr = std::make_shared<LEAdvertisingManager1>(connection, BLUEZ_SERVICE, DEVICE0);
    spdlog::info("LEAdvertisingManager1");
    spdlog::info("  ActiveInstances: {}", mgr->ActiveInstances());
    spdlog::info("  SupportedInstances: {}", mgr->SupportedInstances());
    {
        std::stringstream ss;
        auto includes = mgr->SupportedIncludes();
        for (const auto& include : includes)
        {
            ss << "\"" << include << "\",";
        }
        spdlog::info("  SupportedIncludes: {}", ss.str());
    }

    auto register_adv_callback = [](const Error* error) -> void
    {
        if (error == nullptr)
        {
            spdlog::info("Advertisement registered");
        }
        else
        {
            spdlog::error("Error registering advertisement {} with message {}", error->getName(), error->
                          getMessage());
        }
    };

    auto ad = LEAdvertisement1::create(*connection, ADV_PATH)
              .withLocalName(NAME)
              .withServiceUUIDs(std::vector{std::string{"368a3edf-514e-4f70-ba8f-2d0a5a62bc8c"}})
              .withIncludes(std::vector{std::string{"tx-power"}})
              .onReleaseCall([] { spdlog::info("advertisement released"); })
              .registerWith(mgr, register_adv_callback);

    spdlog::info("Loading complete");

    connection->enterEventLoopAsync();

    while (true);
}
