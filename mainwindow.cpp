#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    gate_in = new Gate_In_Manager(this);
    train_create = new Train_maker(this);
    canvas_animation = new Animation(this);

    map_labels = new QLabel*[MAX_X * MAX_Y];
    for(int i = 0; i < MAX_X * MAX_Y; i++)
    {
        this->map_labels[i] = new QLabel(this);
        int x = i % MAX_X;
        int y = i / MAX_X;
        this->map_labels[i]->setGeometry(40*x, 30*y, 40, 30);
        this->map_labels[i]->setAutoFillBackground(true);
    }

    train_labels = new std::pair<QLabel*, Train*>*[PLATFORM_SUM+PLATFORM_SUM];
    for(int i = 0; i < PLATFORM_SUM+PLATFORM_SUM; i++)
    {
        this->train_labels[i] = new std::pair<QLabel*, Train*>;
        this->train_labels[i]->first = new QLabel(this);
        this->train_labels[i]->second = nullptr;
        this->train_labels[i]->first->setGeometry(40*(MAX_X-1), 0, 40, 30);
        this->train_labels[i]->first->setStyleSheet("font: 10pt; color: rgb(0, 0, 0); background-color: rgb(255, 255, 255); border: 2px solid black");
        train_labels[i]->first->setAutoFillBackground(true);
        this->train_labels[i]->first->hide();
    }

    connect(train_create, SIGNAL(notify_train_incoming(Train*)), gate_in, SLOT(notified_train_incoming(Train*)));

    connect(canvas_animation, SIGNAL(notify_move_train(Infrastructure*)), this, SLOT(notified_move_train(Infrastructure*)));
    connect(canvas_animation, SIGNAL(notify_train_arrived(Infrastructure*)), gate_in, SLOT(notified_train_arrived(Infrastructure*)));

    connect(gate_in, SIGNAL(notify_color(int,int,int)), this, SLOT(notified_color(int,int,int)));
    connect(gate_in, SIGNAL(notify_train_label_attach(Train*)), this, SLOT(notified_train_label_attach(Train*)));
    connect(gate_in, SIGNAL(notify_put_train_on_canvas(Train*)), this, SLOT(notified_put_train_on_canvas(Train*)));
    connect(gate_in, SIGNAL(notify_train_depart(std::deque<Infrastructure*>*)), canvas_animation, SLOT(notified_train_depart(std::deque<Infrastructure*>*)));
    connect(gate_in, SIGNAL(notify_change_color(Train*)), this, SLOT(notified_change_color(Train*)));
    connect(gate_in, SIGNAL(notify_train_label_detach(Train*)), this, SLOT(notified_train_label_detach(Train*)));

    this->gate_in->map_coloring();
    this->start_simulation();
}

MainWindow::~MainWindow()
{
    gate_in->terminate();
    train_create->terminate();
    canvas_animation->terminate();
    delete ui;
}

void MainWindow::notified_train_label_attach(Train *train_input)
{
    for(unsigned int i = 0; i < PLATFORM_SUM+PLATFORM_SUM; i++)
    {
        if(this->train_labels[i]->second == nullptr)
        {
            this->train_labels[i]->second = train_input;
            return;
        }
    }
}

void MainWindow::notified_put_train_on_canvas(Train *train_input)
{
    for(unsigned int i = 0; i < PLATFORM_SUM+PLATFORM_SUM; i++)
    {
        if(this->train_labels[i]->second == train_input)
        {
            train_labels[i]->first->setText(QString::number(train_input->getId()));
            this->train_labels[i]->first->show();
            return;
        }
    }
}

void MainWindow::notified_move_train(Infrastructure *now)
{
    if(now == nullptr)
        return;
    int x = now->getX() - now->getTrain()->getBefore_x();
    int y = now->getY() - now->getTrain()->getBefore_y();
    for(unsigned int i = 0; i < PLATFORM_SUM+PLATFORM_SUM; i++)
    {
        if(this->train_labels[i]->second == now->getTrain())
        {
            this->train_labels[i]->first->move(this->train_labels[i]->first->x() + x*40, this->train_labels[i]->first->y() + y*30);
            return;
        }
    }
}

void MainWindow::notified_train_label_detach(Train *train_input)
{
    for(unsigned int i = 0; i < PLATFORM_SUM+PLATFORM_SUM; i++)
    {
        if(this->train_labels[i]->second == train_input)
        {
            this->train_labels[i]->second = nullptr;
            train_labels[i]->first->move(40*(MAX_X-1), 0);
            train_labels[i]->first->setText("");
            train_labels[i]->first->hide();
            return;
        }
    }
}

void MainWindow::notified_color(int x, int y, int type)
{
    int y2 = (39) * y;
    if(type == 0)
        this->map_labels[y2 + x]->setStyleSheet("background-color: rgb(205, 255, 205); border: 1px solid black");
    else if(type == 1)
        this->map_labels[y2 + x]->setStyleSheet("background-color: rgb(205, 205, 255); border: 1px solid black");
    else
        this->map_labels[y2 + x]->setStyleSheet("background-color: rgb(255, 205, 205); border: 1px solid black");
}

void MainWindow::notified_change_color(Train *train_input)
{
    for(unsigned int i = 0; i < PLATFORM_SUM+PLATFORM_SUM; i++)
    {
        if(this->train_labels[i]->second == train_input)
        {
            this->train_labels[i]->first->setStyleSheet("font: 10pt; color: rgb(0, 0, 0); background-color: rgb(255, 155, 155); border: 2px solid black");
            return;
        }
    }
}

void MainWindow::start_simulation()
{
    gate_in->start();
    canvas_animation->start();
    train_create->start();
}

void MainWindow::background_initialization()
{

}
