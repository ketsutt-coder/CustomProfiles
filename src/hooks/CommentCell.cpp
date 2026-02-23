#include <Geode/Geode.hpp>
#include <Geode/modify/CommentCell.hpp>

using namespace geode::prelude;

// En Geode v5 es mejor usar typeinfo_cast directamente en lugar de templates personalizados
class $modify(CustomCommentCell, CommentCell)
{
    struct Fields {
        bool m_wasInProfilePage = false;
    };

    void loadFromComment(GJComment* comment)
    {
        CommentCell::loadFromComment(comment);

        // Si el comentario no tiene nivel asociado (comentario de perfil)
        if (comment->m_levelID == 0 || comment->m_hasLevelID == 1)
        {
            auto children = this->getChildren();
            if (!children || children->count() == 0) return;

            // Ocultamos el fondo original (índice 0)
            if (auto commentBGColor = typeinfo_cast<CCSprite*>(children->objectAtIndex(0))) {
                commentBGColor->setVisible(false);
            }

            CCSize smallContentSize = { 890, 85 };
            float Y = this->getPositionY() + 18;
            float X = this->getPositionX() + 170;

            if (this->m_height == 36)
            {
                auto smallCommentSquare = cocos2d::extension::CCScale9Sprite::create("square02b_001.png");
                smallCommentSquare->setColor(ccBLACK);
                smallCommentSquare->setOpacity(32);
                smallCommentSquare->setScale(0.375);
                smallCommentSquare->setContentSize(smallContentSize);
                smallCommentSquare->setPositionX(X);
                smallCommentSquare->setPositionY(Y);
                smallCommentSquare->setZOrder(-1);
                this->addChild(smallCommentSquare);
            }
            else
            {
                // SOLUCIÓN AL ERROR: Usamos CCArrayExt en lugar de CCARRAY_FOREACH
                if (children->count() > 1) {
                    if (auto layer = typeinfo_cast<CCLayer*>(children->objectAtIndex(1))) {
                        
                        for (auto pObj : CCArrayExt<CCNode*>(layer->getChildren())) {
                            if (auto scale9Sprite = typeinfo_cast<cocos2d::extension::CCScale9Sprite*>(pObj)) {
                                scale9Sprite->setColor(ccBLACK);
                                scale9Sprite->setOpacity(32);
                            }
                        }
                    }
                }
            }
        }
    }

    // Unificamos el fix de dibujo para Android, macOS e iOS para evitar repetición
#if defined(GEODE_IS_ANDROID) || defined(GEODE_IS_MACOS) || defined(GEODE_IS_IOS)
    void draw()
    {
        auto scene = CCDirector::get()->getRunningScene();

        // Fix para que no se vean bordes raros en transiciones de perfil
        if (scene && scene->getChildByID("ProfilePage")) {
            m_fields->m_wasInProfilePage = true;
        }

        if (!m_fields->m_wasInProfilePage) {
            CommentCell::draw();
        }
    }
#endif
};
