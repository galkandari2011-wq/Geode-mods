#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class $modify(BufferPlayLayer, PlayLayer) {
    int m_bufferedAction = -1;
    int m_bufferTimer = 0;

    void postUpdate(float dt) {
        PlayLayer::postUpdate(dt);

        // Decrement the buffer window timer each tick if active
        if (m_bufferTimer > 0) {
            m_bufferTimer--;
            if (m_bufferTimer == 0) {
                m_bufferedAction = -1;
            }
        }

        // If we have a stored action waiting in our 3-tick buffer, attempt to fire it
        if (m_bufferedAction != -1 && m_player1 && !m_player1->m_isDead) {
            // Execute buffered jump/action when safe
            m_player1->pushButton(static_cast<PlayerButton>(m_bufferedAction));
            m_bufferedAction = -1;
        }
    }

    bool handleButton(bool down, int button, bool isState) {
        // Intercept player actions to apply the predictive 3-tick window
        if (down && m_player1 && m_player1->m_isDead) {
            // Buffer inputs slightly early if near a transition or dead state edge
            m_bufferedAction = button;
            m_bufferTimer = 3; // 3-tick window
        }
        
        return PlayLayer::handleButton(down, button, isState);
    }
};

class $modify(BufferPlayerObject, PlayerObject) {
    void pushPlayerButton(PlayerButton button) {
        // Standard player push override logic for input smoothing
        PlayerObject::pushPlayerButton(button);
    }
};
