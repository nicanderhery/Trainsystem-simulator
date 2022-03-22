#include "infrastructure.h"

Infrastructure::Infrastructure(int new_type)
{  
    this->type = new_type;
    if(this->type == PLATFORM)
        this->stay_duration = 10;
    else if(this->type == MINE)
        this->stay_duration = 115;
}

int Infrastructure::getType() const
{
    return type;
}

bool Infrastructure::getIs_occupied() const
{
    return is_occupied;
}

void Infrastructure::setIs_occupied(bool newIs_occupied)
{
    is_occupied = newIs_occupied;
}

int Infrastructure::getStay_duration() const
{
    return stay_duration;
}

void Infrastructure::setLeft_list(Infrastructure *&newLeft)
{
    this->left_list.push_back(newLeft);
}

const std::vector<Infrastructure *> &Infrastructure::getLeft_list() const
{
    return left_list;
}

void Infrastructure::setRight_list(Infrastructure *&newRight)
{
    this->right_list.push_back(newRight);
}

const std::vector<Infrastructure *> &Infrastructure::getRight_list() const
{
    return right_list;
}

Train *Infrastructure::getCurrent_train() const
{
    return current_train;
}

void Infrastructure::setCurrent_train(Train *newCurrent_train)
{
    current_train = newCurrent_train;
}