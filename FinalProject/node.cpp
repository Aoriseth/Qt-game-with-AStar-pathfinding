#include "node.h"

node::node(std::shared_ptr<Tile> pos, std::shared_ptr<Tile> pre)
{
    this->pos = pos;
    this->pre = pre;
}


std::shared_ptr<Tile> node::getPre() const
{
    return pre;
}

void node::setPre(const std::shared_ptr<Tile> &value)
{
    pre = value;
}

std::shared_ptr<Tile> node::getPos() const
{
    return pos;
}

void node::setPos(const std::shared_ptr<Tile> &value)
{
    pos = value;
}
