#ifndef NODE_H
#define NODE_H
#include "lib/world.h"
#include "lib/world_global.h"
#include <memory>

class node
{
public:
    node(std::shared_ptr<Tile> pos,std::shared_ptr<Tile> pre);

    std::shared_ptr<Tile> getPre() const;
    void setPre(const std::shared_ptr<Tile> &value);

    std::shared_ptr<Tile> getPos() const;
    void setPos(const std::shared_ptr<Tile> &value);

private:
    std::shared_ptr<Tile> pos;
    std::shared_ptr<Tile> pre;
};

#endif // NODE_H
