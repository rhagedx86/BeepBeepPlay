// BeepBeepPlay_Dll.cpp
#include <windows.h>
#include <dshow.h>
#include <string>
#include <thread>
#pragma comment(lib, "strmiids.lib")

std::wstring utf8_to_wstring(const char* str) {
    if (!str) return L"";
    int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
    if (len == 0) return L"";
    std::wstring wstr(len, 0);
    MultiByteToWideChar(CP_UTF8, 0, str, -1, &wstr[0], len);
    wstr.resize(len - 1);
    return wstr;
}

long linearVolumeToDS(float v)
{
    if (v <= 0.0f) return -10000;
    if (v >= 1.0f) return 0;

    float t = 1.0f - v;
    long ds = (long)(-10000.0f * t * t);

    return ds;
}


extern "C" __declspec(dllexport)
int BeepBeepPlay(const char* path, float volume)
{
    if (!path) return 1;

    std::string pathCopy(path);

    std::thread([pathCopy, volume]() {

        HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        if (FAILED(hr)) return;

        IGraphBuilder* pGraph = nullptr;
        IMediaControl* pControl = nullptr;
        IMediaEvent* pEvent = nullptr;
        IBasicAudio* pAudio = nullptr;

        std::wstring wpath = utf8_to_wstring(pathCopy.c_str());
        long dsVolume = linearVolumeToDS(volume);

        hr = CoCreateInstance(CLSID_FilterGraph, nullptr, CLSCTX_INPROC_SERVER,
            IID_IGraphBuilder, (void**)&pGraph);
        if (FAILED(hr)) goto cleanup;

        if (FAILED(pGraph->QueryInterface(IID_IMediaControl, (void**)&pControl))) goto cleanup;
        if (FAILED(pGraph->QueryInterface(IID_IMediaEvent, (void**)&pEvent))) goto cleanup;
        pGraph->QueryInterface(IID_IBasicAudio, (void**)&pAudio);

        hr = pGraph->RenderFile(wpath.c_str(), nullptr);
        if (FAILED(hr)) goto cleanup;

        if (pAudio) pAudio->put_Volume(dsVolume);

        if (FAILED(pControl->Run())) goto cleanup;

        if (pEvent) {
            long evCode = 0;
            pEvent->WaitForCompletion(INFINITE, &evCode);
        }

    cleanup:
        if (pAudio)   pAudio->Release();
        if (pEvent)   pEvent->Release();
        if (pControl) pControl->Release();
        if (pGraph)   pGraph->Release();
        CoUninitialize();

        }).detach();

    return 0;
}
