#pragma once

#include <winrt/base.h>
#include <wil/result.h>
#include <wil/com.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Input.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <microsoft.ui.xaml.media.dxinterop.h> 
#include <d3d11_4.h>
#include <dxgi1_3.h> 
#include <d2d1_3.h>
#include <dwrite.h>
#include <vector>
#include <string>
#include <functional>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <algorithm>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#undef max
#undef min 
inline float Lerp(float a, float b, float t) noexcept {
    return a + (b - a) * t;
}

inline D2D1_COLOR_F LerpColor(const D2D1_COLOR_F& a, const D2D1_COLOR_F& b, float t) noexcept {
    return D2D1::ColorF(
        Lerp(a.r, b.r, t),
        Lerp(a.g, b.g, t),
        Lerp(a.b, b.b, t),
        Lerp(a.a, b.a, t)
    );
}

class PerformanceRingBuffer {
private:
    std::vector<float> m_buffer;
    size_t m_head = 0;
    size_t m_size = 0;
    size_t m_capacity = 0;

public:
    void Resize(size_t newCapacity) {
        if (newCapacity == m_capacity) return;

        std::vector<float> newBuffer(newCapacity, 0.0f);
        size_t currentSize = Size();
        size_t itemsToCopy = std::min(currentSize, newCapacity);

        size_t startOffset = (currentSize > newCapacity) ? (currentSize - newCapacity) : 0;

        for (size_t i = 0; i < itemsToCopy; ++i) {
            newBuffer[i] = At(startOffset + i);
        }

        m_buffer = std::move(newBuffer);
        m_capacity = newCapacity;
        m_size = itemsToCopy;
        m_head = 0;
    }

    void PushBack(float value) noexcept {
        if (m_capacity == 0) return;

        if (m_size < m_capacity) {
            m_buffer[m_size] = value;
            m_size++;
        }
        else {
            m_buffer[m_head] = value;
            m_head = (m_head + 1) % m_capacity;
        }
    }

    [[nodiscard]] size_t Size() const noexcept { return m_size; }
    [[nodiscard]] size_t Capacity() const noexcept { return m_capacity; }
    [[nodiscard]] bool Empty() const noexcept { return m_size == 0; }

    [[nodiscard]] float At(size_t index) const noexcept {
        if (index >= m_size) return 0.0f;
        return m_buffer[(m_head + index) % m_capacity];
    }
};

enum class GraphLineStyle {
    Solid,
    Dashed,
    Dotted,
    Custom
};

struct LineSettings {
    std::wstring Name;
    D2D1_COLOR_F LineColor = D2D1::ColorF(D2D1::ColorF::White);
    D2D1_COLOR_F FillColor = D2D1::ColorF(D2D1::ColorF::Black, 0.0f);
    bool ShowFill = true;
    bool IsVisible = true;
    float LineThickness = 1.5f;
    GraphLineStyle Style = GraphLineStyle::Solid;
    std::vector<float> CustomDashes;
};

enum class ThresholdNameAlignment {
    Left,
    Right
};

struct ThresholdLineSettings {
    std::wstring Name;
    float Value = 0.8f; // Значение от 0.0 до 1.0, где должна быть линия
    D2D1_COLOR_F LineColor = D2D1::ColorF(D2D1::ColorF::Red, 0.8f);
    float LineThickness = 1.0f;
    GraphLineStyle Style = GraphLineStyle::Dashed;
    std::vector<float> CustomDashes;
    bool ShowName = true;
    D2D1_COLOR_F TextColor = D2D1::ColorF(D2D1::ColorF::White, 0.8f);
    ThresholdNameAlignment NameAlignment = ThresholdNameAlignment::Right;
    std::function<void(size_t lineIndex, float actualValue)> OnTrigger = nullptr;
};

struct GraphSettings {
    size_t MaxPoints = 500;
    float PointSpacing = 4.0f;
    bool MoveGridWithLines = true;
    D2D1_COLOR_F BgColor = D2D1::ColorF(0x111111);
    D2D1_COLOR_F GridColor = D2D1::ColorF(0x333333);
    bool ShowGrid = true;
    float GridCellWidth = 40.0f;
    float GridCellHeight = 40.0f;
    bool ShowTooltip = true;
    bool ShowHoverLine = true;
    bool ShowHoverPoints = true;
    D2D1_COLOR_F TooltipBgColor = D2D1::ColorF(0x222222, 0.9f); // Альфа-канал задает прозрачность
    D2D1_COLOR_F TooltipBorderColor = D2D1::ColorF(0x555555, 1.0f);
    D2D1_COLOR_F TooltipTextColor = D2D1::ColorF(D2D1::ColorF::White);
    float TooltipPaddingX = 10.0f;
    float TooltipPaddingY = 6.0f;
    float TooltipCornerRadius = 4.0f; // Скругление краев тултипа
    D2D1_COLOR_F HoverLineColor = D2D1::ColorF(D2D1::ColorF::White, 0.5f);
    std::wstring FontFamily = L"Segoe UI";
    float FontSize = 12.0f;
    std::function<std::wstring(const std::vector<std::pair<std::wstring, float>>&)> TooltipFormatter =
        [](const std::vector<std::pair<std::wstring, float>>& data) {
        std::wstringstream wss;
        for (size_t i = 0; i < data.size(); ++i) {
            wss << data[i].first << L": " << std::fixed << std::setprecision(1) << (data[i].second * 100.0f) << L" %";
            if (i + 1 < data.size()) wss << L"\n";
        }
        return wss.str();
        };
};

class PerformanceGraph {
private:
    struct LineInternalData {
        LineSettings Current;
        LineSettings Target;
        LineSettings Start;
        PerformanceRingBuffer Data;

        winrt::com_ptr<ID2D1SolidColorBrush> LineBrush;
        winrt::com_ptr<ID2D1SolidColorBrush> FillBrush;
        winrt::com_ptr<ID2D1StrokeStyle> StrokeStyle;
    };

    struct ThresholdInternalData {
        ThresholdLineSettings Settings;
        winrt::com_ptr<ID2D1SolidColorBrush> Brush;
        winrt::com_ptr<ID2D1SolidColorBrush> TextBrush;
        winrt::com_ptr<ID2D1StrokeStyle> StrokeStyle;
    };

    GraphSettings m_current;
    GraphSettings m_target;
    GraphSettings m_start;

    float m_animProgress = 1.0f;
    float m_animDuration = 0.5f;

    std::vector<LineInternalData> m_lines;
    std::vector<ThresholdInternalData> m_thresholds;

    bool m_isHovered = false;
    float m_hoverDipX = 0.0f;
    float m_hoverDipY = 0.0f;
    size_t m_hoverIndex = static_cast<size_t>(-1);
    bool m_isDirty = true;
    uint64_t m_lastRenderTime = 0;
    size_t m_totalPointsAdded = 0;

    float m_logicalWidth = 0.0f;
    float m_logicalHeight = 0.0f;
    float m_compositionScaleX = 1.0f;
    float m_compositionScaleY = 1.0f;

    winrt::Microsoft::UI::Xaml::Controls::SwapChainPanel m_swapChainPanel{ nullptr };

    winrt::Microsoft::UI::Xaml::Controls::SwapChainPanel::SizeChanged_revoker m_sizeRevoker;
    winrt::Microsoft::UI::Xaml::Controls::SwapChainPanel::CompositionScaleChanged_revoker m_scaleRevoker;
    winrt::Microsoft::UI::Xaml::UIElement::PointerMoved_revoker m_ptrMovedRevoker;
    winrt::Microsoft::UI::Xaml::UIElement::PointerExited_revoker m_ptrExitedRevoker;
    winrt::Microsoft::UI::Xaml::Media::CompositionTarget::Rendering_revoker m_renderRevoker;

    winrt::com_ptr<ID3D11Device> m_d3dDevice;
    winrt::com_ptr<ID3D11DeviceContext> m_d3dContext;
    winrt::com_ptr<IDXGISwapChain2> m_swapChain;

    winrt::com_ptr<ID2D1Factory1> m_pD2DFactory;
    winrt::com_ptr<ID2D1Device> m_d2dDevice;
    winrt::com_ptr<ID2D1DeviceContext> m_d2dContext;
    winrt::com_ptr<ID2D1Bitmap1> m_d2dTargetBitmap;

    winrt::com_ptr<ID2D1SolidColorBrush> m_pBgBrush;
    winrt::com_ptr<ID2D1SolidColorBrush> m_pGridBrush;
    winrt::com_ptr<ID2D1SolidColorBrush> m_pTooltipBgBrush;
    winrt::com_ptr<ID2D1SolidColorBrush> m_pTooltipBorderBrush;
    winrt::com_ptr<ID2D1SolidColorBrush> m_pTooltipTextBrush;
    winrt::com_ptr<ID2D1SolidColorBrush> m_pHoverLineBrush;

    winrt::com_ptr<IDWriteFactory> m_pDWriteFactory;
    winrt::com_ptr<IDWriteTextFormat> m_pTextFormat;

    void CreateTextFormat() {
        if (!m_pDWriteFactory) return;
        m_pTextFormat = nullptr;
        THROW_IF_FAILED(m_pDWriteFactory->CreateTextFormat(
            m_current.FontFamily.c_str(), nullptr,
            DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
            m_current.FontSize, L"en-us", m_pTextFormat.put()
        ));
        m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
        m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
    }

    void CreateDeviceIndependentResources() {
        if (!m_pD2DFactory) {
            D2D1_FACTORY_OPTIONS options{};
#if defined(_DEBUG)
            options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif
            THROW_IF_FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory1), &options, m_pD2DFactory.put_void()));
        }
        if (!m_pDWriteFactory) {
            THROW_IF_FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(m_pDWriteFactory.put())));
            CreateTextFormat();
        }
    }

    winrt::com_ptr<ID2D1StrokeStyle> CreateStrokeStyleObj(GraphLineStyle style, const std::vector<float>& customDashes) {
        if (style == GraphLineStyle::Solid || !m_pD2DFactory) return nullptr;
        winrt::com_ptr<ID2D1StrokeStyle> strokeStyle;
        D2D1_STROKE_STYLE_PROPERTIES props = D2D1::StrokeStyleProperties(
            D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE_ROUND,
            D2D1_LINE_JOIN_MITER, 10.0f,
            D2D1_DASH_STYLE_CUSTOM, 0.0f);
        std::vector<float> dashes;
        switch (style) {
        case GraphLineStyle::Dashed:
            dashes = { 4.0f, 4.0f };
            break;
        case GraphLineStyle::Dotted:
            dashes = { 1.0f, 3.0f };
            props.dashCap = D2D1_CAP_STYLE_ROUND;
            props.startCap = D2D1_CAP_STYLE_ROUND;
            props.endCap = D2D1_CAP_STYLE_ROUND;
            break;
        case GraphLineStyle::Custom:
            dashes = customDashes;
            break;
        default:
            break;
        }
        if (!dashes.empty()) {
            THROW_IF_FAILED(m_pD2DFactory->CreateStrokeStyle(
                &props, dashes.data(), static_cast<UINT32>(dashes.size()), strokeStyle.put()));
        }
        return strokeStyle;
    }

    void CreateDeviceResources() {
        UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
        creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        D3D_FEATURE_LEVEL featureLevels[] = {
            D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0
        };
        winrt::com_ptr<ID3D11Device> device;
        winrt::com_ptr<ID3D11DeviceContext> context;
        THROW_IF_FAILED(D3D11CreateDevice(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, creationFlags,
            featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
            device.put(), nullptr, context.put()
        ));
        m_d3dDevice = device;
        m_d3dContext = context;
        winrt::com_ptr<IDXGIDevice> dxgiDevice = m_d3dDevice.as<IDXGIDevice>();
        THROW_IF_FAILED(m_pD2DFactory->CreateDevice(dxgiDevice.get(), m_d2dDevice.put()));
        THROW_IF_FAILED(m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, m_d2dContext.put()));
    }

    void CreateWindowSizeDependentResources() {
        if (m_logicalWidth <= 0 || m_logicalHeight <= 0) return;
        m_d2dContext->SetTarget(nullptr);
        m_d2dTargetBitmap = nullptr;
        DiscardDeviceResources();
        UINT renderWidth = static_cast<UINT>(std::max(1.0f, std::round(m_logicalWidth * m_compositionScaleX)));
        UINT renderHeight = static_cast<UINT>(std::max(1.0f, std::round(m_logicalHeight * m_compositionScaleY)));
        if (m_swapChain) {
            HRESULT hr = m_swapChain->ResizeBuffers(2, renderWidth, renderHeight, DXGI_FORMAT_B8G8R8A8_UNORM, 0);
            if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
                CreateDeviceResources();
                CreateWindowSizeDependentResources();
                return;
            }
            THROW_IF_FAILED(hr);
        }
        else {
            DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
            swapChainDesc.Width = renderWidth;
            swapChainDesc.Height = renderHeight;
            swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
            swapChainDesc.Stereo = false;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.BufferCount = 2;
            swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
            swapChainDesc.Flags = 0;
            swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
            winrt::com_ptr<IDXGIDevice2> dxgiDevice = m_d3dDevice.as<IDXGIDevice2>();
            winrt::com_ptr<IDXGIAdapter> dxgiAdapter;
            THROW_IF_FAILED(dxgiDevice->GetAdapter(dxgiAdapter.put()));
            winrt::com_ptr<IDXGIFactory2> dxgiFactory;
            THROW_IF_FAILED(dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), dxgiFactory.put_void()));

            winrt::com_ptr<IDXGISwapChain1> swapChain1;
            THROW_IF_FAILED(dxgiFactory->CreateSwapChainForComposition(
                m_d3dDevice.get(), &swapChainDesc, nullptr, swapChain1.put()
            ));
            m_swapChain = swapChain1.as<IDXGISwapChain2>();
            auto panelNative = m_swapChainPanel.as<ISwapChainPanelNative>();
            THROW_IF_FAILED(panelNative->SetSwapChain(m_swapChain.get()));
        }
        DXGI_MATRIX_3X2_F inverseScale = { 0 };
        inverseScale._11 = 1.0f / m_compositionScaleX;
        inverseScale._22 = 1.0f / m_compositionScaleY;
        THROW_IF_FAILED(m_swapChain->SetMatrixTransform(&inverseScale));
        winrt::com_ptr<IDXGISurface> dxgiBackBuffer;
        THROW_IF_FAILED(m_swapChain->GetBuffer(0, __uuidof(IDXGISurface), dxgiBackBuffer.put_void()));
        D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
            m_compositionScaleX * 96.0f,
            m_compositionScaleY * 96.0f
        );
        THROW_IF_FAILED(m_d2dContext->CreateBitmapFromDxgiSurface(dxgiBackBuffer.get(), &bitmapProperties, m_d2dTargetBitmap.put()));
        m_d2dContext->SetTarget(m_d2dTargetBitmap.get());
        m_d2dContext->SetDpi(m_compositionScaleX * 96.0f, m_compositionScaleY * 96.0f);
        m_isDirty = true;
    }

    HRESULT EnsureDeviceDependentResources() noexcept {
        if (!m_d2dContext) return S_OK;

        if (!m_pBgBrush) RETURN_IF_FAILED(m_d2dContext->CreateSolidColorBrush(m_current.BgColor, m_pBgBrush.put()));
        if (!m_pGridBrush) RETURN_IF_FAILED(m_d2dContext->CreateSolidColorBrush(m_current.GridColor, m_pGridBrush.put()));
        if (!m_pTooltipBgBrush) RETURN_IF_FAILED(m_d2dContext->CreateSolidColorBrush(m_current.TooltipBgColor, m_pTooltipBgBrush.put()));
        if (!m_pTooltipBorderBrush) RETURN_IF_FAILED(m_d2dContext->CreateSolidColorBrush(m_current.TooltipBorderColor, m_pTooltipBorderBrush.put()));
        if (!m_pTooltipTextBrush) RETURN_IF_FAILED(m_d2dContext->CreateSolidColorBrush(m_current.TooltipTextColor, m_pTooltipTextBrush.put()));
        if (!m_pHoverLineBrush) RETURN_IF_FAILED(m_d2dContext->CreateSolidColorBrush(m_current.HoverLineColor, m_pHoverLineBrush.put()));

        for (auto& line : m_lines) {
            if (!line.LineBrush) {
                RETURN_IF_FAILED(m_d2dContext->CreateSolidColorBrush(line.Current.LineColor, line.LineBrush.put()));
            }
            if (!line.FillBrush) {
                RETURN_IF_FAILED(m_d2dContext->CreateSolidColorBrush(line.Current.FillColor, line.FillBrush.put()));
            }
        }

        for (auto& threshold : m_thresholds) {
            if (!threshold.Brush) {
                RETURN_IF_FAILED(m_d2dContext->CreateSolidColorBrush(threshold.Settings.LineColor, threshold.Brush.put()));
            }
            if (!threshold.TextBrush) {
                RETURN_IF_FAILED(m_d2dContext->CreateSolidColorBrush(threshold.Settings.TextColor, threshold.TextBrush.put()));
            }
        }

        return S_OK;
    }

    void DiscardDeviceResources() noexcept {
        m_pBgBrush = nullptr;
        m_pGridBrush = nullptr;
        m_pTooltipBgBrush = nullptr;
        m_pTooltipBorderBrush = nullptr;
        m_pTooltipTextBrush = nullptr;
        m_pHoverLineBrush = nullptr;

        for (auto& line : m_lines) {
            line.LineBrush = nullptr;
            line.FillBrush = nullptr;
        }

        for (auto& threshold : m_thresholds) {
            threshold.Brush = nullptr;
            threshold.TextBrush = nullptr;
        }
    }

    void OnRendering() {
        LARGE_INTEGER qpc;
        QueryPerformanceCounter(&qpc);
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);

        if (m_lastRenderTime == 0) m_lastRenderTime = qpc.QuadPart;
        float deltaTime = static_cast<float>(qpc.QuadPart - m_lastRenderTime) / static_cast<float>(freq.QuadPart);
        m_lastRenderTime = qpc.QuadPart;

        if (Update(deltaTime)) {
            m_isDirty = true;
        }

        if (m_isDirty) {
            Render();
            m_isDirty = false;
        }
    }

    void Render() {
        if (!m_d2dContext || m_logicalWidth <= 0 || m_logicalHeight <= 0) return;
        if (FAILED(EnsureDeviceDependentResources())) return;
        m_d2dContext->BeginDraw();
        D2D1_RECT_F rect = D2D1::RectF(0, 0, m_logicalWidth, m_logicalHeight);
        if (m_pBgBrush) m_pBgBrush->SetColor(m_current.BgColor);
        if (m_pGridBrush) m_pGridBrush->SetColor(m_current.GridColor);
        if (m_pTooltipBgBrush) m_pTooltipBgBrush->SetColor(m_current.TooltipBgColor);
        if (m_pTooltipBorderBrush) m_pTooltipBorderBrush->SetColor(m_current.TooltipBorderColor);
        if (m_pTooltipTextBrush) m_pTooltipTextBrush->SetColor(m_current.TooltipTextColor);
        if (m_pHoverLineBrush) m_pHoverLineBrush->SetColor(m_current.HoverLineColor);
        for (auto& line : m_lines) {
            if (line.LineBrush) line.LineBrush->SetColor(line.Current.LineColor);
            if (line.FillBrush) line.FillBrush->SetColor(line.Current.FillColor);
        }
        m_d2dContext->Clear(m_current.BgColor);
        float width = rect.right - rect.left;
        float height = rect.bottom - rect.top;
        float stepX = m_current.PointSpacing;
        if (m_current.ShowGrid && m_current.GridCellWidth > 0 && m_current.GridCellHeight > 0) {
            float gridOffsetX = 0.0f;
            if (m_current.MoveGridWithLines) {
                gridOffsetX = fmodf(static_cast<float>(m_totalPointsAdded) * stepX, m_current.GridCellWidth);
            }
            for (float x = rect.right - gridOffsetX; x >= rect.left; x -= m_current.GridCellWidth) {
                m_d2dContext->DrawLine(D2D1::Point2F(x, rect.top), D2D1::Point2F(x, rect.bottom), m_pGridBrush.get(), 0.5f);
            }
            for (float y = rect.bottom; y >= rect.top; y -= m_current.GridCellHeight) {
                m_d2dContext->DrawLine(D2D1::Point2F(rect.left, y), D2D1::Point2F(rect.right, y), m_pGridBrush.get(), 0.5f);
            }
        }
        for (auto& line : m_lines) {
            if (!line.Current.IsVisible || line.Data.Size() < 2) continue;
            winrt::com_ptr<ID2D1PathGeometry> pFillGeometry;
            winrt::com_ptr<ID2D1PathGeometry> pLineGeometry;
            THROW_IF_FAILED(m_pD2DFactory->CreatePathGeometry(pFillGeometry.put()));
            THROW_IF_FAILED(m_pD2DFactory->CreatePathGeometry(pLineGeometry.put()));
            winrt::com_ptr<ID2D1GeometrySink> pFillSink;
            winrt::com_ptr<ID2D1GeometrySink> pLineSink;
            THROW_IF_FAILED(pFillGeometry->Open(pFillSink.put()));
            THROW_IF_FAILED(pLineGeometry->Open(pLineSink.put()));
            float firstX = rect.right - static_cast<float>(line.Data.Size() - 1) * stepX;
            float firstY = rect.bottom - (line.Data.At(0) * height);
            pFillSink->BeginFigure(D2D1::Point2F(firstX, rect.bottom), D2D1_FIGURE_BEGIN_FILLED);
            pFillSink->AddLine(D2D1::Point2F(firstX, firstY));
            pLineSink->BeginFigure(D2D1::Point2F(firstX, rect.bottom), D2D1_FIGURE_BEGIN_HOLLOW);
            pLineSink->AddLine(D2D1::Point2F(firstX, firstY));
            for (size_t i = 1; i < line.Data.Size(); ++i) {
                float x = rect.right - static_cast<float>(line.Data.Size() - 1 - i) * stepX;
                float y = rect.bottom - (line.Data.At(i) * height);
                D2D1_POINT_2F pt = D2D1::Point2F(x, y);
                pFillSink->AddLine(pt);
                pLineSink->AddLine(pt);
            }
            pLineSink->EndFigure(D2D1_FIGURE_END_OPEN);
            THROW_IF_FAILED(pLineSink->Close());
            float lastX = rect.right;
            pFillSink->AddLine(D2D1::Point2F(lastX, rect.bottom));
            pFillSink->EndFigure(D2D1_FIGURE_END_CLOSED);
            THROW_IF_FAILED(pFillSink->Close());
            if (line.Current.ShowFill && line.FillBrush) {
                m_d2dContext->FillGeometry(pFillGeometry.get(), line.FillBrush.get());
            }
            if (line.LineBrush) {
                m_d2dContext->DrawGeometry(pLineGeometry.get(), line.LineBrush.get(), line.Current.LineThickness, line.StrokeStyle.get());
            }
        }
        for (const auto& threshold : m_thresholds) {
            if (!threshold.Brush) continue;
            float y = rect.bottom - (std::clamp(threshold.Settings.Value, 0.0f, 1.0f) * height);
            m_d2dContext->DrawLine(
                D2D1::Point2F(rect.left, y),
                D2D1::Point2F(rect.right, y),
                threshold.Brush.get(),
                threshold.Settings.LineThickness,
                threshold.StrokeStyle.get()
            );
            if (threshold.Settings.ShowName && !threshold.Settings.Name.empty() && m_pTextFormat && threshold.TextBrush) {
                winrt::com_ptr<IDWriteTextLayout> pTextLayout;
                HRESULT hrLayout = m_pDWriteFactory->CreateTextLayout(
                    threshold.Settings.Name.c_str(),
                    static_cast<UINT32>(threshold.Settings.Name.length()),
                    m_pTextFormat.get(),
                    m_logicalWidth, 
                    m_logicalHeight, 
                    pTextLayout.put()
                );
                if (SUCCEEDED(hrLayout)) {
                    DWRITE_TEXT_METRICS metrics;
                    pTextLayout->GetMetrics(&metrics);
                    float paddingX = 6.0f;
                    float paddingY = 2.0f;
                    float textX = rect.left + paddingX; 
                    if (threshold.Settings.NameAlignment == ThresholdNameAlignment::Right) {
                        textX = rect.right - metrics.width - paddingX;
                    }
                    float textY = y - metrics.height - paddingY;
                    if (textY < rect.top) {
                        textY = y + paddingY;
                    }
                    m_d2dContext->DrawTextLayout(
                        D2D1::Point2F(textX, textY),
                        pTextLayout.get(),
                        threshold.TextBrush.get(),
                        D2D1_DRAW_TEXT_OPTIONS_NONE
                    );
                }
            }
        }
        m_d2dContext->DrawRectangle(rect, m_pGridBrush.get(), 1.0f);
        if (m_isHovered && !m_lines.empty() && m_lines[0].Data.Size() > 1 && m_hoverIndex != static_cast<size_t>(-1)) {
            float pointX = rect.right - static_cast<float>(m_lines[0].Data.Size() - 1 - m_hoverIndex) * stepX;
            if (m_current.ShowHoverLine) {
                m_d2dContext->DrawLine(D2D1::Point2F(pointX, rect.top), D2D1::Point2F(pointX, rect.bottom), m_pHoverLineBrush.get(), 1.0f);
            }
            std::vector<std::pair<std::wstring, float>> hoverData;
            hoverData.reserve(m_lines.size());
            for (auto& line : m_lines) {
                if (!line.Current.IsVisible || line.Data.Size() < 2) continue;
                size_t safeIndex = std::min(m_hoverIndex, line.Data.Size() - 1);
                float val = line.Data.At(safeIndex);
                float pointY = rect.bottom - (val * height);
                if (m_current.ShowHoverPoints) {
                    D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(pointX, pointY), 4.0f, 4.0f);
                    m_d2dContext->FillEllipse(ellipse, m_pBgBrush.get());
                    m_d2dContext->DrawEllipse(ellipse, line.LineBrush.get(), 2.0f);
                }
                hoverData.emplace_back(line.Current.Name, val);
            }
            if (m_current.ShowTooltip && m_pTextFormat && m_current.TooltipFormatter && !hoverData.empty()) {
                std::wstring tooltipText = m_current.TooltipFormatter(hoverData);
                winrt::com_ptr<IDWriteTextLayout> pTextLayout;
                THROW_IF_FAILED(m_pDWriteFactory->CreateTextLayout(
                    tooltipText.c_str(), static_cast<UINT32>(tooltipText.length()),
                    m_pTextFormat.get(), 1000.0f, 1000.0f, pTextLayout.put()
                ));
                DWRITE_TEXT_METRICS metrics;
                THROW_IF_FAILED(pTextLayout->GetMetrics(&metrics));
                float paddingX = m_current.TooltipPaddingX;
                float paddingY = m_current.TooltipPaddingY;
                float boxWidth = metrics.width + paddingX * 2.0f;
                float boxHeight = metrics.height + paddingY * 2.0f;
                float tooltipX = pointX + 12.0f;
                float tooltipY = m_hoverDipY - boxHeight - 10.0f;
                if (tooltipX + boxWidth > rect.right) tooltipX = pointX - boxWidth - 12.0f;
                if (tooltipY < rect.top) tooltipY = m_hoverDipY + 12.0f;
                D2D1_RECT_F tooltipRect = D2D1::RectF(tooltipX, tooltipY, tooltipX + boxWidth, tooltipY + boxHeight);
                D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(tooltipRect, m_current.TooltipCornerRadius, m_current.TooltipCornerRadius);
                m_d2dContext->FillRoundedRectangle(roundedRect, m_pTooltipBgBrush.get());
                m_d2dContext->DrawRoundedRectangle(roundedRect, m_pTooltipBorderBrush.get(), 1.0f);
                D2D1_RECT_F textRect = D2D1::RectF(tooltipX + paddingX, tooltipY + paddingY, tooltipRect.right, tooltipRect.bottom);
                m_d2dContext->DrawTextLayout(D2D1::Point2F(textRect.left, textRect.top), pTextLayout.get(), m_pTooltipTextBrush.get(), D2D1_DRAW_TEXT_OPTIONS_NONE);
            }
        }
        HRESULT hr = m_d2dContext->EndDraw();
        if (hr == D2DERR_RECREATE_TARGET) {
            DiscardDeviceResources();
            CreateWindowSizeDependentResources();
        }
        else {
            THROW_IF_FAILED(hr);
            DXGI_PRESENT_PARAMETERS parameters = { 0 };
            m_swapChain->Present1(1, 0, &parameters);
        }
    }
public:
    explicit PerformanceGraph(const GraphSettings& initialSettings) {
        m_current = m_target = m_start = initialSettings;
        CreateDeviceIndependentResources();
    }
    void Initialize(winrt::Microsoft::UI::Xaml::Controls::SwapChainPanel panel) {
        m_swapChainPanel = panel;
        m_logicalWidth = static_cast<float>(m_swapChainPanel.ActualWidth());
        m_logicalHeight = static_cast<float>(m_swapChainPanel.ActualHeight());
        m_compositionScaleX = m_swapChainPanel.CompositionScaleX();
        m_compositionScaleY = m_swapChainPanel.CompositionScaleY();
        CreateDeviceResources();
        CreateWindowSizeDependentResources();
        m_sizeRevoker = m_swapChainPanel.SizeChanged(winrt::auto_revoke, [this](auto&&, winrt::Microsoft::UI::Xaml::SizeChangedEventArgs const& e) {
            m_logicalWidth = e.NewSize().Width;
            m_logicalHeight = e.NewSize().Height;
            CreateWindowSizeDependentResources();
            });
        m_scaleRevoker = m_swapChainPanel.CompositionScaleChanged(winrt::auto_revoke, [this](auto&&, auto&&) {
            m_compositionScaleX = m_swapChainPanel.CompositionScaleX();
            m_compositionScaleY = m_swapChainPanel.CompositionScaleY();
            CreateWindowSizeDependentResources();
            });
        m_ptrMovedRevoker = m_swapChainPanel.PointerMoved(winrt::auto_revoke, [this](auto&&, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e) {
            auto pt = e.GetCurrentPoint(m_swapChainPanel).Position();
            UpdateHoverState(true, pt.X, pt.Y);
            });
        m_ptrExitedRevoker = m_swapChainPanel.PointerExited(winrt::auto_revoke, [this](auto&&, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const&) {
            UpdateHoverState(false, 0, 0);
            });
        m_renderRevoker = winrt::Microsoft::UI::Xaml::Media::CompositionTarget::Rendering(winrt::auto_revoke, [this](auto&&, auto&&) {
            OnRendering();
            });
    }
    size_t AddLine(const LineSettings& settings) {
        LineInternalData line;
        line.Current = line.Target = line.Start = settings;
        line.Data.Resize(m_current.MaxPoints);
        line.StrokeStyle = CreateStrokeStyleObj(settings.Style, settings.CustomDashes);
        m_lines.push_back(std::move(line));
        DiscardDeviceResources();
        m_isDirty = true;
        return m_lines.size() - 1;
    }
    void RemoveLine(size_t index) {
        if (index < m_lines.size()) {
            m_lines.erase(m_lines.begin() + index);
            DiscardDeviceResources();
            m_isDirty = true;
        }
    }
    void ClearLines() {
        if (!m_lines.empty()) {
            m_lines.clear();
            DiscardDeviceResources();
            m_isDirty = true;
        }
    }
    size_t AddThresholdLine(const ThresholdLineSettings& settings) {
        ThresholdInternalData threshold;
        threshold.Settings = settings;
        threshold.StrokeStyle = CreateStrokeStyleObj(settings.Style, settings.CustomDashes);

        m_thresholds.push_back(std::move(threshold));

        DiscardDeviceResources();
        m_isDirty = true;
        return m_thresholds.size() - 1;
    }
    void RemoveThresholdLine(size_t index) {
        if (index < m_thresholds.size()) {
            m_thresholds.erase(m_thresholds.begin() + index);
            DiscardDeviceResources();
            m_isDirty = true;
        }
    }
    void ClearThresholds() {
        if (!m_thresholds.empty()) {
            m_thresholds.clear();
            DiscardDeviceResources();
            m_isDirty = true;
        }
    }
    void ApplySettings(const GraphSettings& newGraphSettings, const std::vector<LineSettings>& newLineSettings, bool animate = true) {
        if (animate) {
            m_start = m_current;
            m_target = newGraphSettings;
            m_current.ShowGrid = newGraphSettings.ShowGrid;
            m_current.MoveGridWithLines = newGraphSettings.MoveGridWithLines;
            m_current.ShowTooltip = newGraphSettings.ShowTooltip;
            m_current.ShowHoverLine = newGraphSettings.ShowHoverLine;
            m_current.ShowHoverPoints = newGraphSettings.ShowHoverPoints;
            m_current.TooltipFormatter = newGraphSettings.TooltipFormatter;
            m_current.TooltipPaddingX = newGraphSettings.TooltipPaddingX;
            m_current.TooltipPaddingY = newGraphSettings.TooltipPaddingY;
            m_current.TooltipCornerRadius = newGraphSettings.TooltipCornerRadius;
            if (m_current.FontFamily != newGraphSettings.FontFamily || m_current.FontSize != newGraphSettings.FontSize) {
                m_current.FontFamily = newGraphSettings.FontFamily;
                m_current.FontSize = newGraphSettings.FontSize;
                CreateTextFormat();
            }
            if (m_current.MaxPoints != m_target.MaxPoints) {
                m_current.MaxPoints = m_target.MaxPoints;
                for (auto& line : m_lines) {
                    line.Data.Resize(m_current.MaxPoints);
                }
            }
            for (size_t i = 0; i < m_lines.size() && i < newLineSettings.size(); ++i) {
                m_lines[i].Start = m_lines[i].Current;
                m_lines[i].Target = newLineSettings[i];
                m_lines[i].Current.ShowFill = newLineSettings[i].ShowFill;
                m_lines[i].Current.IsVisible = newLineSettings[i].IsVisible;
                m_lines[i].Current.Name = newLineSettings[i].Name;
                if (m_lines[i].Current.Style != newLineSettings[i].Style ||
                    m_lines[i].Current.CustomDashes != newLineSettings[i].CustomDashes) {
                    m_lines[i].Current.Style = newLineSettings[i].Style;
                    m_lines[i].Current.CustomDashes = newLineSettings[i].CustomDashes;
                    m_lines[i].StrokeStyle = CreateStrokeStyleObj(m_lines[i].Current.Style, m_lines[i].Current.CustomDashes);
                }
                m_lines[i].Current.LineThickness = newLineSettings[i].LineThickness;
            }
            m_animProgress = 0.0f;
        }
        else {
            if (m_current.FontFamily != newGraphSettings.FontFamily || m_current.FontSize != newGraphSettings.FontSize) {
                m_current.FontFamily = newGraphSettings.FontFamily;
                m_current.FontSize = newGraphSettings.FontSize;
                CreateTextFormat();
            }
            m_current = m_target = m_start = newGraphSettings;
            for (size_t i = 0; i < m_lines.size() && i < newLineSettings.size(); ++i) {
                m_lines[i].Current = m_lines[i].Target = m_lines[i].Start = newLineSettings[i];
                m_lines[i].Data.Resize(m_current.MaxPoints);
                m_lines[i].StrokeStyle = CreateStrokeStyleObj(m_lines[i].Current.Style, m_lines[i].Current.CustomDashes);
            }
            m_animProgress = 1.0f;
        }
        m_isDirty = true;
    }
    bool Update(float deltaTime) noexcept {
        if (m_animProgress < 1.0f) {
            m_animProgress += deltaTime / m_animDuration;
            if (m_animProgress > 1.0f) m_animProgress = 1.0f;
            float t = 1.0f - std::pow(1.0f - m_animProgress, 3.0f);
            m_current.BgColor = LerpColor(m_start.BgColor, m_target.BgColor, t);
            m_current.GridColor = LerpColor(m_start.GridColor, m_target.GridColor, t);
            m_current.TooltipBgColor = LerpColor(m_start.TooltipBgColor, m_target.TooltipBgColor, t);
            m_current.TooltipBorderColor = LerpColor(m_start.TooltipBorderColor, m_target.TooltipBorderColor, t);
            m_current.TooltipTextColor = LerpColor(m_start.TooltipTextColor, m_target.TooltipTextColor, t);
            m_current.HoverLineColor = LerpColor(m_start.HoverLineColor, m_target.HoverLineColor, t);
            m_current.GridCellWidth = Lerp(m_start.GridCellWidth, m_target.GridCellWidth, t);
            m_current.GridCellHeight = Lerp(m_start.GridCellHeight, m_target.GridCellHeight, t);
            m_current.PointSpacing = Lerp(m_start.PointSpacing, m_target.PointSpacing, t);
            for (auto& line : m_lines) {
                line.Current.LineColor = LerpColor(line.Start.LineColor, line.Target.LineColor, t);
                line.Current.FillColor = LerpColor(line.Start.FillColor, line.Target.FillColor, t);
            }
            return true;
        }
        return false;
    }
    void AddPoint(size_t lineIndex, float value) noexcept {
        if (lineIndex >= m_lines.size()) return;
        float clampedValue = std::clamp(value, 0.0f, 1.0f);
        float prevValue = m_lines[lineIndex].Data.Size() > 0 ?
            m_lines[lineIndex].Data.At(m_lines[lineIndex].Data.Size() - 1) : 0.0f;
        m_lines[lineIndex].Data.PushBack(clampedValue);
        for (const auto& threshold : m_thresholds) {
            if (threshold.Settings.OnTrigger) {
                if (clampedValue >= threshold.Settings.Value && prevValue < threshold.Settings.Value) {
                    threshold.Settings.OnTrigger(lineIndex, clampedValue);
                }
            }
        }
        if (lineIndex == 0) {
            m_totalPointsAdded++;
        }
        m_isDirty = true;
    }
    void UpdateHoverState(bool isHovered, float dipX, float dipY) noexcept {
        if (m_isHovered != isHovered || m_hoverDipX != dipX || m_hoverDipY != dipY) {
            m_isDirty = true;
        }
        m_isHovered = isHovered && (dipX >= 0 && dipX <= m_logicalWidth && dipY >= 0 && dipY <= m_logicalHeight);
        if (m_isHovered && !m_lines.empty() && m_lines[0].Data.Size() > 1) {
            m_hoverDipX = dipX;
            m_hoverDipY = dipY;
            float stepX = m_current.PointSpacing;
            float distanceFromRight = m_logicalWidth - dipX;
            int idx = static_cast<int>(m_lines[0].Data.Size() - 1) - static_cast<int>(std::roundf(distanceFromRight / stepX));
            if (idx >= 0 && idx < static_cast<int>(m_lines[0].Data.Size())) {
                m_hoverIndex = static_cast<size_t>(idx);
            }
            else {
                m_hoverIndex = static_cast<size_t>(-1);
            }
        }
    }
};