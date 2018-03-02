#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include <ui/UIWebView.h>
#include "cocos2d.h"

#include <string>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include "AdmobManager.h"

class HelloWorld : public cocos2d::Layer, public AdmobManagerDelegate
{
public:
    
    std::string urlDecode(const std::string &toDecode) {
        std::ostringstream out;
        
        for(std::string::size_type i=0; i < toDecode.length(); ++i) {
            if(toDecode.at(i) == '%') {
                std::string str(toDecode.substr(i+1, 2));
                out << hexToChar(str);
                i += 2;
            } else {
                out << toDecode.at(i);
            }
        }
        
        return out.str();
    }
    
    std::string charToHex(unsigned char c) {
        short i = c;
        
        std::stringstream s;
        
        s << "%" << std::setw(2) << std::setfill('0') << std::hex << i;
        
        return s.str();
    }
    
    unsigned char hexToChar(const std::string &str) {
        short c = 0;
        
        if(!str.empty()) {
            std::istringstream in(str);
            
            in >> std::hex >> c;
            
            if(in.fail()) {
                throw std::runtime_error("stream decode failure");
            }
        }
        
        return static_cast<unsigned char>(c);
    }

    
    ~HelloWorld();
    
	void callbackFromJS(cocos2d::experimental::ui::WebView* webview, const std::string &answer);
    virtual bool init();
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    cocos2d::experimental::ui::WebView *_webView;
    
    // a selector callback
    void redeemClick(std::string url);
    void adCouponOnClickToUnlock();

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    //AdManager Delegate
    virtual void AdmobManagerOnInterstitialClose();
    virtual void AdmobManagerOnVideoClose();
    
    void inititial();
    void showCoupon();
    void showInterstitial();
};

#endif // __HELLOWORLD_SCENE_H__
