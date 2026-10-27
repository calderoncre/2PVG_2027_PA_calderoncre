#pragma once
#ifndef UNO_INTERFACES_ICONTROLLER_HPP
#define UNO_INTERFACES_ICONTROLLER_HPP
#include "IView.hpp"
#include "IModel.hpp"
//! Game_Manager
class IController
{
public:
    //! a implementar
    virtual ~IController() = default;
    virtual void run() = 0;
};
class Controller : public IController
{

private:
    Model model;
    UI ui{model};

public:
    void run() override;
    void init_game();
    Controller() {};
    ~Controller() {};
};

#endif // UNO_INTERFACES_ICONTROLLER_HPP