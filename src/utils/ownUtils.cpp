#include "ownUtils.h"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

void ownUtils::FixLayerv3(CCLayer* layer, int width, int height) {
    if (!layer || layer->getChildrenCount() == 0) return;

    // Usamos typeinfo_cast para seguridad y comprobamos el índice 0
    auto layerChild = typeinfo_cast<CCLayer*>(layer->getChildren()->objectAtIndex(0));
    if (!layerChild) return;

    // SOLUCIÓN v5: Usamos getChildrenExt() para iterar directamente
    for (auto currentNode : layerChild->getChildrenExt()) {
        CCSize contentSize = currentNode->getContentSize();

        if (contentSize.height == height && contentSize.width == width) {
            currentNode->setVisible(false);
        }
    }
}

void ownUtils::FixLayerv2(CCLayer* layer, int width, int height) {
    if (!layer) return;

    // SOLUCIÓN v5: Iteración moderna sobre los hijos del layer
    for (auto currentNode : layer->getChildrenExt()) {
        CCSize contentSize = currentNode->getContentSize();

        if (contentSize.height == height && contentSize.width == width) {
            currentNode->setVisible(false);
        }
    }
}

void ownUtils::FixLayer(CCLayer* layer, int width, int height) {
    if (!layer) return;

    // SOLUCIÓN v5: Iteración moderna
    for (auto currentNode : layer->getChildrenExt()) {
        CCSize contentSize = currentNode->getContentSize();

        if (contentSize.height == height && contentSize.width == width) {
            CCSize newNodeSize = { 0, 100 };
            currentNode->setContentSize(newNodeSize);
            currentNode->setVisible(false); // Opcional, según tu lógica original

            auto children = currentNode->getChildren();
            if (children && children->count() >= 4) {
                currentNode->setZOrder(1);
                for (int i = 0; i < 4; i++) {
                    if (auto sprite = typeinfo_cast<CCSprite*>(children->objectAtIndex(i))) {
                        sprite->setVisible(false);
                    }
                }
                currentNode->setZOrder(2);
            }
        }
    }
}

cocos2d::ccColor4B ownUtils::ToColor4B(ccColor3B sourceColor) {
    return ccColor4B{ sourceColor.r, sourceColor.g, sourceColor.b, 255 };
}
