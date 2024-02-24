#pragma once

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_freetype.h"

#include "../crypt/xor_string.hpp"
#include "../main.hpp"

namespace render_system
{
    HWND window = NULL;
    ID3D11Device* device = NULL;
    IDXGISwapChain* swap_chain = NULL;
    ID3D11DeviceContext* device_context = NULL;
    ID3D11RenderTargetView* render_target_view = NULL;

    ImFont* main_font;

    ImFont* menu_font;

    ImFont* ammo_font;

    bool initialization()
    {
        while (!window)
        {
            window = FindWindowA(_("MedalOverlayClass"), _("MedalOverlay"));
        }

        ShowWindow(window, SW_MAXIMIZE); UpdateWindow(window);
        MoveWindow(window, 0, 0, screen_x, screen_y, TRUE);

        DXGI_SWAP_CHAIN_DESC swap_chain_desc;
        swap_chain_desc.BufferCount = 2;
        swap_chain_desc.BufferDesc.Width = 0;
        swap_chain_desc.BufferDesc.Height = 0;
        swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swap_chain_desc.BufferDesc.RefreshRate.Numerator = 1000;
        swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
        swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swap_chain_desc.OutputWindow = window;
        swap_chain_desc.SampleDesc.Count = 1;
        swap_chain_desc.SampleDesc.Quality = 0;
        swap_chain_desc.Windowed = true;
        swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        D3D_FEATURE_LEVEL feature_level;
        const D3D_FEATURE_LEVEL feature_level_array[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
        D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, feature_level_array, 2, D3D11_SDK_VERSION, &swap_chain_desc, &swap_chain, &device, &feature_level, &device_context);

        ID3D11Texture2D* back_buffer{ };
        swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer));
        device->CreateRenderTargetView(back_buffer, NULL, &render_target_view);

        back_buffer->Release();
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplWin32_Init(window);
        ImGui_ImplDX11_Init(device, device_context);

        ImGuiStyle* style = &ImGui::GetStyle();
        ImGuiIO* io = &ImGui::GetIO();

        io->IniFilename = nullptr;
        io->LogFilename = nullptr;

        style->Alpha = 1.0f;
        style->WindowBorderSize = 0.f;
        style->ChildBorderSize = 0.f;
        style->WindowTitleAlign = ImVec2(0.5f, 0.5f);

        style->WindowRounding = 5.f;
        style->FrameRounding = 12.f;
        style->ChildRounding = 5.f;

        style->WindowPadding = ImVec2(5.f, 5.f);
        style->ItemSpacing = ImVec2(5.f, 5.f);
        style->GrabMinSize = 18.f;
        style->GrabRounding = 12.f;

        style->Colors[ImGuiCol_Text] = ImColor(240, 240, 240);
        style->Colors[ImGuiCol_TitleBg] = ImColor(70, 70, 70);
        style->Colors[ImGuiCol_TitleBgActive] = ImColor(70, 70, 70);
        style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(70, 70, 70);
        style->Colors[ImGuiCol_WindowBg] = ImColor(25, 25, 25);
        style->Colors[ImGuiCol_CheckMark] = ImColor(70, 70, 70);
        style->Colors[ImGuiCol_Border] = ImColor(70, 70, 70);
        style->Colors[ImGuiCol_Button] = ImColor(32, 32, 32);
        style->Colors[ImGuiCol_ButtonActive] = ImColor(42, 42, 42);
        style->Colors[ImGuiCol_ButtonHovered] = ImColor(42, 42, 42);
        style->Colors[ImGuiCol_ChildBg] = ImColor(42, 42, 42);
        style->Colors[ImGuiCol_FrameBg] = ImColor(32, 32, 32);
        style->Colors[ImGuiCol_FrameBgActive] = ImColor(42, 42, 42);
        style->Colors[ImGuiCol_FrameBgHovered] = ImColor(42, 42, 42);
        style->Colors[ImGuiCol_SliderGrab] = ImColor(255, 255, 255);
        style->Colors[ImGuiCol_SliderGrabActive] = ImColor(255, 255, 255);

        ImFontConfig cfg;

        cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_MonoHinting | ImGuiFreeTypeBuilderFlags_Monochrome;
        main_font = io->Fonts->AddFontFromFileTTF(_("C:\\Windows\\Fonts\\Arial.ttf"), var::font_size, NULL, io->Fonts->GetGlyphRangesCyrillic());

        cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_MonoHinting | ImGuiFreeTypeBuilderFlags_Monochrome;
        menu_font = io->Fonts->AddFontFromFileTTF(_("C:\\Windows\\Fonts\\Arial.ttf"), 14.f, NULL, io->Fonts->GetGlyphRangesCyrillic());

        cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_MonoHinting | ImGuiFreeTypeBuilderFlags_Monochrome;
        ammo_font = io->Fonts->AddFontFromFileTTF(_("C:\\Windows\\Fonts\\Arial.ttf"), 32.f, NULL, io->Fonts->GetGlyphRangesCyrillic());

        return true;
    }

    void start()
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void end()
    {
        ImGui::Render();
        ImVec4 clear = { 0.0f, 0.0f, 0.0f, 0.0f };
        device_context->OMSetRenderTargets(1, &render_target_view, NULL);
        device_context->ClearRenderTargetView(render_target_view, (f32*)&clear);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        swap_chain->Present(1, 0);
    }

    void text(f32 x, f32 y, ImColor color, chars const format, ...)
    {
        va_list list; char text[1024] = { 0 };
        va_start(list, format); _vsnprintf_s(text, sizeof(text), format, list); va_end(list);

        ImGui::GetBackgroundDrawList()->AddText(ImVec2(x - ImGui::CalcTextSize(text).x / 2 + 1, y + 1), IM_COL32_BLACK, text);
        ImGui::GetBackgroundDrawList()->AddText(ImVec2(x - ImGui::CalcTextSize(text).x / 2 - 1, y - 1), IM_COL32_BLACK, text);
        ImGui::GetBackgroundDrawList()->AddText(ImVec2(x - ImGui::CalcTextSize(text).x / 2 + 1, y - 1), IM_COL32_BLACK, text);
        ImGui::GetBackgroundDrawList()->AddText(ImVec2(x - ImGui::CalcTextSize(text).x / 2 - 1, y + 1), IM_COL32_BLACK, text);
        ImGui::GetBackgroundDrawList()->AddText(ImVec2(x - ImGui::CalcTextSize(text).x / 2 + 1, y), IM_COL32_BLACK, text);
        ImGui::GetBackgroundDrawList()->AddText(ImVec2(x - ImGui::CalcTextSize(text).x / 2 - 1, y), IM_COL32_BLACK, text);
        ImGui::GetBackgroundDrawList()->AddText(ImVec2(x - ImGui::CalcTextSize(text).x / 2, y + 1), IM_COL32_BLACK, text);
        ImGui::GetBackgroundDrawList()->AddText(ImVec2(x - ImGui::CalcTextSize(text).x / 2, y - 1), IM_COL32_BLACK, text);
        ImGui::GetForegroundDrawList()->AddText(ImVec2(x - ImGui::CalcTextSize(text).x / 2, y), color, text);

    }



    void line(i32 x, i32 y, i32 x1, i32 y1, ImColor color)
    {
        ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y), ImVec2(x1, y1), color, 1.0f);
    }

    void rect(i32 x, i32 y, i32 w, i32 h, ImColor color)
    {
        ImGui::GetForegroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0, 1.0f);
    }

    void circle(i32 x, i32 y, f32 radius, ImColor color, uint32_t segments)
    {
        ImGui::GetForegroundDrawList()->AddCircle(ImVec2(x, y), radius, color, segments, 1.0f);
    }
}