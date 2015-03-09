#pragma once

#include "GAFObject.h"

NS_GAF_BEGIN

class GAFTextField;

class GAFButton : public GAFObject
{
public:
    GAFButton();
    virtual ~GAFButton();

    bool init(GAFAsset * anAnimationData, GAFTimeline* timeline) override;

    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    void setEnabled(bool enabled);
    void setCaption(std::string caption);

    void setButtonPressedCallback(GAFButtonCallback_t callback);

private:
    bool m_enabled;
    cocos2d::Rect m_rect;

    GAFTextField *m_textfield;

    GAFButtonCallback_t m_button_pressed_callback;
};

NS_GAF_END