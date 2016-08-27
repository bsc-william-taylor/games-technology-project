
#include "Video.h"

Video::Video()
{
}

Video::~Video()
{
}

HRESULT Video::InitializeEVR(IBaseFilter * pEVR, HWND hwnd, IMFVideoDisplayControl ** ppDisplayControl)
{
    IMFVideoDisplayControl * Display = nullptr;
    IMFGetService * pGS = nullptr;

    pEVR->QueryInterface(IID_PPV_ARGS(&pGS));
    pGS->GetService(MR_VIDEO_RENDER_SERVICE, IID_PPV_ARGS(&Display));

    Display->SetVideoWindow(hwnd);
    Display->SetAspectRatioMode(0);

    *ppDisplayControl = Display;
    (*ppDisplayControl)->AddRef();

    if (pGS != nullptr)
    {
        pGS->Release();
    }

    if (Display != nullptr)
    {
        Display->Release();
    }

    return S_OK;
}

HRESULT Video::RemoveUnconnectedRenderer(IGraphBuilder *pGraph, IBaseFilter *pRenderer)
{
    IPin *pPin = nullptr;

    HRESULT hr = FindConnectedPin(pRenderer, PINDIR_INPUT, &pPin);

    pPin->Release();

    if (FAILED(hr))
    {
        hr = pGraph->RemoveFilter(pRenderer);
    }

    return hr;
}

HRESULT Video::IsPinConnected(IPin *pPin, BOOL *pResult)
{
    IPin *pTmp = nullptr;
    HRESULT hr = pPin->ConnectedTo(&pTmp);
    if (SUCCEEDED(hr))
    {
        *pResult = TRUE;
    }
    else if (hr == VFW_E_NOT_CONNECTED)
    {
        *pResult = FALSE;
        hr = S_OK;
    }

    pTmp->Release();
    return hr;
}

HRESULT Video::IsPinDirection(IPin *pPin, PIN_DIRECTION dir, BOOL *pResult)
{
    PIN_DIRECTION pinDir;
    HRESULT hr = pPin->QueryDirection(&pinDir);
    if (SUCCEEDED(hr))
    {
        *pResult = (pinDir == dir);
    }
    return hr;
}

HRESULT Video::FindConnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin)
{
    *ppPin = nullptr;

    IEnumPins *pEnum = nullptr;
    IPin *pPin = nullptr;

    HRESULT hr = pFilter->EnumPins(&pEnum);
    if (FAILED(hr))
    {
        return hr;
    }

    BOOL bFound = FALSE;
    while (S_OK == pEnum->Next(1, &pPin, nullptr))
    {
        BOOL bIsConnected;
        hr = IsPinConnected(pPin, &bIsConnected);
        if (SUCCEEDED(hr))
        {
            if (bIsConnected)
            {
                hr = IsPinDirection(pPin, PinDir, &bFound);
            }
        }

        if (FAILED(hr))
        {
            pPin->Release();
            break;
        }
        if (bFound)
        {
            *ppPin = pPin;
            break;
        }
        pPin->Release();
    }

    pEnum->Release();

    if (!bFound)
    {
        hr = VFW_E_NOT_FOUND;
    }
    return hr;
}

HRESULT Video::AddFilterByCLSID(IGraphBuilder *pGraph, REFGUID clsid, IBaseFilter **ppF, LPCWSTR wszName)
{
    IBaseFilter * pFilter = nullptr;
    *ppF = nullptr;

    HRESULT hr = CoCreateInstance(clsid, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFilter));

    hr = pGraph->AddFilter(pFilter, wszName);
    *ppF = pFilter;
    (*ppF)->AddRef();

    pFilter->Release();

    return hr;
}

void Video::unload()
{
    graphBuilder->Release();
    mediaControl->Release();
    mediaSeeking->Release();

    mediaEvents->SetNotifyWindow((OAHWND)nullptr, 0, 0);
    mediaEvents->Release();

    videoDisplayControl->Release();
    evrBaseFilter->Release();
}

void Video::streamFrom(std::string f)
{
    BOOL RenderedAnyPin = FALSE;
    IFilterGraph2 * Graph2 = nullptr;
    IBaseFilter * AudioRenderer = nullptr;
    IBaseFilter * pEVR = nullptr;
    IBaseFilter * Source = nullptr;
    IEnumPins * Enum = nullptr;
    IPin * Pin = nullptr;

    auto slength = f.length() + 1;
    auto Length = MultiByteToWideChar(CP_ACP, 0, f.c_str(), slength, nullptr, 0);
    auto wideBuffer = new wchar_t[Length];
    
    MultiByteToWideChar(CP_ACP, 0, f.c_str(), slength, wideBuffer, Length);
    filename = std::wstring(wideBuffer);
    
    delete[] wideBuffer;

    playbackState = StateStopped;
    wndHwnd = GetActiveWindow();

    CoCreateInstance(CLSID_FilterGraph, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&graphBuilder));

    graphBuilder->QueryInterface(IID_IMediaControl, (void**)&mediaControl);
    graphBuilder->QueryInterface(IID_IMediaEventEx, (void**)&mediaEvents);
    graphBuilder->QueryInterface(IID_IMediaSeeking, (void**)&mediaSeeking);

    graphBuilder->AddSourceFilter(filename.c_str(), nullptr, &Source);
    graphBuilder->QueryInterface(IID_PPV_ARGS(&Graph2));
    show = TRUE;

    AddFilterByCLSID(graphBuilder, CLSID_EnhancedVideoRenderer, &pEVR, L"EVR");
    InitializeEVR(pEVR, wndHwnd, &videoDisplayControl);

    evrBaseFilter = pEVR;
    evrBaseFilter->AddRef();

    AddFilterByCLSID(graphBuilder, CLSID_DSoundRender, &AudioRenderer, L"Audio Renderer");

    Source->EnumPins(&Enum);

    while (S_OK == Enum->Next(1, &Pin, nullptr))
    {
        auto Result = Graph2->RenderEx(Pin, AM_RENDEREX_RENDERTOEXISTINGRENDERERS, nullptr);

        Pin->Release();

        if (SUCCEEDED(Result)) {
            RenderedAnyPin = TRUE;
        }
    }

    RemoveUnconnectedRenderer(graphBuilder, evrBaseFilter);
    RemoveUnconnectedRenderer(graphBuilder, AudioRenderer);

    AudioRenderer->Release();
    Graph2->Release();
    Source->Release();
    Enum->Release();
    pEVR->Release();
}

void Video::pause()
{
    if (playbackState == StateRunning)
    {
        playbackState = StatePaused;
        mediaControl->Pause();
    }
}

void Video::stop()
{
    if (playbackState == StateRunning || playbackState == StatePaused)
    {
        auto StopTimes = 0LL;
        auto Start = 0LL;

        playbackState = StateStopped;
        
        mediaControl->Stop();
        mediaSeeking->GetPositions(nullptr, &StopTimes);
        mediaSeeking->SetPositions(&Start, 0x01 | 0x04, &StopTimes, 0x01 | 0x04);
    }


    show = FALSE;
}

void Video::play()
{
    PAINTSTRUCT ps;
    RECT rc;
    HDC hdc;

    GetClientRect(wndHwnd, &rc);
    videoDisplayControl->SetVideoPosition(nullptr, &rc);

    hdc = BeginPaint(wndHwnd, &ps);

    if (playbackState != StateNoGraph)
    {
        videoDisplayControl->RepaintVideo();
    }

    EndPaint(wndHwnd, &ps);

    if (playbackState == StatePaused || playbackState == StateStopped)
    {
        playbackState = StateRunning;
        mediaControl->Run();
    }

    show = TRUE;
}

void Video::display()
{
    if(show == TRUE)
    {
        RECT rc;
        GetClientRect(wndHwnd, &rc);
        videoDisplayControl->SetVideoPosition(nullptr, &rc);
    }
}