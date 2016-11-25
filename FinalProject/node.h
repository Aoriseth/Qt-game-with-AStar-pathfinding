#ifndef NODE_H
#define NODE_H
#include "lib/world.h"
#include "lib/world_global.h"
#include <memory>

class node
{
public:
    node(std::shared_ptr<Tile> tile,std::shared_ptr<node> pre);

    std::shared_ptr<node> getPre() const;
    void setPre(const std::shared_ptr<node> &value);

    std::shared_ptr<Tile> getTile() const;
    void setTile(const std::shared_ptr<Tile> &value);


private:
    std::shared_ptr<Tile> tile;
    std::shared_ptr<node> pre;
};

#endif // NODE_H
