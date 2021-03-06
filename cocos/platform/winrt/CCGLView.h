/****************************************************************************
Copyright (c) 2010 cocos2d-x.org
Copyright (c) Microsoft Open Technologies, Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __CC_EGLVIEW_WINRT_H__
#define __CC_EGLVIEW_WINRT_H__

#include "base/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)

#include "CCStdC.h"
#include "CCGL.h"
#include "platform/CCCommon.h"
#include "DirectXBase.h"
#include "platform/CCGLViewProtocol.h"
#include <agile.h>

#include <wrl/client.h>

#include <agile.h>
#include <DirectXMath.h>
#include <mutex>
#include <queue>

NS_CC_BEGIN

class CCEGL;
class GLView;

ref class WinRTWindow sealed : public DirectXBase
{

public:
	WinRTWindow(Windows::UI::Core::CoreWindow^ window, Windows::UI::Xaml::Controls::SwapChainPanel^ panel);

private:
	cocos2d::Vec2 GetCCPoint(Windows::UI::Core::PointerEventArgs^ args);

	void OnPointerWheelChanged(Windows::UI::Core::CoreWindow^, Windows::UI::Core::PointerEventArgs^ args);
	void OnPointerMoved(Windows::UI::Core::CoreWindow^, Windows::UI::Core::PointerEventArgs^ args);
	void OnPointerPressed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args);
	void OnPointerReleased(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args);
	void OnKeyDown(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ args);
	void OnKeyUp(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ args);
	void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args);
    void OnRendering(Platform::Object^ sender, Platform::Object^ args);
    void OnSuspending();
    void ResizeWindow();
    
	// Window event handlers.
	void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);

	// DisplayInformation event handlers.
	void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
	void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
	void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);

	// Other event handlers.	
	void OnCompositionScaleChanged(Windows::UI::Xaml::Controls::SwapChainPanel^ sender, Object^ args);
	void OnSwapChainPanelSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);

	EventKeyboard::KeyCode GetCocosKeyCode(Windows::System::VirtualKey key);	

	void ShowKeyboard(Windows::UI::ViewManagement::InputPane^ inputPane, Windows::UI::ViewManagement::InputPaneVisibilityEventArgs^ args);
	void HideKeyboard(Windows::UI::ViewManagement::InputPane^ inputPane, Windows::UI::ViewManagement::InputPaneVisibilityEventArgs^ args);

	Windows::Foundation::Point m_lastPoint;
	Windows::Foundation::EventRegistrationToken m_eventToken;
	bool m_lastPointValid;
	bool m_textInputEnabled;

    friend GLView;
};

class CC_DLL GLView : public Ref, public GLViewProtocol
{
public:
    GLView();
    virtual ~GLView();

    /* override functions */
    virtual bool isOpenGLReady();
    virtual void end();
    virtual void swapBuffers();
    virtual void setFrameSize(float width, float height);
    virtual void setIMEKeyboardState(bool bOpen);
	void ShowKeyboard(Windows::Foundation::Rect r);
	void HideKeyboard(Windows::Foundation::Rect r);
	virtual bool Create(Windows::UI::Core::CoreWindow^ window, Windows::UI::Xaml::Controls::SwapChainPanel^ panel);
	void UpdateForWindowSizeChange();
	void OnRendering();
    void OnSuspending();
	void OnResume();

private:
	Windows::Foundation::EventRegistrationToken m_eventToken;
	Windows::Foundation::Point m_lastPoint;
	bool m_lastPointValid;

public:

    // winrt platform functions
	Windows::UI::Core::CoreWindow^ getWindow() { return m_window.Get(); };
	
	int Run();

    void resize(int width, int height);
    /* 
     * Set zoom factor for frame. This method is for debugging big resolution (e.g.new ipad) app on desktop.
     */
    void setFrameZoomFactor(float fZoomFactor);
	float getFrameZoomFactor();
    void centerWindow();

	void OnPointerPressed(Windows::UI::Core::PointerEventArgs^ args) {}
	void OnPointerMoved(Windows::UI::Core::PointerEventArgs^ args) {}
	void OnPointerReleased(Windows::UI::Core::PointerEventArgs^ args) {}
	void OnPointerPressed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args) {}
	void OnBackKeyPress() {}
    
    // static function
    /**
    @brief    get the shared main open gl window
    */
	static GLView* sharedOpenGLView();

private:
    Platform::Agile<Windows::UI::Core::CoreWindow> m_window;
	bool m_running;
	bool m_initialized;
    bool m_bSupportTouch;
    float m_fFrameZoomFactor;
	WinRTWindow^ m_winRTWindow;
	Windows::Foundation::Rect m_keyboardRect;

public:
	ID3D11Device1* GetDevice()
	{
		return m_winRTWindow->m_d3dDevice.Get();
	}

	ID3D11DeviceContext1* GetContext()
	{
		return m_winRTWindow->m_d3dContext.Get();
	}

	ID3D11DepthStencilView* GetDepthStencilView()
	{
		return m_winRTWindow->m_depthStencilView.Get();
	}

	ID3D11RenderTargetView*const* GetRenderTargetView() const
	{
		return m_winRTWindow->m_renderTargetView.GetAddressOf();
	}

	IDXGIDevice3* GetDXDevice()
	{
		return m_winRTWindow->m_dxDevice.Get();
	}
};

NS_CC_END

#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)

#endif    // end of __CC_EGLVIEW_WINRT_H__
