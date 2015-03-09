#include "GAFPrecompiled.h"
#include "GAFButton.h"
#include "GAFTimeline.h"
#include "GAFTextField.h"

NS_GAF_BEGIN

const std::string GAFButton::s_sequenceIdle = "idle";
const std::string GAFButton::s_sequencePressed = "pressed";
const std::string GAFButton::s_sequenceDisabled = "disabled";

const std::string GAFButton::s_subobjectTextfield = "textfield";

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
    result = result && (timeline->getSequence(s_sequenceIdle) != nullptr);
    result = result && (timeline->getSequence(s_sequencePressed) != nullptr);
    result = result && (timeline->getSequence(s_sequenceDisabled) != nullptr);

    if (result)
    {
        playSequence(s_sequenceIdle, false);
    }

    m_textfield = reinterpret_cast<GAFTextField*>(getObjectByName(s_subobjectTextfield));

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
        playSequence(s_sequencePressed, false);
        return true;
    }
    return false;
}

void GAFButton::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    // callback                                                    
    playSequence(s_sequenceIdle, false);

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

        std::string sequence = m_enabled ? s_sequenceIdle : s_sequenceDisabled;

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