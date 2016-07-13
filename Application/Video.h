
#pragma once

#include "EngineLayer.h"

enum State : unsigned int {
	STATE_NO_GRAPH,
	STATE_RUNNING,
	STATE_STOPPED,
	STATE_PAUSED,
};

class Video
{
private:
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
	~Video();

	void streamFrom(std::string);
	void display();
	void unload();
	void pause();
	void stop();
	void play();
private:
	State playbackState;
	std::wstring filename;
private:
	HRESULT RemoveUnconnectedRenderer(IGraphBuilder *g, IBaseFilter * p);
	HRESULT AddFilterByCLSID(IGraphBuilder * g, REFGUID c, IBaseFilter ** p, LPCWSTR n);
	HRESULT InitializeEVR(IBaseFilter * e, HWND w, IMFVideoDisplayControl ** d);
	
	HRESULT FindConnectedPin(IBaseFilter * f, PIN_DIRECTION p, IPin ** ip);
	HRESULT IsPinDirection(IPin * p, PIN_DIRECTION dir, BOOL * r);
	HRESULT IsPinConnected(IPin * p, BOOL * r);
};