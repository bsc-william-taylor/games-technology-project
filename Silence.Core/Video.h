
#pragma once

#include "EngineLayer.h"

enum State 
{
    StateNoGraph,
    StateRunning,
    StateStopped,
    StatePaused,
};

class SILENCE_EXPORT Video
{
    IMFVideoDisplayControl * videoDisplayControl;
    IGraphBuilder * graphBuilder;
    IMediaControl * mediaControl;
    IMediaSeeking * mediaSeeking;
    IMediaEventEx * mediaEvents;
    IBaseFilter * evrBaseFilter;

    HWND wndHwnd;
    BOOL show;
public:
    Video();
    virtual ~Video();

    void streamFrom(std::string);
    void display();
    void unload();
    void pause();
    void stop();
    void play();
private:
    std::wstring filename;
    State playbackState;

    HRESULT RemoveUnconnectedRenderer(IGraphBuilder *, IBaseFilter *);
    HRESULT AddFilterByCLSID(IGraphBuilder *, REFGUID, IBaseFilter **, LPCWSTR);
    HRESULT InitializeEVR(IBaseFilter *, HWND, IMFVideoDisplayControl **);
    HRESULT FindConnectedPin(IBaseFilter *, PIN_DIRECTION, IPin **);
    HRESULT IsPinDirection(IPin *, PIN_DIRECTION, BOOL *);
    HRESULT IsPinConnected(IPin *, BOOL *);
};