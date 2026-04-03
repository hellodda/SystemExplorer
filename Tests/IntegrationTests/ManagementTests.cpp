#include "pch.h"
#include "CppUnitTest.h"
#include <comdef.h>
#include <mutex>
#include <vector>
#include <string>
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace 
{
    std::wstring VariantToWString(_variant_t const& value)
    {
        if (value.vt == VT_BSTR && value.bstrVal != nullptr)
        {
            return std::wstring(value.bstrVal, SysStringLen(value.bstrVal));
        }

        _variant_t converted;
        if (SUCCEEDED(VariantChangeType(&converted, &value, 0, VT_BSTR)) &&
            converted.vt == VT_BSTR &&
            converted.bstrVal != nullptr)
        {
            return std::wstring(converted.bstrVal, SysStringLen(converted.bstrVal));
        }

        return {};
    }

    const _variant_t* FindProperty(
        std::vector<std::pair<winrt::hstring, _variant_t>> const& properties,
        std::wstring_view name
    )
    {
        for (auto const& [propName, propValue] : properties)
        {
            if (propName == name)
                return &propValue;
        }
        return nullptr;
    }
}

namespace Tests::IntegrationTests
{
    TEST_CLASS(ManagementIntegrationTests)
    {
    public:

        // ---------------------------------------------------------------------
        // Query should return at least one real WMI object from root\cimv2.
        // This validates:
        //   - ManagementConnection::ExecuteQueryAsync
        //   - QuerySink::Indicate / SetStatus
        //   - asynchronous completion path
        // ---------------------------------------------------------------------
        TEST_METHOD(ExecuteQueryAsync_ReturnsOperatingSystemObject)
        {
            winrt::SystemExplorer::Core::Management::ManagementConnection connection{ L"root\\cimv2" };

            auto results = connection.ExecuteQueryAsync(
                L"SELECT Caption, Version, BuildNumber FROM Win32_OperatingSystem"
            ).get();

            Assert::IsTrue(!results.empty(), L"WMI query returned no results.");

            auto properties = results.front().GetProperties();
            Assert::IsTrue(!properties.empty(), L"GetProperties() returned no properties.");

            auto caption = FindProperty(properties, L"Caption");
            auto version = FindProperty(properties, L"Version");
            auto buildNumber = FindProperty(properties, L"BuildNumber");

            Assert::IsNotNull(caption, L"Caption property was not found.");
            Assert::IsNotNull(version, L"Version property was not found.");
            Assert::IsNotNull(buildNumber, L"BuildNumber property was not found.");

            Assert::IsFalse(VariantToWString(*caption).empty(), L"Caption is empty.");
            Assert::IsFalse(VariantToWString(*version).empty(), L"Version is empty.");
            Assert::IsFalse(VariantToWString(*buildNumber).empty(), L"BuildNumber is empty.");
        }

        // ---------------------------------------------------------------------
        // GetProperties should enumerate non-system properties correctly.
        // ---------------------------------------------------------------------
        TEST_METHOD(GetProperties_ContainsKnownNonSystemProperties)
        {
            winrt::SystemExplorer::Core::Management::ManagementConnection connection{ L"root\\cimv2" };

            auto results = connection.ExecuteQueryAsync(
                L"SELECT Caption, Version, BuildNumber, OSArchitecture FROM Win32_OperatingSystem"
            ).get();

            Assert::IsTrue(!results.empty(), L"WMI query returned no results.");

            auto properties = results.front().GetProperties();

            Assert::IsTrue(
                FindProperty(properties, L"Caption") != nullptr,
                L"Caption must be present in GetProperties()."
            );

            Assert::IsTrue(
                FindProperty(properties, L"Version") != nullptr,
                L"Version must be present in GetProperties()."
            );

            Assert::IsTrue(
                FindProperty(properties, L"BuildNumber") != nullptr,
                L"BuildNumber must be present in GetProperties()."
            );

            Assert::IsTrue(
                FindProperty(properties, L"OSArchitecture") != nullptr,
                L"OSArchitecture must be present in GetProperties()."
            );
        }

        // ---------------------------------------------------------------------
        // GetProperty should return the exact requested property value.
        // ---------------------------------------------------------------------
        TEST_METHOD(GetProperty_ReturnsExpectedValue)
        {
            winrt::SystemExplorer::Core::Management::ManagementConnection connection{ L"root\\cimv2" };

            auto results = connection.ExecuteQueryAsync(
                L"SELECT Caption FROM Win32_OperatingSystem"
            ).get();

            Assert::IsTrue(!results.empty(), L"WMI query returned no results.");

            auto [name, value] = results.front().GetProperty(L"Caption");

            Assert::AreEqual(std::wstring_view(L"Caption"), std::wstring_view(name));
            Assert::IsTrue(value.vt == VT_BSTR || value.vt == VT_EMPTY || value.vt == VT_NULL,
                L"Caption should normally be a string-like variant.");

            auto caption = VariantToWString(value);
            Assert::IsFalse(caption.empty(), L"Caption value is empty.");
        }

        // ---------------------------------------------------------------------
        // Missing property should not crash and should return an empty variant.
        // This validates graceful behavior of GetProperty().
        // ---------------------------------------------------------------------
        TEST_METHOD(GetProperty_MissingProperty_ReturnsEmptyVariant)
        {
            winrt::SystemExplorer::Core::Management::ManagementConnection connection{ L"root\\cimv2" };

            auto results = connection.ExecuteQueryAsync(
                L"SELECT Caption FROM Win32_OperatingSystem"
            ).get();

            Assert::IsTrue(!results.empty(), L"WMI query returned no results.");

            auto [name, value] = results.front().GetProperty(L"ThisPropertyDoesNotExist_12345");

            Assert::AreEqual(std::wstring_view(L"ThisPropertyDoesNotExist_12345"), std::wstring_view(name));
            Assert::IsTrue(value.vt == VT_EMPTY || value.vt == VT_NULL,
                L"Missing property should return an empty/null variant.");
        }

        // ---------------------------------------------------------------------
        // Integration test for a broader query result shape.
        // This helps catch issues in sink completion and object wrapping.
        // ---------------------------------------------------------------------
        TEST_METHOD(ExecuteQueryAsync_ReturnsMultiplePropertiesPerObject)
        {
            winrt::SystemExplorer::Core::Management::ManagementConnection connection{ L"root\\cimv2" };

            auto results = connection.ExecuteQueryAsync(
                L"SELECT Caption, Version, BuildNumber, OSArchitecture, SerialNumber FROM Win32_OperatingSystem"
            ).get();

            Assert::IsTrue(!results.empty(), L"WMI query returned no results.");

            auto properties = results.front().GetProperties();

            Assert::IsTrue(properties.size() >= 4, L"Expected at least 4 properties.");

            for (auto const& expected : { L"Caption", L"Version", L"BuildNumber", L"OSArchitecture" })
            {
                Assert::IsTrue(
                    FindProperty(properties, expected) != nullptr,
                    (std::wstring(L"Missing property: ") + expected).c_str()
                );
            }
        }
    };
}