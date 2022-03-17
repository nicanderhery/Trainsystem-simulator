#include "train_maker.h"

Train_maker::Train_maker(QObject *parent) : QThread(parent)
{

}

void Train_maker::run()
{
    while(true)
    {
        Train* new_train = new Train(1+rand()%100, 20);
        emit notify_gate_in(new_train);
        this->sleep(this->trail_interval);
    }
}
