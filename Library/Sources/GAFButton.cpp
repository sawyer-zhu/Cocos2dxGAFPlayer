#include "GAFPrecompiled.h"
#include "GAFButton.h"
#include "GAFTimeline.h"
#include "GAFTextField.h"

NS_GAF_BEGIN

GAFButton::GAFButton()
: m_enabled(true)
, m_textfield(nullptr)
, m_button_pressed_callback(nullptr)
{
}

GAFButton::~GAFButton()
{

}

bool GAFButton::init(GAFAsset* anAnimationData, GAFTimeline* timeline)
{
    bool result = GAFObject::init(anAnimationData, timeline);

    // check named sequences
    result = result && (timeline->getSequence("idle") != nullptr);
    result = result && (timeline->getSequence("pressed") != nullptr);
    result = result && (timeline->getSequence("disabled") != nullptr);

    if (result)
    {
        playSequence("idle", false);
    }

    m_textfield = reinterpret_cast<GAFTextField*>(getObjectByName("textfield"));

    auto touchListener = cocos2d::EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(GAFButton::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(GAFButton::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    m_rect = getBoundingBoxForCurrentFrame();

    return result;
}

bool GAFButton::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    // TODO: use hitzone
    if (!m_enabled)
        return false;

    cocos2d::Vec2 localPoint = convertTouchToNodeSpace(touch);

    if (m_rect.containsPoint(localPoint))
    {
        playSequence("pressed", false);
        return true;
    }
    return false;
}

void GAFButton::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    // callback                                                    
    playSequence("idle", false);

    if (m_button_pressed_callback)
    {
        m_button_pressed_callback(this);
    }
}

void GAFButton::setEnabled(bool enabled)
{
    if (enabled != m_enabled)
    {
        m_enabled = enabled;

        std::string sequence = m_enabled ? "idle" : "disabled";

        playSequence(sequence, false);
    }
}

void GAFButton::setCaption(std::string caption)
{
    if (m_textfield == nullptr)
    {
        CCAssert(false, "No dynamic textfield in object");
        return;
    }

    m_textfield->setText(caption);
}

void GAFButton::setButtonPressedCallback(GAFButtonCallback_t callback)
{
    m_button_pressed_callback = callback;
}

NS_GAF_END