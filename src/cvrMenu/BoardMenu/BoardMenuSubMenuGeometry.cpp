#include <cvrMenu/BoardMenu/BoardMenuSubMenuGeometry.h>
#include <cvrMenu/SubMenu.h>
#include <cvrUtil/Bounds.h>

#include <osg/Geometry>
#include <cvrConfig/ConfigManager.h>

using namespace cvr;

BoardMenuSubMenuGeometry::BoardMenuSubMenuGeometry(bool head) :
        BoardMenuGeometry()
{
    _head = head;
    _open = false;
    _width = 0;
}

BoardMenuSubMenuGeometry::~BoardMenuSubMenuGeometry()
{
}

void BoardMenuSubMenuGeometry::selectItem(bool on)
{
    if(_head)
    {
        if(on)
        {
            _node->removeChild(_geode);
            _node->removeChild(_geodeSelected);
            _node->addChild(_geodeSelected);
        }
        else
        {
            _node->removeChild(_geode);
            _node->removeChild(_geodeSelected);
            _node->addChild(_geode);
        }
    }
    else if(!_open)
    {
        if(on)
        {
            _node->removeChild(_geode);
            _node->removeChild(_geodeSelected);
            _node->addChild(_geodeSelected);
        }
        else
        {
            _node->removeChild(_geode);
            _node->removeChild(_geodeSelected);
            _node->addChild(_geode);
        }
    }
}

void BoardMenuSubMenuGeometry::openMenu(bool open)
{
    if(!_head)
    {
        if(open)
        {
            _node->removeChild(_geode);
            _node->removeChild(_geodeSelected);
            _node->addChild(_geodeSelected);
            if(_openIcon)
            {
                osg::StateSet * stateset = _geodeIcon->getOrCreateStateSet();
                stateset->setTextureAttributeAndModes(0,_openIcon,
                        osg::StateAttribute::ON);
            }
        }
        else
        {
            _node->removeChild(_geode);
            _node->removeChild(_geodeSelected);
            _node->addChild(_geode);
            if(_closedIcon)
            {
                osg::StateSet * stateset = _geodeIcon->getOrCreateStateSet();
                stateset->setTextureAttributeAndModes(0,_closedIcon,
                        osg::StateAttribute::ON);
            }
        }
        _open = open;
    }
}

void BoardMenuSubMenuGeometry::createGeometry(MenuItem * item)
{
    _node = new osg::MatrixTransform();
    _geode = new osg::Geode();
    _geodeSelected = new osg::Geode();
    _intersect = new osg::Geode();
    _node->addChild(_intersect);
    _node->addChild(_geode);
    _item = item;

    SubMenu * submenu = dynamic_cast<SubMenu*>(item);

    if(_head)
    {
        _geodeLine = new osg::Geode();
        _node->addChild(_geodeLine);

        osgText::Text * textNode = makeText(submenu->getTitle(),
                1.15f * _textSize, osg::Vec3(0, ConfigManager::CONTENT_BOARD_DIST, 0),_textColor,
                osgText::Text::LEFT_TOP);

        _geode->addDrawable(textNode);

        osg::BoundingBox bb = getBound(textNode);
        _width = bb.xMax() - bb.xMin();
        _height = bb.zMax() - bb.zMin() + _border;


        textNode = makeText(submenu->getTitle(),1.15 * _textSize,
                osg::Vec3(0,ConfigManager::CONTENT_BOARD_DIST,0),_textColorSelected,osgText::Text::LEFT_TOP);

        _geodeSelected->addDrawable(textNode);
    }
    else
    {
        _geodeIcon = new osg::Geode();
        _node->addChild(_geodeIcon);

        osg::Geometry * geo = makeQuad(_iconHeight,-_iconHeight,
                osg::Vec4(1.0,1.0,1.0,1.0),osg::Vec3(0,ConfigManager::CONTENT_BOARD_DIST,0));
        _geodeIcon->addDrawable(geo);

        osgText::Text * textNode = makeText(submenu->getName(),_textSize,
                osg::Vec3(_iconHeight + _border,ConfigManager::CONTENT_BOARD_DIST,-_iconHeight / 2.0),
                _textColor);
        /*osgText::Text * textNode = new osgText::Text();
         textNode->setCharacterSize(_textSize);
         textNode->setAlignment(osgText::Text::LEFT_CENTER);
         textNode->setPosition(osg::Vec3(_iconHeight + _border, -2,
         -_iconHeight / 2.0));
         textNode->setColor(_textColor);
         textNode->setBackdropColor(osg::Vec4(0, 0, 0, 0));
         textNode->setAxisAlignment(osgText::Text::XZ_PLANE);
         textNode->setText(submenu->getName());*/

        osg::BoundingBox bb = getBound(textNode);
        _width = bb.xMax() - bb.xMin() + _iconHeight + _border;
        //mg->height = bb.zMax() - bb.zMin();
        _height = _iconHeight;

        _geode->addDrawable(textNode);

        textNode = makeText(submenu->getName(),_textSize,
                osg::Vec3(_iconHeight + _border,ConfigManager::CONTENT_BOARD_DIST,-_iconHeight / 2.0),
                _textColorSelected);
        /*textNode = new osgText::Text();
         textNode->setCharacterSize(_textSize);
         textNode->setAlignment(osgText::Text::LEFT_CENTER);
         textNode->setPosition(osg::Vec3(_iconHeight + _border, -2,
         -_iconHeight / 2.0));
         textNode->setColor(_textColorSelected);
         textNode->setBackdropColor(osg::Vec4(0, 0, 0, 0));
         textNode->setAxisAlignment(osgText::Text::XZ_PLANE);
         textNode->setText(submenu->getName());*/

        _geodeSelected->addDrawable(textNode);

        _openIcon = loadIcon("arrow-left-highlighted.rgb");
        _closedIcon = loadIcon("arrow-left.rgb");

        if(_closedIcon)
        {
            osg::StateSet * stateset = _geodeIcon->getOrCreateStateSet();
            stateset->setTextureAttributeAndModes(0,_closedIcon,
                    osg::StateAttribute::ON);
        }
    }
}

void BoardMenuSubMenuGeometry::processEvent(InteractionEvent * event)
{
    if(event->getInteraction() == BUTTON_DOWN)
    {
        if(_item->getCallback())
        {
            _item->getCallback()->menuCallback(_item,
                    event->asHandEvent() ? event->asHandEvent()->getHand() : 0);
        }
    }
}

bool BoardMenuSubMenuGeometry::isMenuHead()
{
    return _head;
}

bool BoardMenuSubMenuGeometry::isMenuOpen()
{
    return _open;
}

void BoardMenuSubMenuGeometry::resetMenuLine(float width)
{
    _geodeLine->removeDrawables(0,_geodeLine->getNumDrawables());
    _geodeLine->addDrawable(
            makeLine(osg::Vec3(0,ConfigManager::CONTENT_BOARD_DIST,-(_height)),
                     osg::Vec3(width,ConfigManager::CONTENT_BOARD_DIST,-(_height)),
                    _textColor));
}
