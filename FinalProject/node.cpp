#include "node.h"

node::node(std::shared_ptr<Tile> tile, std::shared_ptr<node> pre)
{
    this->tile = tile;
    this->pre = pre;
}


std::shared_ptr<node> node::getPre() const
{
    return pre;
}

void node::setPre(const std::shared_ptr<node> &value)
{
    pre = value;
}

std::shared_ptr<Tile> node::getTile() const
{
    return tile;
}

void node::setTile(const std::shared_ptr<Tile> &value)
{
    tile = value;
}

double node::getDistance() const
{
    return distance;
}

void node::setDistance(double value)
{
    distance = value;
}

