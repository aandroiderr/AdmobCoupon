//
//  AdCoupon.cpp
//  Components-mobile
//
//  Created by Ahmet Abak on 22.02.2018.
//

#include "AdCoupon.hpp"
#include "external/json/document.h"
#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"
#include <string>
#include <fstream>

class AdCoupon;

#define AD_COUPON_VIEW_NAME "kAdCouponView"
#define AD_COUPON_FILE_NAME "kAdCouponFile"
#define INDICATOR_NAME "kAdCouponIndicatorView"
#define COUPON_REQ_URL "http://35.200.205.177:3002/coupons"

#define BOLD_FONT "adcoupon_assets/SourceSansPro-Bold.ttf"
#define REGULAR_FONT "adcoupon_assets/SourceSansPro-Regular.ttf"
#define LIGHT_FONT "adcoupon_assets/SourceSansPro-Light.ttf"

#define SELECTED_BUTTON "adcoupon_assets/rounded_rect.png"
#define UNSELECTED_BUTTON "adcoupon_assets/rounded_rect_unselected.png"

const cocos2d::Color4B TITLE_COLOR = cocos2d::Color4B(222,105,105,255);
const cocos2d::Color4B OFFER_TEXT_COLOR = cocos2d::Color4B(102,102,102,255);
const cocos2d::Color4B STORE_COLOR = cocos2d::Color4B(56,56,56, 255);
const cocos2d::Color4B SELECTED_BLUE_COLOR = cocos2d::Color4B(79, 164, 221, 255);

namespace act_ind {
    void addIndicator() {
        USING_NS_CC;
        auto size = Director::getInstance()->getVisibleSize();
        LayerColor *lc = LayerColor::create(Color4B(102, 102, 102, 102), size.width, size.height);
        lc->setName(INDICATOR_NAME);
        
        auto listener = cocos2d::EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [&](Touch* touch, Event* event) { return true; };
        listener->onTouchEnded = [=](Touch* touch, Event* event) { };
        
        cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, lc);
        
        for (int i = 0; i < 3; i++) {
            DrawNode *dn = DrawNode::create();
            dn->drawSolidRect(Vec2(size.width * 0.2 - 3, size.height * 0.5 -3), Vec2(size.width * 0.2 +3, size.height * 0.5 +3), Color4F::WHITE);
            dn->setPosition(Vec2::ZERO);
            lc->addChild(dn);
            
            dn->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.8 + i * 0.2, Vec2(size.width * 0.6, 0)),
                                                                 DelayTime::create((2-i) * 0.2),
                                                                 MoveBy::create(0.0, Vec2(size.width * -0.6, 0)), NULL)));
        }
        Director::getInstance()->getRunningScene()->addChild(lc, INT_MAX);
    }
    void removeIndicator() {
        USING_NS_CC;
        auto ind = Director::getInstance()->getRunningScene()->getChildByName(INDICATOR_NAME);
        if (ind) {
            ind->removeFromParentAndCleanup(true);
        }
    }
}

namespace rwfile {
    using namespace std;
    
    void writeFile(string filename, string data) {
        std::string path = cocos2d::FileUtils::getInstance()->getWritablePath() + "\\" + filename;
        ofstream outfile;
        outfile.open(path);
        outfile << data;
        outfile.close();
    }
    
    string readFile(string filename) {
        std::string path = cocos2d::FileUtils::getInstance()->getWritablePath() + "\\" + filename;
        ifstream infile;
        infile.open(path);
        string data((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
        infile.close();
        return data;
    }
    
    template <typename T>
    std::string stringify(T value)
    {
        std::ostringstream os;
        os << value ;
        return os.str();
    }
}

class AsyncSprite : public cocos2d::Sprite {
private:
    cocos2d::network::Downloader *downloader;
    std::string srcID;
    cocos2d::Size originalSize;
    
    void onDownloadCompleted(const cocos2d::network::DownloadTask &task) {
        
        std::string name = "__asyncSprite__" + srcID;
        std::string filePath = cocos2d::FileUtils::getInstance()->getWritablePath() + name;
        
        initWithFile(filePath);
        
        setScale(originalSize.width / getTextureRect().size.width, originalSize.height / getTextureRect().size.height);
        
        delete downloader;
    }
public:
    CREATE_FUNC(AsyncSprite);
    
    static AsyncSprite* createFromURL(const std::string url, cocos2d::Size size, std::string id) {
        AsyncSprite *pRet = new(std::nothrow) AsyncSprite();
        if (pRet && pRet->initFromURL(url, size, id)) {
            pRet->autorelease();
            return pRet;
        }
        else {
            delete pRet;
            pRet = nullptr;
            return nullptr;
        }
    }
    
    bool initFromURL(const std::string url, cocos2d::Size size, std::string id) {
        
        if (!Sprite::init()) {
            return false;
        }
        
        originalSize = size;
        srcID = id;
        
        downloader = new (std::nothrow) cocos2d::network::Downloader();
        downloader->setOnFileTaskSuccess(CC_CALLBACK_1(AsyncSprite::onDownloadCompleted, this));
        downloader->setOnTaskError([this](const cocos2d::network::DownloadTask &t, int a, int b, const std::string &st) {
            delete this->downloader;
        });
        
        std::string filepath = cocos2d::FileUtils::getInstance()->getWritablePath() + "__asyncSprite__" + id;
        downloader->createDownloadFileTask(url, filepath);
        
        return true;
    }
};

class Coupon {
public:
    std::string lmd_id;
    std::string store;
    std::string offer_text;
    std::string title;
    std::string description;
    std::string code;
    std::string terms_and_conditions;
    std::string categories;
    std::string featured;
    std::string url;
    std::string smartLink;
    std::string type;
    std::string offer_value;
    std::string status;
    std::string start_date;
    std::string end_date;
    std::string imageURL;
public:
    Coupon(const rapidjson::Value& docx) {
        if(docx.HasMember("lmd_id")) (lmd_id = docx["lmd_id"].GetString());
        if(docx.HasMember("store")) store = docx["store"].GetString();
        if(docx.HasMember("offer text")) offer_text = docx["offer text"].GetString();
        if(docx.HasMember("title")) title = docx["title"].GetString();
        if(docx.HasMember("description")) description = docx["description"].GetString();
        if(docx.HasMember("code")) code = docx["code"].GetString();
        if(docx.HasMember("terms_and_conditions")) terms_and_conditions = docx["terms_and_conditions"].GetString();
        if(docx.HasMember("categories")) categories = docx["categories"].GetString();
        if(docx.HasMember("featured")) featured = docx["featured"].GetString();
        if(docx.HasMember("url")) url = docx["url"].GetString();
        if(docx.HasMember("smartLink")) smartLink = docx["smartLink"].GetString();
        if(docx.HasMember("type")) type = docx["type"].GetString();
        if(docx.HasMember("offer value")) offer_value = docx["offer value"].GetString();
        if(docx.HasMember("status")) status = docx["status"].GetString();
        if(docx.HasMember("start_date")) start_date = docx["start_date"].GetString();
        if(docx.HasMember("end_date")) end_date = docx["end_date"].GetString();
        if(docx.HasMember("imageURL")) imageURL = docx["imageURL"].GetString();
    }
};

#pragma mark -CouponUI class

class CouponUI : public cocos2d::Node {
    
public:
    CREATE_FUNC(CouponUI);
    AdCouponDelegate *delegate;
    virtual bool init() {
        if (!cocos2d::Node::init()) {
            return false;
        }
        
        auto listener = cocos2d::EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event) { return true; };
        listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event) { };
        
        cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
        
        return true;
    }
    
    void initOptions(const std::vector<Coupon>& coupons, cocos2d::Size size, AdCouponDelegate *del) {
        using namespace cocos2d;
        
        setName(AD_COUPON_VIEW_NAME);
        delegate = del;
        
        LayerColor *lc = LayerColor::create(STORE_COLOR, size.width, 44);
        lc->setPosition(Vec2(0, size.height - 44));
        addChild(lc);
        
        LayerColor *bg = LayerColor::create(Color4B::WHITE, size.width, size.height - 44);
        bg->setPosition(Vec2::ZERO);
        addChild(bg);
        
        Label *top = Label::createWithTTF("Collect Coupons", BOLD_FONT, 24);
        top->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        top->setPosition(Vec2(size.width * 0.5, 22));
        top->setTextColor(Color4B::WHITE);
        lc->addChild(top,1);
        
        ui::Button *close = ui::Button::create();
        close->setTitleFontName(BOLD_FONT);
        close->setContentSize(Size(40, 40));
        close->setTitleText("X");
        close->setTitleColor(Color3B::RED);
        close->setPressedActionEnabled(true);
        close->setTitleFontSize(25);
        close->setPosition(Vec2(size.width - 26, 22));
        close->addTouchEventListener([this](Ref *ref, ui::Widget::TouchEventType t){
            if (t == ui::Widget::TouchEventType::ENDED) {
                auto view = Director::getInstance()->getRunningScene()->getChildByName(AD_COUPON_VIEW_NAME);
                if (view) {
                    if(delegate) delegate->adCouponOnClose();
                    
                    view->removeFromParentAndCleanup(true);
                }
            }
        });
        
        lc->addChild(close);
        
        ui::ScrollView *sc = ui::ScrollView::create();
        sc->setScrollBarOpacity(0);
        sc->setContentSize(Size(size.width, size.height - 44));
        sc->setAnchorPoint(Vec2::ZERO);
        
        float IMAGE_P = 0.28f;
        Size boxSize = Size(size.width, size.width * 0.27);
        int couponSize = coupons.size();
        float scrollHeight = couponSize * boxSize.height;
        scrollHeight = scrollHeight < (size.height - 44) ? (size.height - 44) : scrollHeight;
        
        for(int i = 0; i < couponSize; i++) {
            
            auto box = LayerColor::create(Color4B::WHITE);
            box->setContentSize(boxSize);
            box->setPosition(Vec2(0, scrollHeight - boxSize.height * (i+1)));
            
            AsyncSprite *pp = AsyncSprite::createFromURL(coupons[i].imageURL, Size(size.width * 0.2, size.width * 0.2), rwfile::stringify(i));
            pp->setPosition(Vec2(boxSize.width * IMAGE_P * 0.5f, boxSize.height * 0.5f));
            box->addChild(pp);
            
            LayerColor *line = LayerColor::create(Color4B::BLACK, boxSize.width - 6, 1);
            line->setPosition(Vec2(3, 0));
            box->addChild(line);
            
            Label *title = Label::createWithTTF(coupons[i].title, BOLD_FONT, 25);
            title->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            title->setTextColor(TITLE_COLOR);
            title->setPosition(Vec2(boxSize.width * IMAGE_P, boxSize.height * 0.76f));
            box->addChild(title);
            
            Label *offer_text = Label::createWithTTF(coupons[i].offer_text, LIGHT_FONT, 20);
            offer_text->setAnchorPoint(Vec2(0.0f, 1.0f));
            offer_text->setTextColor(OFFER_TEXT_COLOR);
            offer_text->setMaxLineWidth(size.width * 0.4);
            offer_text->setPosition(Vec2(boxSize.width * IMAGE_P, boxSize.height * 0.73f));
            box->addChild(offer_text);
            
            Label *store = Label::createWithTTF(coupons[i].store, REGULAR_FONT, 20);
            store->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            store->setTextColor(STORE_COLOR);
            store->setMaxLineWidth(size.width * 0.6);
            store->setPosition(Vec2(boxSize.width * IMAGE_P, boxSize.height * 0.04));
            box->addChild(store);
            
            ui::Button *bt = ui::Button::create(UNSELECTED_BUTTON);
            bt->setTitleFontName(REGULAR_FONT);
            float scale = boxSize.width * 0.28 / bt->getBoundingBox().size.width;
            bt->setScale(scale);
            bt->setTitleText("CLICK TO UNLOCK");
            bt->setTitleColor(Color3B(SELECTED_BLUE_COLOR));
            bt->setPressedActionEnabled(true);
            bt->setTitleFontSize(20 / scale);
            bt->setTag(i);
            bt->setPosition(Vec2(boxSize.width * 0.83, boxSize.height * 0.5f));
            bt->addTouchEventListener([this, coupons](Ref *r, ui::Widget::TouchEventType t) {
                
                if (t == ui::Widget::TouchEventType::ENDED) {
                    auto n = dynamic_cast<ui::Button *>(r);
                    
                    if (n->getTag() < coupons.size()) {
                        if(delegate) delegate->adCouponOnClickToUnlock();
                        n->setTag(n->getTag() + 1000);
                        n->loadTextureNormal(SELECTED_BUTTON);
                        n->setTitleColor(Color3B::WHITE);
                        n->setTitleText("Redeem");
                        
                        auto node = cocos2d::Director::getInstance()->getRunningScene()->getChildByName(AD_COUPON_VIEW_NAME);
                        
                        if (node) {
                            node->setVisible(false);
                            node->pause();
                        }
                    }
                    else {
                        if(delegate) delegate->adCouponOnRedirectToLink();
                        Application::getInstance()->openURL(coupons[n->getTag()-1000].url);
                        if(delegate) delegate->adCouponOnClose();
                        
                        auto view = Director::getInstance()->getRunningScene()->getChildByName(AD_COUPON_VIEW_NAME);
                        view->removeFromParentAndCleanup(true);
                    }
                    
                }
            });
            
            box->addChild(bt);
            sc->addChild(box);
        }
        
        sc->setInnerContainerSize(Size(size.width, boxSize.height * coupons.size()));
        sc->setPosition(Vec2::ZERO);
        addChild(sc,1);
    }
};

#pragma mark -CouponManager class

class CouponManager {
private:
    std::function<void(std::string)> callback;
private:
    void onCouponRequestSuccess(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response) {
        using namespace cocos2d::network;
        
        if (response && response->getResponseCode() == 200 && response->getResponseData()) {
            std::vector<char> *data = response->getResponseData();
            std::string ret(&(data->front()), data->size());
            if (ret.length() > 10) {
                rwfile::writeFile(AD_COUPON_FILE_NAME, ret);
            }
        }
        else {
            cocos2d::log("error getting data");
        }
        
        sendDataToUI();
    }
    
    void sendDataToUI() {
        auto data = rwfile::readFile(AD_COUPON_FILE_NAME);
        cocos2d::log("result of data %s", data.c_str());
        callback(data);
    }
    
public:
    void requestCoupons(std::function<void(std::string)> func) {
        callback = func;
        
        using namespace cocos2d::network;
        
        HttpRequest* request = new HttpRequest();
        request->setRequestType(HttpRequest::Type::GET);
        request->setUrl(COUPON_REQ_URL);
        request->setResponseCallback(CC_CALLBACK_2(CouponManager::onCouponRequestSuccess, this));
        request->setTag("GetTest");
        HttpClient::getInstance()->send(request);
        request->release();
    }
};

#pragma mark -AdCoupon class methods

void AdCoupon::initialize(AdCouponDelegate *del) {
    AdCoupon::getInstance().delegate = del;
}

void AdCoupon::setDelegate(AdCouponDelegate *del) {
    AdCoupon::getInstance().delegate = del;
}

void AdCoupon::showCoupons() {
    
    act_ind::addIndicator();
    
    auto node = cocos2d::Director::getInstance()->getRunningScene()->getChildByName(AD_COUPON_VIEW_NAME);
    
    if (node) {
        node->removeFromParentAndCleanup(true);
    }
    
    if (AdCoupon::getInstance().delegate) {
        AdCoupon::getInstance().delegate->adCouponsOnRequest();
    }
    
    static CouponManager couponManager;
    
    couponManager.requestCoupons([](std::string couponData) ->void {
        rapidjson::Document docx;
        
        if (couponData.length() < 10 || docx.Parse(couponData.c_str()).HasParseError()) {
            if (AdCoupon::getInstance().delegate) {
                AdCoupon::getInstance().delegate->adCouponHasNoCouponsAvailable();
            }
            
            act_ind::removeIndicator();
            
            return;
        }
        
        act_ind::removeIndicator();
        
        std::vector<Coupon> coupons;
        if (docx["coupons"].IsArray()) {
            for (auto& itr : docx["coupons"].GetArray()) {
                Coupon coupon = Coupon(itr);
                
                coupons.push_back(coupon);
            }
        }
        
        auto size = cocos2d::Director::getInstance()->getVisibleSize();
        CouponUI *cui = CouponUI::create();
        cui->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_LEFT);
        cui->initOptions(coupons, size, AdCoupon::getInstance().delegate);
        cui->setPosition(cocos2d::Vec2::ZERO);
        cocos2d::Director::getInstance()->getRunningScene()->addChild(cui, INT_MAX);
        
        if (AdCoupon::getInstance().delegate) {
            AdCoupon::getInstance().delegate->adCouponOnShow();
        }
    });
}

void AdCoupon::showRedeemCoupon() {
    auto node = cocos2d::Director::getInstance()->getRunningScene()->getChildByName(AD_COUPON_VIEW_NAME);
    
    if (node) {
        node->setVisible(true);
        node->resume();
    }
}
