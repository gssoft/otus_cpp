#include "pch.h"
#include "DateTimeFormatter.h"
#if __has_include("DateTimeFormatter.g.cpp")
#include "DateTimeFormatter.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::Ca07MultiLogService01::implementation
{
    int32_t DateTimeFormatter::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void DateTimeFormatter::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void DateTimeFormatter::ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
        Button().Content(box_value(L"Clicked"));
    }
}
