#pragma once

#include "DateTimeFormatter.g.h"

namespace winrt::Ca07MultiLogService01::implementation
{
    struct DateTimeFormatter : DateTimeFormatterT<DateTimeFormatter>
    {
        DateTimeFormatter() 
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void ClickHandler(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::Ca07MultiLogService01::factory_implementation
{
    struct DateTimeFormatter : DateTimeFormatterT<DateTimeFormatter, implementation::DateTimeFormatter>
    {
    };
}
