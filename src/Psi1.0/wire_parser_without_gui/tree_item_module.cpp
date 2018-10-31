#include "tree_item_module.h"

#include <QStringList>

tree_item_module::tree_item_module(std::string name, std::string type, std::string location, tree_item_module *parent)
{
    m_parentItem = parent;
    module_name = name;
    module_type = type;
    module_location = location;
}

tree_item_module::~tree_item_module()
{
    qDeleteAll(m_childItems);
}

void tree_item_module::appendChild(tree_item_module *item)
{
    m_childItems.append(item);
}

tree_item_module *tree_item_module::child(int row)
{
    return m_childItems.value(row);
}

int tree_item_module::childCount() const
{
    return m_childItems.count();
}

int tree_item_module::columnCount() const
{
    return m_itemData.count();
}

QVariant tree_item_module::data(int column) const
{
    return m_itemData.value(column);
}

tree_item_module *tree_item_module::parentItem()
{
    return m_parentItem;
}

int tree_item_module::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<tree_item_module*>(this));

    return 0;
}

void tree_item_module::appendWirePublic(std::string wire){
    wire_extern_public << wire;
}

void tree_item_module::appendWirePrivate(std::string wire){
    wire_extern_private << wire;
}

std::string tree_item_module::getName(){
    return module_name;
}

std::string tree_item_module::getType(){
    return module_type;
}

std::string tree_item_module::getLocation(){
    return module_location;
}

QList<std::string> tree_item_module::getPrivateWires(){
    return wire_extern_private;
}

QList<std::string> tree_item_module::getPublicWires(){
    return wire_extern_public;
}
