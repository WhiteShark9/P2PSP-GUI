#include "addchannels.h"

addChannels::addChannels()
{

}

string addChannels::getName() const
{
    return name;
}

void addChannels::setName(const string &value)
{
    name = value;
}

string addChannels::getDescription() const
{
    return description;
}

void addChannels::setDescription(const string &value)
{
    description = value;
}

string addChannels::getThumbnailUrl() const
{
    return thumbnailUrl;
}

void addChannels::setThumbnailUrl(const string &value)
{
    thumbnailUrl = value;
}

string addChannels::getSplitterAddress() const
{
    return splitterAddress;
}

void addChannels::setSplitterAddress(const string &value)
{
    splitterAddress = value;
}

unsigned int addChannels::getSplitterPort() const
{
    return splitterPort;
}

void addChannels::setSplitterPort(unsigned int value)
{
    splitterPort = value;
}

QPixmap addChannels::getThumbnail() const
{
    return thumbnail;
}

void addChannels::setThumbnail(const QPixmap value)
{
    thumbnail = value;
}
