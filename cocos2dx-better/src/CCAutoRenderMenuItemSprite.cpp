/****************************************************************************
 Author: Luma (stubma@gmail.com)
 
 https://github.com/stubma/cocos2dx-better
 
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
#include "CCAutoRenderMenuItemSprite.h"
#include "CCUtils.h"

NS_CC_BEGIN

CCAutoRenderMenuItemSprite::CCAutoRenderMenuItemSprite() :
m_focus(false),
m_focusIsAttachment(false),
m_focusImage(NULL),
m_selectedEventTarget(NULL),
m_selectedEventSelector(NULL),
m_unselectedEventTarget(NULL),
m_unselectedEventSelector(NULL) {
}

CCAutoRenderMenuItemSprite::~CCAutoRenderMenuItemSprite() {
}

CCAutoRenderMenuItemSprite* CCAutoRenderMenuItemSprite::create(CCNode* sprite, CCObject* target, SEL_MenuHandler selector) {
	CCAutoRenderMenuItemSprite* pRet = new CCAutoRenderMenuItemSprite();
    pRet->initWithNormalSprite(sprite, NULL, NULL, target, selector);
    pRet->autorelease();
    return pRet;
}

CCAutoRenderMenuItemSprite* CCAutoRenderMenuItemSprite::create(CCNode* normalImage, CCNode* focusImage) {
    CCAutoRenderMenuItemSprite* pRet = new CCAutoRenderMenuItemSprite();
    pRet->initWithNormalSprite(normalImage, NULL, NULL, NULL, NULL);
    pRet->setFocusImage(focusImage);
    pRet->autorelease();
    return pRet;
}

CCAutoRenderMenuItemSprite* CCAutoRenderMenuItemSprite::create(CCNode* normalImage, CCNode* focusImage, CCObject* target, SEL_MenuHandler selector) {
	CCAutoRenderMenuItemSprite* pRet = new CCAutoRenderMenuItemSprite();
    pRet->initWithNormalSprite(normalImage, NULL, NULL, target, selector);
	pRet->setFocusImage(focusImage);
    pRet->autorelease();
    return pRet;
}

CCAutoRenderMenuItemSprite* CCAutoRenderMenuItemSprite::create(CCNode* normalImage, CCNode* selectedImage,
															   CCNode* focusImage, CCObject* target, SEL_MenuHandler selector) {
	CCAutoRenderMenuItemSprite* pRet = new CCAutoRenderMenuItemSprite();
    pRet->initWithNormalSprite(normalImage, selectedImage, NULL, target, selector);
	pRet->setFocusImage(focusImage);
    pRet->autorelease();
    return pRet;
}

CCAutoRenderMenuItemSprite* CCAutoRenderMenuItemSprite::create(CCNode* normalImage, CCNode* selectedImage, CCNode* disabledImage,
															   CCNode* focusImage, CCObject* target, SEL_MenuHandler selector) {
	CCAutoRenderMenuItemSprite* pRet = new CCAutoRenderMenuItemSprite();
    pRet->initWithNormalSprite(normalImage, selectedImage, disabledImage, target, selector);
	pRet->setFocusImage(focusImage);
    pRet->autorelease();
    return pRet;
}

void CCAutoRenderMenuItemSprite::setSelectedEvent(CCObject* target, SEL_MenuHandler selector) {
    m_selectedEventTarget = target;
    m_selectedEventSelector = selector;
}

void CCAutoRenderMenuItemSprite::setUnselectedEvent(CCObject* target, SEL_MenuHandler selector) {
    m_unselectedEventTarget = target;
    m_unselectedEventSelector = selector;
}

void CCAutoRenderMenuItemSprite::centerAlignImages() {
    CCSize s = CCSizeZero;
    if(m_pNormalImage) {
        CCSize _s = m_pNormalImage->getContentSize();
        s.width = MAX(s.width, _s.width);
        s.height = MAX(s.height, _s.height);
    }
    if(m_pSelectedImage) {
        CCSize _s = m_pSelectedImage->getContentSize();
        s.width = MAX(s.width, _s.width);
        s.height = MAX(s.height, _s.height);
    }
    if(m_focusImage) {
        CCSize _s = m_focusImage->getContentSize();
        s.width = MAX(s.width, _s.width);
        s.height = MAX(s.height, _s.height);
    }
    if(m_pDisabledImage) {
        CCSize _s = m_pDisabledImage->getContentSize();
        s.width = MAX(s.width, _s.width);
        s.height = MAX(s.height, _s.height);
    }
    setContentSize(s);
    
    if(m_pNormalImage) {
        CCSize _s = m_pNormalImage->getContentSize();
        m_pNormalImage->setPosition(ccp((s.width - _s.width) / 2,
                                        (s.height - _s.height) / 2));
    }
    if(m_pSelectedImage) {
        CCSize _s = m_pSelectedImage->getContentSize();
        m_pSelectedImage->setPosition(ccp((s.width - _s.width) / 2,
                                          (s.height - _s.height) / 2));
    }
    if(m_focusImage) {
        CCSize _s = m_focusImage->getContentSize();
        m_focusImage->setPosition(ccp((s.width - _s.width) / 2,
                                      (s.height - _s.height) / 2));
    }
    if(m_pDisabledImage) {
        CCSize _s = m_pDisabledImage->getContentSize();
        m_pDisabledImage->setPosition(ccp((s.width - _s.width) / 2,
                                          (s.height - _s.height) / 2));
    }
}

void CCAutoRenderMenuItemSprite::selected() {
    CCMenuItemSprite::selected();
    updateImagesVisibility();
	CCMenuItemSprite::selected();
	
	// set a darker color
	if(!getSelectedImage()) {
		m_oldColor = getColor();
		ccColorHSV hsv = CCUtils::ccc32hsv(m_oldColor);
		hsv.v = MAX(0, hsv.v - 0.2f);
		ccColor3B c = CCUtils::hsv2ccc3(hsv);
		setColor(c);
	}
    
    // event
    if (m_selectedEventTarget && m_selectedEventSelector) {
        (m_selectedEventTarget->*m_selectedEventSelector)(this);
    }
}

void CCAutoRenderMenuItemSprite::unselected() {
    updateImagesVisibility();
	CCMenuItemSprite::unselected();
	
	// restore old color
	if(!getSelectedImage()) {
		setColor(m_oldColor);
	}
    
    // event
    if (m_unselectedEventTarget && m_unselectedEventSelector) {
        (m_unselectedEventTarget->*m_unselectedEventSelector)(this);
    }
}

void CCAutoRenderMenuItemSprite::updateImagesVisibility() {
	if (m_pNormalImage)
		m_pNormalImage->setVisible(false);
	if (m_pSelectedImage)
		m_pSelectedImage->setVisible(false);
	if (m_pDisabledImage)
		m_pDisabledImage->setVisible(false);
	if (m_focusImage)
		m_focusImage->setVisible(false);
	
	if(m_focusImage) {
        if(m_focusIsAttachment) {
            m_focusImage->setVisible(m_focus || isSelected());
        } else {
            m_focusImage->setVisible(m_focus);
        }
	}
    CCMenuItemSprite::updateImagesVisibility();
}

void CCAutoRenderMenuItemSprite::setFocus(bool flag) {
	m_focus = flag;
	updateImagesVisibility();
}

void CCAutoRenderMenuItemSprite::setFocusImage(CCNode* focusImage) {
	if (focusImage != m_focusImage) {
        if (focusImage) {
            addChild(focusImage, 0);
            focusImage->setAnchorPoint(ccp(0, 0));
        }
		
        if (m_focusImage) {
            removeChild(m_focusImage, true);
        }
		
        m_focusImage = focusImage;
		updateImagesVisibility();
    }
}

NS_CC_END