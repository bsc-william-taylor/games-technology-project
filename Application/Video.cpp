
#include "Video.h"

Video::Video()
{

}

Video::~Video()
{
}

HRESULT Video::InitializeEVR(IBaseFilter * pEVR, HWND hwnd, IMFVideoDisplayControl ** ppDisplayControl)
{
	IMFVideoDisplayControl * Display = NULL;
	IMFGetService * pGS = NULL;

	pEVR->QueryInterface(IID_PPV_ARGS(&pGS));
	pGS->GetService(MR_VIDEO_RENDER_SERVICE, IID_PPV_ARGS(&Display));

	Display->SetVideoWindow(hwnd);
	Display->SetAspectRatioMode(0);

	*ppDisplayControl = Display;
	(*ppDisplayControl)->AddRef();

	if (pGS != NULL)
	{
		pGS->Release();
	}

	if (Display != NULL)
	{
		Display->Release();
	}

	return S_OK;
}

HRESULT Video::RemoveUnconnectedRenderer(IGraphBuilder *pGraph, IBaseFilter *pRenderer)
{
	IPin *pPin = NULL;

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
	IPin *pTmp = NULL;
	HRESULT hr = pPin->ConnectedTo(&pTmp);
	if (SUCCEEDED(hr))
	{
		*pResult = TRUE;
	}
	else if (hr == VFW_E_NOT_CONNECTED)
	{
		// The pin is not connected. This is not an error for our purposes.
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
	*ppPin = NULL;

	IEnumPins *pEnum = NULL;
	IPin *pPin = NULL;

	HRESULT hr = pFilter->EnumPins(&pEnum);
	if (FAILED(hr))
	{
		return hr;
	}

	BOOL bFound = FALSE;
	while (S_OK == pEnum->Next(1, &pPin, NULL))
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
	IBaseFilter * pFilter = NULL;
	*ppF = NULL;

	HRESULT hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFilter));

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

	mediaEvents->SetNotifyWindow((OAHWND)NULL, NULL, NULL);
	mediaEvents->Release();

	videoDisplayControl->Release();
	evrBaseFilter->Release();
}

void Video::streamFrom(std::string f)
{
	BOOL RenderedAnyPin = FALSE;
	IFilterGraph2 * Graph2 = NULL;
	IBaseFilter * AudioRenderer = NULL;
	IBaseFilter * pEVR = NULL;
	IBaseFilter * Source = NULL;
	IEnumPins * Enum = NULL;
	IPin * Pin = NULL;

	int slength = (int)f.length() + 1;
	int Length = MultiByteToWideChar(CP_ACP, 0, f.c_str(), slength, 0, 0);
	
	wchar_t * wideBuffer = new wchar_t[Length];
	
	MultiByteToWideChar(CP_ACP, 0, f.c_str(), slength, wideBuffer, Length);
	filename = std::wstring(wideBuffer);
	
	delete[] wideBuffer;

	playbackState = STATE_STOPPED;
	wndHwnd = GetActiveWindow();

	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&graphBuilder));

	graphBuilder->QueryInterface(IID_IMediaControl, (void**)&mediaControl);
	graphBuilder->QueryInterface(IID_IMediaEventEx, (void**)&mediaEvents);
	graphBuilder->QueryInterface(IID_IMediaSeeking, (void**)&mediaSeeking);

	graphBuilder->AddSourceFilter(filename.c_str(), NULL, &Source);
	graphBuilder->QueryInterface(IID_PPV_ARGS(&Graph2));
	show = TRUE;

	AddFilterByCLSID(graphBuilder, CLSID_EnhancedVideoRenderer, &pEVR, L"EVR");
	InitializeEVR(pEVR, wndHwnd, &videoDisplayControl);

	evrBaseFilter = pEVR;
	evrBaseFilter->AddRef();

	AddFilterByCLSID(graphBuilder, CLSID_DSoundRender, &AudioRenderer, L"Audio Renderer");

	Source->EnumPins(&Enum);

	while (S_OK == Enum->Next(1, &Pin, NULL))
	{
		HRESULT Result = Graph2->RenderEx(Pin, AM_RENDEREX_RENDERTOEXISTINGRENDERERS, NULL);

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
	if (playbackState == STATE_RUNNING)
	{
		playbackState = STATE_PAUSED;
		mediaControl->Pause();
	}
}

void Video::stop()
{
	if (playbackState == STATE_RUNNING || playbackState == STATE_PAUSED)
	{
		LONGLONG StopTimes = NULL;
		LONGLONG Start = NULL;

		playbackState = STATE_STOPPED;
		
		mediaControl->Stop();
		mediaSeeking->GetPositions(NULL, &StopTimes);
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
	videoDisplayControl->SetVideoPosition(NULL, &rc);

	hdc = BeginPaint(wndHwnd, &ps);

	if (playbackState != STATE_NO_GRAPH)
	{
		videoDisplayControl->RepaintVideo();
	}

	EndPaint(wndHwnd, &ps);

	if (playbackState == STATE_PAUSED || playbackState == STATE_STOPPED)
	{
		playbackState = STATE_RUNNING;
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
		videoDisplayControl->SetVideoPosition(NULL, &rc);
	}
}