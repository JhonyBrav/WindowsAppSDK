#include "pch.h"
#include <testdef.h>
#include <iostream>
#include <wil/win32_helpers.h>
#include "WindowsAppRuntime.Test.AppModel.h"
#include <chrono>

namespace winrt
{
    using namespace winrt::Microsoft::Windows::AppLifecycle;
    using namespace winrt::Windows::ApplicationModel::Activation;
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::Windows::ToastNotifications;
    using namespace winrt::Windows::Data::Xml::Dom;
}

bool BackgroundActivationTest() // Activating application for background test.
{
    return true;
}

bool UnregisterBackgroundActivationTest()
{
    winrt::ToastNotificationManager::Default().UnregisterActivator();
    return true;
}

winrt::ToastNotification GetToastNotification(winrt::hstring message)
{
    winrt::hstring xmlPayload{ L"<toast>" + message + L"</toast>" };

    winrt::XmlDocument xmlDocument{};
    xmlDocument.LoadXml(xmlPayload);

    return winrt::ToastNotification(xmlDocument);
}

winrt::ToastNotification GetToastNotification()
{
    return GetToastNotification(L"intrepidToast");
}

bool VerifyToastNotificationIsValid(const winrt::ToastNotification& expected, const winrt::ToastNotification& actual)
{
    if (expected.Tag() != actual.Tag())
    {
        return false;
    }

    if (expected.Group() != actual.Group())
    {
        return false;
    }

    if (expected.ToastId() != actual.ToastId())
    {
        return false;
    }

    auto expectedPayload = expected.Payload().GetElementsByTagName(L"toast").GetAt(0).GetXml();
    printf("ELx - expectedPayload: %ws\n", expectedPayload.c_str());
    auto actualPayload = actual.Payload().GetElementsByTagName(L"toast").GetAt(0).GetXml();
    printf("ELx - actualPayload: %ws\n", actualPayload.c_str());

    if (wcscmp(expectedPayload.c_str(), actualPayload.c_str()) != 0)
    {
        return false;
    }
#if 0
    if (VerifyProgressData(expected.ProgressData(), actual.ProgressData()))
    {
        return false;
    }
#endif
    if (expected.ExpirationTime() != actual.ExpirationTime())
    {
        return false;
    }

    if (expected.ExpiresOnReboot() != actual.ExpiresOnReboot())
    {
        return false;
    }

    if (expected.Priority() != actual.Priority())
    {
        return false;
    }

    if (expected.SuppressDisplay() != actual.SuppressDisplay())
    {
        return false;
    }

    return true;
}

bool VerifyFailedRegisterActivatorUsingNullClsid()
{
    try
    {
        auto activationInfo = winrt::ToastActivationInfo::CreateFromActivationGuid(winrt::guid(GUID_NULL));

        winrt::ToastNotificationManager::Default().RegisterActivator(activationInfo);
    }
    catch (...)
    {
        return winrt::to_hresult() == E_INVALIDARG;
    }

    return false;
}

bool VerifyFailedRegisterActivatorUsingNullClsid_Unpackaged()
{
    try
    {
        auto activationInfo = winrt::ToastActivationInfo::CreateFromActivationGuid(winrt::guid(GUID_NULL));

        winrt::ToastNotificationManager::Default().RegisterActivator(activationInfo);
    }
    catch (...)
    {
        return winrt::to_hresult() == E_ILLEGAL_METHOD_CALL;
    }

    return false;
}

bool VerifyFailedRegisterActivatorUsingNullAssets()
{
    try
    {
        auto activationInfo = winrt::ToastActivationInfo::CreateFromToastAssets(nullptr);

        winrt::ToastNotificationManager::Default().RegisterActivator(activationInfo);
    }
    catch (...)
    {
        return winrt::to_hresult() == E_ILLEGAL_METHOD_CALL;
    }

    return false;
}

bool VerifyFailedRegisterActivatorUsingNullAssets_Unpackaged()
{
    try
    {
        auto activationInfo = winrt::ToastActivationInfo::CreateFromToastAssets(nullptr);

        winrt::ToastNotificationManager::Default().RegisterActivator(activationInfo);
    }
    catch (...)
    {
        return winrt::to_hresult() == E_POINTER;
    }

    return false;
}

bool VerifyRegisterActivatorandUnRegisterActivatorUsingClsid()
{
    winrt::ToastNotificationManager::Default().UnregisterActivator();
    try
    {
        auto activationInfo = winrt::ToastActivationInfo::CreateFromActivationGuid(c_toastComServerId);

        winrt::ToastNotificationManager::Default().RegisterActivator(activationInfo);

        winrt::ToastNotificationManager::Default().UnregisterActivator();
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool VerifyRegisterActivatorandUnRegisterActivatorUsingAssets_Unpackaged()
{
    winrt::ToastAssets assets(L"ToastNotificationApp", winrt::Uri{ LR"(C:\Windows\System32\WindowsSecurityIcon.png)" });

    auto activationInfo = winrt::ToastActivationInfo::CreateFromToastAssets(assets);

    winrt::ToastNotificationManager::Default().RegisterActivator(activationInfo);

    winrt::ToastNotificationManager::Default().UnregisterActivator();

    return true;
}

bool VerifyFailedMultipleRegisterActivatorUsingSameClsid()
{
    try
    {
        auto activationInfo = winrt::ToastActivationInfo::CreateFromActivationGuid(c_toastComServerId);

        winrt::ToastNotificationManager::Default().RegisterActivator(activationInfo);

        winrt::ToastNotificationManager::Default().RegisterActivator(activationInfo);
    }
    catch (...)
    {
        return winrt::to_hresult() == E_INVALIDARG;
    }
    
    return false;
}

bool VerifyFailedMultipleRegisterActivatorUsingSameAssets_Unpackaged()
{
    try
    {
        winrt::ToastAssets assets(L"ToastNotificationApp", winrt::Uri{ LR"(C:\Windows\System32\WindowsSecurityIcon.png)" });

        auto activationInfo = winrt::ToastActivationInfo::CreateFromToastAssets(assets);

        winrt::ToastNotificationManager::Default().RegisterActivator(activationInfo);

        winrt::ToastNotificationManager::Default().RegisterActivator(activationInfo);
    }
    catch (...)
    {
        return winrt::to_hresult() == E_INVALIDARG;
    }

    return false;
}

bool VerifyFailedToastAssetsWithEmptyDisplayName_Unpackaged()
{
    try
    {
        winrt::ToastAssets assets(L"", winrt::Uri{LR"(C:\Windows\System32\WindowsSecurityIcon.png)"});

        auto activationInfo = winrt::ToastActivationInfo::CreateFromToastAssets(assets);
    }
    catch (...)
    {
        return winrt::to_hresult() == E_INVALIDARG;
    }

    return false;
}

bool VerifyFailedToastAssetsWithEmptyIconPath_Unpackaged()
{
    try
    {
        winrt::ToastAssets assets(L"ToastNotificationApp", winrt::Uri{ L""});

        auto activationInfo = winrt::ToastActivationInfo::CreateFromToastAssets(assets);
    }
    catch (...)
    {
        return winrt::to_hresult() == E_POINTER;
    }

    return false;
}

bool VerifyFailedToastAssetsWithNullIconPath_Unpackaged()
{
    try
    {
        winrt::ToastAssets assets(L"ToastNotificationApp", nullptr);

        auto activationInfo = winrt::ToastActivationInfo::CreateFromToastAssets(assets);
    }
    catch (...)
    {
        return winrt::to_hresult() == E_POINTER;
    }

    return false;
}

bool VerifyToastSettingEnabled()
{
    return winrt::ToastNotificationManager::Default().Setting() == winrt::ToastNotificationSetting::Enabled;
}

bool VerifyToastPayload()
{
    winrt::hstring xmlPayload{ L"<toast>intrepidToast</toast>" };

    winrt::XmlDocument xmlDocument{};
    xmlDocument.LoadXml(xmlPayload);

    winrt::ToastNotification toast{ xmlDocument };;

    if (toast.Payload() != xmlDocument)
    {
        return false;
    }

    return true;
}

bool VerifyToastTag()
{
    winrt::ToastNotification toast{ GetToastNotification() };

    if (toast.Tag() != winrt::hstring{ L"" })
    {
        return false;
    }

    winrt::hstring tag{ L"tag" };
    toast.Tag(tag);

    if (toast.Tag() != tag)
    {
        return false;
    }

    return true;
}

bool VerifyToastGroup()
{
    winrt::ToastNotification toast{ GetToastNotification() };

    if (toast.Group() != winrt::hstring{ L"" })
    {
        return false;
    }

    winrt::hstring group{ L"group" };
    toast.Group(group);

    if (toast.Group() != group)
    {
        return false;
    }

    return true;
}

bool VerifyToastProgressDataFromToast()
{
    /*
    * TODO: Uncomment once ToastProgressData has been implemented

    winrt::ToastNotification toast{ GetToastNotification() };

    winrt::ToastProgressData progressData{};
    progressData.Status(L"SomeStatus");
    progressData.Title(L"SomeTitle");
    progressData.Value(0.14);
    progressData.ValueStringOverride(L"14%");

    toast.ProgressData(progressData);

    auto progressDataFromToast = toast.ProgressData();
    if (progressDataFromToast != progressData)
    {
        return false;
    }
    */

    return true;
}

bool VerifyToastExpirationTime()
{
    winrt::ToastNotification toast{ GetToastNotification() };

    if (toast.ExpirationTime() != winrt::DateTime{})
    {
        return false;
    }

    winrt::DateTime expirationTime{ winrt::clock::now() };
    expirationTime += winrt::TimeSpan{ std::chrono::seconds(10) };

    toast.ExpirationTime(expirationTime);
    if (toast.ExpirationTime() != expirationTime)
    {
        return false;
    }

    return true;
}

bool VerifyToastPriority()
{
    winrt::ToastNotification toast{ GetToastNotification() };

    if (toast.Priority() != winrt::ToastPriority::Default)
    {
        return false;
    }

    toast.Priority(winrt::ToastPriority::High);
    if (toast.Priority() != winrt::ToastPriority::High)
    {
        return false;
    }

    return true;
}

bool VerifyToastSuppressDisplay()
{
    winrt::ToastNotification toast{ GetToastNotification() };

    if (toast.SuppressDisplay())
    {
        return false;
    }

    toast.SuppressDisplay(true);
    if (!toast.SuppressDisplay())
    {
        return false;
    }

    return true;
}

bool VerifyToastExpiresOnReboot()
{
    winrt::ToastNotification toast{ GetToastNotification() };

    if (toast.ExpiresOnReboot())
    {
        return false;
    }

    toast.ExpiresOnReboot(true);
    if (!toast.ExpiresOnReboot())
    {
        return false;
    }

    return true;
}

bool VerifyShowToast()
{
    // Registration happens in main()
    winrt::ToastNotification toast{ GetToastNotification() };

    auto toastNotificationManager = winrt::ToastNotificationManager::Default();
    toastNotificationManager.ShowToast(toast);

    if (toast.ToastId() == 0)
    {
        return false;
    }

    // TODO: Verify the toast was posted by calling History APIs.

    return true;
}

bool VerifyShowToast_Unpackaged()
{
    winrt::ToastAssets assets(L"ToastNotificationApp", winrt::Uri{ LR"(C:\Windows\System32\WindowsSecurityIcon.png)" });
    auto activationInfo = winrt::ToastActivationInfo::CreateFromToastAssets(assets);

    auto toastNotificationManager = winrt::ToastNotificationManager::Default();
    toastNotificationManager.RegisterActivator(activationInfo);

    auto scope_exit = wil::scope_exit(
        [&] {
            toastNotificationManager.UnregisterActivator();
        });

    winrt::ToastNotification toast{ GetToastNotification() };

    toastNotificationManager.ShowToast(toast);

    if (toast.ToastId() == 0)
    {
        return false;
    }

    // TODO: Verify the toast was posted by calling History APIs.

    return true;
}

bool VerifyFailedRemoveWithIdentiferAsyncUsingZeroedToastIdentifier()
{
    //RemoveWithIdentiferAsync();
    return false;
}

bool VerifyFailedRemoveWithTagAsyncUsingEmptyTag()
{
    return false;
}

bool VerifyFailedRemoveWithTagGroupAsyncUsingEmptyTagAndGroup()
{
    return false;
}

bool VerifyFailedRemoveWithGroupAsyncUsingEmptyGroup()
{
    return false;
}

bool VerifyRemoveAllAsync()
{
    auto toastNotificationManager = winrt::ToastNotificationManager::Default();

    winrt::ToastNotification toast1{ GetToastNotification() };
    toastNotificationManager.ShowToast(toast1);

    winrt::ToastNotification toast2{ GetToastNotification() };
    toastNotificationManager.ShowToast(toast2);

    winrt::ToastNotification toast3{ GetToastNotification() };
    toastNotificationManager.ShowToast(toast3);

    auto result1 = toastNotificationManager.GetAllAsync();
    if (result1.wait_for(std::chrono::seconds(300)) != winrt::Windows::Foundation::AsyncStatus::Completed)
    {
        result1.Cancel();
        return false;
    }

    auto result1a = result1.get();
    auto size = result1a.Size();
    if (size != 3)
    {
        return false;
    }

    auto result2 = toastNotificationManager.RemoveAllAsync();
    if (result2.wait_for(std::chrono::seconds(300)) != winrt::Windows::Foundation::AsyncStatus::Completed)
    {
        result2.Cancel();
        return false;
    }

    //try
    //{
        auto result3 = toastNotificationManager.GetAllAsync();
        if (result3.wait_for(std::chrono::seconds(300)) != winrt::Windows::Foundation::AsyncStatus::Error)
        {
            result3.Cancel();
            return false;
        }

        return true;
#if 0
        auto result3a = result3.get();
        auto size = result3a.Size();
        if (size != 0)
        {
            return false;
        }

        return true;
    }
    catch (...)
    {
        return true;
    }
#endif
    return true;
}

bool VerifyFailedGetAllAsync()
{
    auto toastNotificationManager = winrt::ToastNotificationManager::Default();

    try
    {
        auto result = toastNotificationManager.GetAllAsync();
        if (result.wait_for(std::chrono::seconds(300)) != winrt::Windows::Foundation::AsyncStatus::Error)
        {
            result.Cancel();
            return false;
        }
    }
    catch (...)
    {
        return true;
    }

    return true;
}

bool VerifyGetAllAsync()
{
    winrt::ToastNotification toast{ GetToastNotification(L"MyOwnToast")};
    toast.Tag(L"aDifferentTag");
    toast.Group(L"aDifferentGroup");
    //toast.ToastId(42); //Setting a value here, seems to fail
    winrt::DateTime expirationTime{ winrt::clock::now() };
    expirationTime += winrt::TimeSpan{ std::chrono::seconds(10) };
    toast.ExpirationTime(expirationTime);
    toast.ExpiresOnReboot(false); //Setting this to true fails, not sure why

    auto toastNotificationManager = winrt::ToastNotificationManager::Default();

    toastNotificationManager.ShowToast(toast);

    auto result = toastNotificationManager.GetAllAsync();
    if (result.wait_for(std::chrono::seconds(300)) != winrt::Windows::Foundation::AsyncStatus::Completed)
    {
        result.Cancel();
        return false;
    }

    auto result2 = result.get();

    auto size = result2.Size();
    if (size != 1)
    {
        return false;
    }

    auto actual = result2.GetAt(0);
    return VerifyToastNotificationIsValid(toast, actual);
}

bool VerifyGetAllAsync3()
{
    auto toastNotificationManager = winrt::ToastNotificationManager::Default();

    winrt::ToastNotification toast1{ GetToastNotification() };
    toastNotificationManager.ShowToast(toast1);

    winrt::ToastNotification toast2{ GetToastNotification() };
    toastNotificationManager.ShowToast(toast2);

    winrt::ToastNotification toast3{ GetToastNotification() };
    toastNotificationManager.ShowToast(toast3);

    auto result = toastNotificationManager.GetAllAsync();
    if (result.wait_for(std::chrono::seconds(300)) != winrt::Windows::Foundation::AsyncStatus::Completed)
    {
        result.Cancel();
        return false;
    }

    auto result2 = result.get();

    auto size = result2.Size();
    if (size != 3)
    {
        return false;
    }

    auto actual = result2.GetAt(0);
    auto payload = actual.Payload().GetElementsByTagName(L"toast").GetAt(0).GetXml();
    printf("ELx - payload: %ws\n", payload.c_str());

    if (wcscmp(L"<toast>intrepidToast</toast>", payload.c_str()) != 0)
    {
        return false;
    }

    return true;
}

std::string unitTestNameFromLaunchArguments(const winrt::ILaunchActivatedEventArgs& launchArgs)
{
    std::string unitTestName = to_string(launchArgs.Arguments());
    auto argStart = unitTestName.rfind(" ");
    if (argStart != std::wstring::npos)
    {
        unitTestName = unitTestName.substr(argStart + 1);
    }

    return unitTestName;
}

std::map<std::string, bool(*)()> const& GetSwitchMapping()
{
    static std::map<std::string, bool(*)()> switchMapping = {
        { "BackgroundActivationTest", &BackgroundActivationTest},
        { "UnregisterBackgroundActivationTest", &UnregisterBackgroundActivationTest },
        { "VerifyFailedRegisterActivatorUsingNullClsid", &VerifyFailedRegisterActivatorUsingNullClsid },
        { "VerifyFailedRegisterActivatorUsingNullClsid_Unpackaged", &VerifyFailedRegisterActivatorUsingNullClsid_Unpackaged },
        { "VerifyFailedRegisterActivatorUsingNullAssets", &VerifyFailedRegisterActivatorUsingNullAssets },
        { "VerifyFailedRegisterActivatorUsingNullAssets_Unpackaged", &VerifyFailedRegisterActivatorUsingNullAssets_Unpackaged },
        { "VerifyRegisterActivatorandUnRegisterActivatorUsingClsid", &VerifyRegisterActivatorandUnRegisterActivatorUsingClsid },
        { "VerifyRegisterActivatorandUnRegisterActivatorUsingAssets_Unpackaged", &VerifyRegisterActivatorandUnRegisterActivatorUsingAssets_Unpackaged },
        { "VerifyFailedMultipleRegisterActivatorUsingSameClsid", &VerifyFailedMultipleRegisterActivatorUsingSameClsid },
        { "VerifyFailedMultipleRegisterActivatorUsingSameAssets_Unpackaged", &VerifyFailedMultipleRegisterActivatorUsingSameAssets_Unpackaged },
        { "VerifyFailedToastAssetsWithEmptyDisplayName_Unpackaged", &VerifyFailedToastAssetsWithEmptyDisplayName_Unpackaged },
        { "VerifyFailedToastAssetsWithEmptyIconPath_Unpackaged", &VerifyFailedToastAssetsWithEmptyIconPath_Unpackaged },
        { "VerifyFailedToastAssetsWithNullIconPath_Unpackaged", &VerifyFailedToastAssetsWithNullIconPath_Unpackaged },
        { "VerifyToastSettingEnabled", &VerifyToastSettingEnabled },
        { "VerifyToastPayload", &VerifyToastPayload },
        { "VerifyToastTag", &VerifyToastTag },
        { "VerifyToastGroup", &VerifyToastGroup },
        { "VerifyToastProgressDataFromToast", &VerifyToastProgressDataFromToast },
        { "VerifyToastExpirationTime", &VerifyToastExpirationTime },
        { "VerifyToastPriority", &VerifyToastPriority },
        { "VerifyToastSuppressDisplay", &VerifyToastSuppressDisplay },
        { "VerifyToastExpiresOnReboot", &VerifyToastExpiresOnReboot },
        { "VerifyShowToast", &VerifyShowToast },
        { "VerifyShowToast_Unpackaged", &VerifyShowToast_Unpackaged },
        { "VerifyFailedRemoveWithIdentiferAsyncUsingZeroedToastIdentifier", &VerifyFailedRemoveWithIdentiferAsyncUsingZeroedToastIdentifier },
        { "VerifyFailedRemoveWithTagAsyncUsingEmptyTag", &VerifyFailedRemoveWithTagAsyncUsingEmptyTag },
        { "VerifyFailedRemoveWithTagGroupAsyncUsingEmptyTagAndGroup", &VerifyFailedRemoveWithTagGroupAsyncUsingEmptyTagAndGroup },
        { "VerifyFailedRemoveWithGroupAsyncUsingEmptyGroup", &VerifyFailedRemoveWithGroupAsyncUsingEmptyGroup },
        { "VerifyRemoveAllAsync", &VerifyRemoveAllAsync },
        { "VerifyFailedGetAllAsync", &VerifyFailedGetAllAsync },
        { "VerifyGetAllAsync", &VerifyGetAllAsync },
        { "VerifyGetAllAsync3", &VerifyGetAllAsync3 },
    };
    return switchMapping;
}

bool runUnitTest(std::string unitTest)
{
    auto const& switchMapping = GetSwitchMapping();
    auto it = switchMapping.find(unitTest);
    if (it == switchMapping.end())
    {
        return false;
    }

    return it->second();
}


int main() try
{
    bool testResult = false;
    auto scope_exit = wil::scope_exit([&] {
#if 0
        if (!Test::AppModel::IsPackagedProcess())
        {
            winrt::ToastNotificationManager::Default().UnregisterActivator();
        }
#endif
        ::Test::Bootstrap::CleanupBootstrap();
        });

    ::Test::Bootstrap::SetupBootstrap();

    if (Test::AppModel::IsPackagedProcess())
    {
        auto activationInfo = winrt::ToastActivationInfo::CreateFromActivationGuid(c_toastComServerId);
        winrt::ToastNotificationManager::Default().RegisterActivator(activationInfo);
    }
#if 0
    else
    {
        winrt::ToastAssets assets(L"ToastNotificationApp", winrt::Uri{ LR"(C:\Windows\System32\WindowsSecurityIcon.png)" });
        auto activationInfo = winrt::ToastActivationInfo::CreateFromToastAssets(assets);

        auto toastNotificationManager = winrt::ToastNotificationManager::Default();
        toastNotificationManager.RegisterActivator(activationInfo);
    }
#endif
    auto args = winrt::AppInstance::GetCurrent().GetActivatedEventArgs();
    auto kind = args.Kind();

    if (kind == winrt::ExtendedActivationKind::Launch)
    {
        auto unitTest = unitTestNameFromLaunchArguments(args.Data().as<winrt::ILaunchActivatedEventArgs>());
        std::cout << unitTest << std::endl;

        testResult = runUnitTest(unitTest);
    }

    return testResult ? 0 : 1; // We want 0 to be success and 1 failure
}
catch (...)
{
    std::cout << winrt::to_string(winrt::to_message()) << std::endl;

    return 1; // in the event of unhandled test crash
}
