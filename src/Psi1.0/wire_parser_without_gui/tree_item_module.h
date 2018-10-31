#ifndef TREE_ITEM_MODULE_H
#define TREE_ITEM_MODULE_H

#include <QList>
#include <QVariant>

class tree_item_module
{
public:
    explicit tree_item_module(std::string name,std::string type,std::string location,tree_item_module *parentItem = 0);
    ~tree_item_module();

    void appendChild(tree_item_module *child);
    void appendWirePublic(std::string wire);
    void appendWirePrivate(std::string wire);
    std::string getName();
    std::string getType();
    std::string getLocation();
    QList<std::string> getPrivateWires();
    QList<std::string> getPublicWires();




    tree_item_module *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    tree_item_module *parentItem();

private:
    QList<tree_item_module*> m_childItems;
    QList<QVariant> m_itemData;
    tree_item_module *m_parentItem;
    QList<std::string> wire_extern_private;
    QList<std::string> wire_extern_public;
    std::string module_name;
    std::string module_type;
    std::string module_location;
};

#endif // TREE_ITEM_MODULE_H
