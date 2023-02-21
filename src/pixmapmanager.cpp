#include "pixmapmanager.h"

PixmapManager* PixmapManager::ptr = nullptr;

PixmapManager *PixmapManager::Instance()
{
    if(!ptr)
    {
        ptr = new PixmapManager();
    }
    return ptr;
}


QPixmap& PixmapManager::getPixmap(TextureID id)
{
    return m_textures.get(id);
}

PixmapManager::PixmapManager()
{
    m_textures.load(TextureID::Blocks, ":/res/blocks.png");
    m_textures.load(TextureID::Title, ":/res/title.png");
    m_textures.load(TextureID::Buttons, ":/res/buttons.png");

    std::unique_ptr<QPixmap> ButtonUnselected(new QPixmap(getPixmap(TextureID::Buttons).copy(0,0,200,50)));
    m_textures.insertResource(TextureID::ButtonUnselected, std::move(ButtonUnselected));

    std::unique_ptr<QPixmap> ButtonSelected(new QPixmap(getPixmap(TextureID::Buttons).copy(0,50,200,50)));
    m_textures.insertResource(TextureID::ButtonSelected, std::move(ButtonSelected));
}
